import numpy as np
import operator
import collections
import pickle
import tfidf


class IRModel:

    def __init__(self, index_file_path, index_to_entities_file_path, entities_to_id_path, tokens_to_ids_path,
                 matrix_file_path=None):
        self.loadIndex(index_file_path)
        self.tokens_to_ids = pickle.load(open(tokens_to_ids_path, "rb"))
        if matrix_file_path:
            self.term_weights_matrix = pickle.load(open(matrix_file_path, "rb"))
        else:
            self.term_weights_matrix = None
        self.index_values = list(self.index.keys())
        self.entities_lookup = pickle.load(open(index_to_entities_file_path, "rb"))
        self.entities_ids_lookup = pickle.load(open(entities_to_id_path, "rb"))

    def loadIndex(self, index_file):
        self.index = pickle.load(open(index_file, "rb"))

    def getIndex(self):
        return self.index

    def get_entity(self, mention):
        if self.term_weights_matrix:
            return self.retrieve_compact_tf_idf(mention)
        else:
            return self.retrieve_index_only(mention)

    def retrieve_index_only(self, query, nr_desired_entities=10):
        relevant_docs = []
        relevant_docs_dict = collections.defaultdict(int)

        query_term_ids = list()

        for term in query.split():
            if term in self.index:
                query_term_ids.append(term)
                relevant_docs_all = tuple(self.index[term])
                for doc in relevant_docs_all:
                    relevant_docs_dict[doc] = relevant_docs_dict[doc] + 1
                    # if doc in self.term_weights_matrix:

        # how many words overlap each match has
        relevant_docs_dict = sorted(relevant_docs_dict.items(), key=operator.itemgetter(1), reverse=True)



        for key, value in relevant_docs_dict:
            relevant_docs.append(key)

        if not relevant_docs:
            return [], []
        # further sort if the mention is of single word
        if (len(query_term_ids) == 1):
            predictions_strings = self.get_entities_strings(relevant_docs, True)  # sort by shortest
        else:
            predictions_strings = self.get_entities_strings(relevant_docs)

        top_prediction = predictions_strings[0]

        if top_prediction:
            return top_prediction, predictions_strings[0: nr_desired_entities]
        else:
            return top_prediction, predictions_strings


    def retrieve_compact_tf_idf(self, query, k=5):

        ##For every term in the query, retrieve the docID of only those docs that contain the query term
        candidate_entitites_ids = set()
        query_term_ids = list()

        for term in query.split():
            if term in self.index:
                query_term_ids.append(self.tokens_to_ids[term])
                relevant_entities_all = tuple(self.index[term])
                for doc in relevant_entities_all:
                    # if doc in self.term_weights_matrix:
                    candidate_entitites_ids.add(doc)

        ##Dictionary to store the distance between the query and each document that contains the query terms
        ##key: docID; value: distance

        # If there is a single candidate, no need to compute similarities, return directly
        if len(candidate_entitites_ids) == 1:
            top_prediction = self.get_entities_strings(candidate_entitites_ids)[0]
            return top_prediction, [(top_prediction,1)]

        similarities_dict = collections.defaultdict(float)
        query_vec = np.ones(len(query_term_ids)).tolist()

        for candidate_id in candidate_entitites_ids:
            # First generate the vector for the candidate
            candidate_vec = list()
            for query_term_id in query_term_ids:
                query_tuple = (candidate_id, query_term_id)
                score = self.term_weights_matrix[query_tuple]
                if score:
                    candidate_vec.append(score)
                else:
                    candidate_vec.append(0)
            # Then compute the similarity to the query
            similarity = tfidf.compute_cosine_similarity_with_norm(query_vec, candidate_vec)
            similarities_dict[candidate_id] = similarity

        ##Sort the documents by the distance, smallest distance to top
        sorted_similarities_dict = sorted(similarities_dict.items(), key=operator.itemgetter(1), reverse=True)
        prediction_string_with_scores = [(self.entities_lookup[pred[0]], pred[1]) for pred in sorted_similarities_dict]

        relevant_docs = []

        for key, value in sorted_similarities_dict:
            relevant_docs.append(key)

        if not relevant_docs:
            return [],[]

        if (len(query_term_ids) == 1):
            predictions_strings = self.get_entities_strings(relevant_docs, 1) # sort by shortest
        else:
            predictions_strings = self.get_entities_strings(relevant_docs)

        top_prediction = predictions_strings[0]

        if top_prediction:
            return top_prediction, prediction_string_with_scores[0: k]
        else:
            return top_prediction, prediction_string_with_scores

    def get_entities_strings(self, entities_ids, sort=False):
        entities_strings = []
        for ent_id in entities_ids:
            entities_strings.append(self.entities_lookup[ent_id])
        if sort:
            # If the mention is a single word, we want the smalles candidate length
            entities_strings.sort(key=len)
        return entities_strings

    def print_predictions(self, predictions_ids):
        if (len(predictions_ids) > 0):
            for top_pred in predictions_ids:
                print(self.entities_lookup[top_pred])
        else:
            print("Entities not found.")

if __name__ == '__main__':
    # Compact TFIDF ALTERNATIVE
    search = IRModel('ir_data/test_index.p', 'ir_data/id_to_entities_index.p',
                     'ir_data/entities_to_id_index.p', 'ir_data/test_tokens_to_ids.p', 'ir_data/compact_tf_idf_dict.p')
    predictions, relevant_docs_dict = search.get_entity('portugal')
    print(relevant_docs_dict)
    print(predictions)

    # INDEX ONLY ALTERNATIVE
    # search = IRModel('ir_data/test_index.p', 'ir_data/id_to_entities_index.p',
    #                  'ir_data/entities_to_id_index.p', 'ir_data/test_tokens_to_ids.p', None)
    # predictions, relevant_docs_dict = search.get_entity('law')
    # print(relevant_docs_dict)
    # print(predictions)
