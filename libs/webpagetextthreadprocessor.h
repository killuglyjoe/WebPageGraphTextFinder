#ifndef WEBPAGETEXTTHREADPROCESSOR_H
#define WEBPAGETEXTTHREADPROCESSOR_H

#include <QObject>

#include "pageloader.h"
#include "textparser.h"

/**
 * @brief The WebPageTextThreadProcessor class
 * is thread class with its URLs queue to process (find text pattern)
 */
class WebPageTextThreadProcessor : public QObject
{
    Q_OBJECT
public:
    explicit WebPageTextThreadProcessor(const QStringList &urlList, const QString &searchText);
    ~WebPageTextThreadProcessor();

    QStringList urlsList() const;
    void setUrlsList(const QStringList &urlsList);

    QString url() const;

    QString searchPattern() const;
    void setSearchPattern(const QString &searchPattern);

    PageLoader* loader() const;

    TextParser* parser() const;

signals:
    void finished();
    void errorLoadingUrl(const QString &url, const QString &error);
    void foundMatches(const QString &url, const int &matchesCount);

protected slots:
    void threadSatrtInit();

private slots:
    void processParsedPosList(const QList<int> &posList);

private:
    QSharedPointer<QThread>     m_thread;
    QStringList                 m_urlsList;

    QSharedPointer<PageLoader>  m_loader;
    QSharedPointer<TextParser>  m_parser;

    QString                     m_url;
    QString                     m_searchPattern;
};

#endif // WEBPAGETEXTTHREADPROCESSOR_H
