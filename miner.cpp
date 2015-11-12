
#include "miner.h"

bool Miner::isFirstClick = true;
bool Miner::isMouseActive = true;

void Miner::setIsFirstClick()
{
    isFirstClick = true;
}

void Miner::setIsMouseActive(bool flag)
{
    isMouseActive = flag;
}

Miner::Miner(int row, int column, QWidget* parent)
    : row(row), column(column), isMiner(false), curState(ENUM_UNKNOWN), isDouted(false), minerNumAround(0), isInDealQueue(false), QLabel(parent)
{

    clickInterval = new QTimer();
    clickInterval->setInterval(200);

    //setFixedSize(QSize(50, 50));
    setPixmap(QPixmap("./image/block.jpg"));
    QFont labelFont;
    labelFont.setPointSize(15);
    setFont(labelFont);
    setAlignment(Qt::AlignCenter);
    setPalette(QPalette(QColor(180, 180, 180)));

}

Miner::~Miner()
{
    delete clickInterval;
}

bool Miner::getIsMiner()
{
    return isMiner;
}

void Miner::setMinerNumAround(int num)
{
    minerNumAround = num;
}

int Miner::getMinerNumAround()
{
    return minerNumAround;
}

MINER_STATE Miner::getCurState()
{
    return curState;
}

bool Miner::getIsInDealQueue()
{
    return isInDealQueue;
}

void Miner::setIsInDealQueue()
{
    isInDealQueue = true;
}

bool Miner::getIsDouted()
{
    return isDouted;
}

void Miner::setIsMiner()
{
    isMiner = true;
}

void Miner::leftClick()
{
    //����ʱ���Ѿ�����������ֹͣ��
    clickInterval->stop();

    //���������ֻ�е�ǰδ֪״̬����Ӧ��������ף���ը�������ף���ˢ����ʾ��ͬʱ�����źŰ�ȫ�������MinerManager����Χ���ڷ���
    if (curState == ENUM_UNKNOWN)
    {
        if (isMiner == true)
        {
            curState = ENUM_KNOWN;
            setPixmap(QPixmap("./image/miner.jpg"));
            explode();
        }
        else
        {
            openBlock();
            safeClick(row, column);
        }
    }
}

void Miner::mousePressEvent(QMouseEvent* event)
{
    if (!isMouseActive)
        return;

    if (event->button() == Qt::LeftButton)
    {
        //��ʱ�׻�û�г�ʼ������⵽��һ���������ôֱ�Ӵ�������鲢��ʼ������λ��
        //�������˫���¼�������ô��ʱ�����Ѿ���ʼ���ˣ�������Ϊ��˫������Ӧ�򿪶�������������ط�ʱ����ֱ��ײ����
        //ͬʱ��ʼ��ʱ
        if (isFirstClick)
        {
            isFirstClick = false;
            initializeMiners(row, column);
            leftClick();
            startTimer();
            return;
        }
        //��ʱ������û���������������ǵ�һ�ε����¼�
        if (clickInterval->isActive() == false)
        {
            //������ʱ����200ms��û���յ��ٴεĵ����¼�����Ӧ�����¼�
            clickInterval->start();
            connect(clickInterval, SIGNAL(timeout()), this, SLOT(leftClick()));
        }
        //��ʱ���Ѿ����������յ��˵����¼�����ʱҪ����˫���¼�������Χ����
        else
        {
            clickInterval->stop();
            disconnect(clickInterval, SIGNAL(timeout()), this, SLOT(leftClick()));
            //ֻ�е�ǰ�����Ѿ��򿪣�����Ӧ˫���¼�
            if (curState == ENUM_KNOWN)
                doubleClick(row, column);
        }
        
    }

    //�Ҽ������ֻ����ENUM_DOUTED��ENUM_UNKNOW����Ӧ
    if (event->button() == Qt::RightButton)
    {
        //����δ���������Ϊ����
        if (curState == ENUM_UNKNOWN)
        {
            curState = ENUM_DOUTED;
            isDouted = true;
            setPixmap(QPixmap("./image/flag.jpg"));
            setDouted();
        }
        else if (curState == ENUM_DOUTED)
        {
            curState = ENUM_UNKNOWN;
            isDouted = false;
            setPixmap(QPixmap("./image/block.jpg"));
            unsetDouted();
        }
    }

    //˫����ֻ�е�ǰ��ENUM_KNOWN״̬����Ӧ
}

void Miner::openBlock()
{
    curState = ENUM_KNOWN;
    if (minerNumAround != 0)
        setPixmap(QPixmap(QString("./image/blank_") + QString::number(minerNumAround) + QString(".jpg")));
    else
        setPixmap(QPixmap("./image/blank.jpg"));
    addSafeBlockNum();
}

