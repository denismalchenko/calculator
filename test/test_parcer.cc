#include <gtest/gtest.h>

#include "../model/parcer.h"

TEST(CheckChar, IsNumber) {
  s21::CheckChar checkCh;
  std::string str = "\t13.78e[R !0O";
  auto strIt = str.begin();
  bool result[] = {false, true,  true,  false, true, true, false,
                   false, false, false, false, true, false};
  for (int i = 0; i < 13; ++i) {
    EXPECT_EQ(result[i], checkCh.IsNumber(strIt));
    ++strIt;
  }
}

TEST(CheckChar, IsNumberWithSeparatorNo) {
  s21::CheckChar checkCh(s21::DecimalSeparator::kPoint,
                         s21::DigitSeparator::kNo);
  std::string str = "\'13.78e[R !0O";
  auto strIt = str.begin();
  bool result[] = {false, true,  true,  false, true, true, false,
                   false, false, false, false, true, false};
  for (int i = 0; i < 13; ++i) {
    EXPECT_EQ(result[i], checkCh.IsNumberWithSeparator(strIt));
    ++strIt;
  }
}

TEST(CheckChar, IsNumberWithSeparatorSpase) {
  s21::CheckChar checkCh(s21::DecimalSeparator::kPoint,
                         s21::DigitSeparator::kSpase);
  std::string str = "\'13.78e[R !0O";
  auto strIt = str.begin();
  bool result[] = {false, true,  true, false, true, true, false,
                   false, false, true, false, true, false};
  for (int i = 0; i < 13; ++i) {
    EXPECT_EQ(result[i], checkCh.IsNumberWithSeparator(strIt));
    ++strIt;
  }
}

TEST(CheckChar, IsNumberWithSeparatorApostrophe) {
  s21::CheckChar checkCh(s21::DecimalSeparator::kPoint,
                         s21::DigitSeparator::kApostrophe);
  std::string str = "\'13.78e[R !0O";
  auto strIt = str.begin();
  bool result[] = {true,  true,  true,  false, true, true, false,
                   false, false, false, false, true, false};
  for (int i = 0; i < 13; ++i) {
    EXPECT_EQ(result[i], checkCh.IsNumberWithSeparator(strIt));
    ++strIt;
  }
}

TEST(CheckChar, IsNumberWithSeparatorAny) {
  s21::CheckChar checkCh(s21::DecimalSeparator::kPoint,
                         s21::DigitSeparator::kAny);
  std::string str = "\'13.78e[R !0O";
  auto strIt = str.begin();
  bool result[] = {true,  true,  true, false, true, true, false,
                   false, false, true, false, true, false};
  for (int i = 0; i < 13; ++i) {
    EXPECT_EQ(result[i], checkCh.IsNumberWithSeparator(strIt));
    ++strIt;
  }
}

TEST(CheckChar, IsDecimalSeparatorPoint) {
  s21::CheckChar checkCh(s21::DecimalSeparator::kPoint);
  std::string str = "\t13.7,. !0,O";
  auto strIt = str.begin();
  bool result[] = {false, false, false, true,  false, false,
                   true,  false, false, false, false, false};
  for (int i = 0; i < 12; ++i) {
    EXPECT_EQ(result[i], checkCh.IsDecimalSeparator(strIt));
    ++strIt;
  }
}

TEST(CheckChar, IsDecimalSeparatorComma) {
  s21::CheckChar checkCh(s21::DecimalSeparator::kComma);
  std::string str = "\t13.7,. !0,O";
  auto strIt = str.begin();
  bool result[] = {false, false, false, false, false, true,
                   false, false, false, false, true,  false};
  for (int i = 0; i < 12; ++i) {
    EXPECT_EQ(result[i], checkCh.IsDecimalSeparator(strIt));
    ++strIt;
  }
}

TEST(CheckChar, IsDecimalSeparatorAny) {
  s21::CheckChar checkCh(s21::DecimalSeparator::kAny);
  std::string str = "\t13.7,. !0,O";
  auto strIt = str.begin();
  bool result[] = {false, false, false, true,  false, true,
                   true,  false, false, false, true,  false};
  for (int i = 0; i < 12; ++i) {
    EXPECT_EQ(result[i], checkCh.IsDecimalSeparator(strIt));
    ++strIt;
  }
}

