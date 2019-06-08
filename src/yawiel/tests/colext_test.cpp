#include <yawiel/colext/colext.hpp>
#include <yawiel/colext/eam/am/pmi.hpp>
#include <yawiel/colext/eam/am/dice.hpp>
#include <yawiel/colext/eam/am/chi_squared.hpp>
#include <yawiel/colext/eam/am/log_likelihood.hpp>
#include <yawiel/colext/eam/ep/g0.hpp>
#include <yawiel/colext/eam/ep/g1.hpp>
#include <yawiel/colext/eam/ep/g2.hpp>
#include <yawiel/colext/eam/ep/g3.hpp>
#include <yawiel/colext/eam/ep/g4.hpp>
#include <yawiel/colext/eam/ep/g5.hpp>
#include <boost/test/unit_test.hpp>
#include <unordered_map>
#include <map>

using namespace std;
using namespace yawiel;
using namespace yawiel::text;
using namespace yawiel::util;
using namespace yawiel::colext;

BOOST_AUTO_TEST_SUITE(ColextTest);
/*
BOOST_AUTO_TEST_CASE(BasicTest)
{
  //const std::string text = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";
  //Corpus<std::string> corpus(text);
  Corpus<std::string> corpus;
  std::cout << "Loading file..." << std::endl;
  corpus.loadFile("big.txt");
  std::cout << "File loaded..." << std::endl;
  Colext<std::string, G1<std::string, PMI<std::string>>, PMI<std::string>>
    colext(corpus);
  std::unordered_map<std::string, double> scores; 
  colext.getScores(scores, 3);
  
  for (auto kv : scores)
  {
    std::cout << kv.first << ": " << kv.second << std::endl;
  }
  
}
*/
void PrintExtremes(const vector<pair<vector<size_t>, double>>& scores,
                   const Corpus<std::string>& corpus,
                   size_t n = 150)
{
  std::cout << std::endl << "WORST:" << std::endl;
  for (size_t i = 0; i < n; ++i)
  {
    for (auto i : scores[i].first)
      std::cout << (corpus.GetVocabulary()).at(i) << " ";
    std::cout <<" :: " << scores[i].second << std::endl;
  }

  std::cout << std::endl << "BEST:" << std::endl;
  for (size_t i = scores.size() - n; i < scores.size(); ++i)
  {
    for (auto i : scores[i].first)
      std::cout << (corpus.GetVocabulary()).at(i) << " ";
    std::cout <<" :: " << scores[i].second << std::endl;
  }
}

template<typename StringType,
         typename EP,
         typename AM>
void EvalColext(Corpus<StringType>& corpus,
                NGramCounter<StringType>& counter,
                size_t n)
{
  vector<pair<vector<size_t>, double>> scores;
  Colext<StringType, EP, AM> colext(corpus,counter);
  colext.GetSortedScores(scores, n);
  PrintExtremes(scores, corpus);
  scores.clear();
}

BOOST_AUTO_TEST_CASE(SortedScoresTest)
{
  Corpus<std::string> corpus;
  std::cout << "Loading file..." << std::endl;
  corpus.loadFile("big.txt");
  std::cout << "File loaded..." << std::endl;
  NGramCounter<std::string> counter(corpus);
  vector<pair<vector<size_t>, double>> scores;

  // Some tests.
  std::cout << "--- G0 - LogLike - n3 ------" << std::endl;
  EvalColext<string, G0<string, LogLikelihood<string>>, LogLikelihood<string>>
    (corpus, counter, 3);

  std::cout << "--- G0 - ChiSquared - n3 ---" << std::endl;
  EvalColext<string, G0<string, ChiSquared<string>>, ChiSquared<string>>
    (corpus, counter, 3);

  std::cout << "--- G1 - PMI - n3 ----------" << std::endl;
  EvalColext<string, G1<string, PMI<string>>, PMI<string>>
    (corpus, counter, 3);

  std::cout << "--- G2 - Dice - n3 ---------" << std::endl;
  EvalColext<string, G2<string, Dice<string>>, Dice<string>>
    (corpus, counter, 3);

  std::cout << "--- G4 - Dice - n2 ---------" << std::endl;
  EvalColext<string, G4<string, Dice<string>>, Dice<string>>
    (corpus, counter, 2);

  std::cout << "--- G5 - PMI - n2 ----------" << std::endl;
  EvalColext<string, G4<string, PMI<string>>, PMI<string>>
    (corpus, counter, 2);
}

BOOST_AUTO_TEST_SUITE_END();