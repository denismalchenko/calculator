#include "view.h"

#include <QDoubleValidator>

#include "ui_view.h"

using namespace s21;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  SetupValidators();
  SetupCalculator();
  SetupPlot();
  SetupLineEditInt();
  SetupDeposit();
  SetupRun();
  SetupSwitching();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::SetupValidators() {
  QRegularExpression rx("[-+]?([0-9]*[.])?[0-9]+([eE][-+]?\\d+)?");
  QValidator *validatorReg = new QRegularExpressionValidator(rx, this);
  QList<QLineEdit *> lineEditDouble = {ui->lineEditX, ui->lineEditXMax,
                                       ui->lineEditXMin, ui->lineEditYMax,
                                       ui->lineEditYMin};
  for (auto &iter : lineEditDouble) iter->setValidator(validatorReg);

  QDoubleValidator *validatorPercent =
      new QDoubleValidator(0.01, 2000, 2, this);
  validatorPercent->setLocale(QLocale::C);
  validatorPercent->setNotation(QDoubleValidator::StandardNotation);
  ui->lineEditCreditInterestRate->setValidator(validatorPercent);
  ui->lineEditDepositInterestRate->setValidator(validatorPercent);
}

void MainWindow::SetupCalculator() {
  QList<QPushButton *> digitButton = {
      ui->pushButtonNum0,     ui->pushButtonNum1, ui->pushButtonNum2,
      ui->pushButtonNum3,     ui->pushButtonNum4, ui->pushButtonNum5,
      ui->pushButtonNum6,     ui->pushButtonNum7, ui->pushButtonNum8,
      ui->pushButtonNum9,     ui->pushButtonDot,  ui->pushButtonLBracket,
      ui->pushButtonRBracket, ui->pushButtonX,    ui->pushButtonMod,
      ui->pushButtonPow,      ui->pushButtonSum,  ui->pushButtonFactorial};

  for (auto &iter : digitButton)
    connect(iter, SIGNAL(clicked()), this, SLOT(PushTextButton()));
  QList<QPushButton *> functionButton = {ui->pushButtonSin,  ui->pushButtonCos,
                                         ui->pushButtonTan,  ui->pushButtonAcos,
                                         ui->pushButtonAsin, ui->pushButtonAtan,
                                         ui->pushButtonLn,   ui->pushButtonLog};
  for (auto &iter : functionButton)
    connect(iter, SIGNAL(clicked()), this, SLOT(PushFunctionButton()));
  QList<QPushButton *> otherButton = {ui->pushButtonDel,  ui->pushButtonE,
                                      ui->pushButtonPi,   ui->pushButtonExp,
                                      ui->pushButtonSqrt, ui->pushButtonDiv,
                                      ui->pushButtonMult, ui->pushButtonSub};
  for (auto &iter : otherButton)
    connect(iter, SIGNAL(clicked()), this, SLOT(PushOtherButton()));
}

void MainWindow::SetupPlot() {
  QLinearGradient plotGradient;
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 1);
  plotGradient.setColorAt(0, QColor(34, 34, 34, 255));
  plotGradient.setColorAt(1, QColor(34, 34, 34, 255));
  QBrush brash;
  brash.setColor(QColor(0x24, 0x1f, 0x31, 255));
  ui->widgetPlot->clearGraphs();
  ui->widgetPlot->setBackground(brash);
  ui->widgetPlot->axisRect()->setBackground(plotGradient);
  ui->widgetPlot->xAxis->setBasePen(QPen(Qt::white, 1));
  ui->widgetPlot->yAxis->setBasePen(QPen(Qt::white, 1));
  ui->widgetPlot->xAxis->setTickPen(QPen(Qt::white, 1));
  ui->widgetPlot->yAxis->setTickPen(QPen(Qt::white, 1));
  ui->widgetPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
  ui->widgetPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
  ui->widgetPlot->xAxis->setTickLabelColor(Qt::white);
  ui->widgetPlot->yAxis->setTickLabelColor(Qt::white);
  ui->widgetPlot->xAxis->setLabelColor(Qt::white);
  ui->widgetPlot->yAxis->setLabelColor(Qt::white);
  ui->widgetPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
  ui->widgetPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
  ui->widgetPlot->addGraph();
  ui->widgetPlot->graph(0)->setPen(QColor(0xb2, 0xb1, 0xb8, 255));
  ui->widgetPlot->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssCircle, 1));
  ui->widgetPlot->xAxis->setLabel("x");
  ui->widgetPlot->yAxis->setLabel("y");
  ui->widgetPlot->xAxis->setRange(-10, 10);
  ui->widgetPlot->yAxis->setRange(-10, 10);
  QVector<double> x, y;
  s21::PlotIn plotIn;
  plotIn.expression_ = "0";
  plotIn.xMin_ = plotIn.yMin_ = "-10";
  plotIn.xMax_ = plotIn.yMax_ = "10";
  std::tie(x, y) = controller_.Plot(plotIn);
  ui->widgetPlot->graph(0)->setData(x, y);
  ui->widgetPlot->replot();
}

