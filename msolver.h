#ifndef MSOLVER_H
#define MSOLVER_H

#include <QObject>
#include <QHash>
#include <QStack>
#include <QQueue>
#include <QList>
#include <QTimer>
#include <QElapsedTimer>

#include "mazenode.h"

class MSolver : public QObject
{
    Q_OBJECT
public:

    explicit MSolver(QHash<int,MazeNode*>*listOfIds, int rows,int columns, int tick,QObject *parent = 0);
    qint64 getTimeElapsed();
    void setParameters(QHash<int,MazeNode*>*listOfIds, int rows,int columns,int tick);
    void startDFS();
    void startBFS();
    void triggerStopSearch();

private:

    QHash<int,MazeNode*> *nodeHash;
    QStack<MazeNode*> *stack;
    QQueue<MazeNode*> *queue;
    QTimer *tickerDFS;
    QTimer *tickerBFS;
    QElapsedTimer *stopwatch;
    bool interruptSearch;
    int rows;
    int columns;
    int tickInterval;
    qint64 timeElapsed;
    void stopSearch(MazeNode* stopSearch);
    QList<MazeNode*>* getAdjacentUnvisitedNodes(int currentID);
    void traceBack(MazeNode* lastNode);

private slots:

    void takeFromStack();
    void takeFromQueue();
signals:

    void displayExit(MazeNode *exitNode);
};

#endif // MSOLVER_H
