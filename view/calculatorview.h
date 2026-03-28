#pragma once

#include <memory>

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>

#include "icalculatorview.h"

class ButtonsView;

class CalculatorView : public QMainWindow
                     , public ICalculatorView
{
public:
    explicit CalculatorView(QWidget *parent = nullptr);
    ~CalculatorView();

private:
    void setupUi();
    void setupDisplay();
    void setupButtons();

private:
    std::shared_ptr<QWidget> m_display;
    std::shared_ptr<QWidget> m_buttons;

    QLabel *m_resultDisplay;
    QLineEdit *m_expressionDisplay;
};

