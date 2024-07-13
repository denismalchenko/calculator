#ifndef CPP3_S21_SMARTCALCULATOR_MODEL_EXPRESSION_H_
#define CPP3_S21_SMARTCALCULATOR_MODEL_EXPRESSION_H_
#include <map>
#include <string>
#include <vector>

namespace s21 {

enum class TokenType {
  kNumberOrVariable,
  kBracket,
  kUnaryOperation,
  kBinaryOperation
};
/*
 For kNumberOrVariable
 kLeft means variable (lvalue)
 kRight means number (rvalue)
 For kBracket
 kLeft means left bracket
 kRight means right bracket
 For kUnaryOperation
 kLeft means prefix operation
 kRight means postfix operation
 For kBinaryOperation
 kLeft means left associative operation
 kRight means right associative operation
*/
enum class Associativity { kLeft, kRight };
enum class ErrorCode {
  kNoError,
  kEmpty,
  kUnknownFunc,
  kExtraPoint,
  kExtraE,
  kMissingDegree,
  kExtraLeftBracket,
  kOmmitedLeftBracket,
  kEmptyBrackets,
  kOtherError
};

struct Token {
  std::string value_ = "";
  TokenType type_ = TokenType::kUnaryOperation;
  Associativity associativity_ = Associativity::kLeft;
  unsigned short priority_ = 0;
  bool operator==(const Token &other) const {
    return (value_ == other.value_ && type_ == other.type_ &&
            priority_ == other.priority_ &&
            associativity_ == other.associativity_);
  }
  bool operator!=(const Token &other) const { return !(*this == other); }
};

struct Dictionary {
  Dictionary();
  std::map<std::string, Token> allTokens_;
  std::map<ErrorCode, std::string> allErrors_;
  std::map<std::string, double (*)(double, double)> binaryFunctions_;
  std::map<std::string, double (*)(double)> unaryFunctions_;
  std::vector<std::string> allLexemString_;
};

class Expression {
 public:
  Expression() = default;
  std::string valid() const { return dict_.allErrors_.find(valid_)->second; }
  std::string infixString() const { return infixString_; }
  std::vector<Token> infixNotation() const { return infixNotation_; }
  void setPrecision(size_t precision) { precision_ = precision; }
  std::vector<Token> postfixNotation() const { return postfixNotation_; }

  void SetInfixNotation(const std::string &notation);
  std::string Calculate(std::string x_value);
  std::pair<std::vector<double>, std::vector<double>> Plot(std::string xMin,
                                                           std::string xMax,
                                                           std::string yMin,
                                                           std::string yMax,
                                                           size_t dotAmount);

 protected:
  ErrorCode valid_ = ErrorCode::kEmpty;
  std::string infixString_;
  std::vector<Token> infixNotation_;
  std::vector<Token> postfixNotation_;
  Dictionary dict_;
  int precision_ = 10;

 private:
  std::string toString(double value);
  double stod(const std::string &str);
  double Calculate(double x_value);
  ErrorCode Convert();
  ErrorCode CheckExpression();
  ErrorCode CheckExpressionByTokens();
  void ShuntingYard();
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALCULATOR_MODEL_EXPRESSION_H_
