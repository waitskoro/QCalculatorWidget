#include "calculatorview.h"
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QDebug>

CalculatorView::CalculatorView(QWidget *parent)
    : QMainWindow(parent)
    , m_display(std::make_shared<QWidget>(this))
    , m_buttons(std::make_shared<QWidget>(this))
{
    resize(300, 500);
    setupUi();
}

CalculatorView::~CalculatorView()
{}


void CalculatorView::setIsDarkTheme(bool theme)
{
    m_isDarkTheme = theme;
}

void CalculatorView::setResult(const QString& result)
{
    if (result.isEmpty()) {
        m_resultDisplay->setText("0");
    } else {
        m_resultDisplay->setText(result);
    }
}

void CalculatorView::setExpression(const QString& expression)
{
    if (expression.isEmpty()) {
        m_expressionDisplay->setText("");
    } else {
        m_expressionDisplay->setText(formatExpression(expression));
    }
}

void CalculatorView::setupUi()
{

    QGridLayout *gridLayout = new QGridLayout();
    QWidget *w = new QWidget(this);
    w->setLayout(gridLayout);
    setCentralWidget(w);

    setupDisplay();
    setupButtons();

    gridLayout->addWidget(m_display.get());
    gridLayout->addWidget(m_buttons.get());
}

void CalculatorView::setupDisplay()
{
    m_display->setObjectName("displayWidget");

    QVBoxLayout* mainLayout = new QVBoxLayout(m_display.get());
    mainLayout->setSpacing(5);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(10);

    QLabel* equalsLabel = new QLabel("=");
    equalsLabel->setObjectName("equalsLabel");
    equalsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    equalsLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    m_resultDisplay = new QLabel("0");
    m_resultDisplay->setObjectName("resultDisplay");
    m_resultDisplay->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_resultDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    topLayout->addWidget(equalsLabel);
    topLayout->addWidget(m_resultDisplay);

    m_expressionDisplay = new QLabel();
    m_expressionDisplay->setObjectName("expressionDisplay");
    m_expressionDisplay->setAlignment(Qt::AlignRight);
    m_expressionDisplay->setText("");

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_expressionDisplay);

    mainLayout->setStretchFactor(topLayout, 2);
    mainLayout->setStretchFactor(m_expressionDisplay, 1);

    m_display->setFixedHeight(140);
}

void CalculatorView::setupButtons()
{
    QGridLayout* buttonsLayout = new QGridLayout(m_buttons.get());

    const char* buttonsData[5][4] = {
        {"CE", "+/-", "%", "/"},
        {"7", "8", "9", "*"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"0", ".", "=", nullptr}
    };

    QPushButton* buttons[5][4] = {};

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (buttonsData[row][col]) {
                QPushButton* button = new QPushButton(buttonsData[row][col]);
                QString btnText = buttonsData[row][col];

                button->setMinimumSize(80, 60);
                button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                if (btnText == "=") {
                    button->setProperty("type", "equals");
                }
                else if (btnText == "CE" || btnText == "+/-" || btnText == "%" ||
                         btnText == "/" || btnText == "*" || btnText == "-" || btnText == "+") {
                    button->setProperty("type", "operator");
                }
                else if ((btnText >= "0" && btnText <= "9") || btnText == ".") {
                    button->setProperty("type", "number");
                }

                buttons[row][col] = button;
                buttonsLayout->addWidget(button, row, col);

                connect(button, &QPushButton::clicked,
                        this, &CalculatorView::onButtonClicked);
            }
        }
    }

    // Кнопка "+" занимает 2 ряда
    if (buttons[3][3]) {
        buttonsLayout->removeWidget(buttons[3][3]);
        buttonsLayout->addWidget(buttons[3][3], 3, 3, 2, 1);
    }

    for (int i = 0; i < 4; ++i) {
        buttonsLayout->setColumnStretch(i, 1);
    }

    buttonsLayout->setSpacing(15);
    buttonsLayout->setContentsMargins(10, 10, 10, 10);
}

QString CalculatorView::formatExpression(const QString& expression)
{
    QString formatted;

    for (int i = 0; i < expression.length(); ++i) {
        QChar ch = expression[i];

        if (ch.isDigit() || ch == '.') {
            QString color = m_isDarkTheme ? "white" : "black";
            QString formated = "<span style='color:" + color + ";'>" + QString(ch) + "</span>";
            formatted += formated;
        } else {
            QString op = ch;
            if (ch == '*')
                op = "x";
            if (ch == '%') {
                formatted += "<span style='color: #51C9DC;'>" + op + "</span>";
            } else {
                formatted += "<span style='color: #51C9DC;'> " + op + " </span>";
            }
        }
    }

    return formatted;
}

void CalculatorView::onButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    emit buttonsClicked(button->text());
}