void MainWindow::SetupLineEditInt() {
  QList<QLineEdit *> lineEditInt = {
      ui->lineEditCreditAmount, ui->lineEditCreditPeriod,
      ui->lineEditDepositInitial, ui->lineEditDepositPeriod};
  for (auto &iter : lineEditInt) {
    connect(iter, SIGNAL(textChanged(QString)), this,
            SLOT(LineEditInt(QString)));
    connect(iter, SIGNAL(editingFinished()), this,
            SLOT(LineEditIntEditingFinished()));
  }
}

void MainWindow::SetupDeposit() {
  QList<QLineEdit *> lineEditMovements = {
      ui->lineEditDeposits01,    ui->lineEditDeposits02,
      ui->lineEditDeposits03,    ui->lineEditDeposits04,
      ui->lineEditDeposits05,    ui->lineEditDeposits06,
      ui->lineEditDeposits07,    ui->lineEditDeposits08,
      ui->lineEditDeposits09,    ui->lineEditDeposits10,
      ui->lineEditDeposits11,    ui->lineEditDeposits12,
      ui->lineEditDeposits13,    ui->lineEditWithdrawals01,
      ui->lineEditWithdrawals02, ui->lineEditWithdrawals03,
      ui->lineEditWithdrawals04, ui->lineEditWithdrawals05,
      ui->lineEditWithdrawals06, ui->lineEditWithdrawals07,
      ui->lineEditWithdrawals08, ui->lineEditWithdrawals09,
      ui->lineEditWithdrawals10, ui->lineEditWithdrawals11,
      ui->lineEditWithdrawals12, ui->lineEditWithdrawals13};
  for (auto &iter : lineEditMovements)
    connect(iter, SIGNAL(textChanged(QString)), this,
            SLOT(LineEditInt(QString)));
}

void MainWindow::SetupRun() {
  connect(ui->pushButtonRun, SIGNAL(clicked()), this, SLOT(PushButtonRun()));
  connect(ui->pushButtonBuildGraph, SIGNAL(clicked()), this,
          SLOT(PushButtonGraph()));
  connect(ui->pushButtonCreditCalculate, SIGNAL(clicked()), this,
          SLOT(PushButtonCreditCalculate()));
  connect(ui->pushButtonDepositCalculate, SIGNAL(clicked()), this,
          SLOT(PushButtonDepositCalculate()));
}

void MainWindow::SetupSwitching() {
  connect(ui->radioButtonCredit, SIGNAL(clicked()), this,
          SLOT(RadioButtonCreditClicked()));
  connect(ui->radioButtonDeposit, SIGNAL(clicked()), this,
          SLOT(RadioButtonDepositClicked()));
  connect(ui->comboBoxPaymentType, SIGNAL(currentIndexChanged(int)), this,
          SLOT(ComboBoxPaymentTypeChanged(int)));
  connect(ui->lineEditInput, SIGNAL(textChanged(QString)), this,
          SLOT(LineEditInputChanged(QString)));
}

void MainWindow::PushTextButton() {
  QPushButton *button = (QPushButton *)sender();
  QString expression = ui->lineEditInput->text();
  int position = ui->lineEditInput->cursorPosition();
  expression.insert(position, button->text());
  ui->lineEditInput->setText(expression);
  ui->lineEditInput->setCursorPosition(position + button->text().size());
}

void MainWindow::PushFunctionButton() {
  QPushButton *button = (QPushButton *)sender();
  QString expression = ui->lineEditInput->text();
  int position = ui->lineEditInput->cursorPosition();
  expression.insert(position, button->text() + "()");
  ui->lineEditInput->setText(expression);
  ui->lineEditInput->setCursorPosition(position + button->text().size() + 1);
}

