#include "basetextparser.h"

BaseTextParser::BaseTextParser(QObject *parent) :
    QObject(parent),
    m_totalMatches(0),
    m_rxPattern("")
{
    qRegisterMetaType<QList<int>>("QList<int>");

    if(!parent)//without parent, must have own thread (pretty ugly)
    {
        m_thread = QSharedPointer<QThread>(new QThread);
        m_thread->start();
        connect(m_thread.data(), &QThread::started, this, &BaseTextParser::moveDerived);
    }
}
/**
 * @brief Move all derived classes to base own thread
 */
void BaseTextParser::moveDerived()
{
    if(m_thread)
        moveToBaseThread(m_thread.data());
}
/**
 * @brief Total count of found
 * @return
 */
int BaseTextParser::totalMatches() const
{
    return m_totalMatches;
}

void BaseTextParser::setTotalMatches(int totalMatches)
{
    m_totalMatches = totalMatches;
}

BaseTextParser::~BaseTextParser()
{
    m_thread->quit();
    m_thread->wait();
}
/**
 * @brief Search pattern
 * @return
 */
QString BaseTextParser::rxPattern() const
{
    return m_rxPattern;
}

void BaseTextParser::setRxPattern(const QString &rxPattern)
{
    m_rxPattern = rxPattern;
}

QByteArray BaseTextParser::data() const
{
    return m_data;
}

void BaseTextParser::setData(const QByteArray &data)
{
    m_data = data;
//    qDebug() << Q_FUNC_INFO << data;

    //Got some data, must immediately processing???
    if(!m_data.isEmpty())
    {
        if(!rxPattern().isEmpty())
        {
//            qDebug() << Q_FUNC_INFO << "Begin parse";
            parse();
        }
        else
            qDebug() << Q_FUNC_INFO << "pattern is empty";
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "data is empty";
    }
}
