#include "bankcalculator.h"

#include <algorithm>
#include <cmath>

namespace s21 {

BankCalculator::BankCalculator() {
  keyRateRus_[{2013, 2, 2}] = 5.5;
  keyRateRus_[{2014, 3, 3}] = 7;
  keyRateRus_[{2014, 4, 28}] = 7.5;
  keyRateRus_[{2014, 7, 28}] = 8;
  keyRateRus_[{2014, 11, 5}] = 9.5;
  keyRateRus_[{2014, 12, 12}] = 10.5;
  keyRateRus_[{2014, 12, 16}] = 17;
  keyRateRus_[{2015, 2, 2}] = 15;
  keyRateRus_[{2015, 3, 16}] = 14;
  keyRateRus_[{2015, 5, 5}] = 12.5;
  keyRateRus_[{2015, 6, 16}] = 11.5;
  keyRateRus_[{2015, 8, 3}] = 11;
  keyRateRus_[{2016, 6, 14}] = 10.5;
  keyRateRus_[{2016, 9, 19}] = 10;
  keyRateRus_[{2017, 3, 27}] = 9.75;
  keyRateRus_[{2017, 5, 2}] = 9.25;
  keyRateRus_[{2017, 6, 19}] = 9;
  keyRateRus_[{2017, 9, 18}] = 8.5;
  keyRateRus_[{2017, 10, 30}] = 8.25;
  keyRateRus_[{2017, 12, 18}] = 7.75;
  keyRateRus_[{2018, 2, 12}] = 7.5;
  keyRateRus_[{2018, 3, 26}] = 7.25;
  keyRateRus_[{2018, 9, 17}] = 7.5;
  keyRateRus_[{2018, 12, 17}] = 7.75;
  keyRateRus_[{2019, 6, 17}] = 7.5;
  keyRateRus_[{2019, 7, 29}] = 7.25;
  keyRateRus_[{2019, 9, 9}] = 7;
  keyRateRus_[{2019, 10, 28}] = 6.5;
  keyRateRus_[{2019, 12, 16}] = 6.25;
  keyRateRus_[{2020, 2, 10}] = 6;
  keyRateRus_[{2020, 4, 27}] = 5.5;
  keyRateRus_[{2020, 6, 22}] = 4.5;
  keyRateRus_[{2020, 7, 27}] = 4.25;
  keyRateRus_[{2021, 3, 22}] = 4.5;
  keyRateRus_[{2021, 4, 26}] = 5;
  keyRateRus_[{2021, 6, 15}] = 5.5;
  keyRateRus_[{2021, 7, 26}] = 6.5;
  keyRateRus_[{2021, 9, 13}] = 6.75;
  keyRateRus_[{2021, 10, 25}] = 7.5;
  keyRateRus_[{2021, 12, 20}] = 8.5;
  keyRateRus_[{2022, 2, 14}] = 9.5;
  keyRateRus_[{2022, 2, 28}] = 20;
  keyRateRus_[{2022, 4, 11}] = 17;
  keyRateRus_[{2022, 5, 4}] = 14;
  keyRateRus_[{2022, 5, 27}] = 11;
  keyRateRus_[{2022, 6, 14}] = 9.5;
  keyRateRus_[{2022, 7, 25}] = 8;
  keyRateRus_[{2022, 9, 19}] = 7.5;
  keyRateRus_[{2023, 7, 24}] = 8.5;
  keyRateRus_[{2023, 8, 15}] = 12;
  keyRateRus_[{2023, 9, 18}] = 13;
}

std::tuple<double, double, double> BankCalculator::AnnuityCredit(
    const int &sum, const int &months, const double &interestRate) {
  double result = NAN;
  if (sum >= 0 && months > 0) {
    if (interestRate != 0) {
      double koef = interestRate / 1200 + 1;
      double koef_pow = std::pow(koef, months);
      result = round(100 * static_cast<double>(sum) * koef_pow * (koef - 1) /
                     (koef_pow - 1)) /
               100.0;
    } else {
      result = static_cast<double>(sum) / months;
    }
  }
  return {result, result * months, result * months - sum};
}

std::tuple<std::vector<double>, double, double>
BankCalculator::DifferentiatedCredit(const int &sum, const int &months,
                                     const double &interestRate) {
  double allPayments = NAN;
  std::vector<double> result;
  if (sum >= 0 && months > 0) {
    if (interestRate != 0) {
      allPayments = 0;
      double koef = interestRate / 1200;
      double credit = sum, delta = static_cast<double>(sum) / months;
      for (int i = 0; i < months; ++i) {
        result.push_back(round(100 * (koef * credit + delta)) / 100.0);
        credit -= delta;
        allPayments += result.back();
      }
    } else {
      allPayments = sum;
      for (int i = 0; i < months; i++) {
        result.push_back(static_cast<double>(sum) / months);
      }
    }
  }
  return {result, allPayments, allPayments - sum};
}

std::tuple<double, double, double> BankCalculator::Deposit(
    const double &interestRate, const Periods &capitPeriod,
    const bool &isCapitalizate,
    const std::vector<std::pair<Calendar, double>> &movements) {
  if (movements.size() < 2) return {NAN, NAN, NAN};
  Calendar startDay = movements.front().first, endDay = movements.back().first;
  if (endDay < startDay) return {NAN, NAN, NAN};
  std::vector<std::pair<Calendar, double>> sortMovements;
  double amount = 0, profit = 0, tax = 0;
  for (const auto &it : movements) {
    if (startDay < it.first && it.first < endDay)
      sortMovements.push_back(it);
    else if (startDay == it.first)
      amount += it.second;
  }
  if (startDay == endDay) return {amount, 0, 0};
  std::sort(sortMovements.begin(), sortMovements.end());
  std::vector<Calendar> capitalisationDays =
      CalculateAllDates(startDay, endDay, capitPeriod);
  auto itMovement = sortMovements.begin();
  auto itCapitalisation = ++capitalisationDays.begin();
  double yearProfit = 0, adds = 0, capitalization = 0, oldTax = 0,
         koef = interestRate / 100 / startDay.GetDaysInYear();
  if (startDay.year() < 2016) {
    tax = NAN;
  } else if (startDay.year() < 2021) {
    auto it = keyRateRus_.lower_bound(startDay);
    --it;
    if (it->second < interestRate - 5)
      oldTax = (interestRate - it->second - 5) * 0.35 / interestRate;
  }
  for (Calendar date = ++startDay; date != endDay; ++date) {
    if (date.day() == 1 && date.month() == 1) {
      koef = interestRate / 100 / date.GetDaysInYear();
      if (date.year() > 2021) {
        tax += DepositTaxAfter2021(yearProfit, date.year() - 1);
        yearProfit = 0;
      }
    }
    adds += koef * amount;
    if (date == *itCapitalisation) {
      capitalization = std::round(100 * adds) / 100.0;
      adds = 0;
      if (date.year() > 2020) {
        profit += capitalization;
        yearProfit += capitalization;
      } else {
        // std::cout << date << ": %:" << capitalization
        //           << " tax:" << round(oldTax * capitalization);
        tax += round(oldTax * capitalization);
        capitalization -= round(oldTax * capitalization);
        profit += capitalization;
        // std::cout << " profit: " << capitalization << " all tax: " << tax
        //           << std::endl;
      }
      if (isCapitalizate) amount += capitalization;
      // std::cout << date << ": " << amount << " adds: " << adds <<
      // std::endl;
      ++itCapitalisation;
    }
    for (; itMovement != sortMovements.end() && date == itMovement->first;
         ++itMovement) {
      amount += itMovement->second;
      if (amount < 0) amount = 0;
      // std::cout << date << ": " << amount << std::endl;
    }
  }
  adds += koef * amount;
  capitalization = std::round(100 * adds) / 100.0;
  if (endDay.year() > 2020) {
    profit += capitalization;
    yearProfit += capitalization;
    tax += DepositTaxAfter2021(yearProfit, endDay.year());
  } else {
    // std::cout << endDay << ": %:" << capitalization
    //           << " tax:" << round(oldTax * capitalization);
    tax += round(oldTax * capitalization);
    capitalization -= round(oldTax * capitalization);
    profit += capitalization;
    // std::cout << " profit: " << capitalization << " all tax: " << tax
    //           << std::endl;
  }
  if (isCapitalizate) amount += capitalization;
  // std::cout << endDay << ": " << amount << std::endl;
  return {amount, profit, tax};
}

int BankCalculator::DepositTaxAfter2021(double amount, int year) {
  if (year < 2023) return 0;
  double maxKeyRate = 0;
  for (short unsigned int i = 1; i < 13; ++i) {
    auto it = keyRateRus_.lower_bound({year, i, 1});
    --it;
    maxKeyRate = std::max(maxKeyRate, it->second);
  }
  if (10000 * maxKeyRate > amount) return 0;
  double taxBase = amount - 10000 * maxKeyRate;
  return (taxBase < 5000000) ? 0.13 * taxBase
                             : 0.13 * 5000000 + 0.15 * (taxBase - 5000000);
}

}  // namespace s21
