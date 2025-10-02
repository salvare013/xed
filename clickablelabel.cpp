#include "clickablelabel.h"
#include <qdebug.h>
#include <QMouseEvent>

ClickableLabel::ClickableLabel(QWidget* parent) : QLabel{parent} {}

void ClickableLabel::set_selector(const QString& title,
                                  const QStringList& list) {
  selector_ = new Selector(title, this);
  selector_->list_->addItems(list);

  connect(selector_->input_, &QLineEdit::returnPressed, [=] {
    selector_->list_->clear();
    selector_->list_->addItems(list);

    QStringList tmpList;

    for (auto e : selector_->list_->findItems(
             selector_->input_->text(), Qt::MatchContains)) {
      tmpList << e->text();
    }

    selector_->list_->clear();
    selector_->list_->addItems(tmpList);
  });
  connect(selector_->list_, &QListWidget::itemClicked,
          [=](QListWidgetItem* item) {
            setText(item->text());
            selector_->close();
          });
}

void ClickableLabel::open_selector() {
  if (selector_ == nullptr) {
    return;
  }
  selector_->exec();
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    pressedPos_ = event->pos();
  }
  QLabel::mousePressEvent(event);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton && !pressedPos_.isNull()) {
    if ((event->pos() - pressedPos_).manhattanLength() < 10) {
      emit clicked();
    }
    pressedPos_ = QPoint();
  }
  QLabel::mouseReleaseEvent(event);
}
