#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include "bimap.hpp"
#include "menubar.h"
#include "statusbar.h"
#include "tabwidget.h"
#include "textedit.h"

class MainWindow : public QMainWindow {
  Q_OBJECT

  void connect_components();
  void connect_textedit(TextEdit* t);
  void update_status(int index);

 signals:
  void edit_font_changed();

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 public:
  void file_close(int index);
  void file_open(const QString& fileName);
  QString once_read_text(QFile& file);

  void file_save(int index);
  void file_save_as(int index);
  void file_save_all();

  void file_close_all();
  QWidget* file_new(const QString& name, const QString& content = "");
  void file_reopen();

  void edit_go();

  void view_select_edit_font();

  void zoom_edit_font(int delta);
  void zoom_view_font(int delta);

 private:
  static constexpr int OneMB_ = 1024 * 1024;

  const QString defaultCodec_{"UTF-8"};

  BiMap<QString, QWidget*> openedFiles_;
  QHash<QWidget*, QString> textCodecs_;

  QString xedPath_{QDir::homePath() + "/xed/"};

  QFont editFont_{"Consolas", 24};
  QFont viewFont_{"宋体", 16};

  TabWidget* tabWidget_;
  MenuBar* menuBar_;
  StatusBar* statusBar_;

 protected:
  void closeEvent(QCloseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;
  void showEvent(QShowEvent* event) override;
};
#endif  // MAINWINDOW_H
