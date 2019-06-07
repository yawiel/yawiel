#ifndef YAWIEL_CORE_TEXT_CORPUS_HPP
#define YAWIEL_CORE_TEXT_CORPUS_HPP

#include <yawiel/prereqs.hpp>
#include <yawiel/core/containers/vocabulary.hpp>
#include <vector>

using namespace yawiel;

namespace yawiel{
namespace text{

template<typename StringType = std::wstring>
class Corpus
{
 private:
  containers::Vocabulary<StringType>* vocabulary;

  std::vector<size_t>* corpusTokens;

  StringType separators;

  void TokenizeLine(StringType& text,
                    std::vector<StringType>& tokensStrings);

 public:
  typedef std::pair<size_t, size_t> NGram;

  Corpus();

  ~Corpus();

  void loadFile(const std::string& filePath);

  void loadString(StringType text);

  void SetSeparators(StringType newSeparators);

  size_t totalNGrams(const size_t n) const;

  const std::vector<size_t>& GetTokens() const { return *corpusTokens; };

  const containers::Vocabulary<StringType>& GetVocabulary() const
      { return *vocabulary; }

  template<typename T = StringType>
  T GetDefaultSeparators(const typename std::enable_if<
                         std::is_same<T, std::string>::value>::type* = 0)
  { return " :!?=,.()[]\"/_-\'*+#"; }

  template<typename T = StringType>
  T GetDefaultSeparators(const typename std::enable_if<
                         std::is_same<T, std::wstring>::value>::type* = 0)
  { return L" :!?=,.()[]\"/_-\'*+#"; }

  template<typename T = StringType>
  T GetDefaultSeparators(const typename std::enable_if<
                         std::is_same<T, std::u16string>::value>::type* = 0)
  { return u" :!?=,.()[]\"/_-\'*+#"; }

  template<typename T = StringType>
  T GetDefaultSeparators(const typename std::enable_if<
                         std::is_same<T, std::u32string>::value>::type* = 0)
  { return U" :!?=,.()[]\"/_-\'*+#"; }

  template<typename Archive>
  void serialize(Archive& ar, const unsigned int /* version */);
};

}
}

#include "corpus_impl.cpp"

#endif
