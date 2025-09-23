#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>

class MenuBar : public QMenuBar {
  Q_OBJECT
 public:
  explicit MenuBar(QWidget *parent = nullptr);

 public:
  // menu
  QMenu *menuFile_;
  QMenu *menuEdit_;
  QMenu *menuView_;
  // file action
  QAction *actNewText_;
  QAction *actNew_;
  QAction *actOpen_;
  QAction *actSave_;
  QAction *actSaveAs_;
  QAction *actSaveAll_;
  QAction *actClose_;
  QAction *actCloseAll_;
  QAction *actQuit_;
  //   edit action
  QAction *actUndo_;
  QAction *actRedo_;
  QAction *actCut_;
  QAction *actCopy_;
  QAction *actPaste_;
  QAction *actSelectAll_;
  QAction *actGo_;
  // view action
  QAction *actZoomInFont_;
  QAction *actZoomOutFont_;
  QAction *actZoomIn_;
  QAction *actZoomOut_;

 signals:
};

#endif  // MENUBAR_H
