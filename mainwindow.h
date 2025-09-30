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
  void connect_textedit(TextEdit* t);

 signals:
  void edit_font_changed();

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 public slots:
  void file_close(int index);
  void file_open();
  void file_save(int index);
  void file_save_as(int index);
  void file_close_all();
  void file_new_text();

  void reopen(const QString& codec);

  void zoom_edit_font(int delta);
  void zoom_view_font(int delta);

 private:
  const QString defaultCodec_{"UTF-8"};

  BiMap<QString, QWidget*> openedFiles_;
  QHash<QWidget*, QString> textCodecs_;

  QDir xedPath_{QDir::homePath() + "/xed/"};

  QFont editFont_{"Consolas", 24};
  QFont viewFont_{"宋体", 16};

  TabWidget* tabWidget_;
  MenuBar* menubar_;
  StatuBar* statubar_;

 protected:
  void closeEvent(QCloseEvent* event) override;
  void wheelEvent(QWheelEvent* e) override;
};
#endif  // MAINWINDOW_H
