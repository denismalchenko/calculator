#include <gtest/gtest.h>

#include <cmath>

#include "../model/expression.h"

TEST(SetAndCalc, Test1) {
  s21::Expression expression;
  expression.SetInfixNotation("7.2 + 8*(15)");
  std::vector<s21::Token> sample{
      {"7.2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"8", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"15", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"7.2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"8", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"15", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(), "7.2 + 8*(15)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "127.2");
}

TEST(SetAndCalc, Test2) {
  s21::Expression expression;
  expression.SetInfixNotation("7 + 8.2*(15E1 mod4.2e-1)");
  std::vector<s21::Token> sample{
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"8.2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"15e1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"4.2e-1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"8.2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"15e1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"4.2e-1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(), "7 + 8.2*(15E1 mod4.2e-1)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "7.492");
}

TEST(SetAndCalc, Test3) {
  s21::Expression expression;
  expression.SetInfixNotation("- - 48sIn(-^4.7e-2%");
  std::vector<s21::Token> sample{
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"48", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"4.7e-2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
  };
  EXPECT_EQ(expression.infixString(), "- - 48sIn(-^4.7e-2%");
  EXPECT_EQ(expression.valid(), "Unclosed (");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Unclosed (");
}

TEST(SetAndCalc, Test4) {
  s21::Expression expression;
  expression.SetInfixNotation(
      "+(-3,5e+1x-X%mod))(cos(^.25tg(TAn(+ln(/   "
      "+log(atan(ATg(*sqrt(siN(acos(asin())");
  std::vector<s21::Token> sample{
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"3.5e+1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"cos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {".25", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"ln", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"log", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"sqrt", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"acos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(),
            "+(-3,5e+1x-X%mod))(cos(^.25tg(TAn(+ln(/   "
            "+log(atan(ATg(*sqrt(siN(acos(asin())");
  EXPECT_EQ(expression.valid(), "Unopened (");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Unopened (");
}
TEST(SetAndCalc, Test5) {
  s21::Expression expression;
  expression.SetInfixNotation(
      "(-2.3e-7*sin(2*x^3.14)+ln(+1-acos(tg(3*x))))/(-.4)");
  std::vector<s21::Token> sample{
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"2.3e-7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"3.14", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"ln", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"acos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {".4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"2.3e-7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"3.14", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"acos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"ln", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {".4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(),
            "(-2.3e-7*sin(2*x^3.14)+ln(+1-acos(tg(3*x))))/(-.4)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate("1"), "Invalid result");
}
TEST(SetAndCalc, Test6) {
  s21::Expression expression;
  expression.SetInfixNotation("3e-2-3");
  std::vector<s21::Token> sample{
      {"3e-2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"3e-2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(), "3e-2-3");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "-2.97");
}

TEST(SetAndCalc, Test7) {
  s21::Expression expression;
  expression.SetInfixNotation("");
  EXPECT_EQ(expression.infixString(), "");
  EXPECT_EQ(expression.valid(), "Please enter an expression");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Please enter an expression");
}

TEST(SetAndCalc, Test8) {
  s21::Expression expression;
  expression.SetInfixNotation("lgx");
  EXPECT_EQ(expression.infixString(), "lgx");
  EXPECT_EQ(expression.valid(), "Unknown function");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Unknown function");
  expression.SetInfixNotation("abs(-2.3)");
  EXPECT_EQ(expression.infixString(), "abs(-2.3)");
  EXPECT_EQ(expression.valid(), "Unknown function");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Unknown function");
  expression.SetInfixNotation("4ee.");
  EXPECT_EQ(expression.valid(), "Extra character e");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Extra character e");
  expression.SetInfixNotation(".e-7");
  EXPECT_EQ(expression.valid(), "Extra dot or comma");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Extra dot or comma");
}

TEST(SetAndCalc, Test9) {
  s21::Expression expression;
  expression.SetInfixNotation("(- - 48/sIn(-^4.7e-2%");
  std::vector<s21::Token> sample{
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"48", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"4.7e-2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
  };
  EXPECT_EQ(expression.infixString(), "(- - 48/sIn(-^4.7e-2%");
  EXPECT_EQ(expression.valid(), "Unclosed (");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Unclosed (");
}

TEST(SetAndCalc, Test10) {
  s21::Expression expression;
  expression.SetInfixNotation("tg(x)+77-log(-4) / 5)");
  std::vector<s21::Token> sample{
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"77", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"log", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "tg(x)+77-log(-4) / 5)");
  EXPECT_EQ(expression.valid(), "Unopened (");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Unopened (");
}

TEST(SetAndCalc, Test11) {
  s21::Expression expression;
  expression.SetInfixNotation("7)+(-8");
  std::vector<s21::Token> sample{
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"8", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
  };
  EXPECT_EQ(expression.infixString(), "7)+(-8");
  EXPECT_EQ(expression.valid(), "Unopened (");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Unopened (");
}

TEST(SetAndCalc, Test12) {
  s21::Expression expression;
  expression.SetInfixNotation("atan(3)(-4)");
  std::vector<s21::Token> sample{
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "atan(3)(-4)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "-4.9961830896");
}

TEST(SetAndCalc, Test13) {
  s21::Expression expression;
  expression.SetInfixNotation("7-cos()+2");
  std::vector<s21::Token> sample{
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"cos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "7-cos()+2");
  EXPECT_EQ(expression.valid(), "Empty ()");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Empty ()");
}

TEST(SetAndCalc, Test14) {
  s21::Expression expression;
  expression.SetInfixNotation("(*8)/x");
  std::vector<s21::Token> sample{
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"8", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft}};
  EXPECT_EQ(expression.infixString(), "(*8)/x");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test15) {
  s21::Expression expression;
  expression.SetInfixNotation("asin(^2)");
  std::vector<s21::Token> sample{
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "asin(^2)");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test16) {
  s21::Expression expression;
  expression.SetInfixNotation("atg(1)7");
  std::vector<s21::Token> sample{
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "atg(1)7");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "5.4977871438");
}

TEST(SetAndCalc, Test17) {
  s21::Expression expression;
  expression.SetInfixNotation("(2,4-3)tan(1)");
  std::vector<s21::Token> sample{
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"2.4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"2.4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "(2,4-3)tan(1)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "-0.9344446348");
}

TEST(SetAndCalc, Test18) {
  s21::Expression expression;
  expression.SetInfixNotation("/ 4 -3");
  std::vector<s21::Token> sample{
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "/ 4 -3");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test19) {
  s21::Expression expression;
  expression.SetInfixNotation("7 %");
  std::vector<s21::Token> sample{
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "7 %");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test20) {
  s21::Expression expression;
  expression.SetInfixNotation("3+x^");
  std::vector<s21::Token> sample{
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3}};
  EXPECT_EQ(expression.infixString(), "3+x^");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test21) {
  s21::Expression expression;
  expression.SetInfixNotation("+");
  std::vector<s21::Token> sample{
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "+");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test22) {
  s21::Expression expression;
  expression.SetInfixNotation("x^2+2x+1");
  std::vector<s21::Token> sample{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(), "x^2+2x+1");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "1");
  EXPECT_EQ(expression.Calculate("1.5"), "6.25");
}

TEST(SetAndCalc, Test23) {
  s21::Expression expression;
  expression.SetInfixNotation("x30-2");
  std::vector<s21::Token> sample{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"30", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "x30-2");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test24) {
  s21::Expression expression;
  expression.SetInfixNotation("x(30-2)");
  std::vector<s21::Token> sample{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"30", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"30", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "x(30-2)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "0");
  EXPECT_EQ(expression.Calculate("-1"), "-28");
}

TEST(SetAndCalc, Test25) {
  s21::Expression expression;
  expression.SetInfixNotation("6,4(30 mod 2)");
  std::vector<s21::Token> sample{
      {"6.4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"30", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"6.4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"30", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "6,4(30 mod 2)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "0");
}

TEST(SetAndCalc, Test26) {
  s21::Expression expression;
  expression.SetInfixNotation("3 acos(x)");
  std::vector<s21::Token> sample{
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"acos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"acos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "3 acos(x)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "4.7123889804");
  EXPECT_EQ(expression.Calculate("-0.3"), "5.6264669424");
}

TEST(SetAndCalc, Test27) {
  s21::Expression expression;
  expression.SetInfixNotation("x asin(0)");
  std::vector<s21::Token> sample{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "x asin(0)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "0");
  EXPECT_EQ(expression.Calculate("-68.65416"), "0");
}

TEST(SetAndCalc, Test28) {
  s21::Expression expression;
  expression.SetInfixNotation("asin(x-)");
  std::vector<s21::Token> sample{
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "asin(x-)");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test29) {
  s21::Expression expression;
  expression.SetInfixNotation("asin(x+-4)");
  std::vector<s21::Token> sample{
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "asin(x+-4)");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test30) {
  s21::Expression expression;
  expression.SetInfixNotation("acos(x/^4)");
  std::vector<s21::Token> sample{
      {"acos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "acos(x/^4)");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test31) {
  s21::Expression expression;
  expression.SetInfixNotation("(-)^2");
  std::vector<s21::Token> sample{
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "(-)^2");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test32) {
  s21::Expression expression;
  expression.SetInfixNotation("-^2");
  std::vector<s21::Token> sample{
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "-^2");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test33) {
  s21::Expression expression;
  expression.SetInfixNotation("(+*3)");
  std::vector<s21::Token> sample{
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "(+*3)");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test34) {
  s21::Expression expression;
  expression.SetInfixNotation("-4.5.6");
  std::vector<s21::Token> sample{
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"4.5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {".6", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "-4.5.6");
  EXPECT_EQ(expression.valid(), "Extra dot or comma");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Extra dot or comma");
}

TEST(SetAndCalc, Test35) {
  s21::Expression expression;
  expression.SetInfixNotation("6e7.0");
  std::vector<s21::Token> sample{
      {"6e7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {".0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "6e7.0");
  EXPECT_EQ(expression.valid(), "Extra dot or comma");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Extra dot or comma");
}

TEST(SetAndCalc, Test36) {
  s21::Expression expression;
  expression.SetInfixNotation("6.2e3.0");
  std::vector<s21::Token> sample{
      {"6.2e3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {".0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "6.2e3.0");
  EXPECT_EQ(expression.valid(), "Extra dot or comma");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Extra dot or comma");
}

TEST(SetAndCalc, Test37) {
  s21::Expression expression;
  expression.SetInfixNotation("cosasin(x)");
  std::vector<s21::Token> sample{
      {"cos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "cosasin(x)");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test38) {
  s21::Expression expression;
  expression.SetInfixNotation("ln^2(1)");
  std::vector<s21::Token> sample{
      {"ln", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "ln^2(1)");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test39) {
  s21::Expression expression;
  expression.SetInfixNotation("-x^2");
  std::vector<s21::Token> sample{
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "-x^2");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "0");
  EXPECT_EQ(expression.Calculate("1.4"), "-1.96");
  EXPECT_EQ(expression.Calculate("-1.11"), "-1.2321");
}
TEST(SetAndCalc, Test40) {
  s21::Expression expression;
  expression.SetInfixNotation("3e-");
  EXPECT_EQ(expression.infixString(), "3e-");
  EXPECT_EQ(expression.valid(), "Missing degree value");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Missing degree value");
  expression.SetInfixNotation("3e+");
  EXPECT_EQ(expression.infixString(), "3e+");
  EXPECT_EQ(expression.valid(), "Missing degree value");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Missing degree value");
  expression.SetInfixNotation("4e--2");
  EXPECT_EQ(expression.infixString(), "4e--2");
  EXPECT_EQ(expression.valid(), "Missing degree value");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Missing degree value");
  expression.SetInfixNotation("4e-+2");
  EXPECT_EQ(expression.infixString(), "4e-+2");
  EXPECT_EQ(expression.valid(), "Missing degree value");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Missing degree value");
  expression.SetInfixNotation("4e+-2");
  EXPECT_EQ(expression.infixString(), "4e+-2");
  EXPECT_EQ(expression.valid(), "Missing degree value");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Missing degree value");
  expression.SetInfixNotation("4e++2");
  EXPECT_EQ(expression.infixString(), "4e++2");
  EXPECT_EQ(expression.valid(), "Missing degree value");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Missing degree value");
  expression.SetInfixNotation("4e-.2");
  EXPECT_EQ(expression.infixString(), "4e-.2");
  EXPECT_EQ(expression.valid(), "Missing degree value");
  EXPECT_TRUE(expression.infixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Missing degree value");
}
TEST(SetAndCalc, Test41) {
  s21::Expression expression;
  expression.SetInfixNotation("2^3^4");
  std::vector<s21::Token> sample{
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3}};
  EXPECT_EQ(expression.infixString(), "2^3^4");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "2.4178516392e+24");
}

TEST(SetAndCalc, Test42) {
  s21::Expression expression;
  expression.SetInfixNotation("(sin(2*1e0))^3^(-0.1)+tan(+3)");
  std::vector<s21::Token> sample{
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"1e0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"0.1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"1e0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"0.1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"tan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(), "(sin(2*1e0))^3^(-0.1)+tan(+3)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "0.7757908268");

  // auto result = expression.infixNotation();
  // for (auto item = result.begin(), it = sample.begin(); item != result.end();
  //      ++item, ++it) {
  //   if (*item != *it)
  //     std::cout << item->value_ << " " << int(item->type_) << " "
  //               << int(item->priority_) << " " << int(item->associativity_)
  //               << " sample: " << it->value_ << " " << int(it->type_) << " "
  //               << int(it->priority_) << " " << int(it->associativity_)
  //               << std::endl;
  // }
}

TEST(SetAndCalc, Test43) {
  s21::Expression expression;
  expression.SetInfixNotation("atan(0.1^2-7)+ln(-6/(1-3*sqrt(+5)))^sin(8)");
  std::vector<s21::Token> sample{
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"0.1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"ln", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"6", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"sqrt", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"8", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"0.1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"6", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"sqrt", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"8", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"ln", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(),
            "atan(0.1^2-7)+ln(-6/(1-3*sqrt(+5)))^sin(8)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "-1.3793744941");

  // auto result = expression.postfixNotation();
  // for (auto item = result.begin(), it = sample2.begin(); item !=
  // result.end();
  //      ++item, ++it) {
  //   if (*item != *it)
  //     std::cout << item->value_ << " " << int(item->type_) << " "
  //               << int(item->priority_) << " " << int(item->associativity_)
  //               << " sample2: " << it->value_ << " " << int(it->type_) << " "
  //               << int(it->priority_) << " " << int(it->associativity_)
  //               << std::endl;
  // }
}

TEST(SetAndCalc, Test44) {
  s21::Expression expression;
  expression.SetInfixNotation(
      "asin(( x*(1.0))*1.5)+acos((x*(1.0))) - atan( x + 0.1 )");
  std::vector<s21::Token> sample{
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"1.0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"1.5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"acos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"1.0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"0.1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"1.0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"1.5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"asin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"1.0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"acos", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"0.1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"atan", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(),
            "asin(( x*(1.0))*1.5)+acos((x*(1.0))) - atan( x + 0.1 )");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate("0.456717"), "1.3432167617");
}

TEST(SetAndCalc, Test45) {
  s21::Expression expression;
  expression.SetInfixNotation("2+4/ 5 * 6\t-2+4%0");
  std::vector<s21::Token> sample{
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"6", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"6", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"%", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(), "2+4/ 5 * 6\t-2+4%0");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "Invalid result");
}

TEST(SetAndCalc, Test46) {
  s21::Expression expression;
  expression.SetInfixNotation("1/0");
  std::vector<s21::Token> sample{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "1/0");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "Infinity");
}

TEST(SetAndCalc, Test47) {
  s21::Expression expression;
  expression.SetInfixNotation("log(0)");
  std::vector<s21::Token> sample{
      {"log", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"0", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"log", s21::TokenType::kUnaryOperation, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "log(0)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "-Infinity");
}

TEST(SetAndCalc, Test48) {
  s21::Expression expression;
  expression.SetInfixNotation("-4!^3!!");
  std::vector<s21::Token> sample{
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft}};
  std::vector<s21::Token> sample2{
      {"4", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "-4!^3!!");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "-Infinity");
}

TEST(SetAndCalc, Test49) {
  s21::Expression expression;
  expression.SetInfixNotation("(sin(1))!");
  std::vector<s21::Token> sample{
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft}};
  std::vector<s21::Token> sample2{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"sin", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft}};
  EXPECT_EQ(expression.infixString(), "(sin(1))!");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "0.943048659");
}

TEST(SetAndCalc, Test50) {
  s21::Expression expression;
  expression.SetInfixNotation("+!7");
  std::vector<s21::Token> sample{
      {"+", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "+!7");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
  expression.SetInfixNotation("!7");
  sample.clear();
  sample = {
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"7", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.infixString(), "!7");
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
  expression.SetInfixNotation("6!5");
  sample.clear();
  sample = {
      {"6", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
  expression.SetInfixNotation("6!ln(5)");
  sample.clear();
  sample = {
      {"6", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"ln", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"5", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  EXPECT_EQ(expression.valid(), "Incorrect expression");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation().empty());
  EXPECT_EQ(expression.Calculate(""), "Incorrect expression");
}

TEST(SetAndCalc, Test51) {
  s21::Expression expression;
  expression.SetInfixNotation("3!(-8)");
  std::vector<s21::Token> sample{
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"(", s21::TokenType::kBracket, s21::Associativity::kLeft},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"8", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {")", s21::TokenType::kBracket, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"3", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"!", s21::TokenType::kUnaryOperation, s21::Associativity::kLeft},
      {"8", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kUnaryOperation, s21::Associativity::kRight},
      {"*", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "3!(-8)");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "-48");
}

TEST(SetAndCalc, Test52) {
  s21::Expression expression;
  expression.SetInfixNotation("1-1e10000");
  std::vector<s21::Token> sample{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"1e10000", s21::TokenType::kNumberOrVariable,
       s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"1e10000", s21::TokenType::kNumberOrVariable,
       s21::Associativity::kRight},
      {"-", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(), "1-1e10000");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "-Infinity");
}

TEST(SetAndCalc, Test53) {
  s21::Expression expression;
  expression.SetInfixNotation("1/1e+10000");
  std::vector<s21::Token> sample{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2},
      {"1e+10000", s21::TokenType::kNumberOrVariable,
       s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"1e+10000", s21::TokenType::kNumberOrVariable,
       s21::Associativity::kRight},
      {"/", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 2}};
  EXPECT_EQ(expression.infixString(), "1/1e+10000");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "0");
}

TEST(SetAndCalc, Test54) {
  s21::Expression expression;
  expression.SetInfixNotation("1+2E-10000");
  std::vector<s21::Token> sample{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1},
      {"2e-10000", s21::TokenType::kNumberOrVariable,
       s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"1", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"2e-10000", s21::TokenType::kNumberOrVariable,
       s21::Associativity::kRight},
      {"+", s21::TokenType::kBinaryOperation, s21::Associativity::kLeft, 1}};
  EXPECT_EQ(expression.infixString(), "1+2E-10000");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  EXPECT_EQ(expression.Calculate(""), "1");
}

TEST(Plot, Test1) {
  s21::Expression expression;
  expression.SetInfixNotation("x^2");
  std::vector<s21::Token> sample{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight}};
  std::vector<s21::Token> sample2{
      {"x", s21::TokenType::kNumberOrVariable, s21::Associativity::kLeft},
      {"2", s21::TokenType::kNumberOrVariable, s21::Associativity::kRight},
      {"^", s21::TokenType::kBinaryOperation, s21::Associativity::kRight, 3}};
  EXPECT_EQ(expression.infixString(), "x^2");
  EXPECT_EQ(expression.valid(), "");
  EXPECT_TRUE(expression.infixNotation() == sample);
  EXPECT_TRUE(expression.postfixNotation() == sample2);
  std::pair<std::vector<double>, std::vector<double>> xy =
      expression.Plot("2", "-2", "3", "0.5", 9);
  EXPECT_EQ(xy.first.size(), 9);
  EXPECT_EQ(xy.second.size(), 9);
  auto it = xy.first.begin();
  for (double i = -2; i <= 2; i += 0.5, ++it) EXPECT_EQ(*it, i);
  it = xy.second.begin();
  for (int i = 0; i < 9; ++i) {
    if (i == 1 || i == 7)
      EXPECT_EQ(*it, 2.25);
    else if (i == 2 || i == 6)
      EXPECT_EQ(*it, 1);
    else
      EXPECT_TRUE(std::isnan(*it));
    ++it;
  }
}
