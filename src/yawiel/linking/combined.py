from ir_model import IRModel
from embeddings_model import EmbeddingsModel
import operator
from collections import defaultdict
from itertools import chain

if __name__ == '__main__':
    mention = 'korea'

    # Compact TFIDF ALTERNATIVE
    ir_mod = IRModel('evalData/conll_index.p', 'evalData/conll_id_to_entities_index.p',
                     'evalData/conll_entities_to_id_index.p', 'evalData/conll_tokens_to_ids.p', 'evalData/conll_compact_tf_idf_dict.p') # 'evalData/conll_compact_tf_idf_dict.p'
    print('IR Model')
    top, pred_ir = ir_mod.get_entity(mention)
    print(pred_ir)
    print(top)
    print()

    # Embeddings
    print('Embeddings Model')
    embed_mod = EmbeddingsModel('embedData/all_vocab_vecs.kv', 'embedData/kb_vocab_vecs.kv')
    top, pred_embed = embed_mod.get_entity(mention)
    print(pred_embed)
    print(top)

    # Combine
    ir_dict = dict(pred_ir)
    embed_dict = dict(pred_embed)

    print()
    print('Merge')
    results = {k :  (ir_dict[k]  + embed_dict[k])/2 for k in ir_dict.keys() & embed_dict.keys()}
    sorted = sorted(results.items(), key=operator.itemgetter(1), reverse=True)
    print(sorted)

    # Alternative
    dict3 = defaultdict(list)
    for k, v in chain(ir_dict.items(), embed_dict.items()):
        dict3[k].append(v)

    for k, v in dict3.items():
        print(k, v)