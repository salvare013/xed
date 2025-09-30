#ifndef STATUBAR_H
#define STATUBAR_H

#include <QComboBox>
#include <QLabel>
#include <QStatusBar>
#include "codeclabel.h"

class StatuBar : public QStatusBar {
  Q_OBJECT
 public:
  explicit StatuBar(QWidget* parent = nullptr);

 public:
  QLabel* fontLabel_;
  QLabel* PosLabel_;
  CodecLabel* codecLabel_;

 signals:
};

#endif  // STATUBAR_H