TEST(CheckChar, IsDash) {
  s21::CheckChar checkCh;
  std::string str = "\t-.7--. !0_O---";
  auto strIt = str.begin();
  int result[] = {0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 2, 1};
  for (int i = 0; i < 9; ++i) {
    int res = checkCh.IsDash(strIt);
    EXPECT_EQ(result[i], res);
    if (res == 0) ++strIt;
  }
}

TEST(ParserLexeme, LexemeEqual) {
  s21::ParserLexeme lexem;
  std::string str = "aboba 123fun...---...";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_FALSE(lexem.LexemeEqual(strIt, endString, "ba"));
  EXPECT_EQ(strIt, str.begin());
  EXPECT_TRUE(lexem.LexemeEqual(strIt, endString, "ab"));
  EXPECT_EQ(*strIt, 'o');
  EXPECT_FALSE(lexem.LexemeEqual(strIt, endString, "ab"));
  EXPECT_EQ(*strIt, 'o');
  EXPECT_TRUE(lexem.LexemeEqual(strIt, endString, "oba 1"));
  EXPECT_EQ(*strIt, '2');
  EXPECT_TRUE(lexem.LexemeEqual(strIt, endString, "23fun..."));
  EXPECT_EQ(*strIt, '-');
  EXPECT_FALSE(lexem.LexemeEqual(strIt, endString, "---...7"));
  EXPECT_EQ(*strIt, '-');
  EXPECT_TRUE(lexem.LexemeEqual(strIt, endString, "---..."));
  EXPECT_EQ(strIt, endString);
  EXPECT_FALSE(lexem.LexemeEqual(strIt, endString, "???"));
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, LexemeEqualVector) {
  s21::ParserLexeme lexem;
  std::string str = "aboba 123fun...---...", res = "";
  auto strIt = str.begin(), endString = str.end();
  std::vector<std::string> lexemes = {"ba",    "ab",      "23fun...",
                                      "oba 1", "---...7", "---..."};
  EXPECT_TRUE(lexem.LexemeEqualVector(strIt, endString, lexemes, res));
  EXPECT_EQ(res, "ab");
  EXPECT_EQ(*strIt, 'o');
  EXPECT_TRUE(lexem.LexemeEqualVector(strIt, endString, lexemes, res));
  EXPECT_EQ(res, "oba 1");
  EXPECT_EQ(*strIt, '2');
  EXPECT_TRUE(lexem.LexemeEqualVector(strIt, endString, lexemes, res));
  EXPECT_EQ(res, "23fun...");
  EXPECT_EQ(*strIt, '-');
  EXPECT_TRUE(lexem.LexemeEqualVector(strIt, endString, lexemes, res));
  EXPECT_EQ(res, "---...");
  EXPECT_EQ(strIt, endString);
  EXPECT_FALSE(lexem.LexemeEqualVector(strIt, endString, lexemes, res));
  EXPECT_EQ(res, "---...");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseIntegerNo) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kNo);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7.5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "3");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "6384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "7");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseIntegerSpace) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kSpase);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7.5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "3");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "0476384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "7");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseIntegerApostrophe) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kApostrophe);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7.5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "123");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "6384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "7");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseIntegerAny) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kAny);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7.5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "123");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "0476384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "7");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseInteger(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoublePointNo) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kNo);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3.56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "6384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoublePointSpace) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kSpase);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3.56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0476384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoublePointApostrophe) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kApostrophe);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "123.56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "6384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoublePointAny) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kAny);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "123.56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0476384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoubleCommaNo) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kComma,
                          s21::DigitSeparator::kNo);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "6384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7.5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoubleCommaSpasce) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kComma,
                          s21::DigitSeparator::kSpase);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0476384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7.5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoubleCommaApostrophe) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kComma,
                          s21::DigitSeparator::kApostrophe);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "123");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "6384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7.5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoubleCommaAny) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kComma,
                          s21::DigitSeparator::kAny);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "123");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0476384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7.5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoubleAnyNo) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kAny,
                          s21::DigitSeparator::kNo);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3.56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "6384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7.5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoubleAnySpace) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kAny,
                          s21::DigitSeparator::kSpase);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3.56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0476384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7.5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoubleAnyApostrophe) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kAny,
                          s21::DigitSeparator::kApostrophe);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "123.56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "6384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7.5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseDoubleAnyAny) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kAny,
                          s21::DigitSeparator::kAny);
  std::string str = "a12'3.56e+3t O0 47 6384e-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "123.56");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '+');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "0476384");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "8");
  EXPECT_EQ(*strIt, 'e');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(*strIt, '-');
  ++strIt;
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "7.5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseDouble(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationPointNo) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kNo);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "3.56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "6384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationPointSpace) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kSpase);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "3.56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0476384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationPointApostrophe) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kApostrophe);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "123.56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "6384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationPointAny) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kPoint,
                          s21::DigitSeparator::kAny);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "123.56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0476384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationCommaNo) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kComma,
                          s21::DigitSeparator::kNo);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "3");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "6384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationCommaSpace) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kComma,
                          s21::DigitSeparator::kSpase);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "3");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0476384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationCommaApostrophe) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kComma,
                          s21::DigitSeparator::kApostrophe);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "123");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "6384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationCommaAny) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kComma,
                          s21::DigitSeparator::kAny);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "123");
  EXPECT_EQ(*strIt, '.');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0476384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationAnyNo) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kAny,
                          s21::DigitSeparator::kNo);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "3.56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "6384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationAnySpase) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kAny,
                          s21::DigitSeparator::kSpase);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "12");
  EXPECT_EQ(*strIt, '\'');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "3.56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0476384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationAnyApostrophe) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kAny,
                          s21::DigitSeparator::kApostrophe);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "123.56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "47");
  EXPECT_EQ(*strIt, ' ');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "6384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserLexeme, ParseSciNotationAnyAny) {
  s21::ParserLexeme lexem(s21::DecimalSeparator::kAny,
                          s21::DigitSeparator::kAny);
  std::string str = "a12'3.56e+3t O0 47 6384E-R8e-7,5";
  auto strIt = str.begin(), endString = str.end();
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'a');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "123.56e+3");
  EXPECT_EQ(*strIt, 't');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(*strIt, 'O');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "0476384E-");
  EXPECT_EQ(*strIt, 'R');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "8e-7");
  EXPECT_EQ(*strIt, ',');
  ++strIt;
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "5");
  EXPECT_EQ(strIt, endString);
  EXPECT_EQ(lexem.ParseSciNotation(strIt, endString), "");
  EXPECT_EQ(strIt, endString);
}

