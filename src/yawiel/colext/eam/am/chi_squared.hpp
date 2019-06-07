#ifndef YAWIEL_COLEXT_EAM_AM_CHI_SQUARED_HPP
#define YAWIEL_COLEXT_EAM_AM_CHI_SQUARED_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/util/ngram_counter.hpp>

using namespace std;
using namespace yawiel::util;

namespace yawiel{
namespace colext{

template<typename StringType>
class ChiSquared
{
 private:
  static double EvaluateBinary(const vector<size_t>& ngram1,
                               const vector<size_t>& ngram2,
                               NGramCounter<StringType>& counter);
 public:
  static double Evaluate(const vector<size_t>& ngram1,
                         const vector<size_t>& ngram2,
                         NGramCounter<StringType>& counter);
};

}
}

#include "chi_squared_impl.cpp"

#endif
