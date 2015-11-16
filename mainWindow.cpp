#include <qlayout.h>

#include "mainWindow.h"

MainWindow::MainWindow()
{
    setCentralWidget(MinerManager::shared());
    MinerManager::shared()->initial(16, 30, 99);
    setWindowFlags(Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

    menuNewGame = menuBar()->addMenu(QString("new game"));

    QAction* simpleGame = new QAction(QString("10 * 10"), this);
    connect(simpleGame, SIGNAL(triggered()), this, SLOT(simpleLevelGame()));
    menuNewGame->addAction(simpleGame);

    QAction* middleGame = new QAction(QString("16 * 16"), this);
    connect(middleGame, SIGNAL(triggered()), this, SLOT(middleLevelGame()));
    menuNewGame->addAction(middleGame);
    
    QAction* hardGame = new QAction(QString("16 * 30"), this);
    connect(hardGame, SIGNAL(triggered()), this, SLOT(hardLevelGame()));
    menuNewGame->addAction(hardGame);

    menuHelp = menuBar()->addMenu(QString("tips"));
    QAction* actionHint = new QAction(QString("hint"), this);
    connect(actionHint, SIGNAL(triggered()), this, SLOT(showHint()));
    menuHelp->addAction(actionHint);

}

MainWindow::~MainWindow()
{

}

void MainWindow::simpleLevelGame()
{
    MinerManager::shared()->initial(10, 10, 9);
    adjustSize();
}

void MainWindow::middleLevelGame()
{
    MinerManager::shared()->initial(16, 16, 40);
    adjustSize();
}

void MainWindow::hardLevelGame()
{
    MinerManager::shared()->initial(16, 30, 99);
    adjustSize();
}

void MainWindow::showHint()
{
    MinerManager::shared()->showHint();
}
