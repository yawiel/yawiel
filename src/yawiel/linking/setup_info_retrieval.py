import numpy as np
import collections
import pickle


class InfoRetrievalSetup:
    """
        Index creation
            - dictionary: entities vocabulary from all entities
            - input: list of all KB entities
            - logic: iterate over all tokens contained in the entity, update dictionary
                        for each token, keep track of tokens already updated
            - output: dictionary (k,v) where k= token, v= list of entities
                        the token is in


    """

    def __init__(self, index_file_path= None):
        if index_file_path:
         self.loadIndex(index_file_path)

    def loadIndex(self, index_file):
        self.index = pickle.load(open(index_file, "rb"))
        print('Index initialized.')

    def build_index(self, entities_list):
        print('Constructing index...')
        entities_list = set(entities_list)
        token_entities_index = collections.defaultdict(list)
        # helper mapping structures
        # entities maps
        ids_to_entities = collections.defaultdict(list)
        entities_to_ids = collections.defaultdict(int)
        # vocab tokens maps
        ids_to_tokens = collections.defaultdict(list)
        tokens_to_ids = collections.defaultdict(int)

        if not isinstance(entities_list, list):
            entities_list = list(entities_list) # make sure we're considering only the unique cases

        for i in range(len(entities_list)):
            existing = set()
            entity_id = i
            entity_text = entities_list[i]
            ids_to_entities[entity_id] = entity_text
            entities_to_ids[entity_text] = entity_id
            entity_text = entity_text.split('_')

            for token in entity_text:
                if (token not in existing) and not (token == 'kb'): # idea is to consider only the first occurence of the term in the entity

                    token_entities_index[token].append(entity_id)
                    existing.add(token)

        # Build a token=>int map for the vocabulary
        token_id = 0
        for token in token_entities_index:
            tokens_to_ids[token] = token_id
            ids_to_tokens[token_id] = token
            token_id += 1

        self.index = token_entities_index
        print('Index length...', len(self.index))
        return token_entities_index, ids_to_entities, entities_to_ids, ids_to_tokens, tokens_to_ids

    def loadIndex(self, index_file):
        index = pickle.load(open(index_file, "rb"))
        return index

    def storeIndex(self, path, index):
        pickle.dump(index, open(path + "index.p", "wb"))

    """
        New data structure: {(doc_id, term_id): score}    
    """
    def compute_term_weights_matrix_compact(self, entities_to_ids, vocab_index, tokens_to_ids):
        print('Constructing term-weights matrix...')
        self.index = vocab_index
        tfidf_matrix = collections.defaultdict(list)

        # if not isinstance(entities_list, list):
        #     entities_list = list(entities_list)

        for entity, id in entities_to_ids.items():
            doc_id = id
            entities_text = entity.split('_')
            entities_text = ' '.join(str(elem) for elem in entities_text if not elem =='kb')

            for token in vocab_index:
                tf_idf_score = self.compute_tf_idf(token, entities_text, doc_id, len(entities_to_ids))
                if tf_idf_score != 0:
                    token_id = tokens_to_ids[token]
                    tfidf_matrix[(doc_id, token_id)] = tf_idf_score
        return tfidf_matrix

    def compute_term_weights_matrix_compact_store(self, entities_to_ids, vocab_index, tokens_to_ids, store_file):
        print('Constructing term-weights matrix...')
        self.index = vocab_index

        for entity, id in entities_to_ids.items():
            doc_id = id
            entities_text = entity.split('_')
            entities_text = ' '.join(str(elem) for elem in entities_text if not elem =='kb')

            for token in vocab_index:
                tf_idf_score = self.compute_tf_idf(token, entities_text, doc_id, len(entities_to_ids))
                if tf_idf_score != 0:
                    token_id = tokens_to_ids[token]
                    with open(store_file, "a") as f:
                        f.write(str(doc_id) + "#" + str(token_id) + "#" + str(tf_idf_score) + "\n")

    def read_tw_matrix(self, filename):
        with open(filename, "r") as f:
            dict = {}
            for line in f:
                values = line.split("#")
                dict[(int(values[0]), int(values[1]))] = float(values[2])
        return(dict)


    def compute_tf_idf(self, term, doc, docID, N):
        if (self.isInDoc(term, docID) == False):
            return 0
        else:
            return self.compute_tf(term, doc) * self.compute_idf(term, N)

    def isInDoc(self, term, docID):
        if docID in self.index[term]:
            return True
        return False

    def compute_tf(self, term, doc):
        if (self.raw_frequency(term, doc) > 0):
            return (1 + np.log10(self.raw_frequency(term, doc))) / (1 + np.log10(self.get_most_freq_term(doc)))
        else:
            return 0

    def compute_idf(self, term, N):
        return np.log10(N / self.get_docs_count(self.index, term))

    def get_docs_count(self, index, term):
        return len(index[term])

    def raw_frequency(self, term, doc):
        count = 0

        if isinstance(doc, str):
            for word in doc.split():
                if term == word:
                    count = count + 1

        return count

    def get_most_freq_term(self, doc):

        doc_freq = dict([word, self.raw_frequency(word, doc)] for word in doc.split())
        value, count = collections.Counter(doc_freq).most_common(1)[0]

        return count

    def store_term_weigths_matrix(self, path, matrix):
        pickle.dump(matrix, open(path + "saved_weights_matrix.p", "wb"))

    def retrieve_term_weights_matrix(self, path):
        matrix = pickle.load(open(path + "saved_weights_matrix.p", "rb"))
        return matrix

