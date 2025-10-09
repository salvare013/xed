#include "statusbar.h"
#include <qdebug.h>
#include <QFontDatabase>
#include <QTextCodec>

StatusBar::StatusBar(QWidget* parent) : QStatusBar{parent} {
  fontLabel_ = new ClickableLabel(this);
  PosLabel_ = new ClickableLabel(this);
  codecLabel_ = new ClickableLabel(this);

  QStringList allCodecs;
  for (auto codec : QTextCodec::availableCodecs()) {
    allCodecs << codec;
  }
  codecLabel_->set_selector(tr("选择编码"), allCodecs);
  fontLabel_->set_selector(tr("选择字体"), QFontDatabase{}.families());

  addPermanentWidget(new QLabel(this));
  addPermanentWidget(fontLabel_);
  addPermanentWidget(new QLabel(this));
  addPermanentWidget(codecLabel_);
  addPermanentWidget(new QLabel(this));

  addWidget(PosLabel_);
}
