#include "textparser.h"

TextParser::TextParser(QObject *parent) :
    BaseTextParser(parent)
{
}

TextParser::~TextParser()
{
}
/**
 * @brief TextParser::parse impl
 */
void TextParser::parse()
{
   QRegExp urlExp(rxPattern());

    QList<int> posList;
    int pos(0);

    while((pos = urlExp.indexIn(data(), pos)) != -1)
    {
        if(!posList.contains(pos))//not allowed repet URLs
        {
            posList << pos;
//            qDebug() << Q_FUNC_INFO << pos;
        }
        pos += urlExp.matchedLength();
    }
//    qDebug() << Q_FUNC_INFO << posList << rxPattern();

    if(posList.count())
    {
        setTotalMatches(posList.count());
        emit onParsedPosList(posList);
    }
    else
    {
        emit onNothingFound();
    }
}

void TextParser::moveToBaseThread(QThread *baseThread)
{
    moveToThread(baseThread);

}
