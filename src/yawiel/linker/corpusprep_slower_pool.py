import pandas as pd
import io
import json
import pickle
import base64
import re
import string
import nltk
from nltk.tokenize import word_tokenize
import os

nltk.download('punkt')
nltk.download('stopwords')
from nltk.corpus import stopwords
from rdflib import Graph
from rdflib import URIRef
import logging
from os import listdir
from os.path import isfile, join

logging.basicConfig(filename="corpus.log", level=logging.DEBUG)
from rdflib.term import URIRef
import time
import multiprocessing as mp

class Doc(object):
    def __init__(self, text, doc_id=None, mentions_targets=None):
        self.text = text
        self.doc_id = doc_id
        self.mentions_targets = mentions_targets  # list of tuples (mention, target)

    def load_conll_datasets(self, redirects_graph, conll_file):
        # conll_file = 'AIDA-YAGO2-dataset-with-NER.tsv'
        docs_train = []
        docs_test = []

        with io.open(conll_file, encoding='utf-8') as f:
            doc_text = []
            doc_mention_targets = []

            for line in f.readlines():
                doc_id = ''
                if ('DOCSTART' in line):
                    if (doc_text):
                        doc_text = " ".join(doc_text).replace("\n", "")
                        if 'test' in line:
                            docs_test.append(Doc(doc_text, doc_id, doc_mention_targets))
                        else:
                            docs_train.append(Doc(doc_text, doc_id, doc_mention_targets))

                    doc_text = []
                    doc_mention_targets = []
                    doc_id = line
                else:
                    line_components = line.split("\t")
                    doc_text.append(line_components[0])
                    if len(line_components) > 1:
                        if (line_components[1][0] == 'B'):
                            # position two contains the whole mention string
                            # Resolve target redirects !!!
                            target = line_components[3].encode('utf-8').decode('unicode_escape')
                            target = self.resolve_wikilink_dbpedia_redirect(target, redirects_graph)
                            # lower-casing the mention string !!!
                            mention_target_tuple = (line_components[2].lower(), target)
                            # print(mention_target_tuple)
                            doc_mention_targets.append(mention_target_tuple)

        return docs_train, docs_test

    def get_wiki_redirects_mentions_statistics(self, docs):
        redirected = []
        mentions_total_count = 0
        non_linkable_count = 0

        for doc in docs:
            for mention, current_wikilink in doc.mentions_targets:
                mentions_total_count += 1
                if '--NME--' not in current_wikilink:
                    current_wikilink = current_wikilink.encode('utf-8').decode('unicode_escape')
                    target_wikilink = CorpusPreparator.resolve_wikilink_dbpedia_redirect(current_wikilink)
                    if target_wikilink != current_wikilink:
                        redirected.append((current_wikilink, target_wikilink))
                else:
                    non_linkable_count += 1
        print('Total mentions count: ', mentions_total_count)
        print('Non-linkable count: ', non_linkable_count)
        print('Linked count: ', mentions_total_count - non_linkable_count)
        print('Redirected count: ', len(redirected))
        print('Unique redirects', len(set(redirected)))
        print()
        print(set(redirected))

    def resolve_wikilink_dbpedia_redirect(self, wikilink, redirects_graph):
        db_wikilink = 'http://dbpedia.org/resource/' + wikilink
        target = list(redirects_graph.triples((URIRef(db_wikilink), None, None)))

        count_reditects = 0
        while len(target) != 0:
            uri = target[0][2].n3()
            uri = uri[1:len(uri) - 1]  # e.g.'<http://dbpedia.org/resource/700_Series_Shinkans>'
            wikilink = uri.split('/')[4]
            target = list(redirects_graph.triples((URIRef(uri), None, None)))
            count_reditects += 1
        if count_reditects > 1:
            print('Redirects chain of length: ', count_reditects)
            print('Original Entity: ', wikilink)

        return wikilink


