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
    ENUM_UNKNOWN,       //δ֪״̬�����ŵģ���Ӧ������궯��
    ENUM_KNOWN,         //�ѷ�����ȷ�������ף�ֻ��Ӧ˫���¼�����������Χ8�����ӵ���
    ENUM_DOUTED,        //�Ҽ��������ף�ֻ��Ӧ����Ҽ�ȡ������״̬�������������±�ը��
};
class Miner : public QLabel
{
    Q_OBJECT
public:
    Miner(int row, int column, QWidget* parent = 0);
    ~Miner();
    bool                getIsMiner();
    //���ø÷���Ϊ���ף�Ĭ�ϲ�Ϊ�ף�����������Ϊ���׵����
    void                setIsMiner();
    void                setMinerNumAround(int num);
    int                 getMinerNumAround();
    MINER_STATE         getCurState();
    void                mousePressEvent(QMouseEvent* event);
    //���ڰ�ȫ������ǰ���飬����ʾ��Χ�׵�����
    void                openBlock();
    bool                getIsInDealQueue();
    void                setIsInDealQueue();
    bool                getIsDouted();

public slots:
    //����򿪷��飬������Ӧ�ֶ������򿪷��飬�Լ���˫����������ڷ���Ĵ˺����ᱻ��Ӧ��
    void                leftClick();

public:
    static void         setIsFirstClick();
    static void         setIsMouseActive(bool flag);

signals:
    //�������ε��ʱ��ӦMinerManager���е��׳�ʼ������һ�ε����λ���Լ���Χ8�����Ӳ���������
    void                initializeMiners(const int& clickRow, const int& clickColumn); 
    //��ǰ���鱻��ȫ�򿪺󣬷����ź�֪ͨMinerManager����ȫ����ͳ��+1,��Ϊ��Ϸ�������ж�����,openblock()���������
    void                addSafeBlockNum();
    //���׷��鱻�������䱬ը���źţ�֪ͨMinerManager��Ϸ����
    void                explode();
    //��ȫ������飬�����ź�֪ͨMinerManager��������8����minernumAroundΪ0�ķ��顣
    void                safeClick(const int& clickRow, const int& clickColumn);   
    //��Χ���ף������Ѿ������ɣ�˫�������Χ8�񣬱�Ǵ�����ը�����������ź�
    void                doubleClick(const int& clickRow, const int& clickColumn);     
    //�������һ�����鶼���Կ�ʼ��Ϸ�����Կ�ʼ��ʱ���źŷ���Miner��
    void                startTimer();       
    //�Ҽ���ǵ��׺�ȡ�����ʱ��MinerManager�����źţ�������ͳ�Ƶ�ǰʣ��ĵ�������
    void                setDouted();
    void                unsetDouted();
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

    ~MinerManager();
    void                initial(const int& row, const int& column, const int& minerNum);
    void                gameOver(bool isWin);

private:
    MinerManager(QWidget* parent = 0);
    MinerManager(const MinerManager&) {};
    MinerManager operator=(const MinerManager&) {};

    //���ڵ������Զ��������׷���
    void                openBlock(const int& clickRow, const int& clickColumn, std::queue<int>& qIndex);
    //��ʼ����Ϸ����ʱ�ĶԻ���������ʾ��Ϣ����һ�����ȳ�ʼ���ã���Ҫʱ��ʾ���ɡ�
    void                initializeGameOverDialog();
    //��ʼ����ʱ�������׼�������λ��
    void                initializeTimerAndCounter();
    //���ھ�����Ҫͳ��һ��������Χ8�����ӵĵ�����������������ȣ������������ǳ�Ա����ָ�룬����ֵΪtrue��ʱ��ͳ�Ƽ���+1��
    int                 calculateAround(const int& clickRow, const int& clickColumn, bool (Miner::*fun)());
    void                handleAround(const int& clickRow, const int& clickColumn, void (Miner::*fun)());

signals:
    //��Ϸ�Ľ�����MinerManager�жϣ�����ֹͣ��ʱ���ź��ɴ˷���
    void                stopTimer();

public slots:
    //��ʼ����Ϸ���źŲۣ�������Ϸ����������һ��ʱ���³�ʼ�����з���
    void                initial();

private slots:
    //��ʼ������λ��
    void                initialMiners(const int& clickRow, const int& clickColumn);
    void                explode();
    //���Ӱ�ȫ����ͳ��
    void                addSafeBlockNum();
    //��ȫ�����������Ӧ����ʱ�����η������������׷���
    void                safeClick(const int& clickRow, const int& clickColumn);
    //˫���Ѵ򿪷������Ӧ���������׺��Զ���������8��ķ��׷��飬
    void                doubleClick(const int& clickRow, const int& clickColumn);
    //��ǵ��׺�ɾ����Ǻ����������ͳ��,��Ļ���½�������ʾ���µĵ�������
    void                doutedIncrease();
    void                doutedDecrease();

private:
    int                 row;
    int                 column;
    int                 totalNum;                   //��¼�ܵķ�����Ŀ
    int                 minerNum;                   //��¼������Ŀ
    int                 safeBlockNum;               //��¼�Ѿ������ķ�����Ŀ������totalNum-minerNumʱ��Ϸͨ��
    int                 doutedNum;                  //���б��������׵ķ�����Ŀͳ�ƣ�������ʾ��ǰ��ʣ�¶��ٸ���
    QWidget*            minersWidget;               //������������ɨ�׷���
    QWidget*            timerWidget;                //�������ؼ�ʱ�������׼����������淽��Ҫ��ֱ��ʾ
    QGridLayout*        minersLayout;               //�����������з���λ��
    QDialog*            gameOverDialog;             //��Ϸ�����Ի���
    QLabel*             gameFinishLabel;            //��Ϸ��������ʾ��
    QLabel*             doutedNumLabel;             //���׼���
    Timer*              gameTimer;                  //ͳ�Ƶ�ǰ��Ϸʱ��
    std::vector<Miner*> miners;                     //���з���ȫ����������

};

#endif
