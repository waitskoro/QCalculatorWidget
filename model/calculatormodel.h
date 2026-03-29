#pragma once

#include <QObject>
#include <QString>
#include <QStack>

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
    QString m_expression;
    QString m_currentNumber;
    QString m_result;
    bool m_newNumberExpected;

    void updateData();
    QString formatNumber(double value);
    double evaluateExpression(const QString& expr);
    double applyOperator(double a, double b, const QString& op);
    bool isOperator(const QChar& ch);
    int getPrecedence(const QString& op);
};
