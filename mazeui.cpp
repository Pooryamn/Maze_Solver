#include "mazeui.h"

MazeUi::MazeUi(QWidget *parent) : QGraphicsView(parent)
{
    currentTab = parent;
    sceneHeight = 600;
    sceneWidth = 750;
    createGridSizeArray();
    rectSize = rectSizeList[1][0];
    nodeDescriptionThreshold = rectSizeList[1][2];
    listOfRectangles = new QHash<QGraphicsItem *,MazeNode *>();
    listOfIds = new QHash<int,MazeNode*>();
    setupUI();
    setDefaultSelections();
    drawMaze();
    initializeMazeSolver();
}

MazeUi::~MazeUi(){
    delete listOfRectangles;
    delete listOfIds;
}

void MazeUi::createGridSizeArray(){
    rectSizeList = new int*[2];
    for(int ii = 0; ii < 2; ii++)
        rectSizeList[ii] = new int[6];

    rectSizeList[0][0] = 20;
    rectSizeList[0][1] = 80;
    rectSizeList[0][2] = 500;

    rectSizeList[1][0] = 150;
    rectSizeList[1][1] = 75;
    rectSizeList[1][2] = 30;

}

void MazeUi::setupUI(){

    QGridLayout *gridLayout = new QGridLayout();
    currentTab->setLayout(gridLayout);
    setGeometry(0,0,sceneWidth,sceneHeight);
    scene = new QGraphicsScene();
    setScene(scene);

    gridLayout->addWidget(this,0,0,3,1, Qt::AlignLeft| Qt::AlignTop);
    QSpacerItem *viewSpacer = new QSpacerItem(10,0);
    gridLayout->addItem(viewSpacer,0,1);

    QGroupBox *mazeGroupBox = new QGroupBox("Maze",this);
    gridLayout->addWidget(mazeGroupBox,0,2);
    addItemsToMazeGroupBox(mazeGroupBox);

    QGroupBox *toolsGroupBox = new QGroupBox("Tools",this);
    gridLayout->addWidget(toolsGroupBox,1,2);
    addItemsToToolsGroupBox(toolsGroupBox);

    QGroupBox *logGroupBox = new QGroupBox("Log",this);
    gridLayout->addWidget(logGroupBox,2,2);
    addItemsToLogGroupBox(logGroupBox);
}

void MazeUi::addItemsToMazeGroupBox(QGroupBox *mazeGroupBox){
    QFormLayout *controlLayout = new QFormLayout();
    mazeGroupBox->setLayout(controlLayout);

    QLabel *searchDescription = new QLabel("Search algorithm");
    searchSelection = new QComboBox();
    searchSelection->addItem("DFS");
    searchSelection->addItem("BFS");
    controlLayout->addRow(searchDescription,searchSelection);

    QLabel *gridSizeDescription = new QLabel("Select Grid size");
    gridSizeSelection = new QComboBox();
    gridSizeSelection->addItem(QString::number(rectSizeList[0][0]));
    gridSizeSelection->addItem(QString::number(rectSizeList[0][1]));
    gridSizeSelection->addItem(QString::number(rectSizeList[0][2]));
    controlLayout->addRow(gridSizeDescription,gridSizeSelection);

    startSearchButton = new QPushButton("Start");
    controlLayout->addRow(startSearchButton);


    stopSearchButton = new QPushButton("Stop");
    controlLayout->addRow(stopSearchButton);
    stopSearchButton->setVisible(false);

    resetMazeButton = new QPushButton("Restart");
    controlLayout->addRow(resetMazeButton);

    clearMazeButton = new QPushButton("Clear");
    controlLayout->addRow(clearMazeButton);

    connect(gridSizeSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(setNewGridSize()));
    connect(startSearchButton,SIGNAL(clicked()),this,SLOT(startSearch()));
    connect(stopSearchButton,SIGNAL(clicked()),this,SLOT(stopSearch()));
    connect(resetMazeButton,SIGNAL(clicked()),this,SLOT(resetMaze()));
    connect(clearMazeButton,SIGNAL(clicked()),this,SLOT(clearMaze()));

}

void MazeUi::addItemsToToolsGroupBox(QGroupBox *toolsGroupBox)
{
    QFormLayout *controlLayout = new QFormLayout();
    toolsGroupBox->setLayout(controlLayout);

    QPushButton *createRandomMazeButton = new QPushButton("Random");
    controlLayout->addRow(createRandomMazeButton);

    QPushButton *aboutUS = new QPushButton("About Us");
    controlLayout->addRow(aboutUS);
    connect(createRandomMazeButton,SIGNAL(clicked()),this,SLOT(createRandomMaze()));
    connect(aboutUS,SIGNAL(clicked()),this,SLOT(About()));
}

void MazeUi::addItemsToLogGroupBox(QGroupBox *logGroupBox)
{
    QGridLayout *controlLayout = new QGridLayout();
    logGroupBox->setLayout(controlLayout);

    log = new QTextBrowser();
    controlLayout->addWidget(log);

}

void MazeUi::setEntranceAndExit()
{
    listOfIds->value(0)->setEntrance(true);
    listOfIds->value(listOfIds->size()-1)->setExit(true);

}

void MazeUi::resetMaze()
{
    foreach(MazeNode *item, *listOfRectangles){
        if(!item->isWall()){
            item->unsetWall();
            item->setEntrance(false);
            item->setVisited(false);
            item->setExit(false);
            item->setActive(false);

            if(rectSize > nodeDescriptionThreshold){
                item->setDescripionToId();
            }
        }
    }
    setEntranceAndExit();
    startSearchButton->setEnabled(true);
    log->clear();


}

