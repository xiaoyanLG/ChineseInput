#include "xyvirtualkeyboard.h"
#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

XYVirtualKeyboard::XYVirtualKeyboard(QWidget *parent)
    : QWidget(parent)
{
    // 主键盘区域
    QPushButton *main_point = new QPushButton("~\n`");
    QPushButton *main_number1 = new QPushButton("!\n1");
    QPushButton *main_number2 = new QPushButton("@\n2");
    QPushButton *main_number3 = new QPushButton("#\n3");
    QPushButton *main_number4 = new QPushButton("$\n4");
    QPushButton *main_number5 = new QPushButton("%\n5");
    QPushButton *main_number6 = new QPushButton("^\n6");
    QPushButton *main_number7 = new QPushButton("&&\n7");
    QPushButton *main_number8 = new QPushButton("*\n8");
    QPushButton *main_number9 = new QPushButton("(\n9");
    QPushButton *main_number0 = new QPushButton(")\n0");
    QPushButton *main_sub = new QPushButton("__\n-");
    QPushButton *main_equal = new QPushButton("+\n=");
    QPushButton *main_backspace = new QPushButton("BackSpace\n");
    main_backspace->setMinimumWidth(100);

    QHBoxLayout *layout_1 = new QHBoxLayout;
    layout_1->addWidget(main_point);
    layout_1->addWidget(main_number1);
    layout_1->addWidget(main_number2);
    layout_1->addWidget(main_number3);
    layout_1->addWidget(main_number4);
    layout_1->addWidget(main_number5);
    layout_1->addWidget(main_number6);
    layout_1->addWidget(main_number7);
    layout_1->addWidget(main_number8);
    layout_1->addWidget(main_number9);
    layout_1->addWidget(main_number0);
    layout_1->addWidget(main_sub);
    layout_1->addWidget(main_equal);
    layout_1->addWidget(main_backspace);


    QPushButton *main_tab = new QPushButton("Tab\n");
    main_tab->setMinimumWidth(100);
    QPushButton *main_letterQ = new QPushButton("Q\n");
    QPushButton *main_letterW = new QPushButton("W\n");
    QPushButton *main_letterE = new QPushButton("E\n");
    QPushButton *main_letterR = new QPushButton("R\n");
    QPushButton *main_letterT = new QPushButton("T\n");
    QPushButton *main_letterY = new QPushButton("Y\n");
    QPushButton *main_letterU = new QPushButton("U\n");
    QPushButton *main_letterI = new QPushButton("I\n");
    QPushButton *main_letterO = new QPushButton("O\n");
    QPushButton *main_letterP = new QPushButton("P\n");
    QPushButton *main_bracketL = new QPushButton("{\n[");
    QPushButton *main_bracketR = new QPushButton("}\n]");
    QPushButton *main_verticalLine = new QPushButton("|\n\\");

    QHBoxLayout *layout_2 = new QHBoxLayout;
    layout_2->addWidget(main_tab);
    layout_2->addWidget(main_letterQ);
    layout_2->addWidget(main_letterW);
    layout_2->addWidget(main_letterE);
    layout_2->addWidget(main_letterR);
    layout_2->addWidget(main_letterT);
    layout_2->addWidget(main_letterY);
    layout_2->addWidget(main_letterU);
    layout_2->addWidget(main_letterI);
    layout_2->addWidget(main_letterO);
    layout_2->addWidget(main_letterP);
    layout_2->addWidget(main_bracketL);
    layout_2->addWidget(main_bracketR);
    layout_2->addWidget(main_verticalLine);

    QPushButton *main_caps = new QPushButton("Caps\nLock");
    QPushButton *main_letterA = new QPushButton("A\n");
    QPushButton *main_letterS = new QPushButton("S\n");
    QPushButton *main_letterD = new QPushButton("D\n");
    QPushButton *main_letterF = new QPushButton("F\n");
    QPushButton *main_letterG = new QPushButton("G\n");
    QPushButton *main_letterH = new QPushButton("H\n");
    QPushButton *main_letterJ = new QPushButton("J\n");
    QPushButton *main_letterK = new QPushButton("K\n");
    QPushButton *main_letterL = new QPushButton("L\n");
    QPushButton *main_colon = new QPushButton(":\n;");
    QPushButton *main_mark = new QPushButton("\"\n\'");
    QPushButton *main_enter = new QPushButton("Enter\n");

    QHBoxLayout *layout_3 = new QHBoxLayout;
    layout_3->addWidget(main_caps, 1.5);
    layout_3->addWidget(main_letterA);
    layout_3->addWidget(main_letterS);
    layout_3->addWidget(main_letterD);
    layout_3->addWidget(main_letterF);
    layout_3->addWidget(main_letterG);
    layout_3->addWidget(main_letterH);
    layout_3->addWidget(main_letterJ);
    layout_3->addWidget(main_letterK);
    layout_3->addWidget(main_letterL);
    layout_3->addWidget(main_colon);
    layout_3->addWidget(main_mark);
    layout_3->addWidget(main_enter, 1.5);

    QPushButton *main_shiftL = new QPushButton("Shift\n");
    QPushButton *main_letterZ = new QPushButton("Z\n");
    QPushButton *main_letterX = new QPushButton("X\n");
    QPushButton *main_letterC = new QPushButton("C\n");
    QPushButton *main_letterV = new QPushButton("V\n");
    QPushButton *main_letterB = new QPushButton("B\n");
    QPushButton *main_letterN = new QPushButton("N\n");
    QPushButton *main_letterM = new QPushButton("M\n");
    QPushButton *main_angleBracketL = new QPushButton("<\n,");
    QPushButton *main_angleBracketR = new QPushButton(">\n.");
    QPushButton *main_questionMark = new QPushButton("?\n/");
    QPushButton *main_shiftR = new QPushButton("Shift\n");

    QHBoxLayout *layout_4 = new QHBoxLayout;
    layout_4->addWidget(main_shiftL, 2);
    layout_4->addWidget(main_letterZ);
    layout_4->addWidget(main_letterX);
    layout_4->addWidget(main_letterC);
    layout_4->addWidget(main_letterV);
    layout_4->addWidget(main_letterB);
    layout_4->addWidget(main_letterN);
    layout_4->addWidget(main_letterM);
    layout_4->addWidget(main_angleBracketL);
    layout_4->addWidget(main_angleBracketR);
    layout_4->addWidget(main_questionMark);
    layout_4->addWidget(main_shiftR, 2);

    QPushButton *main_ctrlL = new QPushButton("Ctrl\n");
    QPushButton *main_sysL = new QPushButton("SylixOS\n");
    QPushButton *main_altL = new QPushButton("Alt\n");
    QPushButton *main_space = new QPushButton("  \n");
    QPushButton *main_altR = new QPushButton("Alt\n");
    QPushButton *main_sysR = new QPushButton("SylixOS\n");
    QPushButton *main_rect = new QPushButton("[]\n");
    QPushButton *main_ctrlR= new QPushButton("Ctrl\n");

    QHBoxLayout *layout_5 = new QHBoxLayout;
    layout_5->addWidget(main_ctrlL);
    layout_5->addWidget(main_sysL);
    layout_5->addWidget(main_altL);
    layout_5->addWidget(main_space, 2);
    layout_5->addWidget(main_altR);
    layout_5->addWidget(main_sysR);
    layout_5->addWidget(main_rect);
    layout_5->addWidget(main_ctrlR);

    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addLayout(layout_1);
    layout_main->addLayout(layout_2);
    layout_main->addLayout(layout_3);
    layout_main->addLayout(layout_4);
    layout_main->addLayout(layout_5);


    // 小键盘区域
    QPushButton *small_numLock = new QPushButton("Num\nLock");
    QPushButton *small_slash = new QPushButton("/\n");
    QPushButton *small_asterisk = new QPushButton("*\n");
    QPushButton *small_sub = new QPushButton("-\n");
    QPushButton *small_num7 = new QPushButton("7\n");
    QPushButton *small_num8 = new QPushButton("8\n");
    QPushButton *small_num9 = new QPushButton("9\n");
    QPushButton *small_add = new QPushButton("+\n");
    small_add->setMinimumHeight(80);
    QPushButton *small_num4 = new QPushButton("4\n");
    QPushButton *small_num5 = new QPushButton("5\n");
    QPushButton *small_num6 = new QPushButton("6\n");
    QPushButton *small_num1 = new QPushButton("1\n");
    QPushButton *small_num2 = new QPushButton("2\n");
    QPushButton *small_num3 = new QPushButton("3\n");
    QPushButton *small_enter = new QPushButton("Enter\n");
    small_enter->setMinimumHeight(80);
    QPushButton *small_num0 = new QPushButton("0\n");
    QPushButton *small_point = new QPushButton(".\n");

    QGridLayout *layout_small = new QGridLayout;
    layout_small->setColumnStretch(3, 1);
    layout_small->addWidget(small_numLock, 0, 0, 1, 1);
    layout_small->addWidget(small_slash, 0, 1, 1, 1);
    layout_small->addWidget(small_asterisk, 0, 2, 1, 1);
    layout_small->addWidget(small_sub, 0, 3, 1, 1);
    layout_small->addWidget(small_num7, 1, 0, 1, 1);
    layout_small->addWidget(small_num8, 1, 1, 1, 1);
    layout_small->addWidget(small_num9, 1, 2, 1, 1);
    layout_small->addWidget(small_add, 1, 3, 2, 1);
    layout_small->addWidget(small_num4, 2, 0, 1, 1);
    layout_small->addWidget(small_num5, 2, 1, 1, 1);
    layout_small->addWidget(small_num6, 2, 2, 1, 1);
    layout_small->addWidget(small_num1, 3, 0, 1, 1);
    layout_small->addWidget(small_num2, 3, 1, 1, 1);
    layout_small->addWidget(small_num3, 3, 2, 1, 1);
    layout_small->addWidget(small_enter, 3, 3, 2, 1);
    layout_small->addWidget(small_num0, 4, 0, 1, 2);
    layout_small->addWidget(small_point, 4, 2, 1, 1);

    QHBoxLayout *main = new QHBoxLayout(this);
    main->addLayout(layout_main);
    main->addSpacing(15);
    main->addLayout(layout_small);
}

XYVirtualKeyboard::~XYVirtualKeyboard()
{

}

bool XYVirtualKeyboard::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
}

