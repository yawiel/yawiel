#include <yawiel/core/text/corpus.hpp>
#include <boost/test/unit_test.hpp>

using namespace yawiel;

BOOST_AUTO_TEST_SUITE(CorpusTest);

BOOST_AUTO_TEST_CASE(IntantiationTest)
{
  yawiel::text::Corpus<> corpus(std::string("Hi! :)"));
}

BOOST_AUTO_TEST_SUITE_END();
