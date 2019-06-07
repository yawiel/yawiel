#ifndef YAWIEL_CORE_TEXT_CORPUS_GRAM_LEFT_TO_RIGHT_TRAVERSER_IMPL_CPP
#define YAWIEL_CORE_TEXT_CORPUS_GRAM_LEFT_TO_RIGHT_TRAVERSER_IMPL_CPP

#include "gram_left_to_right_traverser.hpp"

namespace yawiel{
namespace text{

template<typename RuleType, typename StringType>
GramLeftToRightTraverser<RuleType, StringType>::
GramLeftToRightTraverser(RuleType& rule, const Corpus<StringType>& corpus) :
  rule(rule),
  corpus(corpus)
{}

template<typename RuleType, typename StringType>
void GramLeftToRightTraverser<RuleType, StringType>::
Traverse(const size_t n, const size_t startIndex, const size_t endIndex)
{
  size_t lastIndex;
  if (endIndex != 0)
    lastIndex = endIndex;
  else
    lastIndex = (corpus.GetTokens()).size();

  size_t firstToken = startIndex;
  size_t lastToken = firstToken + n;
  for (; lastToken <= lastIndex; ++firstToken, ++lastToken)
    rule.Base(std::make_pair(firstToken, lastToken));
}

}
}

#endif
