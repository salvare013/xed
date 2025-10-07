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

  QStringList arguments = QCoreApplication::arguments();
  qDebug() << arguments;
  if (arguments.size() > 1) {
    for (int i = 1; i < arguments.size(); ++i) {
      w->file_open(arguments[i]);
    }
  }

  return a.exec();
}
