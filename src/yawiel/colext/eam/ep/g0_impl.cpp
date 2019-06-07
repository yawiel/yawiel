#ifndef YAWIEL_COLEXT_EAM_EP_G0_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G0_IMPL_CPP

#include "g0.hpp"
#include <yawiel/prereqs.hpp>
#include <vector>

using namespace std;

namespace yawiel{
namespace colext{

template<typename StringType>
double G0<StringType, PMI<StringType>>::
Evaluate(const std::vector<size_t>& ngram, NGramCounter<StringType>& counter)
{
  const size_t totalN = counter.getNumberOfNGrams(ngram.size());
  const double probNGram = (double) counter.getCounts(ngram) / totalN;

  const size_t total1 = counter.getNumberOfNGrams(1);
  double mult = 1;
  for (size_t i = 0; i < ngram.size(); ++i)
    mult *= (double) counter.getCounts(std::vector<size_t> {ngram[i]}) / total1;

  return std::log2(probNGram / mult);
}

template<typename StringType>
double G0<StringType, Dice<StringType>>::
Evaluate(const std::vector<size_t>& ngram,  NGramCounter<StringType>& counter)
{
  const size_t n = ngram.size();

  size_t sum = 0;
  for (size_t i = 0; i < n; ++i)
    sum += counter.getCounts(std::vector<size_t> {ngram[i]});

  return (double) (n * counter.getCounts(ngram)) / sum;
}

template<typename StringType>
double G0<StringType, ChiSquared<StringType>>::
Evaluate(const std::vector<size_t>& ngram,  NGramCounter<StringType>& counter)
{
  return ChiSquared<StringType>::Evaluate(ngram, vector<size_t>(), counter);
}

template<typename StringType>
double G0<StringType, LogLikelihood<StringType>>::
Evaluate(const std::vector<size_t>& ngram,  NGramCounter<StringType>& counter)
{
  return LogLikelihood<StringType>::Evaluate(ngram, vector<size_t>(), counter);
}

}
}

#endif
