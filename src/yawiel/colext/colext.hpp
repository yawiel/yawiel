#ifndef YAWIEL_COLEXT_HPP
#define YAWIEL_COLEXT_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/text/corpus.hpp>
#include <yawiel/core/util/ngram_counter.hpp>

using namespace std;
using namespace yawiel::text;
using namespace yawiel::util;

namespace yawiel{
namespace colext{

template<typename StringType,
         typename EPType,
         typename AMType>
class Colext
{
 private:
  // Comparator for sorting result scores.
  struct SortPred
  {
    inline bool operator()(const std::pair<std::vector<size_t>, double>& left,
                           const std::pair<std::vector<size_t>, double>& right)
    {
        return left.second < right.second;
    }
  };

  const Corpus<StringType>& corpus;

  NGramCounter<StringType>& counter;

 public:
  Colext(const Corpus<StringType>& corpus, NGramCounter<StringType>& counter);

  void GetScores(unordered_map<vector<size_t>, double>& scores,
                 const size_t n = 3);

  void GetSortedScores(vector<pair<vector<size_t>, double>>& scores,
                       const size_t n = 3);
};

}
}

#include "colext_impl.cpp"

#endif
