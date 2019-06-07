#ifndef YAWIEL_COLEXT_EAM_EP_G5_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G5_IMPL_CPP

#include "g5.hpp"
#include <yawiel/prereqs.hpp>
#include <vector>

using namespace std;

namespace yawiel{
namespace colext{

template<typename StringType, typename AMType>
double G5<StringType, AMType>::Evaluate(const std::vector<size_t>& ngram,
                                        NGramCounter<StringType>& counter)
{
  const size_t n = ngram.size();
  double sum = 0;

  for (size_t i = 1; i < n; ++i)
  {
    const vector<size_t> splitFirst(ngram.begin(), ngram.begin() + i);
    const vector<size_t> splitSecond(ngram.begin() + i, ngram.end());
    sum += AMType::Evaluate(splitFirst, splitSecond, counter);
  }

  return sum / (n - 1);
}

}
}

#endif
