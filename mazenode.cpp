#include "mazenode.h"

MazeNode::MazeNode(int newId, int x, int y, int rectSize)
{
    createRectangle(x,y,rectSize);
    nodeIsWall = false;
    visited = false;
    nodeIsEntrance= false;
    nodeIsExit = false;
    position = new QPointF(x,y);
    id = newId;
    previousNode = 0;
    description = new QGraphicsTextItem("",rectangle);
    description->setPos(position->x(),position->y());
}

MazeNode::~MazeNode()
{
    delete position;
}

QGraphicsRectItem *MazeNode::getRectangle()
{
    return rectangle;
}

QPointF *MazeNode::getPosition()
{
    return position;
}

int MazeNode::getId(){
    return id;
}

MazeNode *MazeNode::getPreviousNode()
{
    return previousNode;
}

bool MazeNode::isWall()
{
    return nodeIsWall;
}

bool MazeNode::isEntrance()
{
    return nodeIsEntrance;
}

bool MazeNode::isExit()
{
    return nodeIsExit;
}

bool MazeNode::hasBeenVisited()
{
    return visited;
}

void MazeNode::setActive(bool active)
{
    if(active){
        rectangle->setBrush(QBrush(Qt::yellow));
    }
    else{
        rectangle->setBrush(QColor(167,9,248));
    }
}

void MazeNode::setDescripionToId()
{
    description->setPlainText(QString::number(id));
}
void MazeNode::setEntrance(bool isEntrance)
{
    nodeIsEntrance = isEntrance;
    if(isEntrance){
        rectangle->setBrush(QColor(76,243,9));
    }
}

void MazeNode::setExit(bool isExit)
{
    nodeIsExit = isExit;

    if(isExit){
        rectangle->setBrush(QColor(5,20,134));
    }

}

void MazeNode::setVisited(bool value)
{
    visited = value;

    if(value){
        rectangle->setBrush(QColor(249,188,4));
    }
    else{
        rectangle->setBrush(QColor(167,9,248));
    }
}

void MazeNode::setWall()
{
    if(nodeIsEntrance || nodeIsExit){
        return;
    }

    nodeIsWall = true;
    rectangle->setBrush(QColor(42,1,1));
}

void MazeNode::setPreviousNode(MazeNode *node)
{
    previousNode = node;
}

void MazeNode::tracePath()
{
    rectangle->setBrush(QColor(12,94,84));
}

void MazeNode::unsetWall()
{
    nodeIsWall = false;
    rectangle->setBrush(QColor(167,9,248));

}

void MazeNode::createRectangle(int x, int y, int rectSize)
{
    rectangle = new QGraphicsRectItem(x,y,rectSize,rectSize);
}

