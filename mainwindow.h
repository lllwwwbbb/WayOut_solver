#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QMap>
#include "mainwidget.h"
#include <QPaintEvent>
#include "common.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void onSelectTool(QAction* actn);

protected:

private:
    Ui::MainWindow *ui;
    QActionGroup tool_actns;
    QMap<QAction*, BlockType> actn2bt;

    void initToolBar();
};

#endif // MAINWINDOW_H
