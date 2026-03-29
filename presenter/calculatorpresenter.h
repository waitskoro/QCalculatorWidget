#pragma once

#include <memory>
#include "view/calculatorview.h"

class CalculatorModel;

class CalculatorPresenter : public QObject
{
    Q_OBJECT

public:
    using shared_view = std::shared_ptr<CalculatorView>;

    explicit CalculatorPresenter(shared_view view, QObject *parent = nullptr);
    ~CalculatorPresenter();

private slots:
    void onButtonClicked(const QString& button);

private:
    shared_view m_view;
    std::unique_ptr<CalculatorModel> m_model;

    void updateDisplay(const QString& expression, const QString& result);
};
