#ifndef MAZESOLVER_H
#define MAZESOLVER_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGridLayout>
#include <QFormLayout>
#include <QTransform>
#include <QHash>
#include <QRadioButton>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QElapsedTimer>
#include <QGroupBox>
#include <QTextBrowser>
#include <QSpinBox>
#include <QMessageBox>

#include "mazenode.h"
#include "msolver.h"

class MazeUi :  public QGraphicsView
{
    Q_OBJECT
public:
    explicit MazeUi(QWidget* parent = 0);
public slots:
    void mousePressEvent(QMouseEvent *me);


private:

    QWidget* currentTab;
    QTextBrowser *log;
    QGridLayout *mazeTabLayout;
    QGraphicsScene *scene;
    QPushButton *resetMazeButton;
    QPushButton *startSearchButton;
    QPushButton *clearMazeButton;
    QPushButton *stopSearchButton;
    QComboBox *gridSizeSelection;
    QHash<QGraphicsItem*,MazeNode*> *listOfRectangles;
    QHash<int,MazeNode*> *listOfIds;
    QComboBox *searchSelection;
    MSolver *solver;
    int nodeDescriptionThreshold;
    int sceneHeight;
    int sceneWidth;
    int rectSize;
    int **rectSizeList;
    ~MazeUi();
    void addItemsToMazeGroupBox(QGroupBox *mazeGroupBox);
    void addItemsToToolsGroupBox(QGroupBox *toolsGroupBox);
    void addItemsToLogGroupBox(QGroupBox *logGroupBox);
    void addItemsToScene();
    void createGridSizeArray();
    void drawMaze();
    void fillLogWithPath(QStack<int>* path);
    void initializeMazeSolver();
    void setDefaultSelections();
    void setEntranceAndExit();
    void setupUI();
    void switchUiState();
    int tracePath(MazeNode* lastNode,QStack<int>* nodeStack);

private slots:

    void clearMaze();
    void createRandomMaze();
    void displayResult(MazeNode *lastNode);
    void resetMaze();
    void setNewGridSize();
    void startSearch();
    void stopSearch();
    void About();
};

#endif // MAZESOLVER_H
