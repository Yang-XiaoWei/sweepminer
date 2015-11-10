#pragma once
#ifndef MINRE_H
#define MINER_H

#include <qwidget.h>
#include <qlabel.h>
#include <qevent.h>
#include <qlayout.h>
#include <qdialog.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qobject.h>
#include <qtimer.h>
#include <vector>
#include <queue>
#include <iostream>
#include <random>
#include <time.h>

enum MINER_STATE
{
    ENUM_UNKNOWN,       //未知状态，盖着的，响应所有鼠标动作
    ENUM_KNOWN,         //已翻开，确定不是雷，只响应双击事件，用来打开周围8个格子的雷
    ENUM_DOUTED,        //右键怀疑是雷，只响应鼠标右键取消怀疑状态，避免误点击导致爆炸。
};
class Miner : public QLabel
{
    Q_OBJECT
public:
    Miner(int row, int column, QWidget* parent = 0);
    bool                getIsMiner();
    void                setIsMiner();
    void                setMinerNumAround(int num);
    int                 getMinerNumAround();
    MINER_STATE         getCurState();
    void                mousePressEvent(QMouseEvent* event);
    void                openBlock();
    bool                getIsInDealQueue();
    void                setIsInDealQueue();
    bool                getIsDouted();

public slots:
    void                leftClick();

public:
    static void         setIsFirstClick();
    static void         setIsMouseActive(bool flag);

signals:
    void                initializeMiners(const int& clickRow, const int& clickColumn);      //用来初次点击时响应MinerManager进行地雷初始化，第一次点击的位置不产生地雷
    void                explode();
    void                safeClick(const int& clickRow, const int& clickColumn);             //安全点击，用来打开所有相邻雷数量是0的邻居，
    void                doubleClick(const int& clickRow, const int& clickColumn);           //周围有雷，但是已经标记完成，双击后打开周围8格，标记错误则爆炸

private:
    int                 row;                    //该方块所处的行
    int                 column;                 //该方块所处的列
    bool                isMiner;                //是否是雷
    int                 minerNumAround;         //周围8个方块雷的数目
    MINER_STATE         curState;               //当前状态，决定响应什么鼠标事件
    bool                isDouted;               //是否被判定为雷，用来统计雷数量
    bool                isInDealQueue;          //用来点击后自动翻开雷是，将周围8个无雷的空格放入处理队列，防止重复放入
    QTimer*             clickInterval;          //记录鼠标点击事件间隔，区分单击和双击。

private:
    static bool         isFirstClick;             //是否第一次点击，如果是，需要通知MinerManager初始化地雷位置。
    static bool         isMouseActive;            //记录是否激活鼠标事件，游戏结束时不响应。       

};

class MinerManager : public QWidget
{
    Q_OBJECT
public:
    static MinerManager* shared();
    void                initial(const int& row, const int& column, const int& minerNum);
    void                gameOver(bool isWin);
    ~MinerManager();

private:
    MinerManager(QWidget* parent = 0);
    MinerManager(const MinerManager&) {};
    MinerManager operator=(const MinerManager&) {};

    //用于单击后自动翻开非雷方块
    void                openBlock(const int& clickRow, const int& clickColumn, std::queue<int>& qIndex);

public slots:
    void                initial();
    void                initialMiners(const int& clickRow, const int& clickColumn);
    
private slots:
    void                explode();
    void                safeClick(const int& clickRow, const int& clickColumn);
    void                doubleClick(const int& clickRow, const int& clickColumn);

private:
    int                 row;
    int                 column;
    int                 totalNum;                   //记录总的方格数目
    int                 minerNum;                   //记录地雷数目
    int                 safeBlockNum;               //记录已经翻开的方块数目，等于totalNum-minerNum时游戏通关
    QGridLayout*        minersLayout;
    QDialog*            gameOverDialog;
    QLabel*             gameFinishLabel;
    std::vector<Miner*> miners;

};

#endif
