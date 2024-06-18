#ifndef WIDGET_H
#define WIDGET_H

#include <QState>
#include <QStateMachine>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    Widget(QWidget* parent = nullptr);
    ~Widget();

    void initTimer();
    void initStateMachine();

private:
    Ui::Widget* ui;

    QStateMachine* m_stateMachine;
    QTimer m_redTimer;
    QTimer m_yellowTimer;
    QTimer m_greenTimer;
};
#endif // WIDGET_H
