#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>

class TabWidget : public QTabWidget {
  Q_OBJECT
 public:
  explicit TabWidget(QWidget *parent = nullptr);

 signals:
};

#endif  // TABWIDGET_H
