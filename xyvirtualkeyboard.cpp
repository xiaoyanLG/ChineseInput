#include "xyvirtualkeyboard.h"
#include "xypushbutton.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMouseEvent>
#include <QLineEdit>
#include <QDebug>

XYVirtualKeyboard *XYVirtualKeyboard::instance = NULL;
XYVirtualKeyboard::XYVirtualKeyboard(QWidget *parent)
    : QWidget(parent), mbLeftMousePressed(false)
{
    this->setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool);
#if QT_VERSION >= 0x050000
    this->setWindowFlags(this->windowFlags() | Qt::WindowDoesNotAcceptFocus);
#endif

    // 主键盘区域
    XYPushButton *main_point = new XYPushButton("~\n`", Qt::Key_QuoteLeft, Qt::Key_AsciiTilde);
    XYPushButton *main_number1 = new XYPushButton("!\n1", Qt::Key_1, Qt::Key_Exclam);
    XYPushButton *main_number2 = new XYPushButton("@\n2", Qt::Key_2, Qt::Key_At);
    XYPushButton *main_number3 = new XYPushButton("#\n3", Qt::Key_3, Qt::Key_NumberSign);
    XYPushButton *main_number4 = new XYPushButton("$\n4", Qt::Key_4, Qt::Key_Dollar);
    XYPushButton *main_number5 = new XYPushButton("%\n5", Qt::Key_5, Qt::Key_Percent);
    XYPushButton *main_number6 = new XYPushButton("^\n6", Qt::Key_6, Qt::Key_AsciiCircum);
    XYPushButton *main_number7 = new XYPushButton("&\n7", Qt::Key_7, Qt::Key_Ampersand);
    XYPushButton *main_number8 = new XYPushButton("*\n8", Qt::Key_8, Qt::Key_Asterisk);
    XYPushButton *main_number9 = new XYPushButton("(\n9", Qt::Key_9, Qt::Key_ParenLeft);
    XYPushButton *main_number0 = new XYPushButton(")\n0", Qt::Key_0, Qt::Key_ParenRight);
    XYPushButton *main_sub = new XYPushButton("_\n-", Qt::Key_Minus, Qt::Key_Underscore);
    XYPushButton *main_equal = new XYPushButton("+\n=", Qt::Key_Equal, Qt::Key_Plus);
    XYPushButton *main_backspace = new XYPushButton("BackSpace", Qt::Key_Backspace);
    main_backspace->setMinimumWidth(75);

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


    XYPushButton *main_tab = new XYPushButton("Tab", Qt::Key_Tab);
    main_tab->setMinimumWidth(75);
    XYPushButton *main_letterQ = new XYPushButton("Q", Qt::Key_Q);
    XYPushButton *main_letterW = new XYPushButton("W", Qt::Key_W);
    XYPushButton *main_letterE = new XYPushButton("E", Qt::Key_E);
    XYPushButton *main_letterR = new XYPushButton("R", Qt::Key_R);
    XYPushButton *main_letterT = new XYPushButton("T", Qt::Key_T);
    XYPushButton *main_letterY = new XYPushButton("Y", Qt::Key_Y);
    XYPushButton *main_letterU = new XYPushButton("U", Qt::Key_U);
    XYPushButton *main_letterI = new XYPushButton("I", Qt::Key_I);
    XYPushButton *main_letterO = new XYPushButton("O", Qt::Key_O);
    XYPushButton *main_letterP = new XYPushButton("P", Qt::Key_P);
    XYPushButton *main_bracketL = new XYPushButton("{\n[", Qt::Key_BracketLeft, Qt::Key_BraceLeft);
    XYPushButton *main_bracketR = new XYPushButton("}\n]", Qt::Key_BracketRight, Qt::Key_BraceRight);
    XYPushButton *main_verticalLine = new XYPushButton("|\n\\", Qt::Key_Backslash, Qt::Key_Bar);

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

    XYPushButton *main_caps = new XYPushButton("Caps\nLock", Qt::Key_CapsLock);
    main_caps->setCheckable(true);
    XYPushButton *main_letterA = new XYPushButton("A", Qt::Key_A);
    XYPushButton *main_letterS = new XYPushButton("S", Qt::Key_S);
    XYPushButton *main_letterD = new XYPushButton("D", Qt::Key_D);
    XYPushButton *main_letterF = new XYPushButton("F", Qt::Key_F);
    XYPushButton *main_letterG = new XYPushButton("G", Qt::Key_G);
    XYPushButton *main_letterH = new XYPushButton("H", Qt::Key_H);
    XYPushButton *main_letterJ = new XYPushButton("J", Qt::Key_J);
    XYPushButton *main_letterK = new XYPushButton("K", Qt::Key_K);
    XYPushButton *main_letterL = new XYPushButton("L", Qt::Key_L);
    XYPushButton *main_colon = new XYPushButton(":\n;", Qt::Key_Semicolon, Qt::Key_Colon);
    XYPushButton *main_mark = new XYPushButton("\"\n\'", Qt::Key_Apostrophe, Qt::Key_QuoteDbl);
    XYPushButton *main_enter = new XYPushButton("Enter", Qt::Key_Return);

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

    XYPushButton *main_shiftL = new XYPushButton("Shift", Qt::Key_Shift);
    main_shiftL->setCheckable(true);
    XYPushButton *main_letterZ = new XYPushButton("Z", Qt::Key_Z);
    XYPushButton *main_letterX = new XYPushButton("X", Qt::Key_X);
    XYPushButton *main_letterC = new XYPushButton("C", Qt::Key_C);
    XYPushButton *main_letterV = new XYPushButton("V", Qt::Key_V);
    XYPushButton *main_letterB = new XYPushButton("B", Qt::Key_B);
    XYPushButton *main_letterN = new XYPushButton("N", Qt::Key_N);
    XYPushButton *main_letterM = new XYPushButton("M", Qt::Key_M);
    XYPushButton *main_angleBracketL = new XYPushButton("<\n,", Qt::Key_Comma, Qt::Key_Less);
    XYPushButton *main_angleBracketR = new XYPushButton(">\n.", Qt::Key_Period, Qt::Key_Greater);
    XYPushButton *main_questionMark = new XYPushButton("?\n/", Qt::Key_Slash, Qt::Key_Question);
    XYPushButton *main_shiftR = new XYPushButton("Shift", Qt::Key_Shift);
    main_shiftR->setCheckable(true);
    connect(main_shiftL, SIGNAL(checkedChanged(bool)), main_shiftR, SLOT(setChecked(bool)));
    connect(main_shiftR, SIGNAL(checkedChanged(bool)), main_shiftL, SLOT(setChecked(bool)));

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

    XYPushButton *main_ctrlL = new XYPushButton("Ctrl", Qt::Key_Control);
    main_ctrlL->setCheckable(true);
    XYPushButton *main_sysL = new XYPushButton(QString::fromStdWString(L"缩小"), Qt::Key_Meta);
    connect(main_sysL, SIGNAL(clicked()), this, SLOT(shrink()));
    XYPushButton *main_altL = new XYPushButton("Alt", Qt::Key_Alt);
    main_altL->setCheckable(true);
    XYPushButton *main_space = new XYPushButton(" ", Qt::Key_Space);
    XYPushButton *main_altR = new XYPushButton("Alt", Qt::Key_Alt);
    main_altR->setCheckable(true);
    XYPushButton *main_sysR = new XYPushButton(QString::fromStdWString(L"放大"), Qt::Key_Meta);
    connect(main_sysR, SIGNAL(clicked()), this, SLOT(enlarge()));
    XYPushButton *main_rect = new XYPushButton(QString::fromStdWString(L"关闭"), Qt::Key_Menu);
    connect(main_rect, SIGNAL(clicked()), this, SLOT(close()));
    XYPushButton *main_ctrlR= new XYPushButton("Ctrl", Qt::Key_Control);
    main_ctrlR->setCheckable(true);

    connect(main_ctrlL, SIGNAL(checkedChanged(bool)), main_ctrlR, SLOT(setChecked(bool)));
    connect(main_ctrlR, SIGNAL(checkedChanged(bool)), main_ctrlL, SLOT(setChecked(bool)));
    connect(main_altL, SIGNAL(checkedChanged(bool)), main_altR, SLOT(setChecked(bool)));
    connect(main_altR, SIGNAL(checkedChanged(bool)), main_altL, SLOT(setChecked(bool)));

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

    mainWidget = new QWidget;
    layout_main->setContentsMargins(2, 2, 2, 2);
    mainWidget->setLayout(layout_main);

    // 小键盘区域
    smallNumLockBtn = new XYPushButton("Num\nLock", Qt::Key_NumLock, -1, true);
    smallNumLockBtn->setCheckable(true);
    XYPushButton *small_slash = new XYPushButton("/", Qt::Key_Slash, -1, true);
    XYPushButton *small_asterisk = new XYPushButton("*", Qt::Key_Asterisk, -1, true);
    XYPushButton *small_sub = new XYPushButton("-", Qt::Key_Minus, -1, true);
    XYPushButton *small_num7 = new XYPushButton("7", Qt::Key_7, -1, true);
    XYPushButton *small_num8 = new XYPushButton("8", Qt::Key_8, -1, true);
    XYPushButton *small_num9 = new XYPushButton("9", Qt::Key_9, -1, true);
    XYPushButton *small_add = new XYPushButton("+", Qt::Key_Plus, -1, true);
    small_add->setMinimumHeight(80);
    XYPushButton *small_num4 = new XYPushButton("4", Qt::Key_4, -1, true);
    XYPushButton *small_num5 = new XYPushButton("5", Qt::Key_5, -1, true);
    XYPushButton *small_num6 = new XYPushButton("6", Qt::Key_6, -1, true);
    XYPushButton *small_num1 = new XYPushButton("1", Qt::Key_1, -1, true);
    XYPushButton *small_num2 = new XYPushButton("2", Qt::Key_2, -1, true);
    XYPushButton *small_num3 = new XYPushButton("3", Qt::Key_3, -1, true);
    XYPushButton *small_enter = new XYPushButton("Enter", Qt::Key_Enter, -1, true);
    small_enter->setMinimumHeight(80);
    XYPushButton *small_num0 = new XYPushButton("0", Qt::Key_0, -1, true);
    XYPushButton *small_point = new XYPushButton(".", Qt::Key_Period, -1, true);

    QGridLayout *layout_small = new QGridLayout;
    layout_small->addWidget(smallNumLockBtn, 0, 0, 1, 1);
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

    smallWidget = new QWidget;
    layout_small->setContentsMargins(2, 2, 2, 2);
    smallWidget->setLayout(layout_small);
    QHBoxLayout *main = new QHBoxLayout(this);
    main->setContentsMargins(5, 5, 5, 5);
    main->addWidget(mainWidget);
    main->addWidget(smallWidget);
}

