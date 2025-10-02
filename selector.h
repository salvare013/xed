#ifndef SELECTOR_H
#define SELECTOR_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>

class Selector : public QDialog {
  Q_OBJECT
 public:
  explicit Selector(const QString& title, QWidget* parent);

 public:
  QLineEdit* input_;
  QListWidget* list_;

 signals:
};

#endif  // SELECTOR_H
