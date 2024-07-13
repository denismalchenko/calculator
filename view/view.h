#ifndef CPP3_S21_SMARTCALCULATOR_VIEW_VIEW_H
#define CPP3_S21_SMARTCALCULATOR_VIEW_VIEW_H

#include <QMainWindow>

#include "../3dparty/qcustomplot.h"
#include "../controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void SetupValidators();
  void SetupCalculator();
  void SetupPlot();
  void SetupLineEditInt();
  void SetupDeposit();
  void SetupRun();
  void SetupSwitching();

  void PushTextButton();
  void PushFunctionButton();
  void PushOtherButton();
  void LineEditInt(const QString &arg);
  void LineEditIntEditingFinished();

  void PushButtonRun();
  void PushButtonGraph();
  void PushButtonCreditCalculate();
  void PushButtonDepositCalculate();

  void LineEditInputChanged(const QString &arg);
  void RadioButtonCreditClicked();
  void RadioButtonDepositClicked();
  void ComboBoxPaymentTypeChanged(int index);

 private:
  Ui::MainWindow *ui;
  s21::Controller controller_;
  QVector<QString> depositsValue_, withdrawalsValue_;
  QVector<QDate> depositsDate_, withdrawalsDate_;
};
}  // namespace s21
#endif  // CPP3_S21_SMARTCALCULATOR_VIEW_VIEW_H