static MinerManager* g_MinerManager = NULL;

MinerManager::MinerManager(QWidget* parent) : QWidget(parent)
{
    row = 0;
    column = 0;
    minerNum = 0;
    totalNum = 0;
    safeBlockNum = 0;
    minersLayout = NULL;
    setPalette(QPalette(QColor(150, 150, 150)));

    //�ֱ�����������׷���ͼ�ʱ�����飬�����������δ�ֱ����
    minersWidget = new QWidget();
    timerWidget = new QWidget();

    initializeGameOverDialog();
    initializeTimerAndCounter();
}

MinerManager::~MinerManager()
{
    for (int i = 0; i < miners.size(); ++i)
    {
        delete miners.at(i);
    }
    miners.clear();
    Miner::setIsFirstClick();

    delete minersLayout;
    delete gameOverDialog;
    delete minersWidget;
    delete timerWidget;
    delete gameFinishLabel;
    delete doutedNumLabel;
    delete gameTimer;

}

MinerManager* MinerManager::shared()
{
    if (g_MinerManager == NULL)
        g_MinerManager = new MinerManager;

    return g_MinerManager;
}

void MinerManager::initial(const int& dRow, const int& dColumn, const int& dMinerNum)
{
    Miner::setIsMouseActive(true);
    row = dRow;
    column = dColumn;
    minerNum = dMinerNum;
    totalNum = dRow * dColumn;
    safeBlockNum = 0;
    doutedNum = 0;
    gameTimer->startTimer();
    gameTimer->stopTimer();
    doutedNumLabel->setText(QString::number(minerNum - doutedNum));

    if (minerNum > totalNum)
    {
        std::cout << "miner number is larger than total block number! initialize fail!" << std::endl;
        return;
    }

    //����ϴ���Ϸ����Ŀռ�
    if (minersLayout != NULL)
        delete minersLayout;
    minersLayout = new QGridLayout();
    
    for (int i = 0; i < miners.size(); ++i)
    {
        delete miners.at(i);
    }
    miners.clear();
    Miner::setIsFirstClick();
    gameOverDialog->hide();

    //��ʼ����Ϸ����ʱû�е��ף�һֱ����һ�ε�����е��ס�
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            Miner *newMiner = new Miner(i, j);
            miners.push_back(newMiner);
            connect(newMiner, SIGNAL(initializeMiners(int, int)), this, SLOT(initialMiners(int, int)));
            connect(newMiner, SIGNAL(addSafeBlockNum()), this, SLOT(addSafeBlockNum()));
            connect(newMiner, SIGNAL(explode()), this, SLOT(explode()));
            connect(newMiner, SIGNAL(safeClick(int, int)), this, SLOT(safeClick(int, int)));
            connect(newMiner, SIGNAL(doubleClick(int, int)), this, SLOT(doubleClick(int, int)));
            connect(newMiner, SIGNAL(startTimer()), gameTimer, SLOT(startTimer()));
            connect(newMiner, SIGNAL(setDouted()), this, SLOT(doutedIncrease()));
            connect(newMiner, SIGNAL(unsetDouted()), this, SLOT(doutedDecrease()));
            minersLayout->addWidget(newMiner, i, j);
        }
    }

    minersWidget->setLayout(minersLayout);
}

void MinerManager::initial()
{
    initial(row, column, minerNum);
}

void MinerManager::gameOver(bool isWin)
{
    stopTimer();
    if (isWin)
        gameFinishLabel->setText("you win");
    else
        gameFinishLabel->setText("you lose!");
    QRect mainWindowRect = geometry();
    QRect dialogRect = gameOverDialog->geometry();
    gameOverDialog->setGeometry(mainWindowRect.x() + mainWindowRect.width() / 2 - dialogRect.width() / 2,
        mainWindowRect.y() + mainWindowRect.height() / 2 - dialogRect.height() / 2, dialogRect.width(), dialogRect.height());
    gameOverDialog->show();
    Miner::setIsMouseActive(false);

    for (Miner* miner : miners)
    {
        if (miner->getIsMiner())
            miner->setPixmap(QPixmap("./image/miner.jpg"));
    }
}