void MazeUi::drawMaze(){
    addItemsToScene();
}

void MazeUi::switchUiState()
{
    startSearchButton->setEnabled(!startSearchButton->isEnabled());
    resetMazeButton->setEnabled(!resetMazeButton->isEnabled());
    gridSizeSelection->setEnabled(!gridSizeSelection->isEnabled());
    searchSelection->setEnabled(!searchSelection->isEnabled());
    clearMazeButton->setEnabled(!clearMazeButton->isEnabled());
}

void MazeUi::fillLogWithPath(QStack<int> *path)
{
    while(!path->isEmpty()){
        log->append(QString::number(path->pop()));
    }
}

void MazeUi::initializeMazeSolver()
{
    int columns = sceneWidth / rectSize;
    int rows = sceneHeight / rectSize;
    solver = new MSolver(listOfIds,rows,columns,30);
    connect(solver,SIGNAL(displayExit(MazeNode*)),this,SLOT(displayResult(MazeNode*)));

}

void MazeUi::setDefaultSelections()
{
    gridSizeSelection->setCurrentIndex(2);
    rectSize = rectSizeList[1][gridSizeSelection->currentIndex()];
    searchSelection->setCurrentIndex(1);
}

void MazeUi::addItemsToScene(){

    MazeNode *node;
    int columns = sceneWidth / rectSize;
    int rows = sceneHeight / rectSize;
    int ii = 0;
    int jj = 0;
    int idCounter = 0;
    while(ii < rows){
        while(jj < columns){
            node = new MazeNode(idCounter,jj*rectSize,ii * rectSize,rectSize);
            scene->addItem(node->getRectangle());
            node->unsetWall();
            listOfRectangles->insert(node->getRectangle(), node);
            listOfIds->insert(idCounter,node);
            if(rectSize > nodeDescriptionThreshold){
                node->setDescripionToId();
            }
            jj++;
            idCounter++;
        }
        jj = 0;
        ii++;
    }

    setEntranceAndExit();
}

void MazeUi::clearMaze()
{
    foreach(MazeNode *item, *listOfRectangles){
        item->unsetWall();
        item->setEntrance(false);
        item->setVisited(false);
        item->setExit(false);
        item->setActive(false);

        if(rectSize > nodeDescriptionThreshold){
            item->setDescripionToId();
        }
    }
    setEntranceAndExit();
    startSearchButton->setEnabled(true);
    log->clear();

}

void MazeUi::mousePressEvent(QMouseEvent *me){
    QGraphicsItem *current = itemAt(me->pos());
    if(!current){
        return;
    }

    MazeNode *selectedMazeNode = listOfRectangles->value(current);

    if(selectedMazeNode == 0){
        return;
    }
    if(me->button() == Qt::LeftButton){
        if(selectedMazeNode->isWall()){
            selectedMazeNode->unsetWall();
        }
        else{
            selectedMazeNode->setWall();
        }
    }


}

void MazeUi::setNewGridSize()
{

    if(rectSize == rectSizeList[1][gridSizeSelection->currentIndex()]){
        return;
    }
    rectSize = rectSizeList[1][gridSizeSelection->currentIndex()];
    scene->clear();

    foreach(MazeNode *item, *listOfRectangles){
        delete item;
    }

    listOfRectangles->clear();
    listOfIds->clear();
    addItemsToScene();
    startSearchButton->setEnabled(true);
}

void MazeUi::startSearch()
{
    log->append("Starting search " + searchSelection->currentText());

    switchUiState();
    startSearchButton->setVisible(false);
    stopSearchButton->setVisible(true);

    int columns = sceneWidth / rectSize;
    int rows = sceneHeight / rectSize;

    solver->setParameters(listOfIds,rows,columns,30);

    if(searchSelection->currentText() == "DFS"){
        solver->startDFS();
    }
    else if(searchSelection->currentText() == "BFS"){
        solver->startBFS();
    }
    else{
        log->append("This algorithm hasn't been implemented yet");
        switchUiState();
    }
}

void MazeUi::stopSearch()
{
    stopSearchButton->setVisible(false);
    startSearchButton->setVisible(true);
    solver->triggerStopSearch();
}

int MazeUi::tracePath(MazeNode* lastNode,QStack<int> *nodeStack)
{
    int counter = 0;
    while(lastNode != 0){
        lastNode->tracePath();
        nodeStack->push(lastNode->getId());
        lastNode = lastNode->getPreviousNode();
        counter++;
    }

    return counter;

}

void MazeUi::createRandomMaze()
{
    clearMaze();
    foreach(MazeNode *item, *listOfRectangles){
        if(!item->isEntrance() || !item->isExit() ){
            int isWallDecider = qrand() % 3;
            if(isWallDecider >= 2){
                item->setWall();
            }
        }
    }
    log->append("Created random maze");
}

void MazeUi::displayResult(MazeNode *lastNode)
{
    if(lastNode != 0){
        QStack<int> *path = new QStack<int>;
        int lengthOfPath = tracePath(lastNode,path);
        fillLogWithPath(path);
        log->append("Length of the path: " + QString::number(lengthOfPath));
        log->append("Seconds elapsed: " + QString::number(solver->getTimeElapsed() / 1000.0,'f',3));
        delete path;
    }
    else{
        log->append("No path to the exit found");
    }
    switchUiState();
    startSearchButton->setEnabled(false);
    startSearchButton->setVisible(true);
    stopSearchButton->setVisible(false);
}

void MazeUi::About(){
    QString a= ".: Poorya Mohammadi Nasab\n.: 9621160025\n.: Data Structure Project";
    log->setText(a);
}
