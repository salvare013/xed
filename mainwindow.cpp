#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "textedit.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  tabWidget_ = new TabWidget(this);
  menubar_ = new MenuBar(this);
  statubar_ = new StatuBar(this);

  setCentralWidget(tabWidget_);
  setMenuBar(menubar_);
  setStatusBar(statubar_);

  setWindowTitle(tr("xed-xmx"));
  setWindowIcon(QIcon(":/resfiles/icon/window.jpg"));
  resize(1200, 900);

  connect_components();

  zoom_view_font(0);
}

MainWindow::~MainWindow() {}

void MainWindow::file_close(int index) {
  if (!tabWidget_->widget(index)) {
    return;
  }
  if (!tabWidget_->tabIcon(index).isNull()) {
    auto select = QMessageBox::question(
        this, tr("关闭文件"),
        tr("是否保存对文件\"%1\"的修改?")
            .arg(tabWidget_->tabText(index)),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

    if (select == QMessageBox::Cancel) {
      return;
    } else if (select == QMessageBox::Yes) {
      file_save(index);
    }
  }
  openedFiles_.remove_by_value(tabWidget_->widget(index));
  textCodecIndexs_.remove(tabWidget_->widget(index));

  tabWidget_->removeTab(index);
}
void MainWindow::file_save(int index) {
  if (!tabWidget_->widget(index)) {
    return;
  }
  TextEdit *t = static_cast<TextEdit *>(tabWidget_->widget(index));
  if (!t->document()->isModified()) {
    return;
  }
  if (openedFiles_.is_contains_value(t)) {
    QFile file(openedFiles_.key(t));

    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
      QTextStream out(&file);
      out.setCodec("UTF-8");

      out << t->toPlainText();
      file.close();
      t->document()->setModified(false);

      textCodecIndexs_.insert(t, 0);
      statubar_->codecBox_->setCurrentIndex(0);

      tabWidget_->setTabIcon(index, QIcon());
    } else {
      QMessageBox::critical(this, tr("保存文件"),
                            tr("保存文件失败!"));
    }
  } else {
    file_save_as(index);
  }
}

void MainWindow::file_save_as(int index) {
  if (!tabWidget_->widget(index)) {
    return;
  }
  xedPath_.mkdir(xedPath_.path());

  QString fileName = QFileDialog::getSaveFileName(
      this, tr("保存文件"),
      xedPath_.path() + QDir::separator() +
          tabWidget_->tabText(index),
      tr("文本文件(*.txt)"));
  if (fileName.isNull()) {
    return;
  }
  if (openedFiles_.is_contains_key(fileName)) {
    QMessageBox::warning(this, tr("保存文件"),
                         tr("文件%1已打开,无法保存!").arg(fileName));
    tabWidget_->setCurrentWidget(openedFiles_.value(fileName));
    return;
  }

  QFile file(fileName);
  if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
    TextEdit *t = static_cast<TextEdit *>(tabWidget_->widget(index));

    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << t->toPlainText();
    file.close();
    tabWidget_->setTabText(index, QFileInfo(file).fileName());
    t->document()->setModified(false);

    openedFiles_.insert(file.fileName(), t);

    textCodecIndexs_.insert(t, 0);
    statubar_->codecBox_->setCurrentIndex(0);

    tabWidget_->setTabIcon(index, QIcon());
  } else {
    QMessageBox::critical(this, tr("保存文件"), tr("保存文件失败!"));
  }
}