void MainWindow::PushOtherButton() {
  QPushButton *button = (QPushButton *)sender();
  QString expression = ui->lineEditInput->text();
  if (button->objectName() == "pushButtonDel") {
    ui->lineEditInput->backspace();
  } else {
    int position = ui->lineEditInput->cursorPosition();
    if (button->objectName() == "pushButtonExp") {
      expression.insert(position, 'e');
      position += 1;
    } else if (button->objectName() == "pushButtonSqrt") {
      expression.insert(position, "sqrt()");
      position += 5;
    } else if (button->objectName() == "pushButtonPi") {
      expression.insert(position, "3.141592653589793");
      position += 17;
    } else if (button->objectName() == "pushButtonE") {
      expression.insert(position, "2.718281828459045");
      position += 17;
    } else if (button->objectName() == "pushButtonMult") {
      expression.insert(position, '*');
      position += 1;
    } else if (button->objectName() == "pushButtonDiv") {
      expression.insert(position, '/');
      position += 1;
    } else if (button->objectName() == "pushButtonSub") {
      expression.insert(position, '-');
      position += 1;
    }
    ui->lineEditInput->setText(expression);
    ui->lineEditInput->setCursorPosition(position);
  }
}

void MainWindow::LineEditInt(const QString &arg) {
  QLineEdit *line = (QLineEdit *)sender();
  int position = line->cursorPosition();
  if (position > 0) {
    QChar last_char = arg[position - 1];
    if (last_char.isDigit()) {
      QString numstr = arg;
      numstr.remove(QChar(' '), Qt::CaseInsensitive);
      for (int point = 0, i = numstr.length(); i > 0; --i, ++point) {
        if (point != 0 && point % 3 == 0) {
          numstr.insert(i, ' ');
        }
      }
      line->setText(numstr);
    } else {
      line->backspace();
    }
  }
}

void MainWindow::LineEditIntEditingFinished() {
  QLineEdit *line = (QLineEdit *)sender();
  QString numstr = line->text();
  numstr.remove(QChar(' '), Qt::CaseInsensitive);
  if (numstr.toInt() < 1) line->setText(QString::number(1));
}

void MainWindow::PushButtonRun() {
  ui->lineEditOutput->setText(controller_.ExpressionResult(
      ui->lineEditInput->text(), ui->lineEditX->text()));
}

void MainWindow::PushButtonGraph() {
  ui->widgetPlot->clearGraphs();
  ui->widgetPlot->addGraph();
  QVector<double> x, y;
  s21::PlotIn plotIn;
  plotIn.expression_ = ui->lineEditInput->text();
  plotIn.xMin_ = ui->lineEditXMin->text();
  plotIn.yMin_ = ui->lineEditYMin->text();
  plotIn.xMax_ = ui->lineEditXMax->text();
  plotIn.yMax_ = ui->lineEditYMax->text();
  std::tie(x, y) = controller_.Plot(plotIn);
  double xMin = std::min(plotIn.xMin_.toDouble(), plotIn.xMax_.toDouble());
  double xMax = std::max(plotIn.xMin_.toDouble(), plotIn.xMax_.toDouble());
  double yMin = std::min(plotIn.yMin_.toDouble(), plotIn.yMax_.toDouble());
  double yMax = std::max(plotIn.yMin_.toDouble(), plotIn.yMax_.toDouble());
  if (xMax - xMin < 1e-6) xMax += 1;
  if (yMax - yMin < 1e-6) yMax += 1;
  ui->widgetPlot->xAxis->setRange(xMin, xMax);
  ui->widgetPlot->yAxis->setRange(yMin, yMax);
  ui->widgetPlot->graph(0)->setPen(QColor(0xb2, 0xb1, 0xb8, 255));
  ui->widgetPlot->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssCircle, 1));
  ui->widgetPlot->graph(0)->setData(x, y);
  ui->widgetPlot->replot();
}

void MainWindow::PushButtonCreditCalculate() {
  s21::CreditIn creditIn = {
      ui->lineEditCreditAmount->text().remove(QChar(' '), Qt::CaseInsensitive),
      ui->lineEditCreditPeriod->text().remove(QChar(' '), Qt::CaseInsensitive),
      ui->comboBoxCreditPeriod->currentIndex(),
      ui->lineEditCreditInterestRate->text(),
      ui->comboBoxPaymentType->currentIndex()};
  s21::CreditOut creditOut = controller_.Credit(creditIn);
  int rows = creditOut.monthlyPayment_.count();
  ui->lineEditCreditTotalPayout->setText(creditOut.totalPayment_);
  ui->lineEditCreditOverpayment->setText(creditOut.overPayment_);
  ui->tableWidgetMonthlyPayment->setRowCount(rows);
  for (int i = 0; i < rows; ++i) {
    QTableWidgetItem *tbl = new QTableWidgetItem(1);
    ui->tableWidgetMonthlyPayment->setItem(i, 0, tbl);
    tbl->setText(creditOut.monthlyPayment_[i]);
  }
}

