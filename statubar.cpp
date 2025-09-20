#include "statubar.h"

StatuBar::StatuBar(QWidget *parent) : QStatusBar{parent} {
  codecBox_ = new QComboBox(this);
  codecBox_->addItems({"UTF-8", "ANSI", "GBK", "GB18030", "UTF-16"});

  addPermanentWidget(codecBox_);
}
