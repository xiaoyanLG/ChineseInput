#include "xytranslateitem.h"

XYTranslateItem::XYTranslateItem(const QString &source,
                                 const QString &translate,
                                 const QString &complete,
                                 const QString &extra,
                                 int   times,
                                 bool stick)
    : msSource(source),
      msTranslate(translate),
      msComplete(complete),
      msExtra(extra),
      miTimes(times),
      mbStick(stick)
{

}

XYTranslateItem::~XYTranslateItem()
{

}

