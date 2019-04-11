from gensim.models import Word2Vec
import logging

logging.basicConfig(filename="embed.log", level=logging.DEBUG)
from gensim.models.word2vec import PathLineSentences
import copy
import numpy as np
import pickle

class EmbeddingsLearner(object):

    def __init__(self, path_to_docs):
        self.sentences = PathLineSentences(path_to_docs)
        self.model = Word2Vec(self.sentences, size=150, window=10, min_count=1, workers=10)
        self.word_vectors_all = []
        self.word_vectors_kb = []

    def train(self, ep=10):
        self.model.train(self.sentences, total_examples=self.model.corpus_count, epochs=ep)
        self.word_vectors_all = self.model.wv
        return self.model

    def save(self, model_path, kb_vecs_file, all_vocab_file):
        self.model.save(model_path)
        self.word_vectors_kb.save(kb_vecs_file + ".kv")
        self.word_vectors_all.save(all_vocab_file+".kv")

    def restrict_w2v(self, restricted_word_set):
        new_vectors = []
        new_vocab = {}
        new_index2entity = []
        new_vectors_norm = []
        w2v_original = self.word_vectors_all
        self.model.wv.most_similar(positive='canada') # for some reason without this line the norms are all None objects
        w2v = copy.deepcopy(w2v_original)
        print(w2v)

        for i in range(len(w2v.vocab)):
            word = w2v.index2entity[i]
            vec = w2v.vectors[i]
            vocab = w2v.vocab[word]
            vec_norm = w2v.vectors_norm[i]
            if word in restricted_word_set:
                vocab.index = len(new_index2entity)
                new_index2entity.append(word)
                new_vocab[word] = vocab
                new_vectors.append(vec)
                new_vectors_norm.append(vec_norm)

        # CONVERT THEM TO NUMPY ARRAYS !!!
        w2v.vocab = new_vocab
        w2v.vectors = np.array(new_vectors)
        w2v.index2entity = np.array(new_index2entity)
        w2v.index2word = np.array(new_index2entity)
        w2v.vectors_norm = np.array(new_vectors_norm)

        self.word_vectors_kb = w2v
        return w2v


if __name__ == '__main__':
    embed = EmbeddingsLearner('prepData/')
    model = embed.train(10)

    with open("wiki_entities.txt", "rb") as fp:  # Entities ids gathered during preprocessing.
        kb_entities = pickle.load(fp)

    embed.restrict_w2v(kb_entities)
    #model.save('models/word2vec_first.model')

    print('Model trained and saved.')