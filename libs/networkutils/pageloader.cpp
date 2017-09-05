#include "pageloader.h"

#include <QDebug>

PageLoader::PageLoader(QObject *parent) :
    QObject(parent),
    m_manager(new QNetworkAccessManager(this)),
    m_sslConfiguration(QSslConfiguration::defaultConfiguration())
{
    // to use SSL sockets
    // put OpenSSL binaries
    // in OS environment
    m_sslConfiguration.setProtocol(QSsl::AnyProtocol);

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

    QNetworkRequest request;
    request.setSslConfiguration(m_sslConfiguration);
    request.setRawHeader( "User-Agent" , "PageLoader" );
    request.setUrl(QUrl(m_url));

    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), m_reply, SLOT(ignoreSslErrors()));
}
/**
 * @brief PageLoader::replyFinished
 * @param reply
 */
void PageLoader::replyFinished(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError)
    {
        QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

        if(redirect.isValid() && reply->url() != redirect)
        {
            qDebug() << Q_FUNC_INFO << "redirect" << redirect;
            if(redirect.isRelative())
                redirect = reply->url().resolved(redirect);
            QNetworkRequest req(redirect);
            req.setRawHeader( "User-Agent" , "PageLoader" );
            /*QNetworkReply* reply =*/ m_manager->get(req);
            return;
        }
        QByteArray data(reply->readAll());
//        qDebug() << Q_FUNC_INFO << data;

        emit pageLoaded(data);
    }
    else//handling errors
    {
//        qDebug() << Q_FUNC_INFO << reply->errorString();
        emit errorLoadingUrl(reply->url().toString(), reply->errorString());
    }
}