TEST(ParserString, Parse) {
  s21::ParserString source;
  std::string str = "r8'3,6e-7 oNe-three   two\t sEvEn0 5.7e8";
  std::vector<std::string> result;
  std::vector<std::string> lexemes{"zero",  "one",  "two", "three",
                                   "four",  "five", "six", "seven",
                                   "eight", "nine", "ten"};
  std::vector<std::string> spaces{" ", "\t", "_", ".", "-"};
  std::vector<std::string> unsuccess;
  EXPECT_TRUE(source.Parse(str, result, lexemes, spaces, unsuccess));
  auto it = result.begin();
  EXPECT_EQ(*it, "one");
  ++it;
  EXPECT_EQ(*it, "three");
  ++it;
  EXPECT_EQ(*it, "two");
  ++it;
  EXPECT_EQ(*it, "seven");
  ++it;
  EXPECT_EQ(it, result.end());
  it = unsuccess.begin();
  EXPECT_EQ(*it, "r8'3,6e");
  ++it;
  EXPECT_EQ(*it, "7");
  ++it;
  EXPECT_EQ(*it, "0");
  ++it;
  EXPECT_EQ(*it, "5");
  ++it;
  EXPECT_EQ(*it, "7e8");
  ++it;
  EXPECT_EQ(it, unsuccess.end());
  source.SetFlags(false);
  EXPECT_TRUE(source.Parse(str, result, lexemes, spaces, unsuccess));
  it = result.begin();
  EXPECT_EQ(*it, "three");
  ++it;
  EXPECT_EQ(*it, "two");
  ++it;
  EXPECT_EQ(it, result.end());
  it = unsuccess.begin();
  EXPECT_EQ(*it, "r8'3,6e");
  ++it;
  EXPECT_EQ(*it, "7");
  ++it;
  EXPECT_EQ(*it, "oNe");
  ++it;
  EXPECT_EQ(*it, "sEvEn0");
  ++it;
  EXPECT_EQ(*it, "5");
  ++it;
  EXPECT_EQ(*it, "7e8");
  ++it;
  EXPECT_EQ(it, unsuccess.end());
  EXPECT_FALSE(source.Parse("zero.zero", result, lexemes, spaces, unsuccess));
  it = result.begin();
  EXPECT_EQ(*it, "zero");
  ++it;
  EXPECT_EQ(*it, "zero");
  ++it;
  EXPECT_EQ(it, result.end());
  EXPECT_TRUE(unsuccess.empty());
}

