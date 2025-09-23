#include "menubar.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar{parent} {
  // menu
  menuFile_ = addMenu(tr("文件(&F)"));
  menuEdit_ = addMenu(tr("编辑(&E)"));
  menuView_ = addMenu(tr("视图(&V)"));

  // file action
  actNewText_ = menuFile_->addAction(tr("新建文本文件(&N)"));
  actNewText_->setShortcut(Qt::CTRL + Qt::Key_N);

  actNew_ = menuFile_->addAction(tr("新建文件...(&F)"));
  actNew_->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_N);

  menuFile_->addSeparator();

  actOpen_ = menuFile_->addAction(tr("打开文件...(&O)"));
  actOpen_->setShortcut(Qt::CTRL + Qt::Key_O);

  menuFile_->addSeparator();

  actSave_ = menuFile_->addAction(tr("保存文件(&S)"));
  actSave_->setShortcut(Qt::CTRL + Qt::Key_S);

  actSaveAs_ = menuFile_->addAction(tr("另存为文件...(&A)"));
  actSaveAs_->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_S);

  actSaveAll_ = menuFile_->addAction(tr("保存所有文件(&L)"));
  actSaveAll_->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_S);

  menuFile_->addSeparator();

  actClose_ = menuFile_->addAction(tr("关闭文件(&W)"));
  actClose_->setShortcut(Qt::CTRL + Qt::Key_W);

  actCloseAll_ = menuFile_->addAction(tr("关闭所有文件(&X)"));
  actCloseAll_->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_W);

  menuFile_->addSeparator();

  actQuit_ = menuFile_->addAction(tr("退出程序(&Q)"));
  actQuit_->setShortcut(Qt::CTRL + Qt::Key_Q);

  //   edit action
  actUndo_ = menuEdit_->addAction(tr("撤销(&U)"));
  actUndo_->setShortcut(Qt::CTRL + Qt::Key_Z);

  actRedo_ = menuEdit_->addAction(tr("恢复(&R)"));
  actRedo_->setShortcut(Qt::CTRL + Qt::Key_Y);

  menuEdit_->addSeparator();

  actCut_ = menuEdit_->addAction(tr("剪切(&T)"));
  actCut_->setShortcut(Qt::CTRL + Qt::Key_X);

  actCopy_ = menuEdit_->addAction(tr("复制(&C)"));
  actCopy_->setShortcut(Qt::CTRL + Qt::Key_C);

  actPaste_ = menuEdit_->addAction(tr("粘贴(&V)"));
  actPaste_->setShortcut(Qt::CTRL + Qt::Key_V);

  menuEdit_->addSeparator();

  actSelectAll_ = menuEdit_->addAction(tr("全选(&A)"));
  actSelectAll_->setShortcut(Qt::CTRL + Qt::Key_A);

  menuEdit_->addSeparator();

  actGo_ = menuEdit_->addAction(tr("转到(&G)"));
  actGo_->setShortcut(Qt::CTRL + Qt::Key_G);
  // view action
  actZoomInFont_ = menuView_->addAction(tr("放大编辑字体(&P)"));
  actZoomInFont_->setShortcut(Qt::CTRL + Qt::Key_Equal);

  actZoomOutFont_ = menuView_->addAction(tr("缩小编辑字体(&M)"));
  actZoomOutFont_->setShortcut(Qt::CTRL + Qt::Key_Minus);

  menuView_->addSeparator();

  actZoomIn_ = menuView_->addAction(tr("放大界面字体"));
  actZoomIn_->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Equal);

  actZoomOut_ = menuView_->addAction(tr("缩小界面字体"));
  actZoomOut_->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_Minus);

  menuView_->addSeparator();

  actShowMax_ = menuView_->addAction(tr("窗口最大化"));
  actShowMax_->setShortcut(Qt::CTRL + Qt::Key_M);

  actShowNormal_ = menuView_->addAction(tr("窗口复原"));
}
