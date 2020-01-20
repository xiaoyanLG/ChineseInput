#include "xyinput.h"
#include "xydatabaseoperation.h"
#include <QLineEdit>
#include <QHBoxLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QDesktopWidget>

#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
#include <QWindow>
#endif

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
                   | Qt::Tool);
#if QT_VERSION >= 0x050000
    this->setWindowFlags(this->windowFlags() | Qt::WindowDoesNotAcceptFocus);
#endif
    this->setAttribute(Qt::WA_TranslucentBackground);

    mbEnglish = false;

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(QMargins(13, 13, 13, 13));
    mopTransLateView = new XYTranslateView;
    mopLineEdit = new QLineEdit;
    mopLineEdit->installEventFilter(this);
    layout->addWidget(mopLineEdit);
    connect(mopLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(mslotFindTranslate(const QString &)));

    // 这个接口目前有BUG，暂时不用，不影响输入
//    connect(mopLineEdit, &QLineEdit::textChanged, this, [this](const QString &text){
//        emit this->send_preedit(text);
//    });
    setLayout(layout);
    adjustSize();
}

bool XYInput::initInputBase(const QString &path)
{
    return XYDB->openDatabaseFile(path);
}

XYInput::~XYInput()
{
    delete mopTransLateView;
}

void XYInput::setScreen(QScreen *screen)
{
#if QT_VERSION > QT_VERSION_CHECK(5, 0, 0)
    if (!windowHandle()) {
        setVisible(true);
        setVisible(false);
    }
    if (windowHandle()->screen() != screen) {
        windowHandle()->setScreen(screen);
    }

    if (!mopTransLateView->windowHandle()) {
        mopTransLateView->setVisible(true);
        mopTransLateView->setVisible(false);
    }
    if (mopTransLateView->windowHandle()->screen() != screen) {
        mopTransLateView->windowHandle()->setScreen(screen);
    }
#endif
}

