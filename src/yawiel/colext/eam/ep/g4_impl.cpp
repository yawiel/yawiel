#ifndef YAWIEL_COLEXT_EAM_EP_G4_IMPL_CPP
#define YAWIEL_COLEXT_EAM_EP_G4_IMPL_CPP

#include "g4.hpp"
#include <yawiel/prereqs.hpp>
#include <vector>

using namespace std;

namespace yawiel{
namespace colext{

template<typename StringType, typename AMType>
double G4<StringType, AMType>::Evaluate(const std::vector<size_t>& ngram,
                                        NGramCounter<StringType>& counter)
{
  const vector<size_t> splitFirst(ngram.begin(), ngram.end() - 1);
  const vector<size_t> splitSecond(ngram.begin() + 1, ngram.end());

  return AMType::Evaluate(splitFirst, splitSecond, counter);
}

}
}

#endif
