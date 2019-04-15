#include <yawiel/core/text/corpus.hpp>
#include <yawiel/core/text/gram_left_to_right_traverser.hpp>
#include <boost/test/unit_test.hpp>

using namespace yawiel;
using namespace yawiel::text;

BOOST_AUTO_TEST_SUITE(CorpusTest);

template<typename StringType>
class TestRule
{
 private:
  std::vector<StringType> ngrams;

  //const std::vector<StringType>& tokens;

 public:
  //TestRule(const std::vector<StringType>& tokens) :
  //  tokens(tokens)
  //{}
  void Base(const typename Corpus<StringType>::NGram ngram)
  {
    StringType str;
    for (auto it = ngram.first; it != ngram.second; ++it)
      str += *it + "-";
    ngrams.push_back(str);
  }

  std::vector<StringType> getNGrams(){ return ngrams; }
};

BOOST_AUTO_TEST_CASE(IntantiationTest)
{
  yawiel::text::Corpus<> corpus(std::string("Hi my ou is my name and ou is my name surname"));
}

BOOST_AUTO_TEST_CASE(GramLeftToRightTest)
{
  const std::string text = "Hi my name is Yawiel";
  Corpus<> corpus(text);
  TestRule<std::string> rules;
  typedef GramLeftToRightTraverser<TestRule<std::string>, std::string> TRV;
  TRV traverser(rules, corpus);
  traverser.Traverse(2);
  std::vector<std::string> ngrams = std::move(rules.getNGrams());
  for (auto i: ngrams)
    break; // std::cout << i << std::endl;
}

BOOST_AUTO_TEST_CASE(FileLoadTest)
{
  yawiel::text::Corpus<> corpus;
  corpus.loadFile("SmallText.txt");
  for (auto token: corpus.GetTokens())
    //std::cout << token << std::endl;
    break;
}

BOOST_AUTO_TEST_SUITE_END();
