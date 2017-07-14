#include "xyinput.h"
#include "xydatabaseoperation.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

XYInput *XYInput::mopInstance;
XYInput *XYInput::getInstance()
{
    if (mopInstance == NULL)
    {
        mopInstance = new XYInput;
    }
    return mopInstance;
}

XYInput::XYInput(QWidget *parent)
    : XYBorderShadowWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool
                   | Qt::WindowDoesNotAcceptFocus);
    this->setAttribute(Qt::WA_TranslucentBackground);

    mbEnglish = false;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(QMargins(13, 13, 13, 13));
    mopTransLateView = new XYTranslateView;
    mopLineEdit = new QLineEdit;
    mopLineEdit->installEventFilter(this);
    layout->addWidget(mopLineEdit);
    connect(mopLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(mslotFindTranslate(const QString &)));

    setLayout(layout);
}

bool XYInput::initInputBase(const QString &path)
{
    return XYDB->openDatabaseFile(path);
}

XYInput::~XYInput()
{
    delete mopTransLateView;
}

bool XYInput::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && obj->isWidgetType())
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (obj == mopLineEdit)
        {
            switch (keyEvent->key())
            {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                completeInput(new QKeyEvent(QEvent::KeyPress,
                                            Qt::Key_unknown,
                                            Qt::NoModifier,
                                            mopLineEdit->text()));
                break;
            case Qt::Key_Equal:
                mopTransLateView->nextPage();
                mopTransLateView->update();
                return true;
            case Qt::Key_Minus:
                mopTransLateView->prePage();
                mopTransLateView->update();
                return true;
            case Qt::Key_Shift:
                setEnglish(!mbEnglish);
                load();
                return true;
            case Qt::Key_Tab:
                showMoreInfo();
                load();
                return true;
            case Qt::Key_Space:
                completeInput(new QKeyEvent(QEvent::KeyPress,
                                            Qt::Key_unknown,
                                            Qt::NoModifier,
                                            mopTransLateView->getData(1)));
                return true;
            default:
                break;
            }
            if (keyEvent->key() >= Qt::Key_0 && keyEvent->key() <= Qt::Key_9 && keyEvent->modifiers() != Qt::KeypadModifier)
            {
                int index = keyEvent->text().toInt();
                if (index > 0 && index <= mopTransLateView->miMaxVisibleItem && mopTransLateView->itemCount() >= index)
                {
                    completeInput(new QKeyEvent(QEvent::KeyPress,
                                                Qt::Key_unknown,
                                                Qt::NoModifier,
                                                mopTransLateView->getData(index)));
                }
                return true;
            }
        }
        else if (keyEvent->key() != Qt::Key_unknown)
        {
            mopLatestWidget = static_cast<QWidget *>(obj);
            XYInput *input = XYInput::getInstance();
            if (!input->isVisible())
            {
                if (keyEvent->key() >= Qt::Key_A && keyEvent->key() <= Qt::Key_Z)
                {
                    input->mopLineEdit->clear();
                    input->mopTransLateView->clear();
                    input->show();
                    qApp->postEvent(input->mopLineEdit, new QKeyEvent(QEvent::KeyPress,
                                                                      keyEvent->key(),
                                                                      keyEvent->modifiers(),
                                                                      keyEvent->text()));
                    return true;
                }
            }
            else
            {
                qApp->postEvent(input->mopLineEdit, new QKeyEvent(QEvent::KeyPress,
                                                                  keyEvent->key(),
                                                                  keyEvent->modifiers(),
                                                                  keyEvent->text()));
                return true;
            }
        }
    }
    else if (QEvent::InputMethod == event->type())
    {
        // 这里是输入法传入的信号，不过，我们可以不用
        QInputMethodEvent *me = static_cast<QInputMethodEvent *>(event);
        if (me->attributes().size() == 1 && me->attributes().at(0).type == 1)
        {
            qDebug("Text: %s", me->commitString().toUtf8().data());
        }
        // 如果想屏蔽，直接返回true
        // return true;
    }
    else if (QEvent::FocusOut == event->type())
    {
        XYInput *input = XYInput::getInstance();
        input->close();
    }
    return QWidget::eventFilter(obj, event);
}

