#include "statubar.h"
#include <QTextCodec>

StatuBar::StatuBar(QWidget *parent) : QStatusBar{parent} {
  codecBox_ = new QComboBox(this);
  for (auto e : QTextCodec::availableCodecs()) {
    codecBox_->addItem(e);
  }

  fontLabel_ = new QLabel(this);
  PosLabel_ = new QLabel(this);

  addPermanentWidget(fontLabel_);
  addPermanentWidget(codecBox_);

  addWidget(PosLabel_);
}
