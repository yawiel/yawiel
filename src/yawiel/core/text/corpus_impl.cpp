#ifndef YAWIEL_CORE_TEXT_CORPUS_IMPL_CPP
#define YAWIEL_CORE_TEXT_CORPUS_IMPL_CPP

#include "corpus.hpp"

namespace yawiel{
namespace text{

template<typename StringType>
Corpus<StringType>::Corpus(StringType corpus):
  corpus(new StringType(std::move(corpus)))
{ }

template<typename StringType>
Corpus<StringType>::~Corpus()
{
  if (corpus)
    delete corpus;
}

}
}

#include "corpus_impl.cpp"

#endif
