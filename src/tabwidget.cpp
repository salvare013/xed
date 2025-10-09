#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QTabWidget{parent} {
  setMovable(true);
  setTabsClosable(true);
}
