#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  auto w = std::make_unique<MainWindow>();
  w->show();

  QTranslator translator;
  if (translator.load(":/resfiles/tr/qt_zh_CN.qm")) {
    QCoreApplication::installTranslator(&translator);

    qDebug() << "translator successed";
  } else {
    qDebug() << "translator failed";
  }

  return a.exec();
}