bool XYInput::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (obj == mopLineEdit)
        {
            if (keyEvent->key() >= Qt::Key_A && keyEvent->key() <= Qt::Key_Z)
            {
                // 在输入字母的时候显示
                if (!isVisible()) {
                    show();
                }

                if (msCurrentKeyWords.split("%\'").size() >= 11) // 最大汉子查询数量
                {
                    event->accept();
                    return true;
                }
                else if (!moCompleteItem.msComplete.isEmpty() && !mbEnglish)
                {
                    int num = 0;
                    msCurrentKeyWords = splitePinyin(msCurrentKeyWords.replace("%\'", "") + keyEvent->text(), num);
                    XYTranslateItem item;
                    completeInput(moCompleteItem.msTranslate, &item);
                    return true;
                }
                else if (mopLineEdit->text().isEmpty() || mbEnglish)
                {
                    return false;
                }
            }

            if (!mopLineEdit->text().isEmpty())
            {
                switch (keyEvent->key())
                {
                case Qt::Key_Return:
                case Qt::Key_Enter:
                    completeInput(mopLineEdit->text());
                    break;
                case Qt::Key_Equal:
                    mopTransLateView->nextPage();
                    return true;
                case Qt::Key_Minus:
                    mopTransLateView->prePage();
                    return true;
                case Qt::Key_Shift:
                    clearTemp();
                    setEnglish(!mbEnglish);
                    mslotFindTranslate(mopLineEdit->text());
                    return true;
                case Qt::Key_Tab:
                    showMoreInfo();
                    load();
                    return true;
                case Qt::Key_Space:
                    completeInput(mopTransLateView->getData(1), mopTransLateView->getItem(1));
                    return true;
                case Qt::Key_Backspace:
                    if (!moCompleteItem.msComplete.isEmpty())
                    {
                        moCompleteItem.clear();
                    }
                    msCurrentKeyWords = msCurrentKeyWords.replace("%\'", "");
                    msCurrentKeyWords.remove(msCurrentKeyWords.size() - 1, 1);
                    mopLineEdit->setText(msCurrentKeyWords);
                    mslotFindTranslate(msCurrentKeyWords);
                    return true;
                default:
                    if (keyEvent->key() >= Qt::Key_0 && keyEvent->key() <= Qt::Key_9 && keyEvent->modifiers() != Qt::KeypadModifier)
                    {
                        int index = keyEvent->text().toInt();
                        if (index > 0 && index <= mopTransLateView->miMaxVisibleItem && mopTransLateView->itemCount() >= index)
                        {
                            completeInput(mopTransLateView->getData(index), mopTransLateView->getItem(index));
                        }
                        return true;
                    }
                    break;
                }
            }
            else
            {
                emit send_keyEvent(keyEvent);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

QString XYInput::splitePinyin(const QString &pinyin, int &num)
{
    static QString shenmu = "bpmfdtnlgkhjqxzcsywr";
    static QStringList zcs = QString("z c s").split(" ");
    static QStringList yunmuA = QString("a o e ai ao ou ei er an ang en eng").split(" ");

    QString result;

    if (pinyin.contains("\'"))
    {
        bool endfgf = pinyin.endsWith("\'");
        QStringList children = pinyin.split("\'", QString::SkipEmptyParts);
        for (int i = 0; i < children.size(); ++i)
        {
            int cur_num = 0;
            if (!result.isEmpty() && !result.endsWith("%\'")) // 每次进入一定的新的字的拼音
            {
                result += "%\'";
            }
            result += splitePinyin(children.at(i), cur_num);
            num += cur_num;
        }
        if (endfgf)
        {
            result += "\'";
        }
        return result;
    }
    int cur_index = 0;
    while (cur_index < pinyin.size())
    {
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

            const QStringList &yunmus = getYunMuByShengMu(pinyin.at(cur_index));

            // 贪心查找 （尽可能长的找到满足的）
            while ( (ym + cur_index) < pinyin.size() )
            {
                bool find = false;
                for (int i = 0; i < yunmus.size(); ++i)
                {
                    QString c_py = yunmus.at(i);
                    if (c_py.startsWith(pinyin.mid(cur_index + 1 + h, ym - h)))
                    {
                        find = true;
                    }
                }
                if (!find)
                {
                    if (ym > h)
                    {
                        ym--;
                    }
                    break;
                }
                ym++;
            }

            if (!result.isEmpty() && !result.endsWith("%\'"))
            {
                result += "%\'";
            }
            result += pinyin.mid(cur_index, ym + 1);
            cur_index += ym;
        }
        else
        {
            if ( result.size() > 1 &&
                    (result.endsWith("g") // 如果是特殊的几个韵母结束的，到这里应该截取下来，重新匹配
                    || result.endsWith("n")
                    || result.endsWith("r")) )
            {
                int last_index = result.lastIndexOf("%\'");
                QString last;
                if (last_index != -1)
                {
                    last = result.mid(last_index + 2);
                }
                else
                {
                    last = result;
                }
                last.remove(last.size() - 1, 1);
                if (!last.isEmpty() && getYunMuByShengMu(last.at(0)).contains(last.mid(1))) // 判断截取之后是否是可以组合的拼音
                {
                    result.remove(result.size() - 1, 1);
                    cur_index -= 1;
                    continue;
                }
            }

            if (yunmuA.contains(pinyin.at(cur_index)))
            {
                int ym = 0;
                while ((ym + cur_index) < pinyin.size())
                {
                    bool find = false;
                    for (int i = 0; i < yunmuA.size(); ++i)
                    {
                        QString c_py = yunmuA.at(i);
                        if (c_py.startsWith(pinyin.mid(cur_index, ym + 1)))
                        {
                            find = true;
                        }
                    }
                    if (find)
                    {
                        ym++;
                    }
                    else
                    {
                        break;
                    }
                }

                if (!result.isEmpty() && !result.endsWith("%\'"))
                {
                    result += "%\'";
                }
                result += pinyin.mid(cur_index, ym);
                cur_index += ym - 1;
            }
            else
            {
                if (!result.isEmpty() && !result.endsWith("%\'"))
                {
                    result += "%\'";
                }
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
    if (keyword.trimmed().isEmpty()) // 如果传入的词为空了，代表删完了，应该关闭输入窗口
    {
        close();
        return;
    }

    QList<XYTranslateItem *> list;
    QString splitePY;
    if (mbEnglish)
    {
        splitePY = keyword;
        list = findItemsFromTemp(splitePY, false);
        if (list.isEmpty())
        {
            list = XYDB->findData(splitePY + "%", "", "userEnglishTable");
            list += XYDB->findData(splitePY + "%", "", "englishTable");
            deDuplication(list, true);
        }
    }
    else
    {
        int num = 0;
        splitePY = splitePinyin(keyword, num);
        mopLineEdit->setText(splitePY.replace("%\'", "\'"));
        splitePY.replace("\'", "%\'");
        list = findPossibleMust(splitePY);

        XYTranslateItem *autoTranslate = autoCreateWords(splitePY); // 智能造句
        if (autoTranslate)
        {
            list.prepend(autoTranslate);
        }
    }

    msCurrentKeyWords = splitePY;
    mopTransLateView->setData(list, false);
    load();
}

void XYInput::completeInput(const QString &text, XYTranslateItem *item)
{
    if (!text.isEmpty()) // 如果为空直接退出
    {
        if (!mbEnglish && item)
        {
            if (!moCompleteItem.msComplete.isEmpty())
            {
                moCompleteItem.msComplete += "\'";
            }
            moCompleteItem.msComplete += item->msComplete;
            moCompleteItem.msTranslate += item->msTranslate;
            QStringList all = msCurrentKeyWords.split("%\'", QString::SkipEmptyParts);
            int com = moCompleteItem.msComplete.split("\'", QString::SkipEmptyParts).size();
            int remain = all.size() - com;
            if (remain > 0)
            {
                QString key;
                for (int i = com; i < all.size(); ++i)
                {
                    if (!key.isEmpty())
                    {
                        key += "%\'";
                    }
                    key += all.at(i);
                }
                mopTransLateView->setData(findPossibleMust(key), false);
                mopLineEdit->setText(moCompleteItem.msTranslate + key.replace("%", ""));
                load();
                saveItem(item);
                return;
            }
            else
            {
                if (moCompleteItem.msTranslate == item->msTranslate)
                {
                    moCompleteItem.miTimes = item->miTimes + 1;
                }
                else
                {
                    saveItem(item);
                }

                moCompleteItem.msExtra = QString::number(moCompleteItem.msTranslate.size());
                XYDB->insertData(&moCompleteItem, "userPinyin");

                emit send_commit(moCompleteItem.msTranslate);
            }
        }
        else
        {
            saveItem(item);
            if (mbEnglish && item == NULL)
            {
                XYTranslateItem *temp = new XYTranslateItem;
                temp->msComplete = text;
                XYDB->insertData(temp, "userEnglishTable");
                delete temp;
            }
            emit send_commit(text);
        }
    }
    close();
}

void XYInput::setEnglish(bool english)
{
    mbEnglish = english;
    if (mbEnglish)
    {
        QString s_cur = mopLineEdit->text(); // 如果是英语查询，去掉分隔符
        s_cur.replace("\'", "");
        s_cur.replace("%", "");
        mopLineEdit->setText(s_cur);
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
    mopTransLateView->clear(false);
    mopTransLateView->update(); // 清理view,避免显示的时候刷新
    mopTransLateView->close();
    moCompleteItem.clear();
    moAutoCompleteItem.clear();
    mopLineEdit->clear();
    msCurrentKeyWords.clear();
    clearTemp();
    return XYBorderShadowWidget::close();
}

void XYInput::show()
{
    QWidget::show();

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
}

void XYInput::keyEvent(QKeyEvent *keyEvent)
{
    qApp->sendEvent(mopLineEdit, keyEvent);
}

void XYInput::load()
{
    if (mopTransLateView->itemCount() == 0) // 当没有找到词组，直接关闭退出
    {
        mopTransLateView->close();
        return;
    }

    if (isVisible())
    {
        mopTransLateView->repaint();
        mopTransLateView->show();
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
}

void XYInput::deDuplication(QList<XYTranslateItem *> &items, bool del)
{
    QList<XYTranslateItem *> temp;
    for (int i = 0; i < items.size(); ++i)
    {
        XYTranslateItem *item = items.at(i);
        bool find = false;
        for (int j = 0; j < temp.size(); ++j)
        {
            if (mbEnglish)
            {
                if (item->msComplete == temp.at(j)->msComplete)
                {
                    find = true;
                    if (del)
                    {
                        delete item;
                    }
                    break;
                }
            }
            else
            {
                if (item->msTranslate == temp.at(j)->msTranslate)
                {
                    find = true;
                    if (del)
                    {
                        delete item;
                    }
                    break;
                }
            }
        }
        if (!find)
        {
            temp.append(item);
        }
    }
    items = temp;
}

XYTranslateItem *XYInput::autoCreateWords(const QString &keyword)
{
    moAutoCompleteItem.clear();
    QString exists = keyword;
    auto it = mmapTempItems.find(exists);
    while(it == mmapTempItems.end() || it.value().isEmpty())
    {
        if (exists.contains("%\'"))
        {
            exists = exists.mid(0, exists.lastIndexOf("%\'"));
        }
        else
        {
            break;
        }

        it = mmapTempItems.find(exists);
    };

    if (exists == keyword || it == mmapTempItems.end() || it.value().isEmpty())
    {
        return NULL;
    }

    XYTranslateItem *comAll = &moAutoCompleteItem;
    comAll->msComplete = it.value().at(0)->msComplete;
    comAll->msTranslate = it.value().at(0)->msTranslate;
    QString keys = keyword.mid(exists.size());
    if (keys.startsWith("%\'"))
    {
        keys.remove(0, 2);
    }

    int k_nums = keys.split("%\'", QString::SkipEmptyParts).size();
    int f_nums = 0;
    while (k_nums > f_nums)
    {
        for (int i = 0; i < f_nums; ++i)
        {
            if (!keys.contains("%\'"))
            {
                keys.clear();
            }
            else
            {
                keys = keys.mid(keys.indexOf("%\'") + 2);
            }
        }
        if (keys.isEmpty())
        {
            break;
        }

        QList<XYTranslateItem *> list = findPossibleMust(keys);
        if (!list.isEmpty())
        {
            comAll->msComplete += "\'";
            comAll->msComplete += list.at(0)->msComplete;

            comAll->msTranslate += list.at(0)->msTranslate;

            f_nums = list.at(0)->msComplete.split("\'").size();
        }
        else
        {
            break;
        }
    };

    return &moAutoCompleteItem;
}

QList<XYTranslateItem *> XYInput::findItemsFromTemp(const QString &keyword, bool force)
{
    QList<XYTranslateItem *> list;
    if (force || mmapTempItems.find(keyword) != mmapTempItems.end())
    {
        QString delsuf = keyword.mid(0, keyword.lastIndexOf("%"));
        auto it = mmapTempItems.begin();
        while (mmapTempItems.end() != it)
        {
            QString last_key = it.key();
            if (delsuf.startsWith(last_key))
            {
                list = it.value() + list;
            }
            it++;
        }
    }

    return list;
}

QList<XYTranslateItem *> XYInput::findPossibleMust(const QString &keyword, int max)
{
    QStringList words = keyword.split("%\'");
    QList<XYTranslateItem *> results;
    QString key;
    for (int i = 0; i < words.size(); ++i)
    {
        if (!key.isEmpty())
        {
            key += "%\'";
        }
        key += words.at(i);

        QList<XYTranslateItem *> list;
        auto it = mmapTempItems.find(key);
        bool find = false;
        if (it != mmapTempItems.end())
        {
            find = true;
            list = it.value();
        }

        if (!find)
        {
            bool haveFind = false;
            list = XYDB->findData(key + "%", QString::number(i + 1), "userPinyin", &haveFind, max);
            if (i == 0)
            {
                QList<XYTranslateItem *> single = XYDB->findData(key + "%", "", "singlePinyin", &haveFind, max);

                for (int i = 0; i < single.size(); ++i)
                {
                    XYTranslateItem *singleItem = single.at(i);
                    QStringList singles = singleItem->msTranslate.split(" ", QString::SkipEmptyParts);
                    for (int j = 0; j < singles.size(); ++j)
                    {
                        if (list.size() > 200)
                        {
                            break;
                        }
                        list.append(new XYTranslateItem("singlePinyin", singles.at(j), singleItem->msComplete));
                    }
                }
                qDeleteAll(single);
            }
            list += XYDB->findData(key + "%", QString::number(i + 1), "basePinyin", &haveFind, max);

            deDuplication(list, true);
            if (haveFind)
            {
                mmapTempItems.insert(key, list);
            }
        }
        if (!list.isEmpty())
        {
            results = list + results;
        }
    }
    return results;
}

void XYInput::clearTemp()
{
    auto it = mmapTempItems.begin();
    while (mmapTempItems.end() != it)
    {
        qDeleteAll(it.value());
        it++;
    }
    mmapTempItems.clear();
}

void XYInput::saveItem(XYTranslateItem *item)
{
    if (item && !item->msComplete.isEmpty()) // 保存用户词库
    {
        item->miTimes += 1;
        if (item->msSource.toLower().contains("english"))
        {
            XYDB->insertData(item, "userEnglishTable");
        }
        else
        {
            item->msExtra = QString::number(item->msTranslate.size());
            XYDB->insertData(item, "userPinyin");
        }
    }
}

const QStringList &XYInput::getYunMuByShengMu(const QChar &shenmu)
{
    static QMap<QChar, QStringList> Allyunmus;
    auto it = Allyunmus.find(shenmu);
    if ( it != Allyunmus.end()) {
        return it.value();
    }

    QStringList yunmu;
    switch (shenmu.toLatin1()) // 单独为每一个声母指定可匹配的韵母
    {
    case 'b':
        yunmu = QString("a o ai ei ao an en ang eng i ie iao ian in ing u").split(" ", QString::SkipEmptyParts);
        break;
    case 'p':
        yunmu = QString("a o ai ei ao ou an en ang eng i ie iao ian in ing u").split(" ", QString::SkipEmptyParts);
        break;
    case 'm':
        yunmu = QString("a e o ai ei ao an en ang eng i ie iao iu ian in ing u").split(" ", QString::SkipEmptyParts);
        break;
    case 'f':
        yunmu = QString("a o ei ou an en ang eng u").split(" ", QString::SkipEmptyParts);
        break;
    case 'd':
        yunmu = QString("a o ai ei ao ou an en ang eng i ia ie iao ian iu ian ing u uo ui uan un ong").split(" ", QString::SkipEmptyParts);
        break;
    case 't':
        yunmu = QString("a e ai ei ao ou an ang eng i ie iao ian ing u uo ui uan un ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'n':
    case 'l':
        yunmu = QString("a e o ai ei ao ou an en ang eng i ia ie iao ian iu ian in ing u uo ui uan un ong v ve").split(" ", QString::SkipEmptyParts);
        break;
    case 'g':
    case 'k':
    case 'h':
        yunmu = QString("a e ai ei ao ou an en ang eng u ua uo uai ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'j':
    case 'q':
    case 'x':
        yunmu = QString("i ia ie iao iu ian in iang ing v u ue ve van uan un vn iong").split(" ", QString::SkipEmptyParts);
        break;
    case 'z':
        yunmu = QString("a e i ai ei ao ou an en ang eng u ua uo uai ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'c':
        yunmu = QString("a e i ai ao ou an en ang eng u ua uo uai ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 's':
        yunmu = QString("a e i ai ei ao ou an en ang eng u ua uo uai ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'r':
        yunmu = QString("e i ao ou an en ang eng u ua uo ui uan un uang ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'y':
        yunmu = QString("a e i o ao ou an in ang ing u ue uan un ong").split(" ", QString::SkipEmptyParts);
        break;
    case 'w':
        yunmu = QString("a o ai ei an en ang eng u").split(" ", QString::SkipEmptyParts);
        break;
    default:
        break;
    }

    it = Allyunmus.insert(shenmu, yunmu);
    return it.value();
}

