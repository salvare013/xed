#include "codeclabel.h"
#include <qdebug.h>
#include <QIcon>
#include <QMouseEvent>
#include <QTextCodec>

CodecLabel::CodecLabel(QWidget* parent) : QLabel{parent} {
  cs_ = new CodecSelector(this);

  connect(cs_, &CodecSelector::codec_clicked,
          [=](const QString& codec) {
            setText(codec);
            cs_->close();
          });
}

void CodecLabel::open_selector() {
  cs_->exec();
}

void CodecLabel::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    pressedPos_ = event->pos();
  }
  QLabel::mousePressEvent(event);
}

void CodecLabel::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton && !pressedPos_.isNull()) {
    if ((event->pos() - pressedPos_).manhattanLength() < 10) {
      emit clicked();
    }
    pressedPos_ = QPoint();
  }
  QLabel::mouseReleaseEvent(event);
}
