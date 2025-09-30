#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  QTranslator tr;
  if (tr.load(":/resfiles/tr/qt_zh_CN.qm")) {
    QCoreApplication::installTranslator(&tr);

    qDebug() << "tr success";
  } else {
    qDebug() << "tr failed";
  }

  auto w = std::make_unique<MainWindow>();
  w->show();

  return a.exec();
}
