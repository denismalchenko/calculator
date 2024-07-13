#ifndef CPP3_S21_SMARTCALCULATOR_CONTROLLER_CONTROLLER_H_
#define CPP3_S21_SMARTCALCULATOR_CONTROLLER_CONTROLLER_H_

#include <QDate>
#include <QMainWindow>

#include "../model/bankcalculator.h"
#include "../model/expression.h"

namespace s21 {

struct CreditIn {
  QString creditAmount_;
  QString period_;
  int periodType_;
  QString interestRate_;
  int paymentType_;
};

struct CreditOut {
  QVector<QString> monthlyPayment_;
  QString totalPayment_;
  QString overPayment_;
};

struct PlotIn {
  QString expression_;
  QString xMin_;
  QString xMax_;
  QString yMin_;
  QString yMax_;
};

struct DepositIn {
  QString period_;
  int periodType_;
  int frequencyType_;
  QString interestRate_;
  bool capitalization_;
  QString depositAmount_;
  QDate startDate_;
  QVector<QString> deposits_;
  QVector<QDate> depositsDate_;
  QVector<QString> withdrawals_;
  QVector<QDate> withdrawalsDate_;
};

struct DepositOut {
  QString amount_;
  QString accruedInterest_;
  QString tax_;
};

class Controller {
 public:
  Controller() = default;
  QString ExpressionStatus(QString expression);
  QString ExpressionResult(QString expression, QString xValue);
  std::pair<QVector<double>, QVector<double>> Plot(PlotIn &plotIn);
  s21::CreditOut Credit(s21::CreditIn &creditIn);
  s21::DepositOut Deposit(s21::DepositIn &depositIn);

 private:
  s21::BankCalculator bank_;
  s21::Expression expression_;
};

}  // namespace s21

#endif  // CPP3_S21_SMARTCALCULATOR_CONTROLLER_CONTROLLER_H_
