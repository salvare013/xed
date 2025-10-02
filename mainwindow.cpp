#include "mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextBlock>
#include "textedit.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  tabWidget_ = new TabWidget(this);
  menuBar_ = new MenuBar(this);
  statusBar_ = new StatusBar(this);

  setCentralWidget(tabWidget_);
  setMenuBar(menuBar_);
  setStatusBar(statusBar_);

  setWindowTitle(tr("xed-xmx"));
  setWindowIcon(QIcon(":/resfiles/icon/xed.png"));
  resize(1200, 900);

  connect_components();
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
  textCodecs_.remove(tabWidget_->widget(index));

  tabWidget_->removeTab(index);
}
void MainWindow::file_save(int index) {
  if (!tabWidget_->widget(index)) {
    return;
  }
  if (tabWidget_->tabIcon(index).isNull()) {
    return;
  }

  if (openedFiles_.is_contains_value(tabWidget_->widget(index))) {
    QFile file(openedFiles_.key(tabWidget_->widget(index)));

    if (file.open(QIODevice::Text | QIODevice::WriteOnly)) {
      TextEdit* t =
          static_cast<TextEdit*>(tabWidget_->widget(index));

      QTextStream out(&file);
      out.setCodec(defaultCodec_.toUtf8());  // only utf8

      out << t->toPlainText();
      file.close();

      t->document()->setModified(false);
      textCodecs_.insert(t, defaultCodec_);
      statusBar_->codecLabel_->setText(defaultCodec_);
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
    TextEdit* t = static_cast<TextEdit*>(tabWidget_->widget(index));

    QTextStream out(&file);
    out.setCodec(defaultCodec_.toUtf8());  // only utf8

    out << t->toPlainText();
    file.close();

    tabWidget_->setTabText(index, QFileInfo(file).fileName());
    t->document()->setModified(false);
    openedFiles_.insert(file.fileName(), t);
    textCodecs_.insert(t, defaultCodec_);
    statusBar_->codecLabel_->setText(defaultCodec_);
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
  textCodecs_.clear();

  tabWidget_->clear();
}

void MainWindow::file_new() {
  QString name = QInputDialog::getText(
      this, tr("新建文件"), tr("输入文件名:"), QLineEdit::Normal, "",
      nullptr, Qt::WindowCloseButtonHint);
  if (name.isNull()) {
    return;
  }
  TextEdit* t = new TextEdit(editFont_, this);
  textCodecs_.insert(t, defaultCodec_);

  tabWidget_->addTab(t, name);
  tabWidget_->setCurrentWidget(t);

  connect_textedit(t);
}
void MainWindow::file_new_text() {
  TextEdit* t = new TextEdit(editFont_, this);
  textCodecs_.insert(t, defaultCodec_);

  tabWidget_->addTab(t, tr("无标题"));
  tabWidget_->setCurrentWidget(t);

  connect_textedit(t);
}

void MainWindow::edit_go() {
  if (tabWidget_->currentWidget()) {
    TextEdit* t =
        static_cast<TextEdit*>(tabWidget_->currentWidget());
    while (true) {
      bool ok;
      QString text = QInputDialog::getText(
          this, tr("输入位置"),
          tr("输入要跳转到的行列号\n行列之间用.,;(半角)"
             "中的任意符号或空格分隔(如1,2)"),
          QLineEdit::Normal, "", &ok, Qt::WindowCloseButtonHint);
      auto parts = text.split(QRegularExpression(R"([.,;\s]+)"),
                              Qt::SkipEmptyParts);
      if (!ok) {
        break;
      }
      if (parts.size() > 2 || parts.size() < 1) {
        continue;
      }
      int line, col;
      line = qBound(1, parts[0].toInt(&ok), t->blockCount());
      if (!ok) {
        continue;
      }
      if (parts.size() == 2) {
        col = parts[1].toInt(&ok);
        if (!ok) {
          continue;
        }
      }
      auto block = t->document()->begin();

      for (int i = 1; i < line; i++) {
        block = block.next();
      }
      col = qBound(1, col, block.text().length() + 1);

      QTextCursor cursor(block);
      cursor.setPosition(block.position() + col - 1);
      t->setTextCursor(cursor);
    }
  }
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
      in.setCodec(defaultCodec_.toUtf8());

      TextEdit* t = new TextEdit(editFont_, this);
      textCodecs_.insert(t, defaultCodec_);

      tabWidget_->addTab(t, QFileInfo(file).fileName());
      tabWidget_->setCurrentWidget(t);

      t->setPlainText(in.readAll());
      file.close();

      openedFiles_.insert(file.fileName(), t);

      connect_textedit(t);
    } else {
      QMessageBox::warning(this, tr("打开文件"),
                           tr("文件体积太大,无法打开!"));
    }

  } else {
    QMessageBox::critical(this, tr("打开文件"), tr("打开文件失败!"));
  }
}
void MainWindow::file_reopen() {
  if (openedFiles_.is_contains_value(tabWidget_->currentWidget())) {
    const QString codec = statusBar_->codecLabel_->text();
    statusBar_->codecLabel_->open_selector();
    if (statusBar_->codecLabel_->text() == codec) {
      return;
    }

    QFile file(openedFiles_.key(tabWidget_->currentWidget()));
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
      QTextStream in(&file);
      in.setCodec(statusBar_->codecLabel_->text().toUtf8());

      TextEdit* t =
          static_cast<TextEdit*>(tabWidget_->currentWidget());
      t->setPlainText(in.readAll());
      file.close();

      textCodecs_[t] = statusBar_->codecLabel_->text();
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

  menuBar_->menuFile_->setFont(viewFont_);
  menuBar_->menuEdit_->setFont(viewFont_);
  menuBar_->menuView_->setFont(viewFont_);
}

void MainWindow::closeEvent(QCloseEvent* event) {
  file_close_all();
  QMainWindow::closeEvent(event);
}

void MainWindow::wheelEvent(QWheelEvent* event) {
  if (event->modifiers() & Qt::ControlModifier) {
    if (event->angleDelta().y() > 0) {
      zoom_edit_font(4);
    } else {
      zoom_edit_font(-4);
    }
    event->accept();
    return;
  }
  QMainWindow::wheelEvent(event);
}

void MainWindow::showEvent(QShowEvent* event) {
  QMainWindow::showEvent(event);

  zoom_view_font(0);
  update_status(0);
  // emit
  emit edit_font_changed();
}
void MainWindow::connect_components() {
  connect(tabWidget_, &QTabWidget::currentChanged, this,
          &MainWindow::update_status);
  connect(menuBar_->actReopen_, &QAction::triggered, this,
          &MainWindow::file_reopen);
  connect(statusBar_->codecLabel_, &ClickableLabel::clicked, this,
          &MainWindow::file_reopen);
  connect(menuBar_->actGo_, &QAction::triggered, this,
          &MainWindow::edit_go);
  connect(menuBar_->actShowMax_, &QAction::triggered,
          [=] { showMaximized(); });
  connect(menuBar_->actShowNormal_, &QAction::triggered,
          [=] { showNormal(); });
  connect(menuBar_->actZoomIn_, &QAction::triggered,
          [=] { zoom_view_font(4); });
  connect(menuBar_->actZoomOut_, &QAction::triggered,
          [=] { zoom_view_font(-4); });
  connect(menuBar_->actZoomInFont_, &QAction::triggered,
          [=] { zoom_edit_font(1); });
  connect(menuBar_->actZoomOutFont_, &QAction::triggered,
          [=] { zoom_edit_font(-1); });

  connect(this, &MainWindow::edit_font_changed, [=] {
    statusBar_->fontLabel_->setText(tr("字体:%1|字号:%2")
                                        .arg(editFont_.family())
                                        .arg(editFont_.pointSize()));
  });
  connect(menuBar_->actUndo_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit* t =
          static_cast<TextEdit*>(tabWidget_->currentWidget());
      t->undo();
    }
  });
  connect(menuBar_->actRedo_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit* t =
          static_cast<TextEdit*>(tabWidget_->currentWidget());
      t->redo();
    }
  });
  connect(menuBar_->actCut_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit* t =
          static_cast<TextEdit*>(tabWidget_->currentWidget());
      t->cut();
    }
  });
  connect(menuBar_->actCopy_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit* t =
          static_cast<TextEdit*>(tabWidget_->currentWidget());
      t->copy();
    }
  });
  connect(menuBar_->actPaste_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit* t =
          static_cast<TextEdit*>(tabWidget_->currentWidget());
      t->paste();
    }
  });
  connect(menuBar_->actSelectAll_, &QAction::triggered, [=] {
    if (tabWidget_->currentWidget()) {
      TextEdit* t =
          static_cast<TextEdit*>(tabWidget_->currentWidget());
      t->selectAll();
    }
  });

  connect(menuBar_->actSaveAs_, &QAction::triggered, [=] {
    if (tabWidget_->count()) {
      file_save_as(tabWidget_->currentIndex());
    }
  });
  connect(menuBar_->actSaveAll_, &QAction::triggered, [=] {
    for (int i = 0; i < tabWidget_->count(); i++) {
      file_save(i);
    }
  });
  connect(menuBar_->actCloseAll_, &QAction::triggered, this,
          &MainWindow::file_close_all);

  connect(menuBar_->actSave_, &QAction::triggered, [=] {
    if (tabWidget_->count()) {
      file_save(tabWidget_->currentIndex());
    }
  });
  connect(menuBar_->actOpen_, &QAction::triggered, this,
          &MainWindow::file_open);
  connect(menuBar_->actQuit_, &QAction::triggered,
          [=] { this->close(); });
  connect(menuBar_->actNewText_, &QAction::triggered, this,
          &MainWindow::file_new_text);
  connect(menuBar_->actNew_, &QAction::triggered, this,
          &MainWindow::file_new);
  connect(tabWidget_, &QTabWidget::tabCloseRequested, this,
          &MainWindow::file_close);
  connect(menuBar_->actClose_, &QAction::triggered, [=] {
    if (tabWidget_->count()) {
      file_close(tabWidget_->currentIndex());
    }
  });
}

