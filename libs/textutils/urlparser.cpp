#include "urlparser.h"

/// pattern taken from https://stackoverflow.com/questions/6038061/regular-expression-to-find-urls-within-a-string
static const QString patternUrl("(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?");

UrlParser::UrlParser(QObject *parent) :
    BaseTextParser(parent)

{
    setRxPattern(patternUrl);
}

UrlParser::~UrlParser()
{
}
/**
 * @brief UrlParser::parse impl
 */
void UrlParser::parse()
{
    QRegExp urlExp(rxPattern());

    QStringList listURLs;
    int pos(0);

    while((pos = urlExp.indexIn(data(), pos)) != -1)
    {
        QString matchUrl(urlExp.cap(0));
        if(!listURLs.contains(matchUrl))//not allowed repeated URLs
        {
            listURLs << matchUrl;
//            qDebug() << Q_FUNC_INFO << matchUrl;
        }
        pos += urlExp.matchedLength();
    }
//    qDebug() << Q_FUNC_INFO << list;

    if(listURLs.count())
    {
        setTotalMatches(listURLs.count());
        emit onParsedStringList(listURLs);
    }
    else
    {
//        emit onNothingFound();
    }
}

void UrlParser::moveToBaseThread(QThread *baseThread)
{
    moveToThread(baseThread);
//    qDebug() << Q_FUNC_INFO << thread();
}
