#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QListWidget>
#include "selector.h"

class ClickableLabel : public QLabel {
  Q_OBJECT

  QPoint pressedPos_ = QPoint();
  Selector* selector_ = nullptr;

 public:
  explicit ClickableLabel(QWidget* parent);

  void set_selector(const QString& title, const QStringList& list);
  void open_selector();

 signals:
  void clicked();

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif  // CLICKABLELABEL_H
