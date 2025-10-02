#include "statusbar.h"
#include <qdebug.h>
#include <QFontDatabase>
#include <QTextCodec>

StatusBar::StatusBar(QWidget* parent) : QStatusBar{parent} {
  fontLabel_ = new ClickableLabel(this);
  PosLabel_ = new QLabel(this);
  codecLabel_ = new ClickableLabel(this);

  QStringList allCodecs;
  for (auto codec : QTextCodec::availableCodecs()) {
    allCodecs << codec;
  }
  codecLabel_->set_selector(tr("选择编码"), allCodecs);

  fontLabel_->set_selector(tr("选择字体"),
                           QFontDatabase{}.families());

  addPermanentWidget(fontLabel_);
  addPermanentWidget(codecLabel_);
  addPermanentWidget(codecLabel_);

  addWidget(PosLabel_);
}
