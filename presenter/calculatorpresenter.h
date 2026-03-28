#pragma once

#include <memory>

class ICalculatorView;
class CalculatorModel;

class CalculatorPresenter
{
public:
    explicit CalculatorPresenter(std::shared_ptr<ICalculatorView> view);
    ~CalculatorPresenter();
private:
    std::shared_ptr<ICalculatorView> m_view;
    std::unique_ptr<CalculatorModel> m_model;
};

