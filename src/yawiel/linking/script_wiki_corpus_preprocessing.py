import sys

sys.path.insert(0, "/home/simona/anaconda3/lib/python3.7/site-packages")

from setup_preprocessing import CorpusPreparator
import time

def print_time(elapsed_time):
    print('Elapsed time:', time.strftime("%H:%M:%S", time.gmtime(elapsed_time)))

print('Script Execution starts...')
prep = CorpusPreparator()
start_time = time.time()
print('Corpus Prep object initialized. Loading redirects...')
redirects_graph = prep.load_wiki_redirects('dbpedia/redirects_en.ttl')
elapsed_time = time.time() - start_time
print_time(elapsed_time)
print('Redirects loaded. Starting preprocessing...')
start_time = time.time()
docs = prep.preprocess_wiki_corpus_from_folder('sourceData/data/', 'prepData/')
elapsed_time = time.time() - start_time
print_time(elapsed_time)
print('Corpus preprocessed.')



# print(model.wv.most_similar(positive='society'))
