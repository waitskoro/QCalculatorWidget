#include <QApplication>

#include <QFile>

#include "view/calculatorview.h"
#include "presenter/calculatorpresenter.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto view = std::make_shared<CalculatorView>();
    auto presenter = std::make_shared<CalculatorPresenter>(view);

    QString styleFile;
    if (0) {
        styleFile = ":/resource/dark_calculator_style.qss";
    } else {
        styleFile = ":/resource/light_calculator_style.qss";
    }

    QFile file(styleFile);
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(styleSheet);
    }

    return a.exec();
}
