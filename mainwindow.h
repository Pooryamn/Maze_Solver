#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QHash>
#include <QPoint>
#include <QMouseEvent>
#include <QGridLayout>

#include "mazenode.h"
#include "mazeui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    MazeUi *maze;
    void setupUI();
};

#endif // MAINWINDOW_H
