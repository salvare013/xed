#include "statubar.h"

StatuBar::StatuBar(QWidget *parent) : QStatusBar{parent} {
  codecBox_ = new QComboBox(this);
  codecBox_->addItems({"UTF-8", "ANSI", "GBK", "GB18030", "UTF-16"});

  fontLabel_ = new QLabel(this);
  PosLabel_ = new QLabel(this);

  addPermanentWidget(fontLabel_);
  addPermanentWidget(codecBox_);

  addWidget(PosLabel_);
}
