import io
from ir_model import IRModel
from embeddings_model import EmbeddingsModel
import pickle
import traceback

class Doc(object):
    def __init__(self, text, doc_id=None, mentions_targets=None):
        self.text = text
        self.doc_id = doc_id
        self.mentions_targets = mentions_targets #list of tuples (mention, target)

def evaluate_on_test_data(docs_test, model):
    correct = 0
    queries_total = 0
    not_found = 0
    disambiguated = 0
    correct_at_k = 0

    for doc in docs_test:
        for mention, target in doc.mentions_targets:
            queries_total += 1
            target = 'kb_'+target
            try:
                top_prediction, predictions_list = model.get_entity(mention)
                if top_prediction:
                    disambiguated += 1
                    if top_prediction == target:
                        #print('Correct top prediction!')
                        correct += 1
                    elif predictions_list and target in predictions_list:
                        print('Target in top 10 predictions!')
                        print('mention ', mention, ' target ', target, ' prediction ', top_prediction)
                        print(predictions_list)
                        correct_at_k += 1
                    else:
                        not_found += 1
            except Exception as e:
                print(e)
                print('Error here!!!','mention ', mention, ' ,target ', target)
                traceback.print_exc()
                #print('Mention not in KB', mention)
                not_found += 1

    print()
    print('Disambiguated Mentions', disambiguated)
    print('Disambiguated@1 Mentions', correct)
    print('Number of Disambiguated@k Mentions', correct_at_k)
    print('Mentions not found in KB', not_found)
    print('Disambiguated Precision', correct / disambiguated)
    print('Total Precision', correct / queries_total)
    print('Not Found ', not_found)

def prepare_conll():
    conll_file = 'data/AIDA-YAGO2-dataset-with-NER.tsv'
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
                        # lower-casing the mention string !!!
                        # TODO: Should I preprocess the mention the same way as the entities? i.e. combine the words with _
                        mention_target_tuple = (line_components[2].lower(), line_components[3])
                        # print(mention_target_tuple)
                        doc_mention_targets.append(mention_target_tuple)

    return docs_test, docs_train

if __name__ == '__main__':
    docs_test = pickle.load(open("evalData/conll_test_docs", "rb"))

    # Compact TFIDF ALTERNATIVE
    ir_mod = IRModel('evalData/conll_index.p', 'evalData/conll_id_to_entities_index.p',
                     'evalData/conll_entities_to_id_index.p', 'evalData/conll_tokens_to_ids.p', 'evalData/conll_compact_tf_idf_dict.p') # 'evalData/conll_compact_tf_idf_dict.p'
    print('IR Model')
    evaluate_on_test_data(docs_test, ir_mod)
    print()
    # Embeddings
    # print('Embeddings Model')
    # embed_mod = EmbeddingsModel('embedData/all_vocab_vecs.kv', 'embedData/kb_vocab_vecs.kv')
    # evaluate_on_test_data(docs_test, embed_mod)