TEST(ParserString, ParseMathString) {
  s21::ParserString source;
  std::string str = "r8'3,6e-7 oNe-three   two\t sEvEn0 5.7e8";
  std::vector<std::pair<bool, std::string>> result;
  std::vector<std::string> lexemes{"zero",  "one",  "two", "three",
                                   "four",  "five", "six", "seven",
                                   "eight", "nine", "ten"};
  std::vector<std::string> spaces{" ", "\t", "_", ".", "-"};
  std::vector<std::string> unsuccess;
  EXPECT_TRUE(source.ParseMathString(str, result, lexemes, spaces, unsuccess));
  auto it = result.begin();
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "8");
  ++it;
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "3.6e-7");
  ++it;
  EXPECT_FALSE((*it).first);
  EXPECT_EQ((*it).second, "one");
  ++it;
  EXPECT_FALSE((*it).first);
  EXPECT_EQ((*it).second, "three");
  ++it;
  EXPECT_FALSE((*it).first);
  EXPECT_EQ((*it).second, "two");
  ++it;
  EXPECT_FALSE((*it).first);
  EXPECT_EQ((*it).second, "seven");
  ++it;
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "0");
  ++it;
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "5.7e8");
  ++it;
  EXPECT_EQ(it, result.end());
  auto it2 = unsuccess.begin();
  EXPECT_EQ(*it2, "r");
  ++it2;
  EXPECT_EQ(*it2, "'");
  ++it2;
  EXPECT_EQ(it2, unsuccess.end());
  source.SetFlags(true, true, false, s21::DecimalSeparator::kAny,
                  s21::DigitSeparator::kAny);
  str = "4 0.9 zero\t 70'001";
  EXPECT_FALSE(source.ParseMathString(str, result, lexemes, spaces, unsuccess));
  it = result.begin();
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "40");
  ++it;
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "9");
  ++it;
  EXPECT_FALSE((*it).first);
  EXPECT_EQ((*it).second, "zero");
  ++it;
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "70001");
  ++it;
  EXPECT_EQ(it, result.end());
  EXPECT_TRUE(unsuccess.empty());
  source.SetFlags(false, false, false, s21::DecimalSeparator::kPoint,
                  s21::DigitSeparator::kNo);
  str = ".9 zerO\t 70'001";
  EXPECT_TRUE(source.ParseMathString(str, result, lexemes, spaces, unsuccess));
  it = result.begin();
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, ".9");
  ++it;
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "70");
  ++it;
  EXPECT_TRUE((*it).first);
  EXPECT_EQ((*it).second, "001");
  ++it;
  EXPECT_EQ(it, result.end());
  it2 = unsuccess.begin();
  EXPECT_EQ(*it2, "zerO");
  ++it2;
  EXPECT_EQ(*it2, "\'");
  ++it2;
  EXPECT_EQ(it2, unsuccess.end());
}

TEST(ParserString, UnsuccessType) {
  s21::ParserString source;
  std::string str = "r80r8";
  std::vector<std::string> result{".", "e", "0r8"};
  EXPECT_EQ(source.UnsuccessType(str, result, false), "");
  str = ".87";
  EXPECT_EQ(source.UnsuccessType(str, result, false), ".");
  EXPECT_EQ(source.UnsuccessType(str, result, true), "0");
  str = "e.yu";
  EXPECT_EQ(source.UnsuccessType(str, result, false), "e");
  str = "0r8,";
  EXPECT_EQ(source.UnsuccessType(str, result, false), "0r8");
  str = "3r5";
  EXPECT_EQ(source.UnsuccessType(str, result, true), "0");
}
