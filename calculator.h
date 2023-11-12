#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QStack>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);

private:
    QString expr{};
    QLineEdit* display;
    QString savedExpr{};
    QPushButton *createButton(const QString &text, const char *member);

    int prec(QChar ch);
    double calculate(QString str);
    double result{};

private slots:
    void setNumb();
    void doClear();
    void doClearOneCh();
    void doMemCl();
    void doMemRecall();
    void doMemSave();
    void changeSign();
    void setEnd();
};

#endif // CALCULATOR_H
