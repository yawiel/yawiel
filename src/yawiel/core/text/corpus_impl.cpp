#ifndef YAWIEL_CORE_TEXT_CORPUS_IMPL_CPP
#define YAWIEL_CORE_TEXT_CORPUS_IMPL_CPP

#include "corpus.hpp"
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <fstream>

namespace yawiel{
namespace text{

template<typename StringType>
Corpus<StringType>::Corpus():
  corpus(nullptr),
  corpusTokens(nullptr),
  frecuencies(nullptr)
{}

template<typename StringType>
Corpus<StringType>::Corpus(StringType corpus):
  corpus(new StringType(std::move(corpus))),
  corpusTokens(new std::vector<StringType>()),
  frecuencies(new HashTable())
{
  boost::algorithm::split(*corpusTokens, *(this->corpus), boost::is_any_of(" ,,,(,),."));
}

template<typename StringType>
void Corpus<StringType>::loadFile(const std::string& filePath)
{
  if (corpus)
    delete corpus;
  if (corpusTokens)
    delete corpusTokens;

  corpusTokens = new std::vector<StringType>();
  std::ifstream file(filePath);
  StringType tempString;
  std::vector<StringType> tempTokens;
  while (std::getline(file, tempString))
  {
    
    boost::algorithm::split(tempTokens, tempString, boost::is_any_of(" ,,,(,),."" "));
    for (auto token: tempTokens)
      corpusTokens->push_back(std::move(token));
  }
}

template<typename StringType>
Corpus<StringType>::~Corpus()
{
  if (corpus)
    delete corpus;
  if (corpusTokens)
    delete corpusTokens;
  if (frecuencies)
    delete frecuencies;
}
  /*
template<typename StringType>
size_t Corpus<StringType>::frecuency(const StringType& ngram)
{
  typename HashTable::const_iterator it = frecuencies->find(ngram);
  if (it != frecuencies->cend())
    return it->second;
  else
    return calculateFrecuency(ngram)->second;
}
*/
template<typename StringType>
size_t Corpus<StringType>::totalNGrams(const size_t n) const
{
  return corpusTokens->size() - n + 1;
}
/*
template<typename StringType>
std::unordered_map<StringType, double> Corpus<StringType>::GetScores(const size_t n)
{
  std::unordered_map<StringType, double> scores;
  typename std::vector<StringType>::const_iterator firstWord, lastWord;
  firstWord = corpusTokens->cbegin();
  lastWord = firstWord + n;
  // Iterate through n-grams.
  for (; lastWord != corpusTokens->cend(); ++firstWord, ++lastWord)
  {
    StringType str;
    for (auto it = firstWord; it != lastWord; ++it)
      str += StringType(it->first, it->second) + StringType(" ");
    str.pop_back();
    scores.insert({str, frecuency(str)});
  }
  return scores;
}

template<typename StringType>
typename Corpus<StringType>::HashTable::const_iterator Corpus<StringType>::calculateFrecuency(const StringType& ngram)
{
  size_t count = 0;
  std::vector<Word> ngramTokens;
  Tokenize(ngram, ngramTokens);
  const size_t n = ngramTokens.size();
  for (auto itCo = corpusTokens->cbegin(); itCo != corpusTokens->cend(); ++itCo)
  {
    bool equal = true;
    auto itGr = ngramTokens.cbegin();
    auto itCo2 = itCo;
    while (equal && itGr != ngramTokens.cend())
    {
      if (StringType(itCo2->first, itCo2->second) !=
          StringType(itGr->first, itGr->second))
      {
        equal = false;
      }
      ++itGr;
      ++itCo2;
    }
    if (equal)
      ++count;
  }
  return (frecuencies->insert({ngram, count})).first;
}

template<typename StringType>
void Corpus<StringType>::Tokenize(const StringType& ngram, std::vector<Word>& tokens)
{
  boost::algorithm::split(tokens, ngram, boost::is_any_of(" "));
}
  */
}
}

#include "corpus_impl.cpp"

#endif
