#ifndef YAWIEL_COLEXT_EAM_EP_G2_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G2_IMPL_CPP

#include "g2.hpp"
#include <yawiel/prereqs.hpp>
#include <vector>

using namespace std;

namespace yawiel{
namespace colext{

template<typename StringType, typename AMType>
double G2<StringType, AMType>::Evaluate(const std::vector<size_t>& ngram,
                                        NGramCounter<StringType>& counter)
{
  const size_t splitIndex = ngram.size() / 2;
  const vector<size_t> firstSplitFirst(ngram.begin(), ngram.begin() + splitIndex);
  const vector<size_t> firstSplitSecond(ngram.begin() + splitIndex, ngram.end());
  const vector<size_t> secondSplitFirst(ngram.begin(), ngram.begin() + splitIndex + 1);
  const vector<size_t> secondSplitSecond(ngram.begin() + splitIndex + 1, ngram.end());

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
