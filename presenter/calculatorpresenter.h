#pragma once

#include <memory>

class CalculatorView;
class CalculatorModel;

class CalculatorPresenter
{
public:
    explicit CalculatorPresenter(std::shared_ptr<CalculatorView> view);
    ~CalculatorPresenter();
private:
    std::shared_ptr<CalculatorView> m_view;
    std::unique_ptr<CalculatorModel> m_model;
};

