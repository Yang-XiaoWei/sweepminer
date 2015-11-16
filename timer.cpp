#include <qmessagebox.h>
#include <time.h>
#include <iostream>

#include "timer.h"

Timer::Timer(QWidget* parent)
{
    timer = new QTimer();
    showTime = new QLabel();
    usedSecond = 0;
    usedMsecond = 0;

    QFont timerFont;
    timerFont.setPointSize(15);
    showTime->setFont(timerFont);
    showTime->setText(QString::number(usedSecond) + "." + QString::number(usedMsecond));
    showTime->setAlignment(Qt::AlignCenter);

    connect(timer, SIGNAL(timeout()), this, SLOT(myTimerUpdate()));

    timer->setInterval(100);

    QHBoxLayout *timerLayout = new QHBoxLayout();
    timerLayout->addWidget(showTime);
    setLayout(timerLayout);
}

Timer::~Timer()
{
    delete timer;
    delete showTime;
}

void Timer::startTimer()
{
    timer->start();
    usedSecond = 0;
    usedMsecond = 0;
    showTime->setText(QString::number(usedSecond) + "." + QString::number(usedMsecond));
}

void Timer::stopTimer()
{
    timer->stop();
}

void Timer::myTimerUpdate()
{
    usedMsecond += 1;
    if (usedMsecond == 10)
    {
        usedMsecond = 0;
        usedSecond += 1;
    }
    showTime->setText(QString::number(usedSecond) + "." + QString::number(usedMsecond));
}