QString XYInput::splitePinyin(const QString &pinyin, int &num)
{
    static QString shenmu = "bpmfdtnlgkhjqxzcsywr";
    static QStringList zcs = QString("z c s").split(" ");
    static QStringList zhchsh = QString("zh ch sh").split(" ");
    static QStringList yunmu = QString("a o e i u v ai ei ao ou iu ie ue er an en in un ang eng ing ong uan uang ian iang").split(" ");
    static QStringList yunmuA = QString("a o e ai ei an ang en ao ou").split(" ");
    QString result;
    int cur_index = 0;
    while (cur_index < pinyin.size())
    {
        if (!result.isEmpty()) // 每次进入一定的新的字的拼音
        {
            result += "%\'";
        }
        if (shenmu.contains(pinyin.at(cur_index))) // 是声母
        {
            int ym = 1;
            int h = 0; // zh ch sh标记
            // zh ch sh 多加一位
            if (zcs.contains(pinyin.at(cur_index)) && cur_index + 1 < pinyin.size() && pinyin.at(cur_index + 1) == 'h')
            {
                h = 1;
                ym++;
            }

            // 贪心查找 （尽可能长的找到满足的） 注意：这里有可能还有没有判断全的特殊情况
            while ((ym + cur_index) < pinyin.size() &&
                   (yunmu.contains(pinyin.mid(cur_index + 1 + h, ym - h))
                    || pinyin.mid(cur_index + 1 + h, ym - h) == "ua"
                    || pinyin.mid(cur_index + 1 + h, ym - h) == "ia"
                    || pinyin.mid(cur_index + 1 + h, ym - h) == "on")) // uan ian ong比较特殊
            {
                ym++;
            }

            result += pinyin.mid(cur_index, ym);
            cur_index += ym - 1;
        }
        else
        {
            // 处理独成一字的韵母
            if (yunmuA.contains(pinyin.at(cur_index)))
            {
                int ym = 1;
                while ((ym + cur_index) < pinyin.size()
                       && yunmuA.contains(pinyin.mid(cur_index, ym + 1)))
                {
                    ym++;
                }
                result += pinyin.mid(cur_index, ym);
                cur_index += ym - 1;
            }
            else
            {
                result += pinyin.at(cur_index);
            }
        }
        num++;
        cur_index++;
    }
    if (num == 0)
    {
        num++;
    }
    return result;
}

void XYInput::mslotFindTranslate(const QString &keyword)
{
    if (keyword.isEmpty()) // 如果传入的词为空了，代表删完了，应该关闭输入窗口
    {
        close();
        return;
    }

    QList<XYTranslateItem *> list;
    if (mbEnglish)
    {
        list = XYDB->findData(keyword + "%", "", "userEnglishTable");
        list += XYDB->findData(keyword + "%", "", "englishTable");
    }
    else
    {
        int num = 0;
        QString splitePY = splitePinyin(keyword, num);
        list = XYDB->findData(splitePY + "%", QString::number(num), "userPingying");
        if (num == 1)
        {
            QList<XYTranslateItem *> single = XYDB->findData(splitePY + "%", "", "singlePingying");

            for (int i = 0; i < single.size(); ++i)
            {
                XYTranslateItem *singleItem = single.at(i);
                QStringList singles = singleItem->msTranslate.split(" ", QString::SkipEmptyParts);
                for (int j = 0; j < singles.size(); ++j)
                {
                    list.append(new XYTranslateItem("", singles.at(j), singleItem->msComplete));
                }
            }
            qDeleteAll(single);
        }
        list += XYDB->findData(splitePY + "%", QString::number(num), "basePintying");
    }

    mopTransLateView->setData(list);
    load();
}

void XYInput::completeInput(QKeyEvent *event)
{
    qApp->postEvent(mopLatestWidget, event);
    emit complete(event->text());
    close();
}

void XYInput::setEnglish(bool english)
{
    mbEnglish = english;
    if (mbEnglish)
    {
        mopTransLateView->showType = XYTranslateModel::COMPLETE;
    }
    else
    {
        mopTransLateView->showType = XYTranslateModel::TRANSLATE;
    }
}

void XYInput::showMoreInfo()
{
    if (mbEnglish)
    {
        if (mopTransLateView->showType == XYTranslateModel::COMP_TRAN)
        {
            mopTransLateView->showType = XYTranslateModel::COMPLETE;
        }
        else
        {
            mopTransLateView->showType = XYTranslateModel::COMP_TRAN;
        }
    }
    else
    {
        if (mopTransLateView->showType == XYTranslateModel::TRAN_COMP)
        {
            mopTransLateView->showType = XYTranslateModel::TRANSLATE;
        }
        else
        {
            mopTransLateView->showType = XYTranslateModel::TRAN_COMP;
        }
    }
}

bool XYInput::close()
{
    mopTransLateView->clear();
    mopTransLateView->repaint(); // 清理view,避免显示的时候刷新
    mopTransLateView->close();
    return XYBorderShadowWidget::close();
}

void XYInput::show()
{
    QDesktopWidget *desk = qApp->desktop();
    int pos_x, pos_y;
    pos_x = QCursor::pos().x();
    pos_y = QCursor::pos().y();
    if (pos_x + this->width() > desk->width())
    {
        pos_x = desk->width() - this->width();
    }
    if (pos_y + this->height() > desk->height())
    {
        pos_y = desk->height() - this->height();
    }

    this->move(pos_x, pos_y);
    QWidget::show();
}

void XYInput::load()
{
    if (mopTransLateView->itemCount() == 0) // 当没有找到词组，直接关闭退出
    {
        mopTransLateView->close();
        return;
    }
    QDesktopWidget *desk = qApp->desktop();
    int pos_x, pos_y;
    pos_x = this->pos().x();
    pos_y = this->pos().y() + this->height() - 15;
    if (pos_x + mopTransLateView->width() > desk->width())
    {
        pos_x = desk->width() - mopTransLateView->width();
    }
    if (pos_y + mopTransLateView->height() > desk->height())
    {
        pos_y = this->pos().y() - mopTransLateView->height() + 15;
    }

    if (mopTransLateView->pos() != QPoint(pos_x, pos_y))
    {
        mopTransLateView->move(pos_x, pos_y);
    }
    mopTransLateView->repaint();
    mopTransLateView->show();
}

