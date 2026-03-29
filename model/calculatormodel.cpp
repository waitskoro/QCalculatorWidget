#include "calculatormodel.h"
#include <QRegularExpression>
#include <cmath>
#include <QDebug>

CalculatorModel::CalculatorModel(QObject *parent)
    : QObject(parent)
    , m_expression("")
    , m_currentNumber("")
    , m_result("0")
    , m_newNumberExpected(false)
{
}

void CalculatorModel::clear()
{
    m_expression.clear();
    m_currentNumber.clear();
    m_result = "0";
    m_newNumberExpected = false;
    updateData();
}

void CalculatorModel::backspace()
{
    if (!m_currentNumber.isEmpty()) {
        m_currentNumber.chop(1);
        if (m_currentNumber.isEmpty()) {
            m_currentNumber = "0";
        }
        updateData();
    } else if (!m_expression.isEmpty()) {
        m_expression.chop(1);
        updateData();
    }
}

void CalculatorModel::addDigit(const QString& digit)
{
    if (m_newNumberExpected) {
        m_currentNumber.clear();
        m_newNumberExpected = false;
    }

    if (m_currentNumber.length() < 15) {
        m_currentNumber += digit;
        updateData();
    }
}

void CalculatorModel::addDecimal()
{
    if (m_newNumberExpected) {
        m_currentNumber.clear();
        m_newNumberExpected = false;
    }

    if (!m_currentNumber.contains('.')) {
        if (m_currentNumber.isEmpty()) {
            m_currentNumber = "0.";
        } else {
            m_currentNumber += ".";
        }
        updateData();
    }
}

void CalculatorModel::toggleSign()
{
    if (!m_currentNumber.isEmpty() && m_currentNumber != "0") {
        double num = m_currentNumber.toDouble();
        num = -num;
        m_currentNumber = formatNumber(num);
        updateData();
    }
}

void CalculatorModel::addOperator(const QString& op)
{
    if (m_expression.isEmpty() && m_currentNumber.isEmpty()) {
        if (op == "-") {
            m_currentNumber = "-";
            updateData();
        }
        return;
    }

    if (!m_currentNumber.isEmpty()) {
        m_expression += m_currentNumber;
        m_currentNumber.clear();
    }

    if (!m_expression.isEmpty()) {
        QChar lastChar = m_expression.back();
        if (isOperator(lastChar)) {
            m_expression.chop(1);
        }
        m_expression += op;
        m_newNumberExpected = true;
        updateData();
    }
}

void CalculatorModel::calculate()
{
    QString fullExpression = m_expression;

    if (!m_currentNumber.isEmpty()) {
        fullExpression += m_currentNumber;
    }

    if (fullExpression.isEmpty()) {
        m_result = "0";
        updateData();
        return;
    }

    QChar lastChar = fullExpression.back();
    if (isOperator(lastChar)) {
        fullExpression.chop(1);
    }

    double result = evaluateExpression(fullExpression);

    if (std::isnan(result) || std::isinf(result)) {
        m_result = "Error";
        m_expression.clear();
        m_currentNumber.clear();
        updateData();
        return;
    }

    m_result = formatNumber(result);

    updateData();
}

void CalculatorModel::updateData()
{
    QString expression = m_expression;
    if (!m_currentNumber.isEmpty()) {
        expression += m_currentNumber;
    }

    emit dataChanged(expression, m_result);
}

QString CalculatorModel::formatNumber(double value)
{
    if (std::isnan(value) || std::isinf(value)) {
        return "Error";
    }

    QString str = QString::number(value, 'f', 10);
    str.remove(QRegularExpression("0+$"));
    str.remove(QRegularExpression("\\.$"));

    if (str.length() > 20) {
        str = QString::number(value, 'g', 12);
    }

    return str;
}

double CalculatorModel::evaluateExpression(const QString& expr)
{
    QStack<double> values;
    QStack<QString> operators;
    QString currentNumber;
    bool expectUnary = true;

    for (int i = 0; i < expr.length(); ++i) {
        QChar ch = expr[i];

        if (ch.isDigit() || ch == '.') {
            currentNumber += ch;
            expectUnary = false;
        }
        else if (isOperator(ch)) {
            if (!currentNumber.isEmpty()) {
                values.push(currentNumber.toDouble());
                currentNumber.clear();
            }

            QString op(ch);

            if (op == "-" && expectUnary) {
                currentNumber = "-";
                continue;
            }

            while (!operators.isEmpty() &&
                   getPrecedence(operators.top()) >= getPrecedence(op)) {
                double b = values.pop();
                double a = values.pop();
                values.push(applyOperator(a, b, operators.pop()));
            }
            operators.push(op);
            expectUnary = true;
        }
    }

    if (!currentNumber.isEmpty()) {
        values.push(currentNumber.toDouble());
    }

    while (!operators.isEmpty()) {
        double b = values.pop();
        double a = values.pop();
        values.push(applyOperator(a, b, operators.pop()));
    }

    return values.isEmpty() ? 0 : values.top();
}

double CalculatorModel::applyOperator(double a, double b, const QString& op)
{
    switch (op[0].toLatin1()) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return (b != 0) ? a / b : 0;
        default: return 0;
    }
}

bool CalculatorModel::isOperator(const QChar& ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int CalculatorModel::getPrecedence(const QString& op)
{
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}
