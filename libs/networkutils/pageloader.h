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

signals:
    void pageLoaded(const QByteArray &);
    void errorLoadingUrl(const QString &url, const QString &error);

public slots:
    void loadUrl(const QString &url);

private slots:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
};

#endif // PAGELOADER_H
