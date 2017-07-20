#include "mainwidget.h"
#include <QPoint>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent),
    ws(B_WID, B_HGT), bt(RAW)
{
#ifndef VERSION2
    setFixedSize(SCR_WID, SCR_HGT);
#else
    setFixedSize(SCR_WID + B_LEN / 2, SCR_HGT);
#endif
    bt2res.insert({
                      {RAW,        ":/img/rawblock"},
                      {H_DIR,      ":/img/hdblock"},
                  #ifdef    VERSION2
                      {ADD,        ":/img/addblock2"},
                      {LU_DIR,     ":/img/ludblock"},
                      {RU_DIR,     ":/img/rudblock"},
                  #else
                      {ADD,        ":/img/addblock"},
                      {V_DIR,      ":/img/vdblock"},
                  #endif
                      {OUT,        ":/img/outblock"},
                      {YELLOW,     ":/img/ylblock"},
                      {DPOINT,     ":/img/dpblock"},
                  });
    ct2res.insert({
                      {ClickType::SINGLE,   ":/img/click"},
                      {ClickType::DOUBLE,   ":/img/dclick"},
                      {ClickType::DBFIRST,  ":/img/dfclick"},
                      {ClickType::FIRST,    ":/img/fclick"},
                  });
}

void MainWidget::setBT(BlockType bt)
{
    this->bt = bt;
}

void MainWidget::showSolution()
{
    int ok = ws.enableSolution();
    if (!ok) {
        QMessageBox::information(NULL, "无解","没有找到解决方案",QMessageBox::Yes);
    }
    repaint();
}

void MainWidget::clrBlocks()
{
    for (int r = 0; r < B_HGT; r ++) {
        for (int c = 0; c < B_WID; c ++) {
            ws.setBlock(EMPTY, c, r);
        }
    }
    repaint();
}

void MainWidget::paintEvent(QPaintEvent* e)
{
    Q_UNUSED(e);
    QPainter pnter(this);
    paintGrid(pnter);
    paintState(pnter);
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) {
        return;
    }
    checkPos(e->x(), e->y());
}

void MainWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!(e->buttons() & Qt::LeftButton)) {
        return;
    }
    checkPos(e->x(), e->y());
}

void MainWidget::checkPos(int x, int y)
{
#ifndef VERSION2
    ws.setBlock(bt, x / B_LEN, y / B_LEN);
#else
    int dy = y / B_LEN;
    int dx = (dy % 2 == 0 ? x - B_LEN / 2 : x) / B_LEN;
    ws.setBlock(bt, dx, dy);
#endif
    repaint();
}

void MainWidget::paintGrid(QPainter &p)
{
#ifndef VERSION2
    p.setPen(QColor(0, 0, 0));
    for (int i = 0; i < B_WID + 1; i ++) {
        p.drawLine(i * B_LEN, 0, i * B_LEN, SCR_HGT);
    }
    for (int i = 0; i < B_HGT + 1; i ++) {
        p.drawLine(0, i * B_LEN, SCR_WID, i * B_LEN);
    }
#else
    p.setPen(QColor(0, 0, 0));
    for (int i = 0; i < B_HGT + 1; i ++) {
        p.drawLine(0, i * B_LEN, SCR_WID + B_LEN / 2, i * B_LEN);
        int off = (i % 2 == 0 ? B_LEN / 2 : 0);
        for (int k = 0; k < B_WID + 1; k ++) {
            p.drawLine(off + k * B_LEN, i * B_LEN, off + k * B_LEN, i * B_LEN + B_LEN);
        }
    }
#endif
}

void MainWidget::paintState(QPainter &p)
{
    auto sltn = ws.getSolution();
    for (int r = 0; r < B_HGT; r ++) {
        for (int c = 0; c < B_WID; c ++) {
            auto state = ws.getState(r, c);
            if (state != EMPTY) {
                QString res = bt2res[state];
                if (ws.getColor(r, c)) {
                    res += "_color";
                }
#ifndef VERSION2
                int dx = c * B_LEN + PAD, dy = r * B_LEN + PAD;
#else
                int dx = c * B_LEN + PAD + (r % 2 == 0 ? B_LEN / 2 : 0), dy = r * B_LEN + PAD;
#endif
                p.drawPixmap(dx, dy, B_LEN -  2 * PAD, B_LEN - 2 * PAD, QPixmap(res));
            }
        }
    }
    for (int i = 0; i < sltn.size(); i ++) {
        QString cres = ct2res[sltn[i].ct];
#ifndef VERSION2
        int dx = sltn[i].pos.x * B_LEN + PAD, dy = sltn[i].pos.y * B_LEN + PAD;
#else
        int dx = sltn[i].pos.x * B_LEN + PAD + (sltn[i].pos.y % 2 == 0 ? B_LEN / 2 : 0),
                dy = sltn[i].pos.y * B_LEN + PAD;
#endif
        p.drawPixmap(dx, dy, B_LEN -  2 * PAD, B_LEN - 2 * PAD, QPixmap(cres));
    }
}
