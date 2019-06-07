#ifndef YAWIEL_COLEXT_EAM_AM_PMI_IMPL_CPP
#define YAWIEL_COLEXT_EAM_AM_PMI_IMPL_CPP

#include "pmi.hpp"
#include <yawiel/prereqs.hpp>
#include <iostream>

namespace yawiel{
namespace colext{

template<typename StringType>
double PMI<StringType>::Evaluate(const vector<size_t>& ngram1,
                                 const vector<size_t>& ngram2,
                                 NGramCounter<StringType>& counter)
{
  const size_t sizeNGram1 = ngram1.size();
  const size_t sizeNGram2 = ngram2.size();
  const size_t sizeNGram = sizeNGram1 + sizeNGram2;
  const size_t totalN1 = counter.getNumberOfNGrams(sizeNGram1);
  const size_t totalN2 = counter.getNumberOfNGrams(sizeNGram2);
  const size_t totalN = counter.getNumberOfNGrams(sizeNGram);

  vector<size_t> bothGrams;
  bothGrams.reserve(sizeNGram);
  bothGrams.insert(bothGrams.end(), ngram1.begin(), ngram1.end());
  bothGrams.insert(bothGrams.end(), ngram2.begin(), ngram2.end());

  const size_t counts1 = counter.getCounts(ngram1);
  const size_t counts2 = counter.getCounts(ngram2);
  const size_t countsBoth = counter.getCounts(bothGrams);

  const double prob1 = (double) counts1 / totalN1;
  const double prob2 = (double) counts2 / totalN2;
  const double probBoth = (double) countsBoth / totalN;

  return std::log2(probBoth / (prob1 * prob2));
}

}
}

#endif
