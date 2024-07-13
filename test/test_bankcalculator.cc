#include <gtest/gtest.h>

#include <cmath>

#include "../model/bankcalculator.h"

TEST(BankCalculator, AnnuityCredit1) {
  s21::BankCalculator bank;
  double monthlyPayment = 0, totalPayment = 0, overPayment = 0;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.AnnuityCredit(10000, 24, 10.0);
  EXPECT_NEAR(461.45, monthlyPayment, 0.001);
  EXPECT_NEAR(11074.8, totalPayment, 0.001);
  EXPECT_NEAR(1074.8, overPayment, 0.001);
}

TEST(BankCalculator, AnnuityCredit2) {
  s21::BankCalculator bank;
  double monthlyPayment = 0, totalPayment = 0, overPayment = 0;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.AnnuityCredit(10000, 0, 10.0);
  EXPECT_TRUE(std::isnan(monthlyPayment));
  EXPECT_TRUE(std::isnan(totalPayment));
  EXPECT_TRUE(std::isnan(overPayment));
}

TEST(BankCalculator, AnnuityCredit3) {
  s21::BankCalculator bank;
  double monthlyPayment = 0, totalPayment = 0, overPayment = 0;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.AnnuityCredit(-1, 10, 10.0);
  EXPECT_TRUE(std::isnan(monthlyPayment));
  EXPECT_TRUE(std::isnan(totalPayment));
  EXPECT_TRUE(std::isnan(overPayment));
}

TEST(BankCalculator, AnnuityCredit4) {
  s21::BankCalculator bank;
  double monthlyPayment = 0, totalPayment = 0, overPayment = 0;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.AnnuityCredit(10000, 10, 0.0);
  EXPECT_EQ(1000, monthlyPayment);
  EXPECT_EQ(10000, totalPayment);
  EXPECT_EQ(0, overPayment);
}

TEST(BankCalculator, AnnuityCredit5) {
  s21::BankCalculator bank;
  double monthlyPayment = 0, totalPayment = 0, overPayment = 0;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.AnnuityCredit(10000, 10, -10.0);
  EXPECT_NEAR(954.74, monthlyPayment, 0.001);
  EXPECT_NEAR(9547.4, totalPayment, 0.001);
  EXPECT_NEAR(-452.6, overPayment, 0.001);
}

TEST(BankCalculator, DifferentiatedCredit1) {
  s21::BankCalculator bank;
  double totalPayment = 0, overPayment = 0;
  std::vector<double> monthlyPayment;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.DifferentiatedCredit(10000, 3, 10.0);
  EXPECT_EQ(3, monthlyPayment.size());
  EXPECT_NEAR(3416.67, monthlyPayment[0], 0.001);
  EXPECT_NEAR(3388.89, monthlyPayment[1], 0.001);
  EXPECT_NEAR(3361.11, monthlyPayment[2], 0.001);
  EXPECT_NEAR(10166.67, totalPayment, 0.001);
  EXPECT_NEAR(166.67, overPayment, 0.001);
}

TEST(BankCalculator, DifferentiatedCredit2) {
  s21::BankCalculator bank;
  double totalPayment = 0, overPayment = 0;
  std::vector<double> monthlyPayment;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.DifferentiatedCredit(10000, 0, 10.0);
  EXPECT_TRUE(monthlyPayment.empty());
  EXPECT_TRUE(std::isnan(totalPayment));
  EXPECT_TRUE(std::isnan(overPayment));
}

TEST(BankCalculator, DifferentiatedCredit3) {
  s21::BankCalculator bank;
  double totalPayment = 0, overPayment = 0;
  std::vector<double> monthlyPayment;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.DifferentiatedCredit(-1, 10, 10.0);
  EXPECT_TRUE(monthlyPayment.empty());
  EXPECT_TRUE(std::isnan(totalPayment));
  EXPECT_TRUE(std::isnan(overPayment));
}

TEST(BankCalculator, DifferentiatedCredit4) {
  s21::BankCalculator bank;
  double totalPayment = 0, overPayment = 0;
  std::vector<double> monthlyPayment;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.DifferentiatedCredit(10000, 10, 0.0);
  EXPECT_EQ(10, monthlyPayment.size());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(1000, monthlyPayment[i]);
  }
  EXPECT_EQ(10000, totalPayment);
  EXPECT_EQ(0, overPayment);
}

