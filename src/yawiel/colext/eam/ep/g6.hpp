#ifndef YAWIEL_COLEXT_EAM_EP_G6_HPP
#define YAWIEL_COLEXT_EAM_EP_G6_HPP

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
class G6
{
 public:
  // Throw error in compile time if G6 is not defined for selected AM.
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter) = delete;
};

template<typename StringType>
class G6<StringType, PMI<StringType>>
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

template<typename StringType>
class G6<StringType, Dice<StringType>>
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

template<typename StringType>
class G6<StringType, ChiSquared<StringType>>
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

template<typename StringType>
class G6<StringType, LogLikelihood<StringType>>
{
 public:
  static double Evaluate(const std::vector<size_t>& ngram,
                         NGramCounter<StringType>& counter);
};

}
}

#include "g6_impl.cpp"

#endif
