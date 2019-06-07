#ifndef YAWIEL_COLEXT_EAM_EP_G0_HPP
#define YAWIEL_COLEXT_EAM_EP_G0_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/util/ngram_counter.hpp>
#include <yawiel/colext/eam/am/pmi.hpp>
#include <yawiel/colext/eam/am/dice.hpp>
#include <yawiel/colext/eam/am/chi_squared.hpp>
#include <yawiel/colext/eam/am/log_likelihood.hpp>

using namespace std;
using namespace yawiel::util;

namespace yawiel{
namespace colext{

template<typename StringType, typename AMType>
class G0
{
 public:
  // Throw error in compile time if G0 is not defined for selected AM.
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter) = delete;
};

template<typename StringType>
class G0<StringType, PMI<StringType>>
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

template<typename StringType>
class G0<StringType, Dice<StringType>>
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

template<typename StringType>
class G0<StringType, ChiSquared<StringType>>
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

template<typename StringType>
class G0<StringType, LogLikelihood<StringType>>
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

}
}

#include "g0_impl.cpp"

#endif
