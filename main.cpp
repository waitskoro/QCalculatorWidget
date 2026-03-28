#include <QApplication>

#include "view/calculatorview.h"
#include "presenter/calculatorpresenter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto view = std::make_shared<CalculatorView>();
    view->resize(300, 450);

    auto presenter = std::make_shared<CalculatorPresenter>(view);

    view->show();

    return a.exec();
}