TEST(BankCalculator, DifferentiatedCredit5) {
  s21::BankCalculator bank;
  double totalPayment = 0, overPayment = 0;
  std::vector<double> monthlyPayment;
  std::tie(monthlyPayment, totalPayment, overPayment) =
      bank.DifferentiatedCredit(10000, 10, -12.0);
  EXPECT_EQ(10, monthlyPayment.size());
  EXPECT_EQ(900, monthlyPayment[0]);
  EXPECT_EQ(910, monthlyPayment[1]);
  EXPECT_EQ(920, monthlyPayment[2]);
  EXPECT_EQ(930, monthlyPayment[3]);
  EXPECT_EQ(940, monthlyPayment[4]);
  EXPECT_EQ(950, monthlyPayment[5]);
  EXPECT_EQ(960, monthlyPayment[6]);
  EXPECT_EQ(970, monthlyPayment[7]);
  EXPECT_EQ(980, monthlyPayment[8]);
  EXPECT_EQ(990, monthlyPayment[9]);
  EXPECT_EQ(9450, totalPayment);
  EXPECT_EQ(-550, overPayment);
}

TEST(BankCalculator, Deposit1) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2023, 6, 20}, 1000}, {{2024, 6, 20}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(4.0, s21::Periods::kEnd, true, movements);
  EXPECT_NEAR(1040.06, amount, 0.001);
  EXPECT_NEAR(40.06, profit, 0.001);
  EXPECT_NEAR(0, tax, 0.001);
}

TEST(BankCalculator, Deposit2) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2023, 6, 20}, 1000}, {{2024, 1, 20}, 500}, {{2024, 6, 20}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(4.0, s21::Periods::kDay, true, movements);
  EXPECT_NEAR(1549.38, amount, 0.001);
  EXPECT_NEAR(49.38, profit, 0.001);
  EXPECT_NEAR(0, tax, 0.001);
}

TEST(BankCalculator, Deposit3) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2023, 6, 20}, 1000},
      {{2024, 1, 20}, 500},
      {{2023, 12, 20}, -600},
      {{2024, 6, 20}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(4.0, s21::Periods::kMonth, true, movements);
  EXPECT_NEAR(937.05, amount, 0.001);
  EXPECT_NEAR(37.05, profit, 0.001);
  EXPECT_NEAR(0, tax, 0.001);
}

TEST(BankCalculator, Deposit4) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2023, 6, 20}, 1000},
      {{2024, 1, 20}, 500},
      {{2023, 12, 20}, -600},
      {{2024, 6, 20}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(0.0, s21::Periods::kMonth, true, movements);
  EXPECT_NEAR(900, amount, 0.001);
  EXPECT_NEAR(0, profit, 0.001);
  EXPECT_NEAR(0, tax, 0.001);
}

TEST(BankCalculator, Deposit5) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2023, 6, 20}, 1000},
      {{2024, 1, 20}, 500},
      {{2023, 12, 20}, -600},
      {{2024, 6, 20}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(-1.0, s21::Periods::kMonth, true, movements);
  EXPECT_NEAR(890.94, amount, 0.001);
  EXPECT_NEAR(-9.06, profit, 0.001);
  EXPECT_NEAR(0, tax, 0.001);
}

TEST(BankCalculator, Deposit6) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2017, 6, 20}, 1000},
      {{2018, 1, 20}, 500},
      {{2017, 12, 20}, -600},
      {{2018, 6, 20}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(-1.0, s21::Periods::kMonth, true, movements);
  EXPECT_NEAR(890.95, amount, 0.001);
  EXPECT_NEAR(-9.05, profit, 0.001);
  EXPECT_NEAR(0, tax, 0.001);
}

TEST(BankCalculator, Deposit7) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2018, 9, 24}, 700000}, {{2020, 9, 24}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(10.0, s21::Periods::kMonth, true, movements);
  EXPECT_NEAR(854335.36, amount, 0.001);
  EXPECT_NEAR(154335.36, profit, 0.001);
  EXPECT_NEAR(0, tax, 0.001);
}

TEST(BankCalculator, Deposit8) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2018, 9, 24}, 700000}, {{2020, 9, 24}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(13.0, s21::Periods::kMonth, true, movements);
  EXPECT_NEAR(903532.31, amount, 0.001);
  EXPECT_NEAR(203532.31, profit, 0.001);
  EXPECT_NEAR(2777, tax, 0.001);
}

