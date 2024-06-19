#include "widget.h"
#include "./ui_widget.h"
#include <QDebug>

#define GETLABELTEXT(A) QString("%1ing...").arg(A)

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_stateMachine(new QStateMachine(this))
{
    ui->setupUi(this);

    initStateMachine();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initStateMachine()
{
    // 根状态为并行状态
    QState *rootState = new QState(QState::ParallelStates, m_stateMachine);
    // 操作状态（包含画操作、选择模式操作）
    {
        QState *operatorState = new QState(rootState);
        QState *paintOperState = new QState(operatorState);
        QState *paintLineState = new QState(paintOperState);
        QState *paintArcState = new QState(paintOperState);
        QState *paintRectState = new QState(paintOperState);
        QState *paintCircleState = new QState(paintOperState);
        paintOperState->setInitialState(paintLineState);
        operatorState->setInitialState(paintOperState);

        QState *modelOperState = new QState(operatorState);
        QState *selLineState = new QState(modelOperState);
        QState *selFaceState = new QState(modelOperState);
        modelOperState->setInitialState(selLineState);

        paintLineState->assignProperty(ui->paintLabel, "text", GETLABELTEXT("画直线"));
        paintArcState->assignProperty(ui->paintLabel, "text", GETLABELTEXT("画弧线"));
        paintRectState->assignProperty(ui->paintLabel, "text", GETLABELTEXT("画矩形"));
        paintCircleState->assignProperty(ui->paintLabel, "text", GETLABELTEXT("画圆"));

        selLineState->assignProperty(ui->modelLabel, "text", GETLABELTEXT("选择线模式"));
        selFaceState->assignProperty(ui->modelLabel, "text", GETLABELTEXT("选择面模式"));

        struct bindTransitionObj
        {
            QPushButton *m_sender;
            QState      *m_state;
        };
        QVector<bindTransitionObj> bindTransitions;
        bindTransitions.append({ui->paintLineBtn, paintLineState});
        bindTransitions.append({ui->paintArcBtn, paintArcState});
        bindTransitions.append({ui->paintCircleBtn, paintCircleState});
        bindTransitions.append({ui->paintRectBtn, paintRectState});
        bindTransitions.append({ui->selectLineBtn, selLineState});
        bindTransitions.append({ui->selectFaceBtn, selFaceState});
        for (auto fromObj : bindTransitions) {
            for (auto toObj : bindTransitions) {
                if (fromObj.m_state != toObj.m_state) {
                    addTransition(fromObj.m_state, toObj.m_sender, toObj.m_state);
                }
            }
        }
    }
    {
        // 显示状态，属于并行状态
        QState *showState = new QState(QState::ParallelStates, rootState);
        // 初始化显示背景网格状态
        QState *gridState = new QState(showState);
        QState *showGridState = new QState(gridState);
        QState *notShowGridState = new QState(gridState);
        // 初始化显示鼠标坐标状态
        QState *positionState = new QState(showState);
        QState *showPositionState = new QState(positionState);
        QState *notShowPositionState = new QState(positionState);
        gridState->setInitialState(notShowGridState);
        positionState->setInitialState(notShowPositionState);

        showGridState->assignProperty(ui->gridLabel, "text", GETLABELTEXT("显示背景网格"));
        notShowGridState->assignProperty(ui->gridLabel, "text", GETLABELTEXT("不显示背景网格"));
        showPositionState->assignProperty(ui->positionLabel, "text", GETLABELTEXT("显示鼠标坐标"));
        notShowPositionState->assignProperty(ui->positionLabel, "text", GETLABELTEXT("不显示鼠标坐标"));

        // 显示背景网格内部的状态转换
        addTransition(showGridState, ui->showGridBtn, notShowGridState);
        addTransition(notShowGridState, ui->showGridBtn, showGridState);
        // 显示鼠标坐标内部的状态转换
        addTransition(showPositionState, ui->showPositionBtn, notShowPositionState);
        addTransition(notShowPositionState, ui->showPositionBtn, showPositionState);
    }
    //// 初始化状态机
    m_stateMachine->setInitialState(rootState);
    m_stateMachine->start();
}

void Widget::addTransition(QState *fromState, QPushButton *btn, QState *toState)
{
    fromState->addTransition(btn, &QPushButton::clicked, toState);
}
