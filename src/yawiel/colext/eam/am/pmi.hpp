#ifndef YAWIEL_COLEXT_EAM_AM_PMI_HPP
#define YAWIEL_COLEXT_EAM_AM_PMI_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/util/ngram_counter.hpp>

using namespace std;
using namespace yawiel::util;

namespace yawiel{
namespace colext{

template<typename StringType>
class PMI
{
 public:
  static double Evaluate(const vector<size_t>& ngram1,
                         const vector<size_t>& ngram2,
                         NGramCounter<StringType>& counter);
};

}
}

#include "pmi_impl.cpp"

#endif
