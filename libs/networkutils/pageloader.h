#ifndef PAGELOADER_H
#define PAGELOADER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

class PageLoader : public QObject
{
    Q_OBJECT
public:
    explicit PageLoader(QObject *parent = 0);
    ~PageLoader();

    QString curURL()const;

signals:
    void pageLoaded(const QByteArray &);
    void errorLoadingUrl(const QString &url, const QString &error);

public slots:
    void loadUrl(const QString &url);

private slots:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager   *m_manager;
    QString                 m_url;
    QSslConfiguration       m_sslConfiguration;
    QNetworkReply           *m_reply;
};

#endif // PAGELOADER_H
