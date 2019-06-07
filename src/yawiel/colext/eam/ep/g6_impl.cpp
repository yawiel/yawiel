#ifndef YAWIEL_COLEXT_EAM_EP_G6_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G6_IMPL_CPP

#include "g6.hpp"
#include <yawiel/prereqs.hpp>
#include <vector>
#include <yawiel/colext/eam/ep/g0.hpp>

using namespace std;

namespace yawiel{
namespace colext{

template<typename StringType>
double G6<StringType, PMI<StringType>>::
Evaluate(const std::vector<size_t>& ngram, NGramCounter<StringType>& counter)
{
  const size_t totalN = counter.getNumberOfNGrams(ngram.size());
  const size_t probNGram = (double) counter.getCounts(ngram) / totalN;

  const size_t totalBi = counter.getNumberOfNGrams(2);
  double mult = 1;
  for (size_t i = 0; i < ngram.size(); i += 2)
  {
    mult *=
        (double) counter.getCounts(vector<size_t> {ngram[i], ngram[i + 1]}) /
        totalBi;
  }

  return std::log2(probNGram / mult);
}

template<typename StringType>
double G6<StringType, Dice<StringType>>::
Evaluate(const std::vector<size_t>& ngram, NGramCounter<StringType>& counter)
{
  const size_t n = ngram.size();

  size_t sum = 0;
  for (size_t i = 0; i < n; i += 2)
    sum += counter.getCounts(std::vector<size_t> {ngram[i], ngram[i + 1]});

  return (double) (n * counter.getCounts(ngram)) / sum;
}

template<typename StringType>
double G6<StringType, ChiSquared<StringType>>::
Evaluate(const std::vector<size_t>& ngram, NGramCounter<StringType>& counter)
{
  return ChiSquared<StringType>::Evaluate(ngram, vector<size_t>(), counter);
}

template<typename StringType>
double G6<StringType, LogLikelihood<StringType>>::
Evaluate(const std::vector<size_t>& ngram, NGramCounter<StringType>& counter)
{
  return LogLikelihood<StringType>::Evaluate(ngram, vector<size_t>(), counter);
}

}
}

#endif
