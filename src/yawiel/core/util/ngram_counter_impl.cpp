#ifndef YAWIEL_CORE_UTIL_COUNT_NGRAMS_IMPL_CPP
#define YAWIEL_CORE_UTIL_COUNT_NGRAMS_IMPL_CPP

#include "ngram_counter.hpp"

namespace yawiel{
namespace util{

template<typename StringType>
void NGramCounterRule<StringType>::
Base(const typename text::Corpus<StringType>::NGram ngram)
{
  size_t firstGram = ngram.first;
  const size_t lastGram = ngram.second;
  std::vector<size_t> vectorGram;
  vectorGram.reserve(n);
  for (; firstGram != lastGram; ++firstGram)
    vectorGram.push_back((corpus.GetTokens()).at(firstGram));
  auto it = counts.find(vectorGram);
  if (it != counts.end())
    ++(it->second);
  else
    counts.emplace(std::move(vectorGram), 1);
}

template<typename StringType>
NGramCounter<StringType>::NGramCounter(const text::Corpus<StringType>& corpus) :
  corpus(corpus),
  counts(new std::map<size_t, NGramCounter<StringType>::CountsType>)
{
  // Nothing to do.
}

template<typename StringType>
NGramCounter<StringType>::~NGramCounter()
{
  delete counts;
}

template<typename StringType>
void NGramCounter<StringType>::
Count(const size_t n, NGramCounter<StringType>::CountsType& map)
{
  NGramCounterRule<StringType> rule(map, corpus, n);
  typedef text::GramLeftToRightTraverser<
    NGramCounterRule<StringType>, StringType> TRV;
  TRV traverser(rule, corpus);
  traverser.Traverse(n);
}

template<typename StringType>
const typename NGramCounter<StringType>::CountsType&
NGramCounter<StringType>::GetCounts(const size_t n)
{
  auto it = counts->find(n);
  if (it == counts->cend())
  {
    counts->emplace(n, NGramCounter<StringType>::CountsType());
    Count(n, counts->at(n));
    return counts->at(n);
  }
  return it->second;
}

template<typename StringType>
size_t NGramCounter<StringType>::GetCounts(const std::vector<size_t>& ngram)
{
  auto it = counts->find(ngram.size());
  if (it == counts->cend())
  {
    counts->emplace(ngram.size(), NGramCounter<StringType>::CountsType());
    Count(ngram.size(), counts->at(ngram.size()));
    it = counts->find(ngram.size());
  }
  auto findNGram = (it->second).find(ngram);
  if (findNGram != (it->second).end())
    return findNGram->second;
  else
    return 0;
}

template<typename StringType>
template<typename Archive>
void NGramCounter<StringType>::
serialize(Archive& ar, const unsigned int /* version */)
{
  // Modify const element. Other than the constructor this is the only place
  // it's modified.
  ar & BOOST_SERIALIZATION_NVP(const_cast<text::Corpus<StringType>&>(corpus));

  // Clean memory if necessary.
  if (Archive::is_loading::value)
  {
    delete counts;
  }

  ar & BOOST_SERIALIZATION_NVP(counts);
}

}
}

#endif