class CorpusPreparator(object):

    def __init__(self, source_folder = None, target_folder = None):
        self.redirected_global_count = 0
        self.docs_global_count = 0  # use as id for the processed documents
        self.redirects_graph = None
        if source_folder:
            self.source_folder = source_folder
        if target_folder:
            self.target_folder = target_folder

    def load_wiki_redirects(self, path_to_file):
        g = Graph()
        self.redirects_graph = g.parse(path_to_file, format='turtle')
        logging.debug("Loaded wiki redirects in rdflib Graph.")
        print("Loaded wiki redirects in rdflib Graph.")

        return self.redirects_graph

    # Assume that source and target folders are set.
    def preprocess_wiki_corpus_from_folder(self):
        all_entities = []
        current_folder_files = [f for f in listdir(self.source_folder) if isfile(join(self.source_folder, f)) and '.txt' not in f]
        pool = mp.Pool(processes=4)
        docs_entities = pool.map(self.preprocess_files_from_folder, current_folder_files)
        all_entities.extend(docs_entities)
        pool.close()
        pool.join()
        # Check if subfolders are available
        subfolders = [f.path for f in os.scandir(self.source_folder) if f.is_dir()]
        if subfolders:
            print('Processing subfolder files.')
            for sf in subfolders:
                start_time = time.time()
                print('Subfolder:', sf)
                subfolder_files = [f for f in listdir(sf) if isfile(join(sf, f)) and '.txt' not in f]
                self.source_folder = sf+'/'
                pool = mp.Pool(processes=4)
                docs_entities = pool.map(self.preprocess_files_from_folder, subfolder_files)
                all_entities.extend(docs_entities)
                pool.close()
                pool.join()
                elapsed_time = time.time() - start_time
                print('Elapsed time:', time.strftime("%H:%M:%S", time.gmtime(elapsed_time)))
                print()
        print('Corpus successfully preprocessed. Saving all entities in kb_entities_list.txt file.')
        self.store_kb_entities(all_entities)

        return

    def extract_kb_entities(self, source_folder):
        current_folder_files = [f for f in listdir(source_folder) if isfile(join(source_folder, f)) and '.txt' not in f]
        kb_entities = []
        for file in current_folder_files:
            file = source_folder + '/' + file
            with open(file) as f:
                for line in f.readlines():
                    for token in word_tokenize(line):
                        if 'kb_' in token:
                            # print(token)
                            kb_entities.append(token)
        with open("kb_entities_list.txt", "wb") as fp:  # Pickling
            pickle.dump(set(kb_entities), fp)
        print('KB Entities extracted and saved to file.')

    def preprocess_files_from_folder(self, extracted_wiki_files):
        local_entities = []
        if not isinstance(extracted_wiki_files, list):
            extracted_wiki_files = [extracted_wiki_files]
        for file in extracted_wiki_files:
            source_file = self.source_folder + file
            target_file = self.target_folder + file
            with io.open(source_file) as f:
                for wiki_article in f.readlines():
                    # each line is a dict with keys ['id', 'url', 'title', 'text', 'internal_links']
                    wiki_article = json.loads(wiki_article)
                    updated_text, doc_entities = self.replace_mentions_with_entities(wiki_article)
                    local_entities.append(doc_entities)
                    prep_sentences = self.preprocess_text_no_parallel(updated_text)
                    store_file_name = target_file + "_" + str(self.docs_global_count)
                    for sent in prep_sentences:
                        with open(store_file_name, 'a') as the_file:
                            the_file.write(sent + '\n')
            self.docs_global_count += 1

        return local_entities

    # e.g. extracted_wiki_files = ['data/wiki_01']
    def preprocess_wiki_corpus(self, extracted_wiki_files, file_name):
        all_entities = []

        for file in extracted_wiki_files:
            source_file = 'sourceData/' + file
            target_file = 'prepData/' + file
            with io.open(source_file) as f:
                print('Processing file', file)
                for wiki_article in f.readlines():
                    # each line is a dict with keys ['id', 'url', 'title', 'text', 'internal_links']
                    wiki_article = json.loads(wiki_article)
                    updated_text, doc_entities = self.replace_mentions_with_entities(wiki_article)
                    all_entities.append(doc_entities)

                    pool = mp.Pool(processes=4)
                    prep_sentences = self.preprocess_text(updated_text, pool)
                    pool.close()
                    pool.join()
                    # save into a file where each line is a new sentence, this file will be read on the fly by
                    # LineSentence for the gensim model,
                    # see https://radimrehurek.com/gensim/models/word2vec.html#gensim.models.word2vec.BrownCorpus
                    store_file_name = target_file + "_" + file_name
                    # print(updated_text)
                    for sent in prep_sentences:
                        with open(store_file_name, 'a') as the_file:
                            the_file.write(sent + '\n')
                    # break

        print('Corpus successfully preprocessed. Saving all entities in kb_entities_list.txt file.')
        self.store_kb_entities(all_entities)

        return all_entities

    def store_kb_entities(self, all_docs_entities):
        all_entities = [item.lower() for sublist in all_docs_entities for item in sublist[0]]
        with open("kb_entities_list.txt", "wb") as fp:  # Pickling
            pickle.dump(all_entities, fp)

    def preprocess_text_no_parallel(self, article_text):
        processed_article = article_text
        processed_article = re.sub(r'\s+', ' ', processed_article)

        # Preparing the dataset
        all_sentences = nltk.sent_tokenize(processed_article)
        prep_sentences = [self.preprocess_sentence(sent) for sent in all_sentences]

        return prep_sentences

    # ref: https://stackabuse.com/implementing-word2vec-with-gensim-library-in-python/
    def preprocess_text(self, article_text, pool):
        processed_article = article_text
        processed_article = re.sub(r'\s+', ' ', processed_article)

        # Preparing the dataset
        all_sentences = nltk.sent_tokenize(processed_article)
        prep_sentences = pool.map(self.preprocess_sentence, all_sentences)

        return prep_sentences

    def preprocess_sentence(self, sentence):
        remove = string.punctuation
        remove = remove.replace("_", "")  # don't remove underscores (part of entity names)
        remove = remove.replace(".", "")  # don't remove points for next split in sentences
        remove = remove.replace("(", "")
        remove = remove.replace(")", "")

        pattern = r"[{}]".format(remove)  # create the pattern
        processed_sent = re.sub(pattern, '', sentence)
        all_words = processed_sent.split(' ')
        all_words = [w for w in all_words if w not in stopwords.words('english')]
        return ' '.join(all_words).lower()

    def replace_mentions_with_entities(self, wiki_article):
        updated_text = wiki_article['text']
        redirect_stats = 0
        offset = 0
        redirected = []
        doc_entities = []

        for (char_offsets, (mention, wiki_page_name)) in pickle.loads(
                base64.b64decode(wiki_article['internal_links'].encode('utf-8'))).items():
            wiki_page_name = wiki_page_name.replace(' ', '_')
            if self.redirects_graph:
                target_wikilink = self.resolve_wikilink_dbpedia_redirect(wiki_page_name) # Have the option to not use DBPedia redirects.
            else:
                target_wikilink = wiki_page_name

            if target_wikilink != wiki_page_name:
                redirect_stats += 1
                redirected.append((wiki_page_name, target_wikilink))
            # We want to learn the embedding for the whole token of the entity, not it's separate parts...
            concat_entity = "KB_" + target_wikilink
            doc_entities.append(concat_entity)
            updated_text = re.sub(' '+ mention +' ', ' '+concat_entity+' ', updated_text)

        return updated_text, doc_entities

    def replace_mention(self, token, mention, entity_target):
        if token == mention:
            token = entity_target
        return token

    def resolve_wikilink_dbpedia_redirect(self, wikilink):
        db_wikilink = 'http://dbpedia.org/resource/' + wikilink
        target = list(self.redirects_graph.triples((URIRef(db_wikilink), None, None)))

        count_reditects = 0
        while len(target) != 0:
            uri = target[0][2].n3()
            uri = uri[1:len(uri) - 1]  # e.g.'<http://dbpedia.org/resource/700_Series_Shinkans>'
            wikilink = uri.split('/')[4]
            target = list(self.redirects_graph.triples((URIRef(uri), None, None)))
            count_reditects += 1
        if count_reditects > 1:
            print('Redirects chain of length: ', count_reditects)
            print('Original Entity: ', wikilink)
        return wikilink

    def resolve_wikilink_dbpedia_redirect_pandas(self, wikilink, redirects_df):
        target = redirects_df.loc[redirects_df['source'] == wikilink, 'target']

        while not target.empty:
            wikilink = target.values[0]
            target = redirects_df.loc[redirects_df['source'] == wikilink, 'target']

        return wikilink


if __name__ == '__main__':
    prep = CorpusPreparator('sourceData/data/', 'prepData/')
    docs = prep.preprocess_wiki_corpus_from_folder()

    # prep.extract_kb_entities('./prepdata')
    # with open("kb_entities_list.txt", "rb") as fp:   # entities ids gathered during preprocessing.
    #     kb_entities = pickle.load(fp)
    # print(kb_entities)
    # print(len(kb_entities))
    # print('kb_emi' in kb_entities)
