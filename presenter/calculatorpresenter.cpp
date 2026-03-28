#include "calculatorpresenter.h"

#include <model/calculatormodel.h>

CalculatorPresenter::CalculatorPresenter(std::shared_ptr<ICalculatorView> view)
    : m_view(view)
    , m_model(std::make_unique<CalculatorModel>())
{}

CalculatorPresenter::~CalculatorPresenter()
{

}
