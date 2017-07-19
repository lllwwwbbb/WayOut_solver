#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
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
};

#endif // MAINWIDGET_H
