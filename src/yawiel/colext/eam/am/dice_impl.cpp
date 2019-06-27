#ifndef YAWIEL_COLEXT_EAM_AM_DICE_IMPL_CPP
#define YAWIEL_COLEXT_EAM_AM_DICE_IMPL_CPP

#include "dice.hpp"
#include <yawiel/prereqs.hpp>
#include <iostream>

namespace yawiel{
namespace colext{

template<typename StringType>
double Dice<StringType>::Evaluate(const vector<size_t>& ngram1,
                                  const vector<size_t>& ngram2,
                                  NGramCounter<StringType>& counter)
{
  const size_t sizeNGram1 = ngram1.size();
  const size_t sizeNGram2 = ngram2.size();
  const size_t sizeNGram = sizeNGram1 + sizeNGram2;

  vector<size_t> bothGrams;
  bothGrams.reserve(sizeNGram);
  bothGrams.insert(bothGrams.end(), ngram1.begin(), ngram1.end());
  bothGrams.insert(bothGrams.end(), ngram2.begin(), ngram2.end());

  const size_t frec1 = counter.GetCounts(ngram1);
  const size_t frec2 = counter.GetCounts(ngram2);
  const size_t frecBoth = counter.GetCounts(bothGrams);

  return ((double) (2 * frecBoth)) / (frec1 + frec2);
}

}
}

#endif
