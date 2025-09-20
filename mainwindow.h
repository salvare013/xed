#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include "bimap.hpp"
#include "menubar.h"
#include "statubar.h"
#include "tabwidget.h"
#include "textedit.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

  void connect_components();
  void connect_textedit(const TextEdit *t);

 signals:

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 public slots:
  void file_close(int index);
  void file_open();
  void file_save(int index);
  void file_save_as(int index);
  void file_close_all();
  void file_new_text();

  void reopen(const QString &codec);

 private:
  BiMap<QString, QWidget *> openedFiles_;
  QHash<QWidget *, int> textCodecIndexs_;

  QDir xedPath_{QDir::homePath() + "/xed/"};

  TabWidget *tabWidget_;
  MenuBar *menubar_;
  StatuBar *statubar_;

 protected:
  void closeEvent(QCloseEvent *event) override;
};
#endif  // MAINWINDOW_H
