#ifndef YAWIEL_COLEXT_EAM_EP_G1_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G1_IMPL_CPP

#include "g1.hpp"
#include <yawiel/prereqs.hpp>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>

using namespace std;

namespace yawiel{
namespace colext{

template<typename StringType, typename AMType>
double G1<StringType, AMType>::Evaluate(const std::vector<size_t>& ngram,
                                        NGramCounter<StringType>& counter)
{
  const vector<size_t> firstSplitFirst{ngram[0]};
  const vector<size_t> firstSplitSecond(ngram.begin() + 1, ngram.end());
  const vector<size_t> secondSplitFirst(ngram.begin(), ngram.end() - 1);
  const vector<size_t> secondSplitSecond{ngram[ngram.size() - 1]};

  const double am1 = AMType::Evaluate(firstSplitFirst,
                                      firstSplitSecond,
                                      counter);
  const double am2 = AMType::Evaluate(secondSplitFirst,
                                      secondSplitSecond,
                                      counter);
  return (am1 + am2) / 2.0;
}

}
}

#endif
