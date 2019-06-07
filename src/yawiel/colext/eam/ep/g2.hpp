#ifndef YAWIEL_COLEXT_EAM_EP_G2_HPP
#define YAWIEL_COLEXT_EAM_EP_G2_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/util/ngram_counter.hpp>

using namespace std;
using namespace yawiel::util;

namespace yawiel{
namespace colext{

template<typename StringType, typename AMType>
class G2
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

}
}

#include "g2_impl.cpp"

#endif
