#ifndef WIDGET_H
#define WIDGET_H

#include <QHistoryState>
#include <QPushButton>
#include <QState>
#include <QStateMachine>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void initStateMachine();
    void addTransition(QState *fromState, QPushButton *btn, QState *toState);

private:
    Ui::Widget *ui;

    QStateMachine *m_stateMachine;
};
#endif // WIDGET_H
