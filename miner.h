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
    ENUM_UNKNOWN,       //δ֪״̬�����ŵģ���Ӧ������궯��
    ENUM_KNOWN,         //�ѷ�����ȷ�������ף�ֻ��Ӧ˫���¼�����������Χ8�����ӵ���
    ENUM_DOUTED,        //�Ҽ��������ף�ֻ��Ӧ����Ҽ�ȡ������״̬�������������±�ը��
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
    void                initializeMiners(const int& clickRow, const int& clickColumn);      //�������ε��ʱ��ӦMinerManager���е��׳�ʼ������һ�ε����λ�ò���������
    void                explode();
    void                safeClick(const int& clickRow, const int& clickColumn);             //��ȫ���������������������������0���ھӣ�
    void                doubleClick(const int& clickRow, const int& clickColumn);           //��Χ���ף������Ѿ������ɣ�˫�������Χ8�񣬱�Ǵ�����ը

private:
    int                 row;                    //�÷�����������
    int                 column;                 //�÷�����������
    bool                isMiner;                //�Ƿ�����
    int                 minerNumAround;         //��Χ8�������׵���Ŀ
    MINER_STATE         curState;               //��ǰ״̬��������Ӧʲô����¼�
    bool                isDouted;               //�Ƿ��ж�Ϊ�ף�����ͳ��������
    bool                isInDealQueue;          //����������Զ��������ǣ�����Χ8�����׵Ŀո���봦����У���ֹ�ظ�����
    QTimer*             clickInterval;          //��¼������¼���������ֵ�����˫����

private:
    static bool         isFirstClick;             //�Ƿ��һ�ε��������ǣ���Ҫ֪ͨMinerManager��ʼ������λ�á�
    static bool         isMouseActive;            //��¼�Ƿ񼤻�����¼�����Ϸ����ʱ����Ӧ��       

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

    //���ڵ������Զ��������׷���
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
    int                 totalNum;                   //��¼�ܵķ�����Ŀ
    int                 minerNum;                   //��¼������Ŀ
    int                 safeBlockNum;               //��¼�Ѿ������ķ�����Ŀ������totalNum-minerNumʱ��Ϸͨ��
    QGridLayout*        minersLayout;
    QDialog*            gameOverDialog;
    QLabel*             gameFinishLabel;
    std::vector<Miner*> miners;

};

#endif
