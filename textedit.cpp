#include "textedit.h"
#include <QApplication>
#include <QClipboard>
#include <QWheelEvent>

TextEdit::TextEdit(const QFont& font, QWidget* parent)
    : QPlainTextEdit{parent} {
  setFont(font);
  setFocus();
}

void TextEdit::wheelEvent(QWheelEvent* e) {
  if (e->modifiers() & Qt::ControlModifier) {
    e->ignore();
    return;
  }
  QPlainTextEdit::wheelEvent(e);
}
