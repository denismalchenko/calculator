#ifndef CPP3_S21_SMARTCALCULATOR_MODEL_BANKCALCULATOR_H_
#define CPP3_S21_SMARTCALCULATOR_MODEL_BANKCALCULATOR_H_
#include <map>
#include <tuple>
#include <utility>
#include <vector>

#include "calendar.h"

namespace s21 {

class BankCalculator {
 public:
  BankCalculator();
  std::tuple<double, double, double> AnnuityCredit(const int &sum,
                                                   const int &months,
                                                   const double &interestRate);
  std::tuple<std::vector<double>, double, double> DifferentiatedCredit(
      const int &sum, const int &months, const double &interestRate);
  std::tuple<double, double, double> Deposit(
      const double &interestRate, const Periods &capitPeriod,
      const bool &isCapitalizate,
      const std::vector<std::pair<Calendar, double>> &movements);

 private:
  std::map<Calendar, double> keyRateRus_;
  int DepositTaxAfter2021(double amount, int year);
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALCULATOR_MODEL_BANKCALCULATOR_H_
