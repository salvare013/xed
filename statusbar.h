#ifndef STATUBAR_H
#define STATUBAR_H

#include <QComboBox>
#include <QLabel>
#include <QStatusBar>
#include "codeclabel.h"

class StatusBar : public QStatusBar {
  Q_OBJECT
 public:
  explicit StatusBar(QWidget* parent = nullptr);

 public:
  QLabel* fontLabel_;
  QLabel* PosLabel_;
  CodecLabel* codecLabel_;

 signals:
};

#endif  // STATUBAR_H
