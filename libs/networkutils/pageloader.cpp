#include "pageloader.h"

#include <QDebug>

PageLoader::PageLoader(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this))
{;
    connect(m_manager, &QNetworkAccessManager::finished,
            this, &PageLoader::replyFinished);
}

PageLoader::~PageLoader()
{
}

QString PageLoader::curURL() const
{
    return m_url;
}
/**
 * @brief PageLoader::loadUrl
 * @param url
 */
void PageLoader::loadUrl(const QString &url)
{
    if(url.isEmpty()) return;
    m_url = url;
    m_manager->get(QNetworkRequest(QUrl(m_url)));
}
/**
 * @brief PageLoader::replyFinished
 * @param reply
 */
void PageLoader::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data(reply->readAll());
        qDebug() << Q_FUNC_INFO << data;

        emit pageLoaded(data);
    }
    else//handling errors
    {
        emit errorLoadingUrl(reply->url().toString(), reply->errorString());
    }
}
