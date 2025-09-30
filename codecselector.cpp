#include "codecselector.h"
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QTextCodec>
#include <QVBoxLayout>

CodecSelector::CodecSelector(QWidget* parent) {
  QLineEdit* input = new QLineEdit(this);
  QVBoxLayout* vl = new QVBoxLayout();
  QListWidget* codecList = new QListWidget(this);

  setWindowTitle(tr("选择编码"));

  for (auto codec : QTextCodec::availableCodecs()) {
    allCodecs_ << codec;
  }

  codecList->addItems(allCodecs_);

  vl->addWidget(input);
  vl->addWidget(codecList);

  vl->setSpacing(0);

  setLayout(vl);
  resize(800, 400);

  connect(input, &QLineEdit::returnPressed, [=] {
    codecList->clear();
    codecList->addItems(allCodecs_);

    QStringList tmpList;

    for (auto e :
         codecList->findItems(input->text(), Qt::MatchContains)) {
      tmpList << e->text();
    }

    codecList->clear();
    codecList->addItems(tmpList);
  });
  connect(codecList, &QListWidget::itemClicked,
          [=](QListWidgetItem* item) {
            emit codec_clicked(item->text());
          });
}
