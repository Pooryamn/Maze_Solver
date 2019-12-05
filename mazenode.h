#ifndef MAZENODE_H
#define MAZENODE_H

#include <QWidget>
#include <QRect>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QPen>
#include <QBrush>
#include <QDebug>

class MazeNode
{

public:
    explicit MazeNode(int newId, int x, int y, int rectSize);
    ~MazeNode();
    QGraphicsRectItem *getRectangle();
    QPointF *getPosition();
    int getId();
    MazeNode *getPreviousNode();
    bool isWall();
    bool isEntrance();
    bool isExit();
    bool hasBeenVisited();
    void setActive(bool active);
    void setDescripionToId();
    void setEntrance(bool isEntrance);
    void setExit(bool isExit);
    void setVisited(bool value);
    void setWall();
    void setPreviousNode(MazeNode *node);
    void tracePath();
    void unsetWall();

private:
    int rectSize;
    int id;
    QGraphicsRectItem *rectangle;
    QPointF *position;
    QGraphicsTextItem *description;
    MazeNode *previousNode;

    bool nodeIsWall;
    bool nodeIsEntrance;
    bool nodeIsExit;
    bool visited;
    void createRectangle(int x,int y,int rectSize);
};

#endif // MAZENODE_H