void MainWindow::PushButtonDepositCalculate() {
  QList<QLineEdit *> lineEditDeposits =
                         {ui->lineEditDeposits01, ui->lineEditDeposits02,
                          ui->lineEditDeposits03, ui->lineEditDeposits04,
                          ui->lineEditDeposits05, ui->lineEditDeposits06,
                          ui->lineEditDeposits07, ui->lineEditDeposits08,
                          ui->lineEditDeposits09, ui->lineEditDeposits10,
                          ui->lineEditDeposits11, ui->lineEditDeposits12,
                          ui->lineEditDeposits13},
                     lineEditWithdrawals = {
                         ui->lineEditWithdrawals01, ui->lineEditWithdrawals02,
                         ui->lineEditWithdrawals03, ui->lineEditWithdrawals04,
                         ui->lineEditWithdrawals05, ui->lineEditWithdrawals06,
                         ui->lineEditWithdrawals07, ui->lineEditWithdrawals08,
                         ui->lineEditWithdrawals09, ui->lineEditWithdrawals10,
                         ui->lineEditWithdrawals11, ui->lineEditWithdrawals12,
                         ui->lineEditWithdrawals13};
  QList<QDateEdit *> dateEditDeposits =
                         {ui->dateEditDeposits01, ui->dateEditDeposits02,
                          ui->dateEditDeposits03, ui->dateEditDeposits04,
                          ui->dateEditDeposits05, ui->dateEditDeposits06,
                          ui->dateEditDeposits07, ui->dateEditDeposits08,
                          ui->dateEditDeposits09, ui->dateEditDeposits10,
                          ui->dateEditDeposits11, ui->dateEditDeposits12,
                          ui->dateEditDeposits13},
                     dateEditWithdrawals = {
                         ui->dateEditWithdrawals01, ui->dateEditWithdrawals02,
                         ui->dateEditWithdrawals03, ui->dateEditWithdrawals04,
                         ui->dateEditWithdrawals05, ui->dateEditWithdrawals06,
                         ui->dateEditWithdrawals07, ui->dateEditWithdrawals08,
                         ui->dateEditWithdrawals09, ui->dateEditWithdrawals10,
                         ui->dateEditWithdrawals11, ui->dateEditWithdrawals12,
                         ui->dateEditWithdrawals13};
  int size = std::min({lineEditDeposits.count(), lineEditWithdrawals.count(),
                       dateEditDeposits.count(), dateEditWithdrawals.count()});
  auto itDValue = lineEditDeposits.begin();
  auto itDDate = dateEditDeposits.begin();
  auto itWValue = lineEditWithdrawals.begin();
  auto itWDate = dateEditWithdrawals.begin();
  s21::DepositIn depositIn;
  for (int i = 0; i < size; ++i, ++itDValue, ++itDDate, ++itWValue, ++itWDate) {
    if ((*itDValue)->text().size() > 0) {
      depositIn.deposits_.push_back(
          (*itDValue)->text().remove(QChar(' '), Qt::CaseInsensitive));
      depositIn.depositsDate_.push_back((*itDDate)->date());
    }
    if ((*itWValue)->text().size() > 0) {
      depositIn.withdrawals_.push_back(
          (*itWValue)->text().remove(QChar(' '), Qt::CaseInsensitive));
      depositIn.withdrawalsDate_.push_back((*itWDate)->date());
    }
  }
  depositIn.depositAmount_ = ui->lineEditDepositInitial->text().remove(
      QChar(' '), Qt::CaseInsensitive);
  depositIn.startDate_ = ui->dateEditStartingDate->date();
  depositIn.interestRate_ = ui->lineEditDepositInterestRate->text();
  depositIn.period_ = ui->lineEditDepositPeriod->text();
  depositIn.periodType_ = ui->comboBoxDepositPeriod->currentIndex();
  depositIn.frequencyType_ =
      ui->comboBoxDepositPaymentFrequency->currentIndex();
  depositIn.capitalization_ = ui->checkBoxCapitalization->isChecked();
  s21::DepositOut depositOut = controller_.Deposit(depositIn);
  ui->lineEditDepositAmount->setText(depositOut.amount_);
  ui->lineEditDepositAccruedInterest->setText(depositOut.accruedInterest_);
  ui->lineEditDepositTax->setText(depositOut.tax_);
}

void MainWindow::LineEditInputChanged(const QString &arg) {
  ui->lineEditOutput->setText(controller_.ExpressionStatus(arg));
}

void MainWindow::RadioButtonCreditClicked() {
  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::RadioButtonDepositClicked() {
  ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::ComboBoxPaymentTypeChanged(int index) {
  ui->tableWidgetMonthlyPayment->setRowCount(1);
  if (index == 0) {
    ui->tableWidgetMonthlyPayment->resize(130, 32);
  } else {
    ui->tableWidgetMonthlyPayment->resize(130, 405);
  }
}
