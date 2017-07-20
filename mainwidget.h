#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <map>
#include "common.h"
#include "wo1_solver.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = nullptr);
    void setBT(BlockType bt);

public slots:
    void showSolution();
    void clrBlocks();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    WO1_Solver ws;
    BlockType bt;
    map<BlockType, QString> bt2res;
    map<ClickType, QString> ct2res;

    void checkPos(int x, int y);
    void paintGrid(QPainter& p);
    void paintState(QPainter& p);
};

#endif // MAINWIDGET_H