XYVirtualKeyboard *XYVirtualKeyboard::getInstance()
{
    if (instance == NULL)
    {
        instance = new XYVirtualKeyboard;
    }
    return instance;
}

XYVirtualKeyboard::~XYVirtualKeyboard()
{

}

void XYVirtualKeyboard::enlarge()
{
    resize(size() + QSize(10, 10));
}

void XYVirtualKeyboard::shrink()
{
    resize(size() - QSize(10, 10));
}

void XYVirtualKeyboard::show()
{
    smallWidget->setVisible(true);
    mainWidget->setVisible(true);
    QWidget::show();
}

void XYVirtualKeyboard::showMain()
{
    smallWidget->setVisible(false);
    mainWidget->setVisible(true);
    smallNumLockBtn->disconnect();
    QWidget::show();
}

void XYVirtualKeyboard::showSmall()
{
    mainWidget->setVisible(false);
    smallWidget->setVisible(true);
    smallNumLockBtn->disconnect();
    connect(smallNumLockBtn, SIGNAL(clicked()), this, SLOT(close()));
    QWidget::show();
}

bool XYVirtualKeyboard::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
}

void XYVirtualKeyboard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = true;
        moLastPos = event->globalPos();
    }
}

void XYVirtualKeyboard::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mbLeftMousePressed = false;
        moLastPos = event->globalPos();
    }
}

void XYVirtualKeyboard::mouseMoveEvent(QMouseEvent *event)
{
    if (mbLeftMousePressed)
    {
        QPoint lastpos = pos();
        lastpos.setX( lastpos.x() + event->globalX() - moLastPos.x());
        lastpos.setY( lastpos.y() + event->globalY() - moLastPos.y());
        move(lastpos);
        moLastPos = event->globalPos();
    }
}

