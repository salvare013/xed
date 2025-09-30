#ifndef CODECSELECTOR_H
#define CODECSELECTOR_H

#include <QDialog>

class CodecSelector : public QDialog {
  Q_OBJECT

  QStringList allCodecs_;

 public:
  explicit CodecSelector(QWidget* parent = nullptr);

 signals:
  void codec_clicked(const QString& codec);
};

#endif  // CODECSELECTOR_H
