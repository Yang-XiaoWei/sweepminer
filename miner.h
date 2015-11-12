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

#include "timer.h"

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
    ~Miner();
    bool                getIsMiner();
    //设置该方块为地雷，默认不为雷，不存在设置为非雷的情况
    void                setIsMiner();
    void                setMinerNumAround(int num);
    int                 getMinerNumAround();
    MINER_STATE         getCurState();
    void                mousePressEvent(QMouseEvent* event);
    //用于安全翻开当前方块，并显示周围雷的数量
    void                openBlock();
    bool                getIsInDealQueue();
    void                setIsInDealQueue();
    bool                getIsDouted();

public slots:
    //左键打开方块，用来响应手动单击打开方块，以及在双击方块后相邻方块的此函数会被响应。
    void                leftClick();

public:
    static void         setIsFirstClick();
    static void         setIsMouseActive(bool flag);

signals:
    //用来初次点击时响应MinerManager进行地雷初始化，第一次点击的位置以及周围8个格子不产生地雷
    void                initializeMiners(const int& clickRow, const int& clickColumn); 
    //当前方块被安全打开后，发射信号通知MinerManager将安全方块统计+1,作为游戏结束的判断依据,openblock()函数里调用
    void                addSafeBlockNum();
    //地雷方块被翻开后发射爆炸的信号，通知MinerManager游戏结束
    void                explode();
    //安全点击方块，发射信号通知MinerManager翻开相邻8格里minernumAround为0的方块。
    void                safeClick(const int& clickRow, const int& clickColumn);   
    //周围有雷，但是已经标记完成，双击后打开周围8格，标记错误则爆炸，用来发射信号
    void                doubleClick(const int& clickRow, const int& clickColumn);     
    //点击任意一个方块都可以开始游戏，所以开始计时的信号放在Miner里
    void                startTimer();       
    //右键标记地雷和取消标记时给MinerManager发射信号，由其来统计当前剩余的地雷数量
    void                setDouted();
    void                unsetDouted();
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

    ~MinerManager();
    void                initial(const int& row, const int& column, const int& minerNum);
    void                gameOver(bool isWin);

private:
    MinerManager(QWidget* parent = 0);
    MinerManager(const MinerManager&) {};
    MinerManager operator=(const MinerManager&) {};

    //用于单击后自动翻开非雷方块
    void                openBlock(const int& clickRow, const int& clickColumn, std::queue<int>& qIndex);
    //初始化游戏结束时的对话框，由于提示信息基本一样，先初始化好，必要时显示即可。
    void                initializeGameOverDialog();
    //初始化计时器和余雷计数器的位置
    void                initializeTimerAndCounter();
    //由于经常需要统计一个方块周围8个格子的地雷数量、标记数量等，第三个参数是成员函数指针，返回值为true的时候统计计数+1；
    int                 calculateAround(const int& clickRow, const int& clickColumn, bool (Miner::*fun)());
    void                handleAround(const int& clickRow, const int& clickColumn, void (Miner::*fun)());

signals:
    //游戏的结束由MinerManager判断，所以停止计时的信号由此发出
    void                stopTimer();

public slots:
    //初始化游戏的信号槽，用于游戏结束后再试一次时重新初始化所有方块
    void                initial();

private slots:
    //初始化地雷位置
    void                initialMiners(const int& clickRow, const int& clickColumn);
    void                explode();
    //增加安全方块统计
    void                addSafeBlockNum();
    //安全单击方块后响应，此时会依次翻开连续的无雷方块
    void                safeClick(const int& clickRow, const int& clickColumn);
    //双击已打开方块后响应，标记完地雷后自动翻开相邻8格的非雷方块，
    void                doubleClick(const int& clickRow, const int& clickColumn);
    //标记地雷和删除标记后的余雷数量统计,屏幕右下角用来显示余下的地雷数量
    void                doutedIncrease();
    void                doutedDecrease();

private:
    int                 row;
    int                 column;
    int                 totalNum;                   //记录总的方格数目
    int                 minerNum;                   //记录地雷数目
    int                 safeBlockNum;               //记录已经翻开的方块数目，等于totalNum-minerNum时游戏通关
    int                 doutedNum;                  //所有被怀疑是雷的方块数目统计，用于提示当前还剩下多少个雷
    QWidget*            minersWidget;               //用来加载所有扫雷方块
    QWidget*            timerWidget;                //用来加载计时器和余雷计数，与上面方块要垂直显示
    QGridLayout*        minersLayout;               //用来排列所有方块位置
    QDialog*            gameOverDialog;             //游戏结束对话框
    QLabel*             gameFinishLabel;            //游戏结束的提示语
    QLabel*             doutedNumLabel;             //余雷计数
    Timer*              gameTimer;                  //统计当前游戏时间
    std::vector<Miner*> miners;                     //所有方块全部存在这里

};

#endif