void MainWindow::file_close_all() {
  if (!tabWidget_->count()) {
    return;
  }
  QList<int> indexs;

  for (int i = 0; i < tabWidget_->count(); i++) {
    if (!tabWidget_->tabIcon(i).isNull()) {
      indexs.append(i);
    }
  }
  if (!indexs.isEmpty()) {
    QString questionText(
        tr("是否保存对下列%1个文件的修改?\n").arg(indexs.count()));
    for (int i : indexs) {
      questionText.append(tabWidget_->tabText(i) + "\n");
    }
    auto select = QMessageBox::question(
        this, tr("关闭文件"), questionText,
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (select == QMessageBox::Cancel) {
      return;
    }
    if (select == QMessageBox::Yes) {
      for (int i : indexs) {
        file_save(i);
      }
    }
  }
  openedFiles_.clear();
  textCodecIndexs_.clear();

  tabWidget_->clear();
}

void MainWindow::file_new_text() {
  TextEdit *t = new TextEdit(editFont_, this);
  tabWidget_->addTab(t, tr("无标题"));
  tabWidget_->setCurrentWidget(t);
  textCodecIndexs_.insert(t, 0);

  connect_textedit(t);
}

void MainWindow::file_open() {
  QString fileName = QFileDialog::getOpenFileName(
      this, tr("打开文件"), QDir::homePath(), tr("所有文件(*.*)"));
  if (fileName.isNull()) {
    return;
  }
  if (openedFiles_.is_contains_key(fileName)) {
    QMessageBox::warning(this, tr("打开文件"),
                         tr("文件%1已打开!").arg(fileName));
    tabWidget_->setCurrentWidget(openedFiles_.value(fileName));
    return;
  }
  QFile file(fileName);

  if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
    if (file.size() < 1024 * 512) {
      QTextStream in(&file);
      in.setCodec("UTF-8");

      TextEdit *t = new TextEdit(editFont_, this);
      tabWidget_->addTab(t, QFileInfo(file).fileName());
      tabWidget_->setCurrentWidget(t);

      t->setPlainText(in.readAll());
      file.close();

      openedFiles_.insert(file.fileName(), t);
      textCodecIndexs_.insert(t, 0);

      connect_textedit(t);
    } else {
      QMessageBox::warning(this, tr("打开文件"),
                           tr("文件体积太大,无法打开!"));
    }

  } else {
    QMessageBox::critical(this, tr("打开文件"), tr("打开文件失败!"));
  }
}
void MainWindow::reopen(const QString &codec) {
  if (openedFiles_.is_contains_value(tabWidget_->currentWidget())) {
    QFile file(openedFiles_.key(tabWidget_->currentWidget()));
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
      QTextStream in(&file);
      in.setCodec(codec.toUtf8());

      TextEdit *t =
          static_cast<TextEdit *>(tabWidget_->currentWidget());
      t->setPlainText(in.readAll());
      file.close();

      textCodecIndexs_[t] = statubar_->codecBox_->currentIndex();
    } else {
      QMessageBox::critical(this, tr("打开文件"),
                            tr("打开文件失败!"));
    }
  }
}

void MainWindow::zoom_edit_font(int delta) {
  editFont_.setPointSize(
      qBound(10, editFont_.pointSize() + delta, 128));
  emit edit_font_changed();
}

void MainWindow::zoom_view_font(int delta) {
  viewFont_.setPointSize(
      qBound(12, viewFont_.pointSize() + delta, 48));
  setFont(viewFont_);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  file_close_all();
}

