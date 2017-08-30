#ifndef BASETEXTPARSER_H
#define BASETEXTPARSER_H

#include <QRegExp>
#include <QThread>
#include <QObject>
#include <QDebug>

/**
 * @brief The BaseTextParser class
 * is the base for any text search routines
 */
class BaseTextParser : public QObject
{
    Q_OBJECT
public:
    explicit BaseTextParser(QObject *parent = 0);
    virtual ~BaseTextParser();

    virtual void parse()=0;

    QString rxPattern() const;
    void setRxPattern(const QString &rxPattern);

    QByteArray data() const;

    int totalMatches() const;
    void setTotalMatches(int totalMatches);

public slots:
    void setData(const QByteArray &data);

signals:
    void onParsedStringList(const QStringList &matchList);
    void onParsedPosList(const QList<int> &posList);

public slots:

protected:
    virtual void moveToBaseThread(QThread*)=0;

private slots:
    void moveDerived();

private:
    int                     m_totalMatches;
    QByteArray              m_data;
    QString                 m_rxPattern;
    QSharedPointer<QThread> m_thread;
};

#endif // BASETEXTPARSER_H
