#include "gui_qt.h"
#include <QBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QApplication>
#include <QStatusBar>
#include <QMessageBox>
#include <QLabel>
#include <QWidget>
#include <QDesktopWidget>
#include <QHeaderView>
#include <QTableWidget>
#include <QFileInfo>
#include <thread>
#include "screen.h"

using namespace std;
using namespace gvbsim;


GuiQt::GuiQt() {
   setWindowTitle(tr("GVBASIC模拟器"));
   
   QWidget *central = new QWidget(this);
   
   auto status = new QLabel(this);
   auto im = new QLabel(this);
   
   auto table = new QTableWidget(this);
   table->setColumnCount(2);
   QStringList ls;
   ls << "变量" << "值";
   table->setHorizontalHeaderLabels(ls);
   table->verticalHeader()->hide();
   table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   table->setSelectionBehavior(QAbstractItemView::SelectRows);
   
   m_screen = new Screen(status, im, table);
   
   connect(m_screen, &Screen::stopped, this, &GuiQt::stop, Qt::QueuedConnection);
   
   loadMenu();
   
   statusBar()->addWidget(status, 1);
   statusBar()->addWidget(im);
   statusBar()->setSizeGripEnabled(false);
   
   im->setFrameStyle(QFrame::Box);
   im->hide();
   
   QVBoxLayout *layout = new QVBoxLayout(central);
   layout->addWidget(m_screen, 0);
   layout->addWidget(table, 0);
   
   setCentralWidget(central);
   
   QRect sz = QApplication::desktop()->screenGeometry();
   move((sz.width() - width()) / 2, (sz.height() - height()) / 2);
}

void GuiQt::loadMenu() {
   QMenu *m = menuBar()->addMenu(tr("文件"));
   m_mnuOpen = m->addAction(tr("打开"), this, &GuiQt::loadFile, QKeySequence::Open);
   m->addSeparator();
   m->addAction(tr("退出"), qApp, &QApplication::quit, QKeySequence(Qt::ALT, Qt::Key_F4));
   
   m = menuBar()->addMenu(tr("程序"));
   m_mnuRun = m->addAction(tr("运行"), this, &GuiQt::run, QKeySequence(Qt::Key_F5));
   m_mnuStop = m->addAction(tr("停止"), this, &GuiQt::stop, QKeySequence(Qt::Key_F6));
   m_mnuReload = m->addAction(tr("重新加载源文件"), this, &GuiQt::reloadFile, QKeySequence(Qt::Key_F9));
   m->addSeparator();
   m->addAction(tr("截图"), m_screen, &Screen::captureScreen, QKeySequence(Qt::Key_F11));
   m->addSeparator();
   m->addAction(tr("重新加载配置文件"), m_screen, &Screen::loadConfig);
   
   m = menuBar()->addMenu(tr("帮助"));
   m->addAction(tr("内容"), this, &GuiQt::showHelp);
   m->addSeparator();
   m->addAction(tr("关于"), this, &GuiQt::showAbout);
   m->addAction(tr("关于Qt"), qApp, &QApplication::aboutQt);
   
   m_mnuRun->setEnabled(false);
   m_mnuStop->setEnabled(false);
   m_mnuReload->setEnabled(false);
}


void GuiQt::loadFile() {
   if (m_screen->loadFile()) {
      m_mnuRun->setEnabled(true);
      m_mnuReload->setEnabled(true);
      setWindowTitle(QFileInfo(m_screen->getOpenFile()).fileName());
   }
}

void GuiQt::reloadFile() {
    if (m_screen->loadFile(true)) {
    }
}

void GuiQt::keyPressEvent(QKeyEvent *e) {
   m_screen->keyDown(e);
   QMainWindow::keyPressEvent(e);
}

void GuiQt::keyReleaseEvent(QKeyEvent *e) {
   m_screen->keyUp(e);
   QMainWindow::keyReleaseEvent(e);
}

void GuiQt::run() {
   switch (m_screen->run()) {
   case Screen::Result::Start:
      m_mnuStop->setEnabled(true);
      m_mnuOpen->setEnabled(false);
      m_mnuReload->setEnabled(false);
      m_mnuRun->setText(tr("暂停"));
      break;
   case Screen::Result::Resume:
      m_mnuStop->setEnabled(true);
      m_mnuRun->setText(tr("暂停"));
      break;
   case Screen::Result::Pause:
      m_mnuRun->setText(tr("继续"));
      break;
   case Screen::Result::Ready:
      break;
   }
}

void GuiQt::stop() {
   m_screen->stop();
   m_mnuRun->setText(tr("运行"));
   m_mnuOpen->setEnabled(true);
   m_mnuReload->setEnabled(true);
   m_mnuStop->setEnabled(false);
}

void GuiQt::showHelp() {
   QMessageBox::about(this,
                tr("内容"),
                tr("● 输入文字时使用Shift切换中英文输入\n"
                   "● 特殊键位：\n"
                   "    跳出: Esc\n"
                   "    帮助: `\n"
                   "    CAPS: Ctrl\n"
                   "● 变量表只有暂停程序时才可用\n"
                   "● 双击变量表的某一项可以修改该变量\n"
                   "● 如果在变量表中看到以下划线开头的变量，那是内部使用的，不用理会"));
}

void GuiQt::showAbout() {
   QMessageBox::about(this,
                tr("About GVBASIC Simulator"),
                tr("Made by Plodsoft (Github: arucil)\n"
                   "Distributed under the MIT License"));
}

GuiQt::~GuiQt() {
}

