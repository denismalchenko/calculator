#include "expression.h"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stack>
#include <utility>

#include "parcer.h"

namespace s21 {

Dictionary::Dictionary() {
  allLexemString_ = {"(",    ")",    "+",    "-",    "*",   "/",   "^",   "%",
                     "x",    "!",    "mod",  "sqrt", "sin", "cos", "tan", "tg",
                     "asin", "acos", "atan", "atg",  "ln",  "log"};
  allTokens_["("] = {"(", TokenType::kBracket, Associativity::kLeft};
  allTokens_[")"] = {")", TokenType::kBracket, Associativity::kRight};
  allTokens_["U+"] = {"+", TokenType::kUnaryOperation, Associativity::kRight};
  allTokens_["U-"] = {"-", TokenType::kUnaryOperation, Associativity::kRight};
  allTokens_["B+"] = {"+", TokenType::kBinaryOperation, Associativity::kLeft,
                      1};
  allTokens_["B-"] = {"-", TokenType::kBinaryOperation, Associativity::kLeft,
                      1};
  allTokens_["*"] = {"*", TokenType::kBinaryOperation, Associativity::kLeft, 2};
  allTokens_["/"] = {"/", TokenType::kBinaryOperation, Associativity::kLeft, 2};
  allTokens_["%"] = {"%", TokenType::kBinaryOperation, Associativity::kLeft, 2};
  allTokens_["x"] = {"x", TokenType::kNumberOrVariable, Associativity::kLeft};
  allTokens_["^"] = {"^", TokenType::kBinaryOperation, Associativity::kRight,
                     3};
  allTokens_["!"] = {"!", TokenType::kUnaryOperation, Associativity::kLeft};
  allTokens_["sqrt"] = {"sqrt", TokenType::kUnaryOperation,
                        Associativity::kRight};
  allTokens_["sin"] = {"sin", TokenType::kUnaryOperation,
                       Associativity::kRight};
  allTokens_["cos"] = {"cos", TokenType::kUnaryOperation,
                       Associativity::kRight};
  allTokens_["tan"] = {"tan", TokenType::kUnaryOperation,
                       Associativity::kRight};
  allTokens_["asin"] = {"asin", TokenType::kUnaryOperation,
                        Associativity::kRight};
  allTokens_["acos"] = {"acos", TokenType::kUnaryOperation,
                        Associativity::kRight};
  allTokens_["atan"] = {"atan", TokenType::kUnaryOperation,
                        Associativity::kRight};
  allTokens_["ln"] = {"ln", TokenType::kUnaryOperation, Associativity::kRight};
  allTokens_["log"] = {"log", TokenType::kUnaryOperation,
                       Associativity::kRight};
  allErrors_[ErrorCode::kNoError] = "";
  allErrors_[ErrorCode::kEmpty] = "Please enter an expression";
  allErrors_[ErrorCode::kUnknownFunc] = "Unknown function";
  allErrors_[ErrorCode::kExtraPoint] = "Extra dot or comma";
  allErrors_[ErrorCode::kExtraE] = "Extra character e";
  allErrors_[ErrorCode::kExtraLeftBracket] = "Unclosed (";
  allErrors_[ErrorCode::kOmmitedLeftBracket] = "Unopened (";
  allErrors_[ErrorCode::kEmptyBrackets] = "Empty ()";
  allErrors_[ErrorCode::kMissingDegree] = "Missing degree value";
  allErrors_[ErrorCode::kOtherError] = "Incorrect expression";
  binaryFunctions_["+"] = [](double x, double y) { return x + y; };
  binaryFunctions_["-"] = [](double x, double y) { return x - y; };
  binaryFunctions_["*"] = [](double x, double y) { return x * y; };
  binaryFunctions_["/"] = [](double x, double y) { return x / y; };
  binaryFunctions_["%"] = [](double x, double y) { return std::fmod(x, y); };
  binaryFunctions_["^"] = [](double x, double y) { return std::pow(x, y); };
  unaryFunctions_["-"] = [](double x) { return -x; };
  unaryFunctions_["+"] = [](double x) { return x; };
  unaryFunctions_["!"] = [](double x) { return std::tgamma(x + 1); };
  unaryFunctions_["sqrt"] = [](double x) { return std::sqrt(x); };
  unaryFunctions_["sin"] = [](double x) { return std::sin(x); };
  unaryFunctions_["cos"] = [](double x) { return std::cos(x); };
  unaryFunctions_["tan"] = [](double x) { return std::tan(x); };
  unaryFunctions_["asin"] = [](double x) { return std::asin(x); };
  unaryFunctions_["acos"] = [](double x) { return std::acos(x); };
  unaryFunctions_["atan"] = [](double x) { return std::atan(x); };
  unaryFunctions_["ln"] = [](double x) { return std::log(x); };
  unaryFunctions_["log"] = [](double x) { return std::log10(x); };
}
void Expression::SetInfixNotation(const std::string &notation) {
  infixString_ = notation;
  infixNotation_.clear();
  postfixNotation_.clear();
  ErrorCode errorcode = Convert();
  valid_ = (errorcode == ErrorCode::kNoError) ? CheckExpression() : errorcode;
  if (valid_ == ErrorCode::kNoError) ShuntingYard();
}

std::string Expression::Calculate(std::string x_value) {
  if (valid_ != ErrorCode::kNoError) return valid();
  double x = (!x_value.empty()) ? stod(x_value) : 0;
  double result = Calculate(x);
  if (std::isfinite(result))
    return (result == 0) ? "0" : toString(result);
  else if (std::isnan(result))
    return "Invalid result";
  else if (result > 0)
    return "Infinity";
  else
    return "-Infinity";
}
std::pair<std::vector<double>, std::vector<double>> Expression::Plot(
    std::string xMin, std::string xMax, std::string yMin, std::string yMax,
    size_t dotAmount) {
  double xmin = stod(xMin), xmax = stod(xMax), ymin = stod(yMin),
         ymax = stod(yMax);
  if (xmin > xmax) std::swap(xmin, xmax);
  if (ymin > ymax) std::swap(ymin, ymax);
  double dx = (xmax - xmin) / (dotAmount - 1);
  std::vector<double> x, y;
  for (size_t i = 0; i < dotAmount; ++i, xmin += dx) x.push_back(xmin);
  for (size_t i = 0; i < dotAmount; ++i) {
    double yValue = Calculate(x[i]);
    if (yValue > ymax || yValue < ymin)
      y.push_back(NAN);
    else
      y.push_back(yValue);
  }
  return std::make_pair(x, y);
}

double Expression::stod(const std::string &str) {
  try {
    return std::stod(str);
  } catch (const std::out_of_range &e) {
    size_t index = str.find('e');
    if (str[index + 1] == '-') return 0;
    return INFINITY;
  } catch (...) {
    return NAN;
  }
}

std::string Expression::toString(double value) {
  std::string result;
  std::ostringstream oss;
  if (std::abs(value) > std::pow(10, precision_) ||
      std::abs(value) < std::pow(10, 7 - precision_)) {
    oss << std::scientific << std::setprecision(precision_) << value;
    result = oss.str();
    int expStart = (value > 0) ? precision_ + 2 : precision_ + 3;
    std::string fractionPart = result.substr(0, expStart),
                expPart = result.substr(expStart);
    while (fractionPart.back() == '0') fractionPart.pop_back();
    if (fractionPart.back() == '.') fractionPart.pop_back();
    result = fractionPart + expPart;
  } else {
    oss << std::fixed << std::setprecision(precision_) << value;
    result = oss.str();
    while (result.back() == '0') result.pop_back();
    if (result.back() == '.') result.pop_back();
  }
  return result;
}

double Expression::Calculate(double x_value) {
  std::stack<double> stack;
  for (size_t i = 0; i < postfixNotation_.size(); ++i) {
    if (postfixNotation_[i].type_ == TokenType::kNumberOrVariable) {
      if (postfixNotation_[i].associativity_ == Associativity::kRight)
        stack.push(stod(postfixNotation_[i].value_));
      else
        stack.push(x_value);
    } else {
      if (postfixNotation_[i].type_ == TokenType::kUnaryOperation) {
        double foo = stack.top();
        stack.pop();
        stack.push(dict_.unaryFunctions_[postfixNotation_[i].value_](foo));
      } else {
        double bar = stack.top();
        stack.pop();
        double baz = stack.top();
        stack.pop();
        stack.push(
            dict_.binaryFunctions_[postfixNotation_[i].value_](baz, bar));
      }
    }
  }
  return stack.top();
}

ErrorCode Expression::Convert() {
  std::vector<std::pair<bool, std::string>> parsedString;
  std::vector<std::string> errorString;
  ErrorCode errorcode = ErrorCode::kNoError;
  ParserString parse(true, false, true, DecimalSeparator::kAny,
                     DigitSeparator::kNo);
  if (parse.ParseMathString(infixString_, parsedString, dict_.allLexemString_,
                            std::vector<std::string>{" ", "\t"}, errorString)) {
    std::string err = parse.UnsuccessType(
        errorString[0], std::vector<std::string>{".", ",", "e"}, true);
    if (err == "." || err == ",")
      errorcode = ErrorCode::kExtraPoint;
    else if (err == "e")
      errorcode = ErrorCode::kExtraE;
    else if (err == "0")
      errorcode = ErrorCode::kMissingDegree;
    else
      errorcode = ErrorCode::kUnknownFunc;
  } else {
    for (const auto &pair : parsedString) {
      if (pair.first) {
        infixNotation_.push_back(Token{pair.second,
                                       TokenType::kNumberOrVariable,
                                       Associativity::kRight, 0});
      } else {
        if (pair.second == "+") {
          if (infixNotation_.empty() || infixNotation_.back().value_ == "(")
            infixNotation_.push_back(dict_.allTokens_.find("U+")->second);
          else
            infixNotation_.push_back(dict_.allTokens_.find("B+")->second);
        } else if (pair.second == "-") {
          if (infixNotation_.empty() || infixNotation_.back().value_ == "(")
            infixNotation_.push_back(dict_.allTokens_.find("U-")->second);
          else
            infixNotation_.push_back(dict_.allTokens_.find("B-")->second);
        } else if (pair.second == "mod") {
          infixNotation_.push_back(dict_.allTokens_.find("%")->second);
        } else if (pair.second == "tg") {
          infixNotation_.push_back(dict_.allTokens_.find("tan")->second);
        } else if (pair.second == "atg") {
          infixNotation_.push_back(dict_.allTokens_.find("atan")->second);
        } else {
          infixNotation_.push_back(dict_.allTokens_.find(pair.second)->second);
        }
      }
    }
    if (parsedString.empty()) errorcode = ErrorCode::kEmpty;
  }
  return errorcode;
}
ErrorCode Expression::CheckExpression() {
  size_t size = infixNotation_.size();
  int sum_bracket = 0;
  for (size_t i = 0; i < size; ++i) {
    if (infixNotation_[i].value_ == "(") ++sum_bracket;
    if (infixNotation_[i].value_ == ")") --sum_bracket;
    if (sum_bracket < 0) return ErrorCode::kOmmitedLeftBracket;
  }
  if (sum_bracket > 0) return ErrorCode::kExtraLeftBracket;
  if (infixNotation_.front().type_ == TokenType::kBinaryOperation)
    return ErrorCode::kOtherError;
  if (infixNotation_.back().type_ == TokenType::kBinaryOperation)
    return ErrorCode::kOtherError;
  if (infixNotation_.front().type_ == TokenType::kUnaryOperation &&
      infixNotation_.front().associativity_ == Associativity::kLeft)
    return ErrorCode::kOtherError;
  if (infixNotation_.back().type_ == TokenType::kUnaryOperation &&
      infixNotation_.back().associativity_ == Associativity::kRight)
    return ErrorCode::kOtherError;
  return CheckExpressionByTokens();
}

ErrorCode Expression::CheckExpressionByTokens() {
  auto itCurrent = infixNotation_.begin();
  auto itNext = itCurrent;
  for (++itNext; itNext != infixNotation_.end(); ++itCurrent, ++itNext) {
    if (itCurrent->type_ == TokenType::kNumberOrVariable) {
      if (itNext->type_ == TokenType::kNumberOrVariable &&
          itNext->associativity_ == Associativity::kRight) {
        if (itCurrent->associativity_ == Associativity::kRight)
          return ErrorCode::kExtraPoint;
        else
          return ErrorCode::kOtherError;
      }
      if (itNext->value_ == "(" ||
          (itNext->type_ == TokenType::kUnaryOperation &&
           itNext->associativity_ == Associativity::kRight) ||
          (itNext->type_ == TokenType::kNumberOrVariable &&
           itNext->associativity_ == Associativity::kLeft)) {
        itNext = itCurrent =
            infixNotation_.insert(itNext, dict_.allTokens_.find("*")->second);
        ++itNext;
      }
    } else if (itCurrent->value_ == "(") {
      if (itNext->type_ == TokenType::kBinaryOperation ||
          (itNext->type_ == TokenType::kUnaryOperation &&
           itNext->associativity_ == Associativity::kLeft))
        return ErrorCode::kOtherError;
      if (itNext->value_ == ")") return ErrorCode::kEmptyBrackets;
    } else if (itCurrent->value_ == ")") {
      if (itNext->value_ == "(" ||
          (itNext->type_ == TokenType::kUnaryOperation &&
           itNext->associativity_ == Associativity::kRight) ||
          itNext->type_ == TokenType::kNumberOrVariable) {
        itNext = itCurrent =
            infixNotation_.insert(itNext, dict_.allTokens_.find("*")->second);
        ++itNext;
      }
    } else if (itCurrent->type_ == TokenType::kBinaryOperation) {
      if (itNext->value_ == ")" ||
          itNext->type_ == TokenType::kBinaryOperation ||
          (itNext->type_ == TokenType::kUnaryOperation &&
           itNext->associativity_ == Associativity::kLeft))
        return ErrorCode::kOtherError;
    } else if (itCurrent->value_ == "+" || itCurrent->value_ == "-") {
      if (itNext->value_ == ")" ||
          itNext->type_ == TokenType::kBinaryOperation ||
          (itNext->type_ == TokenType::kUnaryOperation &&
           itNext->associativity_ == Associativity::kLeft))
        return ErrorCode::kOtherError;
    } else if (itCurrent->associativity_ == Associativity::kLeft) {
      if (itNext->type_ == TokenType::kNumberOrVariable ||
          (itNext->type_ == TokenType::kUnaryOperation &&
           itNext->associativity_ == Associativity::kRight))
        return ErrorCode::kOtherError;
      if (itNext->value_ == "(") {
        itNext = itCurrent =
            infixNotation_.insert(itNext, dict_.allTokens_.find("*")->second);
        ++itNext;
      }
    } else {
      if (itNext->value_ != "(") return ErrorCode::kOtherError;
      // подумать над sin^
    }
  }
  return ErrorCode::kNoError;
}

void Expression::ShuntingYard() {
  std::stack<Token> stack;
  for (size_t i = 0; i < infixNotation_.size(); ++i) {
    if (infixNotation_[i].type_ == TokenType::kNumberOrVariable) {
      postfixNotation_.push_back(infixNotation_[i]);
    } else if (infixNotation_[i].type_ == TokenType::kUnaryOperation) {
      if (infixNotation_[i].associativity_ == Associativity::kRight)
        stack.push(infixNotation_[i]);
      else
        postfixNotation_.push_back(infixNotation_[i]);
    } else if (infixNotation_[i].type_ == TokenType::kBinaryOperation) {
      if (infixNotation_[i].associativity_ == Associativity::kRight) {
        while (!stack.empty()) {
          Token foo = stack.top();
          if (foo.value_ == "(" || foo.type_ == TokenType::kUnaryOperation)
            break;
          if (foo.type_ == TokenType::kBinaryOperation &&
              foo.priority_ <= infixNotation_[i].priority_)
            break;
          postfixNotation_.push_back(foo);
          stack.pop();
        }
      } else {
        while (!stack.empty()) {
          Token foo = stack.top();
          if (foo.value_ == "(") break;
          if (foo.type_ == TokenType::kBinaryOperation &&
              foo.priority_ < infixNotation_[i].priority_)
            break;
          postfixNotation_.push_back(foo);
          stack.pop();
        }
      }
      stack.push(infixNotation_[i]);
    } else if (infixNotation_[i].value_ == "(") {
      stack.push(infixNotation_[i]);
    } else {
      while (!stack.empty()) {
        Token foo = stack.top();
        stack.pop();
        if (foo.value_ != "(")
          postfixNotation_.push_back(foo);
        else
          break;
      }
    }
  }
  while (!stack.empty()) {
    postfixNotation_.push_back(stack.top());
    stack.pop();
  }
}

}  // namespace s21