void MinerManager::initialMiners(const int& clickRow, const int& clickColumn)
{
    int* allBlockIndex = new int[totalNum];
    for (int i = 0; i < totalNum; ++i)
        allBlockIndex[i] = i;
    int indexForBlank = 0;
    //����һ�ε���ķ����Լ���Χ8������ȫ���ŵ�����ǰ�棬��֤�⼸�����Ӳ��������ס�
    for (int i = -1; i < 2; ++i)
        for (int j = -1; j < 2; ++j)
        {
            if ((clickRow + i) >= 0 && (clickRow + i) < row && (clickColumn + j) >= 0 && (clickColumn + j) < column)
            {
                int tmp = allBlockIndex[indexForBlank];
                allBlockIndex[indexForBlank] = allBlockIndex[(clickRow + i) * column + clickColumn + j];
                allBlockIndex[(clickRow + i) * column + clickColumn + j] = tmp;
                indexForBlank++;
            }
        }
    srand(time(NULL));
    //���⿪ʼ��������ķ�����н��������ս�����ǰMinerNum�ķ���������ס�
    for (int i = indexForBlank; i < totalNum; ++i)
    {
        int swapIndex = rand() % (totalNum - indexForBlank) + indexForBlank;
        int tmp = allBlockIndex[swapIndex];
        allBlockIndex[swapIndex] = allBlockIndex[i];
        allBlockIndex[i] = tmp;
    }
    for (int i = indexForBlank; i < indexForBlank + minerNum; ++i)
    {
        miners.at(allBlockIndex[i])->setIsMiner();
    }

    //ͳ�Ʒ������Χ8�������������Ŀ
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < column; ++j)
        {
            int curIndex = i * column + j;
            //������ײ���ͳ��
            if (miners.at(curIndex)->getIsMiner())
            {
                miners.at(curIndex)->setMinerNumAround(-1);
                continue;
            }
            int minerNumAround = calculateAround(i, j, &(Miner::getIsMiner));
            
            miners.at(curIndex)->setMinerNumAround(minerNumAround);

        }
    }

    delete[] allBlockIndex;
}

void MinerManager::explode()
{
    gameOver(false);
}

void MinerManager::addSafeBlockNum()
{
    safeBlockNum++;
}

void MinerManager::safeClick(const int& clickRow, const int& clickColumn)
{
    //����򿪵������һ���ף���Ϸʤ������
    if (safeBlockNum == totalNum - minerNum)
    {
        gameOver(true);
        return;
    } 

    if (miners.at(clickRow * column + clickColumn)->getMinerNumAround() != 0)
        return;

    std::queue<int> blockIndex;
    blockIndex.push(clickRow);
    blockIndex.push(clickColumn);

    do
    {
        int curRow = blockIndex.front();
        blockIndex.pop();
        int curColumn = blockIndex.front();
        blockIndex.pop();
        //������һ��
        if (curRow > 0)
        {
            if (curColumn > 0)
                openBlock(curRow - 1, curColumn - 1, blockIndex);
            openBlock(curRow - 1, curColumn, blockIndex);
            if (curColumn + 1 < column)
                openBlock(curRow - 1, curColumn + 1, blockIndex);
        }

        if (curColumn > 0)
            openBlock(curRow, curColumn - 1, blockIndex);
        if (curColumn + 1 < column)
            openBlock(curRow, curColumn + 1, blockIndex);

        //������һ��
        if (curRow + 1 < row)
        {
            if (curColumn > 0)
                openBlock(curRow + 1, curColumn - 1, blockIndex);
            openBlock(curRow + 1, curColumn, blockIndex);
            if (curColumn + 1 < column)
                openBlock(curRow + 1, curColumn + 1, blockIndex);
        }

    } while (! blockIndex.empty());

    if (safeBlockNum == totalNum - minerNum)
    {
        gameOver(true);
    }

}

void MinerManager::doubleClick(const int& clickRow, const int& clickColumn)
{
    //ͳ����Χ�������׵�����
    int totalDouted = calculateAround(clickRow, clickColumn, &(Miner::getIsDouted));

    //���������׵�����������Χ�׵���������ʾ����˫��������δ֪����
    if (totalDouted == miners.at(clickRow * column + clickColumn)->getMinerNumAround())
    {
        handleAround(clickRow, clickColumn, &(Miner::leftClick));
    }
}

void MinerManager::doutedIncrease()
{
    doutedNum++;
    doutedNumLabel->setText(QString::number(minerNum - doutedNum));
}

void MinerManager::doutedDecrease()
{
    doutedNum--;
    doutedNumLabel->setText(QString::number(minerNum - doutedNum));
}

void MinerManager::openBlock(const int& clickRow, const int& clickColumn, std::queue<int>& qIndex)
{
    Miner* minerForOpen = miners.at(clickRow * column + clickColumn);
    //ֻ����Χ�׵���ĿΪ0������û��������û����ǵķ���ŷ���
    if (minerForOpen->getMinerNumAround() != -1 && minerForOpen->getCurState() == ENUM_UNKNOWN && !minerForOpen->getIsInDealQueue())
    {
        minerForOpen->openBlock();
        minerForOpen->setIsInDealQueue();
        //��Χû��ȫ�ǿհ׵ķ���Ҫ����Χ�ļ�������
        if (minerForOpen->getMinerNumAround() == 0)
        {
            qIndex.push(clickRow);
            qIndex.push(clickColumn);
        }
    }
}

