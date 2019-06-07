#ifndef YAWIEL_CORE_TEXT_CORPUS_GRAM_LEFT_TO_RIGHT_TRAVERSER_HPP
#define YAWIEL_CORE_TEXT_CORPUS_GRAM_LEFT_TO_RIGHT_TRAVERSER_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/text/corpus.hpp>

using namespace yawiel::text;

namespace yawiel{
namespace text{

template<typename RuleType, typename StringType>
class GramLeftToRightTraverser
{
 public:
  GramLeftToRightTraverser(RuleType& rule, const Corpus<StringType>& corpus);

  void Traverse(const size_t n = 2,
                const size_t startIndex = 0,
                const size_t endIndex = 0);

 private:
  RuleType& rule;

  const Corpus<StringType>& corpus;
};

}
}

#include "gram_left_to_right_traverser_impl.cpp"

#endif
