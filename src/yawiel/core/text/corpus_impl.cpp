#ifndef YAWIEL_CORE_TEXT_CORPUS_IMPL_CPP
#define YAWIEL_CORE_TEXT_CORPUS_IMPL_CPP

#include "corpus.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>

#include <ctime>
#include <iostream>

using namespace yawiel;

namespace yawiel{
namespace text{

template<typename StringType>
Corpus<StringType>::Corpus():
  vocabulary(nullptr),
  corpusTokens(nullptr)
{
  SetSeparators(GetDefaultSeparators());
}

template<typename StringType>
void Corpus<StringType>::loadFile(const std::string& filePath)
{
  std::time_t now = time(0);
  std::cout << "LOAD FILE: " << now << std::endl;
  if (vocabulary)
    delete vocabulary;
  if (corpusTokens)
    delete corpusTokens;

  vocabulary = new containers::Vocabulary<StringType>;
  corpusTokens = new std::vector<size_t>;

  std::basic_ifstream<typename StringType::value_type> file(filePath);
  StringType tempString;
  std::vector<StringType> tokens;
  while (std::getline<typename StringType::value_type>(file, tempString))
  {
    TokenizeLine(tempString, tokens);
    // Add new tokens to vocabulary.
    for (auto token = tokens.begin(); token != tokens.end(); ++token)
      vocabulary->AddToken(*token);
    // Add new tokens to the corpus.
    for (auto token = tokens.cbegin(); token != tokens.cend(); ++token)
      corpusTokens->push_back(vocabulary->at(*token));
  }

  now = time(0);
  std::cout << "FINISHED LOADING: " << now << std::endl;
}

template<typename StringType>
void Corpus<StringType>::loadString(StringType text)
{
  if (vocabulary)
    delete vocabulary;
  if (corpusTokens)
    delete corpusTokens;

  vocabulary = new containers::Vocabulary<StringType>;
  corpusTokens = new std::vector<size_t>;

  std::vector<StringType> tokens;

  TokenizeLine(text, tokens);
  // Add new tokens to vocabulary.
  for (auto token = tokens.begin(); token != tokens.end(); ++token)
    vocabulary->AddToken(*token);
  // Add new tokens to the corpus.
  for (auto token = tokens.cbegin(); token != tokens.cend(); ++token)
    corpusTokens->push_back(vocabulary->at(*token));
}

template<typename StringType>
Corpus<StringType>::~Corpus()
{
  if (vocabulary)
    delete vocabulary;
  if (corpusTokens)
    delete corpusTokens;
}

template<typename StringType>
void Corpus<StringType>::
SetSeparators(StringType newSeparators)
{
  separators = std::move(newSeparators);
}

template<typename StringType>
size_t Corpus<StringType>::totalNGrams(const size_t n) const
{
  return corpusTokens->size() - n + 1;
}

template<typename StringType>
void Corpus<StringType>::TokenizeLine(StringType& text,
                                      std::vector<StringType>& tokensStrings)
{
  tokensStrings.clear();

  // Lowercase.
  boost::algorithm::to_lower(text);

  typedef boost::tokenizer<
      boost::char_separator<typename StringType::value_type>,
      typename StringType::const_iterator,
      StringType> tokenizer;

  boost::char_separator<typename StringType::value_type>
      sep(separators.c_str());
  tokenizer tokens(text, sep);

  for (auto it = tokens.begin(); it != tokens.end(); ++it)
    tokensStrings.push_back(*it);
}

template<typename StringType>
template<typename Archive>
void Corpus<StringType>::serialize(Archive& ar,
                                   const unsigned int /* version */)
{
  // Clean memory if necessary.
  if (Archive::is_loading::value)
  {
    delete vocabulary;
    delete corpusTokens;
    //delete[] separators;
  }

  ar & BOOST_SERIALIZATION_NVP(vocabulary);
  ar & BOOST_SERIALIZATION_NVP(corpusTokens);
  //ar & BOOST_SERIALIZATION_NVP(separators);
}

}
}

#include "corpus_impl.cpp"

#endif
