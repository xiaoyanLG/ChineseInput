#include "xytranslateview.h"
#include <QApplication>
#include <QPainter>

XYTranslateView::XYTranslateView(QWidget *parent)
    : XYBorderShadowWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::Tool
                   | Qt::WindowDoesNotAcceptFocus);
    this->setAttribute(Qt::WA_TranslucentBackground);

    mopModel = new XYTranslateModel;
    moFont = qApp->font();
    miCurrentPage = 0;
    miMaxVisibleItem = 7;
    showType = XYTranslateModel::TRANSLATE;
    resize(50, 50);
}

XYTranslateView::~XYTranslateView()
{
    delete mopModel;
}

void XYTranslateView::appendData(const QList<XYTranslateItem *> &list)
{
    mopModel->appendData(list);
}

void XYTranslateView::prependData(const QList<XYTranslateItem *> &list)
{
    mopModel->prependData(list);
}

void XYTranslateView::setData(const QList<XYTranslateItem *> &list)
{
    mopModel->setData(list);
}

void XYTranslateView::clear()
{
    mopModel->clear();
}

void XYTranslateView::setFont(const QFont &font)
{
    moFont = font;
}

void XYTranslateView::nextPage()
{
    double allPages = mopModel->counts() * 1.0 / miMaxVisibleItem;
    if (allPages > miCurrentPage + 1)
    {
        miCurrentPage++;
    }
}

void XYTranslateView::prePage()
{
    if (miCurrentPage - 1 >= 0)
    {
        miCurrentPage--;
    }
}

int XYTranslateView::itemCount()
{
    return mopModel->counts();
}

QString XYTranslateView::getData(int index)
{
    return mopModel->data(miCurrentPage * miMaxVisibleItem + index - 1, showType);
}

void XYTranslateView::show()
{
    repaint();
    QWidget::show();
}

void XYTranslateView::paintEvent(QPaintEvent *event)
{
    XYBorderShadowWidget::paintEvent(event);
    QPainter painter(this);
    painter.setFont(moFont);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QFontMetrics metrics(moFont);
    int i;
    int max_width = 0;
    for (i = 0; i < miMaxVisibleItem; ++i)
    {
        int index = i + miCurrentPage * miMaxVisibleItem;
        if (mopModel->haveData(index))
        {
            QString data = QString("%1.%2").arg(i + 1).arg(mopModel->data(index, showType));
            painter.drawText(15, 25 + i * (metrics.height() + 5), data);
            max_width = qMax(metrics.width(data), max_width);
        }
        else
        {
            break;
        }
    }

    resize(qMax(max_width + 30, 200), 25 + i * (metrics.height() + 5));
}

