#include "webpagetextthreadprocessor.h"

WebPageTextThreadProcessor::WebPageTextThreadProcessor(const QStringList &urlList,
                                                       const QString &searchText) :
    QObject(),
    m_urlsList(urlList),
    m_searchPattern(searchText)
{
    m_thread = QSharedPointer<QThread>(new QThread);
    m_thread->start();
    connect(m_thread.data(), &QThread::started, this, &WebPageTextThreadProcessor::threadSatrtInit);
}
/**
 * @brief Run all in own thread
 */
void WebPageTextThreadProcessor::threadSatrtInit()
{
    m_loader = QSharedPointer<PageLoader>(new PageLoader);
    m_parser = QSharedPointer<TextParser>(new TextParser);

    connect(m_loader.data(), &PageLoader::errorLoadingUrl,
            this, &WebPageTextThreadProcessor::errorLoadingUrl);

    connect(m_loader.data(), &PageLoader::pageLoaded,
            m_parser.data(), &TextParser::setData);

    connect(m_parser.data(), &TextParser::onParsedPosList,
            this, &WebPageTextThreadProcessor::processParsedPosList);

//    qDebug() << Q_FUNC_INFO << m_searchPattern << m_urlsList;

    m_parser->setRxPattern(m_searchPattern);

//    foreach (QString url, m_urlsList) {
        m_url = m_urlsList[0];
        m_loader->loadUrl(m_url);
        m_urlsList.removeFirst();
//    }
}

WebPageTextThreadProcessor::~WebPageTextThreadProcessor()
{
    m_thread->quit();
    m_thread->wait();
}

QStringList WebPageTextThreadProcessor::urlsList() const
{
    return m_urlsList;
}

void WebPageTextThreadProcessor::setUrlsList(const QStringList &urlsList)
{
    m_urlsList = urlsList;
}

void WebPageTextThreadProcessor::processParsedPosList(const QList<int> &posList)
{
    qDebug() << Q_FUNC_INFO << m_urlsList.count() << m_loader->curURL() << posList.count();

    emit foundMatches(m_loader->curURL(), posList.count());

    if(m_urlsList.count())
    {
        m_url = m_urlsList[0];
        m_loader->loadUrl(m_url);
        m_urlsList.removeFirst();
    }
    else
    {
        emit finished();
    }
}

TextParser *WebPageTextThreadProcessor::parser() const
{
    return m_parser.data();
}

PageLoader *WebPageTextThreadProcessor::loader() const
{
    return m_loader.data();
}

QString WebPageTextThreadProcessor::searchPattern() const
{
    return m_searchPattern;
}

void WebPageTextThreadProcessor::setSearchPattern(const QString &searchPattern)
{
    m_searchPattern = searchPattern;
    m_parser->setRxPattern(m_searchPattern);
}

QString WebPageTextThreadProcessor::url() const
{
    return m_loader->curURL();
}
