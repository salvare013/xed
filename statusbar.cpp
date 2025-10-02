#include "statusbar.h"
#include <qdebug.h>
#include <QTextCodec>

StatusBar::StatusBar(QWidget* parent) : QStatusBar{parent} {
  fontLabel_ = new QLabel(this);
  PosLabel_ = new QLabel(this);
  codecLabel_ = new ClickableLabel(this);

  QStringList allCodecs;
  for (auto codec : QTextCodec::availableCodecs()) {
    allCodecs << codec;
  }
  codecLabel_->set_selector("选择编码", allCodecs);

  addPermanentWidget(fontLabel_);
  addPermanentWidget(codecLabel_);
  addPermanentWidget(codecLabel_);

  addWidget(PosLabel_);
}
