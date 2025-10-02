#ifndef STATUBAR_H
#define STATUBAR_H

#include <QComboBox>
#include <QLabel>
#include <QStatusBar>
#include "clickablelabel.h"

class StatusBar : public QStatusBar {
  Q_OBJECT
 public:
  explicit StatusBar(QWidget* parent = nullptr);

 public:
  ClickableLabel* PosLabel_;
  ClickableLabel* codecLabel_;
  ClickableLabel* fontLabel_;

 signals:
};

#endif  // STATUBAR_H
