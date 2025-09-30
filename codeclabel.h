#ifndef CODECLABEL_H
#define CODECLABEL_H

#include <QLabel>
#include "codecselector.h"

class CodecLabel : public QLabel {
  Q_OBJECT

 private:
  CodecSelector* cs_;
  QPoint pressedPos_;

 public:
  explicit CodecLabel(QWidget* parent = nullptr);

 public slots:
  void open_selector();

 signals:
  void clicked();

 protected:
  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif  // CODECLABEL_H
