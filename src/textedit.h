#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>

class TextEdit : public QPlainTextEdit {
  Q_OBJECT
 public:
  explicit TextEdit(const QFont &font, QWidget *parent = nullptr);

 protected:
  void wheelEvent(QWheelEvent *e) override;

 signals:
};

#endif  // TEXTEDIT_H
