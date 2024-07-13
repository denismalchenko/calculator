#include "parcer.h"

#include <utility>

namespace s21 {

bool CheckChar::IsNumberWithSeparator(iterator strIt) {
  if (digitSeparator_ == DigitSeparator::kNo) return (IsNumber(strIt));
  if (digitSeparator_ != DigitSeparator::kAny) {
    char separator = digitSeparator_ == DigitSeparator::kSpase ? ' ' : '\'';
    return (IsNumber(strIt) || *strIt == separator);
  } else {
    return (IsNumber(strIt) || *strIt == ' ' || *strIt == '\'');
  }
}
bool CheckChar::IsDecimalSeparator(iterator strIt) {
  if (decimalSeparator_ == DecimalSeparator::kPoint)
    return *strIt == '.';
  else if (decimalSeparator_ == DecimalSeparator::kComma)
    return *strIt == ',';
  else
    return *strIt == '.' || *strIt == ',';
}

int CheckChar::IsDash(iterator &strIt) {
  if (*strIt == '-') {
    ++strIt;
    if (*strIt == '-') {
      ++strIt;
      return 2;
    }
    return 1;
  }
  return 0;
}

bool ParserLexeme::LexemeEqual(iterator &sourse, const iterator endString,
                               const Str &lexem) {
  auto lexemIt = lexem.begin();
  const auto sourseBegin = sourse;
  while (sourse != endString && lexemIt != lexem.end() && *sourse == *lexemIt) {
    ++sourse;
    ++lexemIt;
  }
  if (lexemIt != lexem.end()) sourse = sourseBegin;
  return lexemIt == lexem.end();
}

bool ParserLexeme::LexemeEqualVector(iterator &sourse, const iterator endString,
                                     const std::vector<Str> &lexemes,
                                     Str &resLexem) {
  auto lexemIt = lexemes.begin();
  bool equality = false;
  while (lexemIt != lexemes.end() && !equality) {
    equality = LexemeEqual(sourse, endString, *lexemIt);
    if (!equality) ++lexemIt;
  }
  if (equality) resLexem = *lexemIt;
  return equality;
}

ParserLexeme::Str ParserLexeme::ParseInteger(iterator &strIt,
                                             iterator endString) {
  Str result = "";
  if (this->digitSeparator_ == DigitSeparator::kNo) {
    while (strIt != endString && IsNumber(strIt)) {
      result.push_back(*strIt);
      ++strIt;
    }
  } else {
    while (strIt != endString && IsNumberWithSeparator(strIt)) {
      if (IsNumber(strIt)) result.push_back(*strIt);
      ++strIt;
    }
  }
  return result;
}

ParserLexeme::Str ParserLexeme::ParseDouble(iterator &strIt,
                                            iterator endString) {
  Str result = ParseInteger(strIt, endString);
  if (IsDecimalSeparator(strIt)) {
    result.push_back('.');
    ++strIt;
    DigitSeparator foo = this->digitSeparator_;
    this->digitSeparator_ = DigitSeparator::kNo;
    result += ParseInteger(strIt, endString);
    this->digitSeparator_ = foo;
  }
  return result;
}

ParserLexeme::Str ParserLexeme::ParseSciNotation(iterator &strIt,
                                                 iterator endString) {
  Str result = ParseDouble(strIt, endString);
  if (*strIt == 'e' || *strIt == 'E') {
    result.push_back(*strIt);
    ++strIt;
    if (*strIt == '+' || *strIt == '-') {
      result.push_back(*strIt);
      ++strIt;
    }
    result += ParseInteger(strIt, endString);
  }
  return result;
}

void ParserString::SetFlags(bool ignoreCase, bool integer,
                            bool scientificNotation,
                            DecimalSeparator decimalSeparator,
                            DigitSeparator digitSeparator) {
  ignoreCase_ = ignoreCase;
  integer_ = integer;
  scientificNotation_ = integer ? false : scientificNotation;
  this->decimalSeparator_ = decimalSeparator;
  this->digitSeparator_ =
      scientificNotation_ ? DigitSeparator::kNo : digitSeparator;
}
ParserString::Str ParserString::UnsuccessType(Str unsuccess,
                                              const std::vector<Str> &lexemes,
                                              const bool unsuccessNumber) {
  auto it = unsuccess.begin();
  if (unsuccessNumber) {
    if (IsNumber(it) || (IsDecimalSeparator(it) && IsNumber(++it))) return "0";
  }
  it = unsuccess.begin();
  Str typeUnsuccess;
  return LexemeEqualVector(it, unsuccess.end(), lexemes, typeUnsuccess)
             ? typeUnsuccess
             : "";
}
bool ParserString::Parse(const Str &str, std::vector<Str> &result,
                         const std::vector<Str> &lexemes,
                         const std::vector<Str> &spaces,
                         std::vector<Str> &unsuccess) {
  Str tempStr = str;
  result.clear();
  unsuccess.clear();
  std::vector<Str> tempLexemes(lexemes), tempSpaces(spaces);
  if (ignoreCase_) {
    ToLowCase(tempStr);
    ToLowCase(tempLexemes);
    ToLowCase(tempSpaces);
  }
  iterator strIt = tempStr.begin();
  const iterator endString = tempStr.end();
  Str successLexeme, unsuccessLexeme;
  while (strIt != endString) {
    bool success = true;
    if (LexemeEqualVector(strIt, endString, tempSpaces, successLexeme)) {
      success = true;
    } else if (LexemeEqualVector(strIt, endString, tempLexemes,
                                 successLexeme)) {
      result.push_back(successLexeme);
      success = true;
    } else {
      unsuccessLexeme.push_back(*strIt);
      success = false;
      ++strIt;
    }
    if (success) {
      if (unsuccessLexeme.size() > 0) {
        unsuccess.push_back(unsuccessLexeme);
        unsuccessLexeme.clear();
      }
      successLexeme.clear();
    }
  }
  if (unsuccessLexeme.size() > 0) unsuccess.push_back(unsuccessLexeme);
  return !unsuccess.empty();
}

bool ParserString::ParseMathString(const Str &str,
                                   std::vector<std::pair<bool, Str>> &result,
                                   const std::vector<Str> &lexemes,
                                   const std::vector<Str> &spaces,
                                   std::vector<Str> &unsuccess) {
  Str tempStr = str;
  result.clear();
  unsuccess.clear();
  std::vector<Str> tempLexemes(lexemes), tempSpaces(spaces);
  if (ignoreCase_) {
    ToLowCase(tempStr);
    ToLowCase(tempLexemes);
    ToLowCase(tempSpaces);
  }
  iterator strIt = tempStr.begin();
  const iterator endString = tempStr.end();
  Str successLexeme, unsuccessLexeme;
  while (strIt != endString) {
    bool success = true;
    if (IsNumber(strIt) || (!integer_ && IsDecimalSeparator(strIt))) {
      if (integer_)
        successLexeme = ParseInteger(strIt, endString);
      else if (scientificNotation_)
        successLexeme = ParseSciNotation(strIt, endString);
      else
        successLexeme = ParseDouble(strIt, endString);
      success = true;
      if (successLexeme.back() == '+' || successLexeme.back() == '-' ||
          (IsDecimalSeparator(successLexeme.begin()) &&
           ((successLexeme.size() == 1) || successLexeme[1] == 'e' ||
            successLexeme[1] == 'E'))) {
        if (unsuccessLexeme.size() > 0) {
          unsuccess.push_back(unsuccessLexeme);
          unsuccessLexeme.clear();
        }
        unsuccess.push_back(successLexeme);
      } else {
        result.push_back(std::make_pair(true, successLexeme));
      }
    } else if (LexemeEqualVector(strIt, endString, tempSpaces, successLexeme)) {
      success = true;
    } else if (LexemeEqualVector(strIt, endString, tempLexemes,
                                 successLexeme)) {
      result.push_back(std::make_pair(false, successLexeme));
      success = true;
    } else {
      unsuccessLexeme.push_back(*strIt);
      success = false;
      ++strIt;
    }
    if (success) {
      if (unsuccessLexeme.size() > 0) {
        unsuccess.push_back(unsuccessLexeme);
        unsuccessLexeme.clear();
      }
      successLexeme.clear();
    }
  }
  if (unsuccessLexeme.size() > 0) unsuccess.push_back(unsuccessLexeme);
  return !unsuccess.empty();
}

void ParserString::ToLowCase(Str &str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}
void ParserString::ToLowCase(std::vector<Str> &str) {
  for (auto strIt = str.begin(); strIt != str.end(); ++strIt) {
    ToLowCase(*strIt);
  }
}

}  // namespace s21