void MainWindow::connect_textedit(TextEdit* t) {
  if (!t) {
    return;
  }
  connect(t, &TextEdit::textChanged, [=] {
    tabWidget_->setTabIcon(tabWidget_->currentIndex(),
                           QIcon(":/resfiles/icon/flag.png"));
  });

  connect(this, &MainWindow::edit_font_changed,
          [=] { t->setFont(editFont_); });
  connect(t, &QPlainTextEdit::cursorPositionChanged, [=] {
    statusBar_->PosLabel_->setText(
        tr("行%1,列%2")
            .arg(t->textCursor().blockNumber() + 1)
            .arg(t->textCursor().columnNumber() + 1));
  });
}

void MainWindow::update_status(int index) {
  if (!tabWidget_->widget(index)) {
    statusBar_->PosLabel_->setText("");
    statusBar_->codecLabel_->setText("");
    statusBar_->showMessage(
        tr("没有打开的文件,按快捷键Ctrl+O打开一个文件..."));

    return;
  }
  statusBar_->clearMessage();

  TextEdit* t = static_cast<TextEdit*>(tabWidget_->widget(index));
  statusBar_->codecLabel_->setText(textCodecs_.value(t));

  statusBar_->PosLabel_->setText(
      tr("行%1,列%2")
          .arg(t->textCursor().blockNumber() + 1)
          .arg(t->textCursor().columnNumber() + 1));
}
