#include <QApplication>
#include <QFile>

#include "view/view.h"

using namespace s21;

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  a.setWindowIcon(QIcon(":/3dparty/icon.png"));
  QFile res(":/3dparty/FreeSerif.ttf");
  QFont f;
  f.setFamily("FreeSerif");
  f.setPointSize(15);
  MainWindow w;
  w.setFont(f);
  w.show();
  return a.exec();
}
