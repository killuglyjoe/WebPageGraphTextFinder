#include "textparser.h"

#include <QDebug>
#include <QRegExp>

/// pattern taken from https://stackoverflow.com/questions/6038061/regular-expression-to-find-urls-within-a-string
const QString patternUrl("(http|ftp|https)://([\\w_-]+(?:(?:\\.[\\w_-]+)+))([\\w.,@?^=%&:/~+#-]*[\\w@?^=%&/~+#-])?");

TextParser::TextParser(QObject *parent) :
    QObject(parent)
{

}

TextParser::~TextParser()
{

}
/**
 * @brief TextParser::setParseData
 * @param data
 */
void TextParser::setParseData(const QByteArray &data)
{
    m_data = data;
    parseUrls();
}
/**
 * @brief TextParser::parseUrls
 * @return
 */
void TextParser::parseUrls()
{
    if(m_data.isEmpty()) return;

    QRegExp urlExp(patternUrl);

    QStringList list;
    int pos = 0;

    while ((pos = urlExp.indexIn(m_data, pos)) != -1) {
        QString matchUrl(urlExp.cap(0));
        if(!list.contains(matchUrl))//not allowed repet URLs
        {
            list << matchUrl;
            qDebug() << Q_FUNC_INFO << matchUrl;
        }
        pos += urlExp.matchedLength();
    }
//    qDebug() << Q_FUNC_INFO << list;

    if(list.count())
        emit parsedUrls(list);
}
/**
 * @brief TextParser::findText
 * @param searchText
 * @return
 */
QStringList TextParser::findText(const QString &searchText)
{
    if(m_data.isEmpty() || searchText.isEmpty()) return QStringList();

    return QStringList();
}
