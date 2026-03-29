#pragma once

#include <QObject>
#include <QString>
#include <stack>
#include <memory>

class CalculatorModel : public QObject
{
    Q_OBJECT

public:
    explicit CalculatorModel(QObject *parent = nullptr);

    void clear();
    void backspace();
    void calculate();
    void toggleSign();
    void addDecimal();
    void addOperator(const QString& op);
    void addDigit(const QString& digit);

signals:
    void dataChanged(const QString& expression, const QString& result);

private:
    QString m_result;
    QString m_currentNumber;
    bool m_waitingForOperand;
    QString m_currentExpression;

    void update();
    QString formatNumber(double value);
    bool isOperator(const QString& str);
    int getPrecedence(const QString& op);
    double evaluateExpression(const QString& expression);
    bool isUnaryMinus(const QString& expression, int index);
    double applyOperator(double a, double b, const QString& op);
};