TEST(BankCalculator, Deposit9) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2018, 9, 24}, 700000}};
  std::tie(amount, profit, tax) =
      bank.Deposit(13.0, s21::Periods::kMonth, true, movements);
  EXPECT_TRUE(std::isnan(amount));
  EXPECT_TRUE(std::isnan(profit));
  EXPECT_TRUE(std::isnan(tax));
}

TEST(BankCalculator, Deposit10) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements;
  std::tie(amount, profit, tax) =
      bank.Deposit(13.0, s21::Periods::kMonth, true, movements);
  EXPECT_TRUE(std::isnan(amount));
  EXPECT_TRUE(std::isnan(profit));
  EXPECT_TRUE(std::isnan(tax));
}

TEST(BankCalculator, Deposit11) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2018, 9, 24}, 700000}, {{2018, 9, 23}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(13.0, s21::Periods::kMonth, true, movements);
  EXPECT_TRUE(std::isnan(amount));
  EXPECT_TRUE(std::isnan(profit));
  EXPECT_TRUE(std::isnan(tax));
}

TEST(BankCalculator, Deposit12) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2018, 9, 24}, 700000}, {{2018, 9, 24}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(130.0, s21::Periods::kMonth, true, movements);
  EXPECT_NEAR(700000, amount, 0.001);
  EXPECT_NEAR(0, profit, 0.001);
  EXPECT_NEAR(0, tax, 0.001);
}

TEST(BankCalculator, Deposit13) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2015, 9, 24}, 700000}, {{2018, 9, 24}, 100}};
  std::tie(amount, profit, tax) =
      bank.Deposit(12.0, s21::Periods::kWeek, true, movements);
  EXPECT_NEAR(1002916.78, amount, 0.001);
  EXPECT_NEAR(302916.78, profit, 0.001);
  EXPECT_TRUE(std::isnan(tax));
}

TEST(BankCalculator, Deposit14) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2021, 8, 31}, 1000000}, {{2024, 8, 31}, 100}};
  std::tie(amount, profit, tax) =
      bank.Deposit(20.0, s21::Periods::kQuarter, true, movements);
  EXPECT_NEAR(1796167.23, amount, 0.001);
  EXPECT_NEAR(796167.23, profit, 0.001);
  EXPECT_NEAR(33806, tax, 0.001);
}

TEST(BankCalculator, Deposit15) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2023, 10, 31}, 1000000}, {{2025, 10, 31}, 100}};
  std::tie(amount, profit, tax) =
      bank.Deposit(20.0, s21::Periods::kHalfYear, true, movements);
  EXPECT_NEAR(1464098.79, amount, 0.001);
  EXPECT_NEAR(464098.79, profit, 0.001);
  EXPECT_NEAR(26532, tax, 0.001);
}

TEST(BankCalculator, Deposit16) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2023, 10, 31}, 500000},
      {{2022, 10, 31}, 500000},
      {{2023, 10, 31}, 500000},
      {{2025, 10, 31}, -100}};
  std::tie(amount, profit, tax) =
      bank.Deposit(20.0, s21::Periods::kYear, true, movements);
  EXPECT_NEAR(1439999.99, amount, 0.001);
  EXPECT_NEAR(439999.99, profit, 0.001);
  EXPECT_NEAR(23399, tax, 0.001);
}

TEST(BankCalculator, Deposit17) {
  s21::BankCalculator bank;
  double amount = 0, profit = 0, tax = 0;
  const std::vector<std::pair<s21::Calendar, double>> movements = {
      {{2021, 3, 30}, 20000000}, {{2022, 1, 20}, 500000},
      {{2023, 7, 15}, 100000},   {{2021, 9, 7}, -1000000},
      {{2022, 1, 20}, -100000},  {{2024, 3, 31}, -1000000},
      {{2024, 03, 30}, 0}};
  std::tie(amount, profit, tax) =
      bank.Deposit(50.0, s21::Periods::kMonth, false, movements);
  EXPECT_NEAR(19500000, amount, 0.001);
  EXPECT_NEAR(29213842.37, profit, 0.001);
  EXPECT_NEAR(1637204, tax, 0.001);
}
