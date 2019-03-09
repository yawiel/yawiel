#ifndef YAWIEL_CORE_TEXT_CORPUS_HPP
#define YAWIEL_CORE_TEXT_CORPUS_HPP

#include <yawiel/prereqs.hpp>

namespace yawiel{
namespace text{

template<typename StringType = std::string>
class Corpus
{
 private:
  StringType* corpus;

 public:
  Corpus(StringType corpus);

  ~Corpus();
};

}
}

#include "corpus_impl.cpp"

#endif
