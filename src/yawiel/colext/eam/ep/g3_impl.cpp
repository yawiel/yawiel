#ifndef YAWIEL_COLEXT_EAM_EP_G3_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G3_IMPL_CPP

#include "g3.hpp"
#include <yawiel/prereqs.hpp>
#include <vector>

using namespace std;

namespace yawiel{
namespace colext{

template<typename StringType, typename AMType>
double G3<StringType, AMType>::Evaluate(const std::vector<size_t>& ngram,
                                        NGramCounter<StringType>& counter)
{
  const size_t n = ngram.size();
  double sum = 0;

  for (size_t i = 0; i < n - 1; ++i)
  {
    const vector<size_t> splitFirst {ngram[i]};
    const vector<size_t> splitSecond {ngram[i + 1]};
    sum += AMType::Evaluate(splitFirst, splitSecond, counter);
  }

  return sum / (n - 1);
}

}
}

#endif
