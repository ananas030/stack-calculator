#include "calculator.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QStringList>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    auto *memClBtn = createButton("MC", SLOT(doMemCl()));
    auto *memRecallBtn = createButton("MR", SLOT(doMemRecall()));
    auto *memSaveBtn = createButton("MS", SLOT(doMemSave()));
    auto *brack1Btn = createButton("(", SLOT(setNumb()));
    auto *brack2Btn = createButton(")", SLOT(setNumb()));

    auto *restBtn =createButton("%", SLOT(setNumb()));
    auto *sevBtn = createButton("7", SLOT(setNumb()));
    auto *eightBtn = createButton("8", SLOT(setNumb()));
    auto *nineBtn = createButton("9", SLOT(setNumb()));
    auto *divBtn = createButton("/", SLOT(setNumb()));

    auto *chSignBtn = createButton("+/-", SLOT(changeSign()));
    auto *fourBtn = createButton("4", SLOT(setNumb()));
    auto *fiveBtn = createButton("5", SLOT(setNumb()));
    auto *sixBtn = createButton("6", SLOT(setNumb()));
    auto *multBtn = createButton("*", SLOT(setNumb()));

    auto *clearBtn = createButton("C", SLOT(doClear()));
    auto *oneBtn = createButton("1", SLOT(setNumb()));
    auto *twoBtn = createButton("2", SLOT(setNumb()));
    auto *threeBtn = createButton("3", SLOT(setNumb()));
    auto *minusBtn = createButton("-", SLOT(setNumb()));

    auto *clearAllBtn = createButton("CE", SLOT(doClearOneCh()));
    auto *nullBtn = createButton("0", SLOT(setNumb()));
    auto *dotBtn = createButton(".", SLOT(setNumb()));
    auto *equalsBtn = createButton("=", SLOT(setEnd()));
    auto *plusBtn = createButton("+", SLOT(setNumb()));

    display = new QLineEdit();
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *grid = new QGridLayout(this);
    grid->addWidget(display, 0, 0, 1, 5);

    grid->addWidget(memClBtn, 1, 0);
    grid->addWidget(memRecallBtn, 1, 1);
    grid->addWidget(memSaveBtn , 1, 2);
    grid->addWidget(brack1Btn, 1, 3);
    grid->addWidget(brack2Btn, 1, 4);

    grid->addWidget(restBtn, 2, 0);
    grid->addWidget(sevBtn, 2, 1);
    grid->addWidget(eightBtn, 2, 2);
    grid->addWidget(nineBtn, 2, 3);
    grid->addWidget(divBtn, 2, 4);

    grid->addWidget(chSignBtn, 3, 0);
    grid->addWidget(fourBtn, 3, 1);
    grid->addWidget(fiveBtn, 3, 2);
    grid->addWidget(sixBtn, 3, 3);
    grid->addWidget(multBtn,3, 4);

    grid->addWidget(clearBtn, 4, 0);
    grid->addWidget(oneBtn, 4, 1);
    grid->addWidget(twoBtn, 4, 2);
    grid->addWidget(threeBtn, 4, 3);
    grid->addWidget(minusBtn, 4, 4);

    grid->addWidget(clearAllBtn, 5, 0);
    grid->addWidget(nullBtn, 5, 1);
    grid->addWidget(dotBtn, 5, 2);
    grid->addWidget(equalsBtn, 5, 3);
    grid->addWidget(plusBtn, 5, 4);
    setLayout(grid);
}

QPushButton *Calculator::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

// returns precedence of an operator
int Calculator::prec(QChar ch) {
    if (ch == '/' || ch == '*'|| ch == '%')
        return 2;
    else if (ch == '+' || ch == '-')
        return 1;
    else
        return -1;
}

double Calculator::calculate(QString str)
{
    QStack<QChar> st;
    QString ans = "";

    //if unary minus after a bracket, convert in binary minus("(0-expr")

    if(str.contains("(-"))
    {
        size_t index = str.indexOf("(-", 0);
        str.insert(++index, '0');

    }

    //if unary minus after a plus, convert in binary minus("+0-expr")

    if(str.contains("+-"))
    {
        size_t index = str.indexOf("+-", 0);
        str.insert(++index, '0');

    }

    //if unary minus after a minus, convert in binary minus("-0-expr")

    if(str.contains("--"))
    {
        size_t index = str.indexOf("--", 0);
        str.insert(++index, '0');

    }

    //if unary minus at the very begining, convert in binary minus("0-expr")

    if(str[0]== '-')
    {
        str.insert(0,'0');
    }

    for (int i = 0; i < str.size(); i++) {

        QChar ch = str[i];
        int k = i+1;

        //if number, append to answer

        if ((ch >= '0' && ch <= '9')||(ch == '.'))
        {
            ans += ch;

            //if next character exists and it is an operator, append a space to differ numbers

            if(str.size() - i >= 2 && (str[k] == '+' || str[k] == '-' || str[k] == '/' || str[k] == '*' || str[k] == '%'))
            {
                ans += ' ';
            }
        }

        else if (ch == '(')
        {
            st.push('(');
        }
        else if (ch == ')')
        {
            while (st.top() != '(')
            {
                ans += ' ';
                ans += st.top();
                ans += ' ';
                st.pop();
            }
            st.pop();
        }
        else
        {
            while (!st.isEmpty() && prec(str[i]) <= prec(st.top()))
            {
                ans += st.top();
                ans += ' ';
                st.pop();
            }
            st.push(ch);
        }
    }

    while (!st.isEmpty())
    {
        ans += ' ';
        ans += st.top();
        st.pop();
    }

    QStringList list = ans.split(' ');
    list.removeAll("");
    list.removeAll(" ");

    // prefix expression evaluation
    QStack<double> stack;
    foreach (QString c, list)
    {
        bool isNum = false;
        bool isDoubleNum = false;

        int value = c.toInt(&isNum);
        double d_value = c.toDouble(&isDoubleNum);

        if (isNum)
        {
            stack.push(value);
            qDebug()<< stack;
        }
        else if (isDoubleNum)
        {
            stack.push(d_value);
            qDebug()<< stack;
        }
        else
        {
            double x = stack.top();
            stack.pop();

            double y = stack.top();
            stack.pop();

            if (c == "+")
            {
                stack.push(y + x);
            }
            else if (c == "-")
            {
                stack.push(y - x);
            }
            else if (c =="*" )
            {
                stack.push(y * x);
            }
            else if (c == "/")
            {
                stack.push(static_cast<double>(y) / x);
            }
            else if (c == "%")
            {
                stack.push(static_cast<int>(y) % static_cast<int>(x));
            }
        }
    }

    display->setText(QString::number(stack.top()));
    return stack.top();
}

void Calculator::setNumb()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    expr.append(clickedButton->text());

    display->setText(display->text() + clickedButton->text());
}

void Calculator::doClear()
{
    display->setText("");
}

void Calculator::doClearOneCh()
{
    QString str = display->text();
    str.chop(1);

    display->setText(str);
}

void Calculator::doMemCl()
{
    savedExpr = "";
}

void Calculator::doMemSave()
{
    savedExpr = display->text();
}

void Calculator::changeSign()
{
    result = -result;

    display->setText(QString::number(result));
}

void Calculator::doMemRecall()
{
    display->setText(savedExpr);
}


void Calculator::setEnd()
{
    result = calculate(display->text());
}




