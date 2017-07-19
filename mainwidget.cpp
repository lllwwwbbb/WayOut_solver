#include "mainwidget.h"
#include <QPainter>
#include <QPoint>
#include <QPushButton>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent),
    ws(B_WID, B_HGT), bt(RAW)
{
    setFixedSize(SCR_WID, SCR_HGT);
}

void MainWidget::setBT(BlockType bt)
{
    this->bt = bt;
}

void MainWidget::showSolution()
{
    ws.enableSolution(true);
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
    pnter.setPen(QColor(0, 0, 0));
    for (int i = 1; i < B_WID + 1; i ++) {
        pnter.drawLine(i * B_LEN, 0, i * B_LEN, SCR_HGT);
    }
    for (int i = 1; i < B_HGT + 1; i ++) {
        pnter.drawLine(0, i * B_LEN, SCR_WID, i * B_LEN);
    }
    ws.paintState(pnter);
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton) {
        return;
    }
    ws.setBlock(bt, e->x() / B_LEN, e->y() / B_LEN);
    repaint();
}

void MainWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (!(e->buttons() & Qt::LeftButton)) {
        return;
    }
    ws.setBlock(bt, e->x() / B_LEN, e->y() / B_LEN);
    repaint();
}
