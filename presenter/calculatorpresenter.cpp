#include "calculatorpresenter.h"
#include "view/calculatorview.h"
#include "model/calculatormodel.h"

CalculatorPresenter::CalculatorPresenter(shared_view view,
                                         QObject *parent)
    : QObject(parent)
    , m_view(view)
    , m_model(std::make_unique<CalculatorModel>())
{
    connect(m_view.get(), &CalculatorView::buttonsClicked,
            this, &CalculatorPresenter::onButtonClicked);

    connect(m_model.get(), &CalculatorModel::dataChanged,
            this, &CalculatorPresenter::updateDisplay);
}

CalculatorPresenter::~CalculatorPresenter()
{}

void CalculatorPresenter::onButtonClicked(const QString& button)
{
    if (button == "CE") {
        m_model->clear();
    } else if (button == "+/-") {
        m_model->toggleSign();
    } else if (button == "%") {
        m_model->addOperator("%");
    } else if (button == "/") {
        m_model->addOperator("/");
    } else if (button == "*") {
        m_model->addOperator("*");
    } else if (button == "-") {
        m_model->addOperator("-");
    } else if (button == "+") {
        m_model->addOperator("+");
    } else if (button == ".") {
        m_model->addDecimal();
    } else if (button == "=") {
        m_model->calculate();
    } else {
        m_model->addDigit(button);
    }
}

void CalculatorPresenter::updateDisplay(const QString& expression, const QString& result)
{
    m_view->setResult(result);
    m_view->setExpression(expression);
}
