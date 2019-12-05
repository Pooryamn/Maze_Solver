#include "msolver.h"

#include <QElapsedTimer>

MSolver::MSolver(QHash<int, MazeNode *> *listOfIds, int nrows, int ncolumns,int tick, QObject *parent) :
    QObject(parent)
{
    rows = nrows;
    columns = ncolumns;
    nodeHash = listOfIds;
    tickInterval = tick;
    interruptSearch = false;
    tickerDFS = new QTimer(this);
    tickerBFS = new QTimer(this);
    stopwatch = new QElapsedTimer();
    timeElapsed = 0;

    connect(tickerDFS, SIGNAL(timeout()), this, SLOT(takeFromStack()));
    connect(tickerBFS, SIGNAL(timeout()), this, SLOT(takeFromQueue()));

}

qint64 MSolver::getTimeElapsed()
{
    return timeElapsed;
}

void MSolver::setParameters(QHash<int, MazeNode *> *listOfIds, int nrows, int ncolumns,int tick)
{
    rows = nrows;
    columns = ncolumns;
    nodeHash = listOfIds;
    tickInterval = tick;
}

void MSolver::startDFS(){
    stack = new QStack<MazeNode*>();
    stopwatch->restart();

    MazeNode *start = nodeHash->value(0);
    stack->push(start);

    tickerDFS->blockSignals(false);
    tickerDFS->start(tickInterval);
}

void MSolver::startBFS(){
    queue = new QQueue<MazeNode*>();
    stopwatch->restart();

    MazeNode *start = nodeHash->value(0);
    queue->enqueue(start);

    tickerBFS->blockSignals(false);
    tickerBFS->start(tickInterval);
}

void MSolver::triggerStopSearch(){
    timeElapsed = stopwatch->elapsed();
    stopwatch->invalidate();
    interruptSearch = true;
}

void MSolver::stopSearch(MazeNode *exitNode)
{
    timeElapsed = stopwatch->elapsed();
    stopwatch->invalidate();

    tickerDFS->blockSignals(true);
    tickerDFS->stop();

    tickerBFS->blockSignals(true);
    tickerBFS->stop();

    if(stack != 0){
        stack->clear();
    }
    if(queue != 0){
        queue->clear();
    }

    emit displayExit(exitNode);
}

QList<MazeNode *> *MSolver::getAdjacentUnvisitedNodes(int currentID)
{
    QList<MazeNode*> *neighbours = new QList<MazeNode*>();
    int totalNumberOfNodes = rows * columns;

    MazeNode *adjacentNode;
    if((currentID) < (totalNumberOfNodes-columns)){
        adjacentNode = nodeHash->value(currentID+columns);
        if(!adjacentNode->isWall() && !adjacentNode->hasBeenVisited()){
            neighbours->append(adjacentNode);
        }
    }
    if(currentID > columns){
        adjacentNode = nodeHash->value(currentID-columns);

        if(!adjacentNode->isWall() && !adjacentNode->hasBeenVisited()){
            neighbours->append(adjacentNode);
        }
    }

    if(((currentID+1) % columns) != 0){
        adjacentNode = nodeHash->value(currentID+1);
        if(!adjacentNode->isWall() && (currentID+1) <= totalNumberOfNodes && !adjacentNode->hasBeenVisited()){
            neighbours->append(adjacentNode);
        }
    }
    if(currentID != 0 && ((currentID) % columns) != 0){
        adjacentNode = nodeHash->value(currentID-1);
        if(!adjacentNode->isWall() && !adjacentNode->hasBeenVisited()){
            neighbours->append(adjacentNode);
        }
    }
    return neighbours;
}

void MSolver::takeFromStack()
{
    if(interruptSearch){
        qDebug() << "Interrupted";
        tickerDFS->blockSignals(true);
        tickerDFS->stop();
        interruptSearch = false;
        stopSearch(0);
        return;
    }

    int currentID;
    MazeNode *currentNode;
    MazeNode *nextNode;
    QList<MazeNode*> *neighbours;
    int validNodeCounter = 0;
    bool exitFound = false;
    if(!stack->isEmpty()){
        currentNode = stack->pop();
        currentNode->setActive(true);
        currentID = currentNode->getId();
        neighbours = getAdjacentUnvisitedNodes(currentID);
        while(!neighbours->isEmpty()){
            nextNode = neighbours->takeAt(rand() % neighbours->size());
            if(nextNode->isWall() || nextNode->hasBeenVisited()){
                continue;
            }
            validNodeCounter++;
            nextNode->setPreviousNode(currentNode);
            stack->push(nextNode);
            if(nextNode->isExit()){
                exitFound = true;
                break;
            }
        }
        currentNode->setVisited(true);

        if(exitFound){
            stopSearch(nextNode);
        }
    }
    else{
        stopSearch(0);
        return;
    }

    if(validNodeCounter == 0){
        validNodeCounter = 0;
        tickerDFS->stop();
        takeFromStack();
        tickerDFS->start(tickInterval);
    }

}
void MSolver::takeFromQueue(){
    if(interruptSearch){
        tickerBFS->blockSignals(true);
        tickerBFS->stop();
        interruptSearch = false;
        stopSearch(0);
        return;
    }

    int currentID;
    MazeNode *currentNode;
    MazeNode *nextNode;
    QList<MazeNode*> *neighbours;
    int validNodeCounter = 0;
    bool exitFound = false;
    if(!queue->isEmpty()){
        currentNode = queue->dequeue();

        if(currentNode->hasBeenVisited()){
            return;
        }

        currentNode->setActive(true);
        currentID = currentNode->getId();
        neighbours = getAdjacentUnvisitedNodes(currentID);

        while(!neighbours->isEmpty()){
            nextNode = neighbours->takeFirst();
            if(nextNode->isWall() || nextNode->hasBeenVisited()){
                continue;
            }
            validNodeCounter++;
            nextNode->setPreviousNode(currentNode);
            queue->enqueue(nextNode);
            if(nextNode->isExit()){
                exitFound = true;
                break;
            }


        }

        currentNode->setVisited(true);

        if(exitFound){
            stopSearch(nextNode);
        }
    }
    else{
        stopSearch(0);
        return;
    }
    if(validNodeCounter == 0){
        validNodeCounter = 0;
        takeFromQueue();
    }


}

