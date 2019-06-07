#include <yawiel/core/util/ngram_counter.hpp>
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
#include <yawiel/colext/eam/ep/g6.hpp>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace yawiel;
using namespace yawiel::text;
using namespace yawiel::util;
using namespace yawiel::colext;

BOOST_AUTO_TEST_SUITE(EAMTest);

const std::string text1 = "this is a test a test";

BOOST_AUTO_TEST_CASE(PMITest)
{
  Corpus<string> corpus;
  corpus.loadString(text1);
  NGramCounter<string> counter(corpus);

  vector<size_t> gram1 {0, 1};
  vector<size_t> gram2 {2};

  const double result = PMI<string>::Evaluate(gram1, gram2, counter);
  // Compare against a manually calculated PMI value.
  BOOST_REQUIRE_CLOSE(result, 1.90833401257, 0.1);
}

BOOST_AUTO_TEST_CASE(DiceTest)
{
  Corpus<string> corpus;
  corpus.loadString(text1);
  NGramCounter<string> counter(corpus);
  const auto& vocab = corpus.GetVocabulary();

  vector<size_t> gram1 {vocab.at("this"), vocab.at("is")};
  vector<size_t> gram2 {vocab.at("a")};

  const double result = Dice<string>::Evaluate(gram1, gram2, counter);
  // Compare against a manually calculated Dice value.
  BOOST_REQUIRE_CLOSE(result, 0.6666666666666, 0.1);
}

BOOST_AUTO_TEST_CASE(G1Test)
{
  Corpus<string> corpus;
  corpus.loadString(text1);
  NGramCounter<string> counter(corpus);
  vector<size_t> aTestA {2, 3, 2};
  const double G1Result =
    G1<string, PMI<string>>::Evaluate(aTestA, counter);
  // Manual result.
  vector<size_t> aTest {2, 3};
  vector<size_t> a {2};
  vector<size_t> testA {3, 2};
  const double PMIResult1 = PMI<string>::Evaluate(aTest, a, counter);
  const double PMIResult2 = PMI<string>::Evaluate(a, testA, counter);
  const double ManualResult = (PMIResult1 + PMIResult2) / 2.0;
  BOOST_REQUIRE_CLOSE(G1Result, ManualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G2Test)
{
  Corpus<string> corpus;
  corpus.loadString(text1);
  NGramCounter<string> counter(corpus);
  const auto& vocab = corpus.GetVocabulary();

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a")};

  const double g2Result = G2<string, Dice<string>>::Evaluate(gram, counter);

  // Manual result.
  vector<size_t> isA {gram[0], gram[1]};
  vector<size_t> testA {gram[2], gram[3]};
  vector<size_t> isATest {gram[0],gram[1], gram[2]};
  vector<size_t> a {gram[3]};
  const double diceResult1 = Dice<string>::Evaluate(isA, testA, counter);
  const double diceResult2 = Dice<string>::Evaluate(isATest, a, counter);
  const double manualResult = (diceResult1 + diceResult2) / 2.0;
  BOOST_REQUIRE_CLOSE(g2Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G3Test)
{
  Corpus<string> corpus;
  corpus.loadString(text1);
  NGramCounter<string> counter(corpus);
  const auto& vocab = corpus.GetVocabulary();

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a"),
                       vocab.at("test")};

  const double g3Result = G3<string, PMI<string>>::Evaluate(gram, counter);

  // Manual result.
  vector<size_t> is {gram[0]};
  vector<size_t> a {gram[1]};
  vector<size_t> test {gram[2]};

  const double pmiResult1 = PMI<string>::Evaluate(is, a, counter);
  const double pmiResult2 = PMI<string>::Evaluate(a, test, counter);
  const double pmiResult3 = PMI<string>::Evaluate(test, a, counter);
  const double pmiResult4 = PMI<string>::Evaluate(a, test, counter);
  const double manualResult =
      (pmiResult1 + pmiResult2 + pmiResult3 + pmiResult4) / (5 - 1);

  BOOST_REQUIRE_CLOSE(g3Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G4Test)
{
  Corpus<string> corpus;
  corpus.loadString(text1);
  NGramCounter<string> counter(corpus);
  const auto& vocab = corpus.GetVocabulary();

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a"),
                       vocab.at("test")};

  const double g4Result = G4<string, Dice<string>>::Evaluate(gram, counter);

  // Manual result.
  vector<size_t> isATestA {gram[0], gram[1], gram[2], gram[3]};
  vector<size_t> aTestATest {gram[1], gram[2], gram[3], gram[4]};

  const double manualResult = Dice<string>::Evaluate(isATestA,
                                                     aTestATest,
                                                     counter);

  BOOST_REQUIRE_CLOSE(g4Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G5Test)
{
  Corpus<string> corpus;
  corpus.loadString(text1);
  NGramCounter<string> counter(corpus);
  const auto& vocab = corpus.GetVocabulary();

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a")};

  const double g5Result = G5<string, Dice<string>>::Evaluate(gram, counter);

  // Manual result.
  vector<size_t> is {gram[0]};
  vector<size_t> aTestA {gram[1], gram[2], gram[3]};
  vector<size_t> isA {gram[0], gram[1]};
  vector<size_t> testA {gram[2], gram[3]};
  vector<size_t> isATest {gram[0], gram[1], gram[2]};
  vector<size_t> a {gram[3]};

  double manualResult = Dice<string>::Evaluate(is, aTestA, counter) +
                        Dice<string>::Evaluate(isA, testA, counter) +
                        Dice<string>::Evaluate(isATest, a, counter);
  manualResult /= 3;

  BOOST_REQUIRE_CLOSE(g5Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_CASE(G6DiceTest)
{
  Corpus<string> corpus;
  corpus.loadString(text1);
  NGramCounter<string> counter(corpus);
  const auto& vocab = corpus.GetVocabulary();

  vector<size_t> gram {vocab.at("is"),
                       vocab.at("a"),
                       vocab.at("test"),
                       vocab.at("a")};

  const double g6Result = G6<string, Dice<string>>::Evaluate(gram, counter);

  // Manual result.
  vector<size_t> isA {gram[0], gram[1]};
  vector<size_t> testA {gram[2], gram[3]};

  const size_t n = gram.size();
  const size_t countGram = counter.getCounts(gram);
  const size_t countBi = counter.getCounts(isA) + counter.getCounts(testA);
  const double manualResult = (n * countGram) / countBi;

  BOOST_REQUIRE_CLOSE(g6Result, manualResult, 1e-8);
}

BOOST_AUTO_TEST_SUITE_END();