if __name__ == '__main__':


    # ALL Entities from WIKI TEST
    # with open("kb_entities_all_list.txt", "rb") as fp:   # Entities ids gathered during preprocessing.
    #     kb_entities = pickle.load(fp)

    # # Data Structures Construction
    # print(len(set(kb_entities)))
    # ir_prep = InfoRetrievalSetup()
    #
    # token_entities_index, ids_to_entities, entities_to_ids, ids_to_tokens, tokens_to_ids = ir_prep.build_index(
    #     kb_entities)
    #
    # tw_matrix = ir_prep.compute_term_weights_matrix_compact(entities_to_ids, token_entities_index, tokens_to_ids)
    # print(len(tw_matrix))
    # pickle.dump(tw_matrix, open("all_compact_tf_idf_dict.p", "wb"))


    # Subsample test
    with open("kb_entities_list.txt", "rb") as fp:   # Entities ids gathered during preprocessing.
        kb_entities = pickle.load(fp)
    #
    #    # print(len(set(kb_entities)))
    ir_prep = InfoRetrievalSetup()

    token_entities_index, ids_to_entities, entities_to_ids, ids_to_tokens, tokens_to_ids = ir_prep.build_index(
        kb_entities)

    ir_prep.compute_term_weights_matrix_compact_store(entities_to_ids, token_entities_index, tokens_to_ids, 'tw_matrix_compact.txt')
    tw_matrix = ir_prep.read_tw_matrix('tw_matrix_compact.txt')
    print(tw_matrix)


    # # Some tests and examples
    #
    # token_entities_index = pickle.load(open('test_index.p', "rb"))
    # tokens_to_ids = pickle.load(open('test_tokens_to_ids.p', "rb"))
    # ids_to_tokens = pickle.load(open('test_ids_to_tokens.p', "rb"))
    # tfidf = pickle.load(open('compact_tf_idf_dict.p', "rb"))
    # ids_to_entities = pickle.load(open('id_to_entities_index.p', "rb"))
    #
    # print(token_entities_index['pop'])
    # print(ids_to_entities[1564])
    # print()
    # print(ids_to_tokens[3])
    # print(ids_to_tokens[4])
    # print(ids_to_tokens[5])
    # print(ids_to_entities[3])

