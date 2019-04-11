from gensim.models import KeyedVectors

class EmbeddingsModel(object):

    def __init__(self, path_to_vocab_vecs, path_to_kb_vecs ):
        self.vocab_vectors = KeyedVectors.load(path_to_vocab_vecs, mmap='r')
        self.kb_vectors = KeyedVectors.load(path_to_kb_vecs, mmap='r')

    def get_entity(self, mention, k=5, use_vector_avg=True):
        orig_mention = mention.lower()

        multi_word = (len(orig_mention.split(' ')) > 1)

        if (multi_word):
            mention = orig_mention.replace(' ', '_')
            mention = "kb_" + mention
        else:
            mention = orig_mention

        if mention in self.vocab_vectors:
            mention_vec = self.vocab_vectors[mention]
            predictions = self.kb_vectors.similar_by_vector(mention_vec, k)
            #print(predictions)

            if len(predictions)>1:
                top_prediction = predictions[0][0]
                predictions_list = [pred[0] for pred in predictions]
            else:
                top_prediction = predictions[0]
                predictions_list = [top_prediction]

            return top_prediction, predictions
        else:
            if use_vector_avg:
                if multi_word:
                    words = orig_mention.split()
                    words_vectors = []
                    for w in words:
                        try:
                            words_vectors.append(self.vocab_vectors[w])
                        except:
                            continue
                    if words_vectors:
                        mention_vec = self.vector_avg(words_vectors)
                        predictions = self.kb_vectors.similar_by_vector(mention_vec, k)
                        top_prediction = predictions[0][0]
                        predictions_list = [pred[0] for pred in predictions]
                        return top_prediction, predictions
                    else:
                        return [], []
                else:
                    return [], []
            else:
                return [], []


    def vector_avg(self, components):
        return (sum(components)) / len(components)

if __name__ == '__main__':

    search = EmbeddingsModel('embedData/all_vocab_vecs.kv', 'embedData/kb_vocab_vecs.kv')
    predictions, predictions_list = search.get_entity('portugal')
    print(predictions_list)
    print(predictions)

