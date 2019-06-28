#ifndef YAWIEL_COLEXT_HPP
#define YAWIEL_COLEXT_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/text/corpus.hpp>
#include <yawiel/core/util/ngram_counter.hpp>

namespace yawiel{
namespace colext{

template<typename EPType, typename StringType = std::string>
class Colext
{
 private:
  // Default strings.
  static const typename StringType::value_type QUOTE = '\"';

  static const typename StringType::value_type STRING_SEPARATOR = ' ';

  static const typename StringType::value_type CSV_SEPARATOR = ',';

  //! Comparator for sorting result scores.
  struct SortScoresPred
  {
    inline bool operator()(const std::pair<std::vector<size_t>, double>& left,
                           const std::pair<std::vector<size_t>, double>& right)
    {
        return left.second < right.second;
    }
  };

  /**
   * Precompute counts needed for the EP, AM and n-gram size.
   *
   * @param n Size of n-gram collocations that we want to extract.
   */
  void Precompute(const size_t n);

  /**
   * Normalize scores.
   *
   * This method uses parallelization when available.
   *
   * @param scores Sorted vector of scores to normalize.
   */
  void NormalizeScores(
      std::vector<std::pair<std::vector<size_t>, double>>& scores);

  //! Corpus to extract collocations from.
  text::Corpus<StringType>* corpus;

  //! NGrams counter for the given corpus.
  util::NGramCounter<StringType>* counter;

  //! If true, the Colext model will be responsible for deleting the corpus.
  bool ownCorpus;

  //! If true, the Colext model will be responsible for deleting the counter.
  bool ownCounter;

  //! Association measure of the model.
  typename EPType::EPAMType am;

  //! Extension pattern of the model.
  EPType ep;

 public:
  //! Default constructor. After its use, it is needed to load a corpus.
  Colext();

  /**
   * Construct Collext from an existing corpus and its counter.
   *
   * @param corpus Corpus to extract collocations from.
   * @param counter Counter of ngrams in the corpus.
   */
  Colext(text::Corpus<StringType>* corpus,
         util::NGramCounter<StringType>* counter);

  //! Destructor.
  ~Colext();

  /**
   * Load corpus form a text file.
   *
   * @param filePath Path to the text file with the corpus to extract
   *                 collocations from.
   * @post Corpus will be loaded.
   */
  void LoadCorpusFile(const std::string& filePath);

  /**
   * Compute scores for a given size, EP and AM for n-grams in the corpus.
   *
   * This method CANNOT use paralellization.
   *
   * @pre Corpus has te be loaded.
   * @param scores Hash table where scores from ngrams will be stored.
   * @param n Size of the n-grams for which scores will be computed.
   * @param normalizeScore Whether to normalize scores or not.
   */
  void GetScores(std::unordered_map<std::vector<size_t>, double>& scores,
                 const size_t n,
                 bool normalizeScore = false);

  /**
   * Compute scores for a given size, EP and AM for n-grams in the corpus.
   *
   * This method can use paralellization when available.
   *
   * @pre Corpus has te be loaded.
   * @param scores Vector where sorted scores from ngrams will be stored.
   * @param n Size of the n-grams for which scores will be computed.
   * @param normalizeScore Whether to normalize scores or not.
   */
  void GetSortedScores(
      std::vector<std::pair<std::vector<size_t>, double>>& scores,
      const size_t n,
      bool normalizeScore = false);

  /**
   * Write computed scores to a CSV file.
   *
   * @param scores Iterable pair (i.e. pair<vector<size_t>, double>) to save in
   *               a file.
   * @param filePath Path where the CSV file will be writen.
   * @param csvSeparator Separator for column values of the CSV file.
   */
  template<typename IterablePairType>
  void ScoresToCSV(
      const IterablePairType& scores,
      const std::string filePath,
      const typename StringType::value_type csvSeparator = CSV_SEPARATOR) const;

  //! Whether a corpus has been loaded or not.
  bool CorpusLoaded() const { return corpus->Loaded(); }
};

}
}

#include "colext_impl.cpp"

#endif
