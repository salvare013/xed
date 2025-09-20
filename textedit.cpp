#include "textedit.h"
#include <QApplication>
#include <QClipboard>
#include <QWheelEvent>

TextEdit::TextEdit(QWidget *parent) : QPlainTextEdit{parent} {
  QFont font = this->font();
  font.setPointSize(24);
  setFont(font);
  setFocus();
}

void TextEdit::zoom_font_size(int delta) {
  QTextCursor cursor = textCursor();
  QFont font = this->font();

  font.setPointSize(qBound(10, font.pointSize() + delta, 128));

  setFont(font);
  setTextCursor(cursor);
}

void TextEdit::wheelEvent(QWheelEvent *e) {
  if (e->modifiers() & Qt::ControlModifier) {
    if (e->angleDelta().y() > 0) {
      zoom_font_size(4);
    } else {
      zoom_font_size(-4);
    }
    e->accept();
    return;
  }
  QPlainTextEdit::wheelEvent(e);
}

void TextEdit::keyPressEvent(QKeyEvent *e) {
  if (e->modifiers() & Qt::ControlModifier) {
    if (e->key() == Qt::Key_V) {
      paste();
      e->accept();
      return;
    }
  }
  QPlainTextEdit::keyPressEvent(e);
}
