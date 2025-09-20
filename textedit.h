#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>

class TextEdit : public QPlainTextEdit {
  Q_OBJECT
 public:
  explicit TextEdit(QWidget *parent = nullptr);

  void zoom_font_size(int delta);

 protected:
  void wheelEvent(QWheelEvent *e) override;
  void keyPressEvent(QKeyEvent *e) override;

 signals:
};

#endif  // TEXTEDIT_H
