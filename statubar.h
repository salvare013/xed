#ifndef STATUBAR_H
#define STATUBAR_H

#include <QComboBox>
#include <QLabel>
#include <QStatusBar>

class StatuBar : public QStatusBar {
  Q_OBJECT
 public:
  explicit StatuBar(QWidget *parent = nullptr);

 public:
  QComboBox *codecBox_;
  QLabel *fontLabel_;
  QLabel *PosLabel_;

 signals:
};

#endif  // STATUBAR_H
