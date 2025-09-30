#include "statubar.h"
#include <QTextCodec>

StatuBar::StatuBar(QWidget* parent) : QStatusBar{parent} {
  fontLabel_ = new QLabel(this);
  PosLabel_ = new QLabel(this);
  codecLabel_ = new CodecLabel(this);

  addPermanentWidget(fontLabel_);
  addPermanentWidget(codecLabel_);

  addWidget(PosLabel_);
}
