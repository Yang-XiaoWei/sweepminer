#pragma once
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <qmainwindow.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qaction.h>
#include <qobject.h>

#include "miner.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    private slots:
    void                simpleLevelGame();
    void                middleLevelGame();
    void                hardLevelGame();
    void                showHint();

private:
    QMenu*              menuNewGame;
    QMenu*              menuHelp;

};


#endif
