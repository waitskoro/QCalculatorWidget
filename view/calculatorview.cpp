#include "calculatorview.h"

#include <QPushButton>
#include <QGridLayout>
#include <QGraphicsAnchorLayout>

CalculatorView::CalculatorView(QWidget *parent)
    : QMainWindow(parent)
    , m_display(std::make_shared<QWidget>(this))
    , m_buttons(std::make_shared<QWidget>(this))
{
    setupUi();
}

CalculatorView::~CalculatorView()
{}

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
        m_expressionDisplay->setText(expression);
    }
}

void CalculatorView::setupUi()
{
    setupDisplay();
    setupButtons();

    QGridLayout *gridLayout = new QGridLayout();
    QWidget *w = new QWidget(this);
    w->setLayout(gridLayout);
    setCentralWidget(w);

    gridLayout->addWidget(m_display.get());
    gridLayout->addWidget(m_buttons.get());
}

void CalculatorView::setupDisplay()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(m_display.get());
    mainLayout->setSpacing(5);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->setSpacing(10);

    QLabel* equalsLabel = new QLabel("=");
    equalsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    equalsLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);

    m_resultDisplay = new QLabel("0");
    m_resultDisplay->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_resultDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_resultDisplay->setMinimumHeight(50);

    topLayout->addWidget(equalsLabel);
    topLayout->addWidget(m_resultDisplay);

    m_expressionDisplay = new QLabel();
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
                buttons[row][col] = new QPushButton(buttonsData[row][col]);
                buttons[row][col]->setMinimumSize(80, 60);
                buttons[row][col]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

                buttonsLayout->addWidget(buttons[row][col], row, col);
            }

            if (buttons[row][col] != nullptr) {
                connect(buttons[row][col], &QPushButton::clicked,
                        this, &CalculatorView::onButtonClicked);
            }
        }
    }

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

void CalculatorView::onButtonClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    emit buttonsClicked(button->text());
}
