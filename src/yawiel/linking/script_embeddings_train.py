from setup_embeddings import EmbeddingsLearner
import pickle
import time

def print_time(elapsed_time):
    print('Elapsed time:', time.strftime("%H:%M:%S", time.gmtime(elapsed_time)))

print('Initializing model...')
embed = EmbeddingsLearner('prepData/')
print('Starting train...')
start_time = time.time()
model = embed.train(10)
elapsed_time = time.time() - start_time
print_time(elapsed_time)

print('Loading kb entities words.')
with open("kb_entities_list.txt", "rb") as fp:   # Entities ids gathered during preprocessing.
    kb_entities = pickle.load(fp)

print("Saving model...")
model.save('models/word2vec_large.model')

print('Extracting vectors for kb entities...')
embed.restrict_w2v(kb_entities)
embed.save('models/word2vec_large.model', 'kb_vecs_new','all_vocab_vecs_new')

print('Model trained and saved.')