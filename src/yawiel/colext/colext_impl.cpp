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

template<typename EPType, typename StringType>
Colext<EPType, StringType>::Colext() :
    corpus(new Corpus<StringType>),
    counter(new NGramCounter<StringType>(corpus)),
    ownCorpus(true),
    ownCounter(true),
    am(EPType::EPAMType(counter)),
    ep(EPType(counter, am))
{
  // Nothing to do here.
}

template<typename EPType, typename StringType>
Colext<EPType, StringType>::
Colext(Corpus<StringType>* corpus, NGramCounter<StringType>* counter) :
  corpus(corpus),
  counter(counter),
  ownCorpus(false),
  ownCounter(false),
  am(EPType::EPAMType(counter)),
  ep(EPType(counter, am))
{
  // Nothing to do here.
}

template<typename EPType, typename StringType>
Colext<EPType, StringType>::~Colext() :
{
  if (ownCorpus)
    delete corpus;
  if (ownCounter)
    delete counter;
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::LoadCorpusFile(const std::string& filePath)
{
  if (ownCorpus)
    corpus->LoadFile(filePath);
  else
    throw std::runtime_error("Corpus not owned by Collext: Text must be loaded "
                             "directly into the corpus object");
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::
GetScores(unordered_map<vector<size_t>, double>& scores,
          const size_t n,
          bool normalizeScore)
{
  const unordered_map<std::vector<size_t>, size_t>& counts = counter.GetCounts(n);
  for (auto it = counts.begin(); it != counts.end(); ++it)
    scores[it->first] = EPType::Evaluate(it->first, counter);
}

template<typename EPType, typename StringType>
void Colext<EPType, StringType>::
GetSortedScores(vector<pair<vector<size_t>, double>>& scores,
                const size_t n,
                bool normalizeScore)
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
  std::sort(scores.begin(), scores.end(), SortScoresPred());
}

template<typename EPType, typename StringType>
template<typename IterablePairType>
void Colext<EPType, StringType>::
ScoresToCSV(const IterablePairType& scores,
            const std::string filePath,
            const typename StringType::value_type CSVSeparator)
{
  std::basic_ofstream<typename StringType::value_type> file(filePath);
  file << "ngram,score,frequency" << std::endl;
  for (const auto it = scores.cbegin(); it != scores.cend(); ++it)
  {
    // Write string to file.
    file << QUOTE << corpus.VectorToString(it->first, STRING_SEPARATOR)
         << QUOTE;
    // Write score.
    file << CSVSeparator << it->second;
    // Write frequency.
    file << CSVSeparator << counter.GetCounts(it->first)
         << std::endl;
  }
  file.close();
}

}
}

#endif
