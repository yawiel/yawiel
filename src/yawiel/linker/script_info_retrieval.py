
import pickle
import setup_info_retrieval

with open("kb_entities_all_list.txt", "rb") as fp:  # Entities ids gathered during preprocessing.
    kb_entities = pickle.load(fp)

print('Entities size...', len(kb_entities))
ir_prep = setup_info_retrieval.InfoRetrievalSetup()

index = ir_prep.build_index(kb_entities)
tw_mat, ids_to_entities = ir_prep.compute_term_weights_matrix(kb_entities, index)
pickle.dump(ids_to_entities, open("all_id_to_entities_index.p", "wb"))
print('Term-weights matrix length...', len(tw_mat))
print('Vector in term-weights matrix length...', len(tw_mat[1]))
ir_prep.storeIndex('all_entities_', index)
ir_prep.store_term_weigths_matrix('all_entities_', tw_mat)