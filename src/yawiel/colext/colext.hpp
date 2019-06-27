#ifndef YAWIEL_COLEXT_HPP
#define YAWIEL_COLEXT_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/text/corpus.hpp>
#include <yawiel/core/util/ngram_counter.hpp>

namespace yawiel{
namespace colext{

template<typename StringType,
         typename EPType,
         typename AMType>
class Colext
{
 private:
  // Default strings.
  static const typename StringType::value_type QUOTE = '\"';

  static const typename StringType::value_type STRING_SEPARATOR = ' ';

  static const typename StringType::value_type CSV_SEPARATOR = ',';

  // Comparator for sorting result scores.
  struct SortPred
  {
    inline bool operator()(const std::pair<std::vector<size_t>, double>& left,
                           const std::pair<std::vector<size_t>, double>& right)
    {
        return left.second < right.second;
    }
  };

  const text::Corpus<StringType>& corpus;

  util::NGramCounter<StringType>& counter;

 public:
  Colext(const text::Corpus<StringType>& corpus,
         util::NGramCounter<StringType>& counter);

  void GetScores(std::unordered_map<std::vector<size_t>, double>& scores,
                 const size_t n = 3);

  void GetSortedScores(
      std::vector<std::pair<std::vector<size_t>, double>>& scores,
      const size_t n = 3);

  void ScoresToCSV(
      const std::vector<std::pair<std::vector<size_t>, double>>& scores,
      const std::string filePath,
      const typename StringType::value_type CSVSeparator = CSV_SEPARATOR);
};

}
}

#include "colext_impl.cpp"

#endif
