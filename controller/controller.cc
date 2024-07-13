#include "controller.h"

namespace s21 {

QString Controller::ExpressionStatus(QString expression) {
  expression_.SetInfixNotation(expression.toStdString());
  return QString::fromStdString(expression_.valid());
}

QString Controller::ExpressionResult(QString expression, QString xValue) {
  expression_.SetInfixNotation(expression.toStdString());
  return QString::fromStdString(expression_.Calculate(xValue.toStdString()));
}

std::pair<QVector<double>, QVector<double>> Controller::Plot(PlotIn &plotIn) {
  unsigned int dotAmount = 20000;
  QVector<double> x(dotAmount), y(dotAmount);
  expression_.SetInfixNotation(plotIn.expression_.toStdString());
  if (QString::fromStdString(expression_.valid()) == "") {
    std::pair<std::vector<double>, std::vector<double>> xy = expression_.Plot(
        plotIn.xMin_.toStdString(), plotIn.xMax_.toStdString(),
        plotIn.yMin_.toStdString(), plotIn.yMax_.toStdString(), dotAmount);
    x = QVector<double>(xy.first.begin(), xy.first.end());
    y = QVector<double>(xy.second.begin(), xy.second.end());
  }
  return std::make_pair(x, y);
}

s21::CreditOut Controller::Credit(s21::CreditIn &creditIn) {
  CreditOut result;
  double totalPayment = 0, overPayment = 0;
  int months = (creditIn.periodType_ == 0) ? creditIn.period_.toInt()
                                           : creditIn.period_.toInt() * 12;
  if (creditIn.paymentType_ == 0) {
    double monthlyPayment;
    std::tie(monthlyPayment, totalPayment, overPayment) =
        bank_.AnnuityCredit(creditIn.creditAmount_.toInt(), months,
                            creditIn.interestRate_.toDouble());
    result.monthlyPayment_.push_back(QString::number(monthlyPayment, 'f', 2));
  } else if (creditIn.paymentType_ == 1) {
    std::vector<double> monthlyPayment;
    std::tie(monthlyPayment, totalPayment, overPayment) =
        bank_.DifferentiatedCredit(creditIn.creditAmount_.toInt(), months,
                                   creditIn.interestRate_.toDouble());
    for (const auto &value : monthlyPayment)
      result.monthlyPayment_.push_back(QString::number(value, 'f', 2));
  } else {
    result.monthlyPayment_.push_back(QString::number(0));
  }
  result.totalPayment_ = QString::number(totalPayment, 'f', 2);
  result.overPayment_ = QString::number(overPayment, 'f', 2);
  return result;
}

s21::DepositOut Controller::Deposit(s21::DepositIn &depositIn) {
  DepositOut result;
  double amount = 0, accruedInterest = 0, tax = 0;
  Periods capitPeriod;
  if (depositIn.frequencyType_ == 0)
    capitPeriod = Periods::kDay;
  else if (depositIn.frequencyType_ == 1)
    capitPeriod = Periods::kMonth;
  else if (depositIn.frequencyType_ == 2)
    capitPeriod = Periods::kYear;
  else
    capitPeriod = Periods::kEnd;
  QDate endDate = depositIn.startDate_;
  if (depositIn.periodType_ == 0)
    endDate = endDate.addMonths(depositIn.period_.toInt());
  else
    endDate = endDate.addYears(depositIn.period_.toInt());

  std::vector<std::pair<Calendar, double>> movements;
  movements.push_back(std::pair(
      Calendar(depositIn.startDate_.year(), depositIn.startDate_.month(),
               depositIn.startDate_.day()),
      depositIn.depositAmount_.toInt()));
  for (int i = 0; i < depositIn.deposits_.count(); ++i) {
    std::pair<Calendar, double> dateValue;
    if (depositIn.deposits_[i].toDouble() > 0 &&
        depositIn.depositsDate_[i] >= depositIn.startDate_) {
      dateValue.first = Calendar(depositIn.depositsDate_[i].year(),
                                 depositIn.depositsDate_[i].month(),
                                 depositIn.depositsDate_[i].day());
      dateValue.second = depositIn.deposits_[i].toDouble();
      movements.push_back(dateValue);
    }
  }
  for (int i = 0; i < depositIn.withdrawals_.count(); ++i) {
    std::pair<Calendar, double> dateValue;
    if (depositIn.withdrawals_[i].toDouble() > 0 &&
        depositIn.withdrawalsDate_[i] >= depositIn.startDate_) {
      dateValue.first = Calendar(depositIn.withdrawalsDate_[i].year(),
                                 depositIn.withdrawalsDate_[i].month(),
                                 depositIn.withdrawalsDate_[i].day());
      dateValue.second = -depositIn.withdrawals_[i].toDouble();
      movements.push_back(dateValue);
    }
  }
  movements.push_back(
      std::pair(Calendar(endDate.year(), endDate.month(), endDate.day()), 0.0));

  std::tie(amount, accruedInterest, tax) =
      bank_.Deposit(depositIn.interestRate_.toDouble(), capitPeriod,
                    depositIn.capitalization_, movements);
  result.amount_ = QString::number(amount, 'f', 2);
  result.accruedInterest_ = QString::number(accruedInterest, 'f', 2);
  result.tax_ = QString::number(tax);
  return result;
}

}  // namespace s21
