#include "calculatormodel.h"
#include <QRegularExpression>
#include <stack>
#include <vector>
#include <sstream>
#include <cmath>

CalculatorModel::CalculatorModel(QObject *parent)
    : QObject(parent)
    , m_result("0")
    , m_waitingForOperand(false)
{
    clear();
}

void CalculatorModel::clear()
{
    m_result = "0";
    m_currentNumber.clear();
    m_currentExpression.clear();
    m_waitingForOperand = false;

    update();
}

void CalculatorModel::backspace()
{
    if (!m_currentNumber.isEmpty()) {
        m_currentNumber.chop(1);
        if (m_currentNumber.isEmpty()) {
            m_currentNumber = "0";
        }
        update();
    } else if (!m_currentExpression.isEmpty()) {
        m_currentExpression.chop(1);
        update();
    }
}

void CalculatorModel::calculate()
{
    QString fullExpression = m_currentExpression;

    if (!m_currentNumber.isEmpty()) {
        fullExpression += m_currentNumber;
    }

    if (fullExpression.isEmpty()) {
        m_result = "0";
        update();
    }

    if (isOperator(fullExpression.right(1))) {
        fullExpression.chop(1);
    }

    if (fullExpression == "-") {
        m_result = "Error";
        update();
    }

    double result = evaluateExpression(fullExpression);

    if (std::isinf(result) || std::isnan(result)) {
        m_result = "Error";
        m_currentExpression.clear();
        m_currentNumber.clear();
        update();
    }

    m_result = formatNumber(result);

    m_currentExpression = fullExpression;
    m_currentNumber.clear();
    m_waitingForOperand = true;

    update();
}

void CalculatorModel::toggleSign()
{
    if (!m_currentNumber.isEmpty()) {
        double num = m_currentNumber.toDouble();
        num = -num;
        m_currentNumber = formatNumber(num);
        update();
    }
}

void CalculatorModel::addDecimal()
{
    if (m_waitingForOperand) {
        m_currentNumber.clear();
        m_waitingForOperand = false;
    }

    if (!m_currentNumber.contains('.')) {
        if (m_currentNumber.isEmpty()) {
            m_currentNumber = "0.";
        } else {
            m_currentNumber += ".";
        }
        update();
    }
}

void CalculatorModel::addOperator(const QString& op)
{
    if (m_currentExpression.isEmpty() && m_currentNumber.isEmpty() && op == "-") {
        m_currentExpression = "-";
        m_waitingForOperand = true;
        update();
        return;
    }

    if (m_currentNumber.isEmpty() && !m_currentExpression.isEmpty() &&
        isOperator(m_currentExpression.right(1))) {
        if (op != "-" || m_currentExpression.right(1) != "-") {
            m_currentExpression.chop(1);
            m_currentExpression += op;
        } else {
            m_currentExpression.chop(1);
        }
        update();
        return;
    }

    if (!m_currentNumber.isEmpty()) {
        m_currentExpression += m_currentNumber;
        m_currentNumber.clear();
    }

    if (!m_currentExpression.isEmpty()) {
        m_currentExpression += op;
        m_waitingForOperand = true;
        update();
    }
}

void CalculatorModel::addDigit(const QString& digit)
{
    if (m_waitingForOperand) {
        m_currentNumber.clear();
        m_waitingForOperand = false;
    }

    if (m_currentNumber.length() < 15) {
        m_currentNumber += digit;
        update();
    }
}

void CalculatorModel::update()
{
    QString expression = m_currentExpression;
    if (!m_currentNumber.isEmpty()) {
        expression += m_currentNumber;
    }

    if (expression.isEmpty()) {
        expression = "";
    }

    emit dataChanged(expression, m_result);
}

QString CalculatorModel::formatNumber(double value)
{
    if (std::isinf(value) || std::isnan(value)) {
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

bool CalculatorModel::isOperator(const QString& str)
{
    return str == "+" || str == "-" || str == "*" || str == "/";
}

int CalculatorModel::getPrecedence(const QString& op)
{
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

double CalculatorModel::evaluateExpression(const QString& expression)
{
    std::stack<double> values;
    std::stack<QString> operators;

    QString currentNumber;

    for (int i = 0; i < expression.length(); ++i) {
        QChar ch = expression[i];

        if (ch.isDigit() || ch == '.') {
            currentNumber += ch;
        } else if (ch == '(') {
            operators.push("(");
        } else if (ch == ')') {
            if (!currentNumber.isEmpty()) {
                values.push(currentNumber.toDouble());
                currentNumber.clear();
            }

            while (!operators.empty() && operators.top() != "(") {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                QString op = operators.top(); operators.pop();
                values.push(applyOperator(a, b, op));
            }
            if (!operators.empty()) operators.pop();
        } else if (isOperator(ch)) {
            if (!currentNumber.isEmpty()) {
                values.push(currentNumber.toDouble());
                currentNumber.clear();
            }

            QString op(ch);

            if (op == "-" && (i == 0 || expression[i-1] == '(' || isOperator(expression[i-1]))) {
                values.push(0.0);
            }

            while (!operators.empty() &&
                   getPrecedence(operators.top()) >= getPrecedence(op) &&
                   operators.top() != "(") {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                QString topOp = operators.top(); operators.pop();
                values.push(applyOperator(a, b, topOp));
            }
            operators.push(op);
        }
    }

    if (!currentNumber.isEmpty()) {
        values.push(currentNumber.toDouble());
    }

    while (!operators.empty()) {
        if (values.size() < 2) {
            if (operators.top() == "-" && values.size() == 1) {
                double a = values.top(); values.pop();
                return -a;
            }
            return 0;
        }
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        QString op = operators.top(); operators.pop();
        values.push(applyOperator(a, b, op));
    }

    return values.empty() ? 0 : values.top();
}

bool CalculatorModel::isUnaryMinus(const QString& expression, int index)
{
    if (index == 0) return true;

    QChar prevChar = expression[index - 1];
    return isOperator(prevChar) || prevChar == '(';
}

double CalculatorModel::applyOperator(double a, double b, const QString& op)
{
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") return b != 0 ? a / b : 0;
    return 0;
}

