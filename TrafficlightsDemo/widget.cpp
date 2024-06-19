#include "widget.h"
#include "./ui_widget.h"
#include <QHistoryState>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_stateMachine(new QStateMachine(this))
{
    ui->setupUi(this);

    initTimer();
    initStateMachine();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initTimer()
{
    m_redTimer.setInterval(3000);
    m_redTimer.setSingleShot(true);
    m_yellowTimer.setInterval(300);
    m_yellowTimer.setSingleShot(true);
    m_greenTimer.setInterval(4500);
    m_greenTimer.setSingleShot(true);
}

void Widget::initStateMachine()
{
    //// 创建状态
    QState *normalState = new QState(m_stateMachine); // 正常的状态组
    QState *redState = new QState(normalState);       // 红灯状态
    QState *yellowState = new QState(normalState);    // 黄灯状态
    QState *greenState = new QState(normalState);     // 绿灯状态
    normalState->setInitialState(redState);
    QHistoryState *normalHisState = new QHistoryState(normalState); // 历史状态

    QState *abnormalState = new QState(m_stateMachine);   // 异常的状态组
    QState *flashYellowState = new QState(abnormalState); // 闪烁黄灯状态
    abnormalState->setInitialState(flashYellowState);

    //// 初始化状态的属性
    // 红灯状态下，各个按钮显示的状态
    redState->assignProperty(ui->redBtn, "visible", true);
    redState->assignProperty(ui->yellowBtn, "visible", false);
    redState->assignProperty(ui->greenBtn, "visible", false);
    // 黄灯状态下，各个按钮显示的状态
    yellowState->assignProperty(ui->redBtn, "visible", false);
    yellowState->assignProperty(ui->yellowBtn, "visible", true);
    yellowState->assignProperty(ui->greenBtn, "visible", false);
    // 绿灯状态下，各个按钮显示的状态
    greenState->assignProperty(ui->redBtn, "visible", false);
    greenState->assignProperty(ui->yellowBtn, "visible", false);
    greenState->assignProperty(ui->greenBtn, "visible", true);
    // 闪烁黄灯状态下，各个按钮显示的状态
    flashYellowState->assignProperty(ui->redBtn, "visible", false);
    flashYellowState->assignProperty(ui->yellowBtn, "visible", true);
    flashYellowState->assignProperty(ui->greenBtn, "visible", false);

    //// 初始化状态转换过程
    // 红灯在红灯计时器超时后转换为绿灯
    redState->addTransition(&m_redTimer, &QTimer::timeout, greenState);
    // 在进入绿灯状态后，要启动绿灯定时器
    connect(greenState, &QState::entered, [&]() { m_greenTimer.start(); });

    // 绿灯在绿灯计时器超时后转换为黄灯
    greenState->addTransition(&m_greenTimer, &QTimer::timeout, yellowState);
    // 在进入黄灯状态后，要启动黄灯定时器
    connect(yellowState, &QState::entered, [&]() { m_yellowTimer.start(); });

    // 黄灯在黄灯计时器超时后转化为红灯
    yellowState->addTransition(&m_yellowTimer, &QTimer::timeout, redState);
    // 在进入红灯状态后，要启动红灯定时器
    connect(redState, &QState::entered, [&]() { m_redTimer.start(); });

    normalState->addTransition(ui->button, &QPushButton::clicked, abnormalState);
    abnormalState->addTransition(ui->button2, &QPushButton::clicked, normalHisState);

    //// 初始化状态机
    // 设置状态机的初始状态
    m_stateMachine->setInitialState(normalState);
    // 开启状态机
    m_stateMachine->start();
}