void MainWindow::wheelEvent(QWheelEvent *e) {
  if (e->modifiers() & Qt::ControlModifier) {
    if (e->angleDelta().y() > 0) {
      zoom_edit_font(4);
    } else {
      zoom_edit_font(-4);
    }
    e->accept();
    return;
  }
  QMainWindow::wheelEvent(e);
}
void MainWindow::connect_components() {
  connect(menubar_->actZoomIn_, &QAction::triggered,
          [=] { zoom_view_font(4); });
  connect(menubar_->actZoomOut_, &QAction::triggered,
          [=] { zoom_view_font(-4); });
  connect(menubar_->actZoomInFont_, &QAction::triggered,
          [=] { zoom_edit_font(1); });
  connect(menubar_->actZoomOutFont_, &QAction::triggered,
          [=] { zoom_edit_font(-1); });

  connect(this, &MainWindow::edit_font_changed, [=] {
    statubar_->fontLabel_->setText(tr("字体:%1|字号:%2")
                                       .arg(editFont_.family())
                                       .arg(editFont_.pointSize()));
  });
  connect(menubar_->actUndo_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit *t =
          static_cast<TextEdit *>(tabWidget_->currentWidget());
      t->undo();
    }
  });
  connect(menubar_->actRedo_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit *t =
          static_cast<TextEdit *>(tabWidget_->currentWidget());
      t->redo();
    }
  });
  connect(menubar_->actCut_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit *t =
          static_cast<TextEdit *>(tabWidget_->currentWidget());
      t->cut();
    }
  });
  connect(menubar_->actCopy_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit *t =
          static_cast<TextEdit *>(tabWidget_->currentWidget());
      t->copy();
    }
  });
  connect(menubar_->actPaste_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit *t =
          static_cast<TextEdit *>(tabWidget_->currentWidget());
      t->paste();
    }
  });
  connect(menubar_->actSelectAll_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit *t =
          static_cast<TextEdit *>(tabWidget_->currentWidget());
      t->selectAll();
    }
  });

  connect(menubar_->actSaveAs_, &QAction::triggered, [=] {
    if (tabWidget_->count()) {
      file_save_as(tabWidget_->currentIndex());
    }
  });
  connect(menubar_->actSaveAll_, &QAction::triggered, [=] {
    for (int i = 0; i < tabWidget_->count(); i++) {
      file_save(i);
    }
  });
  connect(menubar_->actCloseAll_, &QAction::triggered, this,
          &MainWindow::file_close_all);
  connect(tabWidget_, &QTabWidget::currentChanged, [=](int index) {
    if (!tabWidget_->widget(index)) {
      return;
    }
    TextEdit *t = static_cast<TextEdit *>(tabWidget_->widget(index));
    statubar_->codecBox_->setCurrentIndex(textCodecIndexs_.value(t));
  });
  connect(menubar_->actSave_, &QAction::triggered, [=] {
    if (tabWidget_->count()) {
      file_save(tabWidget_->currentIndex());
    }
  });
  connect(menubar_->actOpen_, &QAction::triggered, this,
          &MainWindow::file_open);
  connect(statubar_->codecBox_, &QComboBox::currentTextChanged, this,
          &MainWindow::reopen);
  connect(menubar_->actQuit_, &QAction::triggered,
          [=] { this->close(); });
  connect(menubar_->actNewText_, &QAction::triggered, [=] {
    TextEdit *t = new TextEdit(editFont_, this);
    tabWidget_->addTab(t, tr("无标题"));
    tabWidget_->setCurrentWidget(t);
    textCodecIndexs_.insert(t, 0);

    connect_textedit(t);
  });
  connect(menubar_->actNew_, &QAction::triggered, [=] {
    QString inputName = QInputDialog::getText(
        this, tr("新建文件"), tr("输入文件名:"), QLineEdit::Normal,
        "", nullptr, Qt::WindowCloseButtonHint);
    if (inputName.isNull()) {
      return;
    }
    TextEdit *t = new TextEdit(editFont_, this);
    tabWidget_->addTab(t, inputName);
    tabWidget_->setCurrentWidget(t);

    textCodecIndexs_.insert(t, 0);
    connect_textedit(t);
  });
  connect(tabWidget_, &QTabWidget::tabCloseRequested, this,
          &MainWindow::file_close);
  connect(menubar_->actClose_, &QAction::triggered, [=] {
    if (tabWidget_->count()) {
      file_close(tabWidget_->currentIndex());
    }
  });
  // emit
  emit edit_font_changed();
}

void MainWindow::connect_textedit(TextEdit *t) {
  if (!t) {
    return;
  }
  connect(t, &TextEdit::textChanged, [=] {
    if (t->document()->isModified()) {
      tabWidget_->setTabIcon(tabWidget_->currentIndex(),
                             QIcon(":/resfiles/icon/flag.png"));
    } else {
      tabWidget_->setTabIcon(tabWidget_->currentIndex(), QIcon());
    }
  });
  connect(this, &MainWindow::edit_font_changed,
          [=] { t->setFont(editFont_); });
}
