#include "selector.h"
#include <QVBoxLayout>

Selector::Selector(const QString& title, QWidget* parent)
    : QDialog{parent} {
  setWindowTitle(title);

  input_ = new QLineEdit(this);
  list_ = new QListWidget(this);
  QVBoxLayout* vl = new QVBoxLayout();

  vl->addWidget(input_);
  vl->addWidget(list_);

  vl->setSpacing(0);
  setLayout(vl);
  resize(800, 400);
}
