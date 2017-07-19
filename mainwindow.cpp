#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAction>
#include <QIcon>
#include <QToolBar>
#include <QList>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QColor>
#include <QScrollArea>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tool_actns(this)
{
    ui->setupUi(this);

    auto sa = new QScrollArea(this);
    sa->setWidget(takeCentralWidget());
    setCentralWidget(sa);
    sa->setBackgroundRole(QPalette::Dark);
    sa->show();

    initToolBar();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSelectTool(QAction *actn)
{
    ui->mainWidget->setBT(actn2bt[actn]);
}

void MainWindow::initToolBar()
{
    struct BlockAcnt {
        QString res, text, tip;
        BlockType bt;
    };
    BlockAcnt ba_content[] = {
        //-- resource ----- | -- text ----- | -- status tip ----- | -- block type ----- |
        ":/img/rawblock",     "简单块",          "放置简单块",          RAW,
        ":/img/hdblock",      "水平有向块",       "放置水平有向块",      H_DIR,
        ":/img/vdblock",      "垂直有向块",       "放置垂直有向块",      V_DIR,
        ":/img/outblock",     "圆心块",          "放置圆心块",          OUT,
        ":/img/addblock",     "传递块",          "放置传递块",          ADD,
        ":/img/dpblock",      "双点块",          "放置双点块",          DPOINT,
        ":/img/ylblock",      "黄色块",          "放置黄色块",          YELLOW,
        // 在上面添加方块类型
        ":/img/color",        "着色",            "给方块着色",         COLOR,
        ":/img/delete",       "删除",            "删除方块",           EMPTY,

    };
    for (auto ba : ba_content) {
        auto actn = tool_actns.addAction(QIcon(ba.res), ba.text);
        actn->setStatusTip(ba.tip);
        actn->setCheckable(true);
        actn2bt.insert(actn, ba.bt);
    }
    tool_actns.setExclusive(true);
    tool_actns.connect(&tool_actns, SIGNAL(triggered(QAction*)), this, SLOT(onSelectTool(QAction*)));
    auto list_actns = tool_actns.actions();
    list_actns[0]->trigger();

    auto toolBar = ui->toolBar;
    toolBar->setMovable(false);
    toolBar->setContextMenuPolicy(Qt::PreventContextMenu);
    toolBar->addActions(list_actns);
    toolBar->setFixedHeight(BAR_HGT);

    toolBar->addSeparator();
    struct ToolActn {
        QString res, text, tip;
        const char *signal, *slot;
    };
    ToolActn ta_content[] = {
        // ---- res ---- | ---- text ---- | ---- tip ---- | ---- signal ---- | ---- slot ---- |
        ":/img/click", "解决方案",  "显示解决方案", SIGNAL(triggered(bool)), SLOT(showSolution()),
        ":/img/clr",   "清空",      "清空面板",    SIGNAL(triggered(bool)), SLOT(clrBlocks()),
    };
    for (auto ta : ta_content) {
        auto act = toolBar->addAction(QIcon(ta.res), ta.text);
        act->setStatusTip(ta.tip);
        act->connect(act, ta.signal, ui->mainWidget, ta.slot);
    }
}
