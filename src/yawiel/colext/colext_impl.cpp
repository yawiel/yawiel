#ifndef YAWIEL_COLEXT_IMPL_CPP
#define YAWIEL_COLEXT_IMPL_CPP

#include "colext.hpp"
#include <yawiel/prereqs.hpp>
#include <omp.h>

using namespace std;
using namespace yawiel::util;
using namespace yawiel::text;

namespace yawiel{
namespace colext{

template<typename StringType,
         typename EPType,
         typename AMType>
Colext<StringType, EPType, AMType>::
Colext(const Corpus<StringType>& corpus, NGramCounter<StringType>& counter) :
  corpus(corpus),
  counter(counter)
{
  // Nothing to do here.
}

template<typename StringType,
         typename EPType,
         typename AMType>
void Colext<StringType, EPType, AMType>::
GetScores(std::unordered_map<std::vector<size_t>, double>& scores,
          const size_t n)
{
  const unordered_map<std::vector<size_t>, size_t>& counts = counter.GetCounts(n);
  for (auto it = counts.begin(); it != counts.end(); ++it)
    scores[it->first] = EPType::Evaluate(it->first, counter);
}

template<typename StringType,
         typename EPType,
         typename AMType>
void Colext<StringType, EPType, AMType>::
GetSortedScores(vector<pair<vector<size_t>, double>>& scores, const size_t n)
{
  const unordered_map<std::vector<size_t>, size_t>& counts = counter.GetCounts(n);
  scores.clear();
  scores.reserve(counts.size());
  scores.resize(counts.size());
  // Add ngrams to scores.
  size_t i = 0;
  for ( auto kv : counts)
  {
    scores[i].first = kv.first;
    ++i;
  }

  // Compute scores.
  #pragma omp parallel for
  for (size_t i = 0; i < scores.size(); ++i)
    scores[i].second = EPType::Evaluate(scores[i].first, counter);

  // Sort results
  std::sort(scores.begin(), scores.end(), SortPred());
}

template<typename StringType,
         typename EPType,
         typename AMType>
void Colext<StringType, EPType, AMType>::
ScoresToCSV(const vector<pair<vector<size_t>, double>>& scores,
            const std::string filePath,
            const typename StringType::value_type CSVSeparator)
{
  std::basic_ofstream<typename StringType::value_type> file(filePath);
  file << "ngram,score,frequency" << std::endl;
  for (size_t i = 0; i < scores.size(); ++i)
  {
    // Write string to file.
    file << QUOTE << corpus.VectorToString(scores[i].first, STRING_SEPARATOR)
         << QUOTE;
    // Write score.
    file << CSVSeparator << scores[i].second;
    // Write frequency.
    file << CSVSeparator << counter.GetCounts(scores[i].first)
         << std::endl;
  }
  file.close();
}

}
}

#endif
