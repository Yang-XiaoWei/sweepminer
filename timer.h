#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <qwidget.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qdatetime.h>
#include <qmainwindow.h>

class Timer : public QWidget
{
    Q_OBJECT
public:
    Timer(QWidget *parent = 0);
    ~Timer();

    public slots:
    void                startTimer();
    void                stopTimer();

    private slots:
    void                myTimerUpdate();
private:
    QTimer*     timer;
    QLabel*     showTime;
    int         usedSecond;
    int         usedMsecond;
};



#endif