#ifndef YAWIEL_CORE_UTIL_COUNT_NGRAMS_HPP
#define YAWIEL_CORE_UTIL_COUNT_NGRAMS_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/text/corpus.hpp>
#include <yawiel/core/text/gram_left_to_right_traverser.hpp>
#include <unordered_map>
#include <map>
#include <vector>
#include <yawiel/core/util/vector_hash.cpp>

using namespace std;
using namespace yawiel;

namespace yawiel{
namespace util{

template<typename StringType>
class NGramCounter
{
 public:
  typedef unordered_map<std::vector<size_t>, size_t> CountsType;

 private:
  const Corpus<StringType>& corpus;

  map<size_t, CountsType>* counts;

  void Count(const size_t n,
             CountsType& map);

 public:
  NGramCounter(const Corpus<StringType>& corpus);

  ~NGramCounter();

  const CountsType& GetCounts(const size_t n = 2);

  size_t GetCounts(const std::vector<size_t>& ngram);

  size_t GetNumberOfNGrams(const size_t n) const
      { return corpus.TotalNGrams(n); }

  template<typename Archive>
  void serialize(Archive& ar, const unsigned int /* version */);
};

template<typename StringType>
class NGramCounterRule
{
 private:
  unordered_map<std::vector<size_t>, size_t>& counts;

  const text::Corpus<StringType>& corpus;

  size_t n;

 public:
  NGramCounterRule(unordered_map<std::vector<size_t>, size_t>& counts,
                   const text::Corpus<StringType>& corpus,
                   const size_t n) :
    counts(counts),
    corpus(corpus),
    n(n)
  {};

  void Base(const typename Corpus<StringType>::NGram ngram);
};

}
}

#include "ngram_counter_impl.cpp"

#endif