void MinerManager::initializeGameOverDialog()
{
    gameOverDialog = new QDialog;
    gameOverDialog->hide();
    gameOverDialog->setFixedSize(250, 100);

    QVBoxLayout* gameOverLayout = new QVBoxLayout();
    gameOverDialog->setLayout(gameOverLayout);

    gameFinishLabel = new QLabel("you lose");
    gameOverLayout->addWidget(gameFinishLabel);
    gameFinishLabel->setFixedSize(250, 50);
    gameFinishLabel->setAlignment(Qt::AlignCenter);
    QFont labelFont;
    labelFont.setPointSize(15);
    gameFinishLabel->setFont(labelFont);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    gameOverLayout->addLayout(buttonLayout);

    QPushButton* newGameButton = new QPushButton("try again");
    buttonLayout->addWidget(newGameButton);
    newGameButton->setFixedSize(100, 30);
    connect(newGameButton, SIGNAL(clicked()), this, SLOT(initial()));

    QPushButton *cancleButton = new QPushButton("cancle");
    buttonLayout->addWidget(cancleButton);
    cancleButton->setFixedSize(100, 30);
    connect(cancleButton, SIGNAL(clicked()), gameOverDialog, SLOT(hide()));
}

void MinerManager::initializeTimerAndCounter()
{
    
    gameTimer = new Timer();
    connect(this, SIGNAL(stopTimer()), gameTimer, SLOT(stopTimer()));

    doutedNumLabel = new QLabel();
    QFont labelFont;
    labelFont.setPointSize(15);
    doutedNumLabel->setFont(labelFont);
    doutedNumLabel->setAlignment(Qt::AlignCenter);
    //���������Ҫˮƽ��ʾ
    QHBoxLayout* timerLayout = new QHBoxLayout();
    timerLayout->addWidget(gameTimer);
    timerLayout->addWidget(doutedNumLabel);
    timerWidget->setLayout(timerLayout);
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(minersWidget);
    mainLayout->addWidget(timerWidget);
    setLayout(mainLayout);
}

int MinerManager::calculateAround(const int& clickRow, const int& clickColumn, bool (Miner::*fun)())
{
    //������һ��
    int result = 0;
    if (clickRow > 0)
    {
        //�������Ͻ�
        if (clickColumn > 0)
            result += (miners.at((clickRow - 1) * column + clickColumn - 1)->*fun)() ? 1 : 0;
        result += (miners.at((clickRow - 1) * column + clickColumn)->*fun)() ? 1 : 0;
        if (clickColumn + 1 < column)
            result += (miners.at((clickRow - 1) * column + clickColumn + 1)->*fun)() ? 1 : 0;
    }

    //�жϱ���
    if (clickColumn > 0)
        result += (miners.at(clickRow * column + clickColumn - 1)->*fun)() ? 1 : 0;
    if (clickColumn + 1 < column)
        result += (miners.at(clickRow * column + clickColumn + 1)->*fun)() ? 1 : 0;

    //������һ��
    if (clickRow + 1 < row)
    {
        if (clickColumn > 0)
            result += (miners.at((clickRow + 1) * column + clickColumn - 1)->*fun)() ? 1 : 0;
        result += (miners.at((clickRow + 1) * column + clickColumn)->*fun)() ? 1 : 0;
        if (clickColumn + 1 < column)
            result += (miners.at((clickRow + 1) * column + clickColumn + 1)->*fun)() ? 1 : 0;
    }

    return result;
}

void MinerManager::handleAround(const int& clickRow, const int& clickColumn, void (Miner::*fun)())
{

    if (clickRow > 0)
    {
        //�������Ͻ�
        if (clickColumn > 0)
            (miners.at((clickRow - 1) * column + clickColumn - 1)->*fun)();
        (miners.at((clickRow - 1) * column + clickColumn)->*fun)();
        if (clickColumn + 1 < column)
            (miners.at((clickRow - 1) * column + clickColumn + 1)->*fun)();
    }

    //�жϱ���
    if (clickColumn > 0)
        (miners.at(clickRow * column + clickColumn - 1)->*fun)();
        if (clickColumn + 1 < column)
            (miners.at(clickRow * column + clickColumn + 1)->*fun)();

    //������һ��
    if (clickRow + 1 < row)
    {
        if (clickColumn > 0)
            (miners.at((clickRow + 1) * column + clickColumn - 1)->*fun)();
        (miners.at((clickRow + 1) * column + clickColumn)->*fun)();
        if (clickColumn + 1 < column)
            (miners.at((clickRow + 1) * column + clickColumn + 1)->*fun)();
    }
}


 