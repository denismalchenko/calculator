#ifndef CPP3_S21_SMARTCALCULATOR_MODEL_PARCER_H_
#define CPP3_S21_SMARTCALCULATOR_MODEL_PARCER_H_
#include <algorithm>
#include <string>
#include <vector>

namespace s21 {

enum class DecimalSeparator { kPoint, kComma, kAny };
enum class DigitSeparator { kNo, kSpase, kApostrophe, kAny };

class CheckChar {
 public:
  using Str = std::string;
  using iterator = typename Str::iterator;
  CheckChar(DecimalSeparator decimalSeparator = DecimalSeparator::kPoint,
            DigitSeparator digitSeparator = DigitSeparator::kNo)
      : decimalSeparator_(decimalSeparator), digitSeparator_(digitSeparator) {}
  ~CheckChar() = default;
  bool IsNumber(iterator strIt) { return ('0' <= *strIt && *strIt <= '9'); }
  bool IsNumberWithSeparator(iterator strIt);
  bool IsDecimalSeparator(iterator strIt);
  int IsDash(iterator &strIt);

 protected:
  DecimalSeparator decimalSeparator_ = DecimalSeparator::kPoint;
  DigitSeparator digitSeparator_ = DigitSeparator::kNo;
};

class ParserLexeme : public CheckChar {
 public:
  using Str = std::string;
  using iterator = typename Str::iterator;
  ParserLexeme(DecimalSeparator decimalSeparator = DecimalSeparator::kPoint,
               DigitSeparator digitSeparator = DigitSeparator::kNo)
      : CheckChar(decimalSeparator, digitSeparator) {}
  ~ParserLexeme() = default;
  //  true if, starting from the first character, the string matches the lexeme.
  //  If false, then the iterator remains unchanged, if true, then
  //  the iterator points to the next character after the last matched character
  bool LexemeEqual(iterator &sourse, const iterator endString,
                   const Str &lexem);
  // reflexes is the first matching lexeme from the vector.
  //  If false, then the iterator remains unchanged, if true, then
  //  the iterator points to the next character after the last matched character
  bool LexemeEqualVector(iterator &sourse, const iterator endString,
                         const std::vector<Str> &lexemes, Str &resLexem);

  Str ParseInteger(iterator &strIt, iterator endString);
  Str ParseDouble(iterator &strIt, iterator endString);
  Str ParseSciNotation(iterator &strIt, iterator endString);
};

class ParserString : private ParserLexeme {
 public:
  using Str = std::string;
  using iterator = typename Str::iterator;
  ParserString(bool ignoreCase = true, bool integer = false,
               bool scientificNotation = true,
               DecimalSeparator decimalSeparator = DecimalSeparator::kAny,
               DigitSeparator digitSeparator = DigitSeparator::kNo)
      : ParserLexeme(decimalSeparator, digitSeparator),
        ignoreCase_(ignoreCase),
        integer_(integer),
        scientificNotation_(scientificNotation) {}
  ~ParserString() = default;
  // SetFlags do the same things like constructor to reuse the object.
  void SetFlags(bool ignoreCase = true, bool integer = false,
                bool scientificNotation = true,
                DecimalSeparator decimalSeparator = DecimalSeparator::kAny,
                DigitSeparator digitSeparator = DigitSeparator::kNo);
  Str UnsuccessType(Str unsuccess, const std::vector<Str> &lexemes,
                    const bool unsuccessNumber);
  bool Parse(const Str &str, std::vector<Str> &result,
             const std::vector<Str> &lexemes, const std::vector<Str> &spaces,
             std::vector<Str> &unsuccess);

  bool ParseMathString(const Str &str,
                       std::vector<std::pair<bool, Str>> &result,
                       const std::vector<Str> &lexemes,
                       const std::vector<Str> &spaces,
                       std::vector<Str> &unsuccess);

 protected:
  bool ignoreCase_ = true;
  bool integer_ = true;
  bool scientificNotation_ = true;
  void ToLowCase(Str &str);
  void ToLowCase(std::vector<Str> &str);
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALCULATOR_MODEL_PARCER_H_