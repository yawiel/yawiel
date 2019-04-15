#ifndef YAWIEL_CORE_TEXT_CORPUS_HPP
#define YAWIEL_CORE_TEXT_CORPUS_HPP

#include <yawiel/prereqs.hpp>
#include <unordered_map>
#include <vector>

namespace yawiel{
namespace text{

template<typename StringType = std::string>
class Corpus
{
 public:
  typedef std::unordered_map<StringType, size_t> HashTable;

  typedef std::pair<const typename std::vector<StringType>::const_iterator,
                    const typename std::vector<StringType>::const_iterator>
                      NGram;

  typedef std::pair<typename StringType::const_iterator,
                    typename StringType::const_iterator> Word;

  Corpus();

  Corpus(StringType corpus);

  ~Corpus();

  void loadFile(const std::string& filePath);

  size_t frecuency(const StringType& ngram);

  size_t totalNGrams(const size_t n) const;

  std::unordered_map<StringType, double> GetScores(const size_t n = 2);

  const std::vector<StringType>& GetTokens() const { return *corpusTokens; };

 private:
  StringType* corpus;

  //std::vector<Word>* corpusTokens;
  std::vector<StringType>* corpusTokens;

  HashTable* frecuencies;

  typename HashTable::const_iterator calculateFrecuency(const StringType& ngram);

  void Tokenize(const StringType& ngram, std::vector<Word>& tokens);
};

}
}

#include "corpus_impl.cpp"

#endif
