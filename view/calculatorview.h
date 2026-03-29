#pragma once

#include <memory>

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class ButtonsView;

class CalculatorView : public QMainWindow
{
    Q_OBJECT
public:
    explicit CalculatorView(QWidget *parent = nullptr);
    ~CalculatorView();

    void setIsDarkTheme(bool theme);
    void setResult(const QString& result);
    void setExpression(const QString& expression);

signals:
    void buttonsClicked(const QString &btn);

private:
    void setupUi();
    void setupDisplay();
    void setupButtons();
    QString formatExpression(const QString& expression);

private slots:
    void onButtonClicked();

private:
    std::shared_ptr<QWidget> m_display;
    std::shared_ptr<QWidget> m_buttons;

    QLabel *m_resultDisplay;
    QLabel *m_expressionDisplay;

    bool m_isDarkTheme = false;
};
