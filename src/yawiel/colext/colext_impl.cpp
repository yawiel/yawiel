#ifndef YAWIEL_COLEXT_IMPL_CPP
#define YAWIEL_COLEXT_IMPL_CPP

#include "colext.hpp"
#include <yawiel/prereqs.hpp>
#include <omp.h>

using namespace yawiel::util;

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
  std::time_t now = time(0);
  std::cout << "START COMPUTING: " << now << std::endl;
  const unordered_map<std::vector<size_t>, size_t>& counts = counter.getCounts(n);
  for (auto it = counts.begin(); it != counts.end(); ++it)
    scores[it->first] = EPType::Evaluate(it->first, counter);
  now = time(0);
  std::cout << "FINISH COMPUTING: " << now << std::endl;
}

template<typename StringType,
         typename EPType,
         typename AMType>
void Colext<StringType, EPType, AMType>::
GetSortedScores(vector<pair<vector<size_t>, double>>& scores, const size_t n)
{
  std::time_t now = time(0);
  std::cout << "START COMPUTING: " << now << std::endl;

  const unordered_map<std::vector<size_t>, size_t>& counts = counter.getCounts(n);
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
  //counter.getCounts(4);
  counter.getCounts(3);
  counter.getCounts(2);
  counter.getCounts(1);
  #pragma omp parallel for
  for (size_t i = 0; i < scores.size(); ++i)
    scores[i].second = EPType::Evaluate(scores[i].first, counter);

  // Sort results
  std::sort(scores.begin(), scores.end(), SortPred());

  now = time(0);
  std::cout << "FINISH COMPUTING: " << now << std::endl;
}

}
}

#endif
