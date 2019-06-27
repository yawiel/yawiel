#ifndef YAWIEL_CORE_UTIL_COUNT_NGRAMS_IMPL_CPP
#define YAWIEL_CORE_UTIL_COUNT_NGRAMS_IMPL_CPP

#include "ngram_counter.hpp"

namespace yawiel{
namespace util{

template<typename CorpusType>
void NGramCounterRule<CorpusType>::Base(const typename CorpusType::NGram ngram)
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
  counts(new std::map<size_t, NGramCounter<StringType>::CountsType>),
  ownCounts(true)
{
  // Nothing to do.
}

template<typename StringType>
NGramCounter<StringType>::NGramCounter(const NGramCounter<StringType>& other) :
  corpus(other.corpus),
  counts(new std::map<size_t, NGramCounter<StringType>::CountsType>
             (*other.counts)),
  ownCounts(true)
{
  // Nothing to do.
}

template<typename StringType>
NGramCounter<StringType>::NGramCounter(NGramCounter<StringType>&& other) :
  corpus(other.corpus),
  counts(other.counts),
  ownCounts(true)
{
  other.counts = new std::map<size_t, NGramCounter<StringType>::CountsType>;
  other.ownCounts = true;
}

template<typename StringType>
NGramCounter<StringType>::~NGramCounter()
{
  if (ownCounts)
    delete counts;
}

template<typename StringType>
void NGramCounter<StringType>::
Count(const size_t n, NGramCounter<StringType>::CountsType& map)
{
  //Create rule.
  NGramCounterRule<text::Corpus<StringType>> rule(map, corpus, n);

  // Create corpus traverser for the rule.
  typedef text::GramLeftToRightTraverser<
      NGramCounterRule<text::Corpus<StringType>>, StringType> TRV;
  TRV traverser(rule, corpus);

  // Count ngrams.
  traverser.Traverse(n);
}

template<typename StringType>
void NGramCounter<StringType>::ComputeCounts(const size_t n)
{
  auto it = counts->find(n);
  if (it == counts->cend())
  {
    counts->emplace(n, NGramCounter<StringType>::CountsType());
    Count(n, counts->at(n));
  }
}

template<typename StringType>
size_t NGramCounter<StringType>::GetCounts(const std::vector<size_t>& ngram)
{
  const NGramCounter<StringType>::CountsType& nCounts =
      counts->at(ngram.size());
  const auto it = nCounts.find(ngram);

  if (it != nCounts.cend())
    return it->second;
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
    if (ownCounts)
      delete counts;

    // Now it owns the counts.
    ownCounts = true;
  }

  ar & BOOST_SERIALIZATION_NVP(counts);
}

}
}

#endif
