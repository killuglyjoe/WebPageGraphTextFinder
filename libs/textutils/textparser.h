#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include <QObject>

class TextParser : public QObject
{
    Q_OBJECT
public:
    explicit TextParser(QObject *parent = 0);
    ~TextParser();

signals:
    void parsedUrls(const QStringList &);
    void matchresults(const int &);

public slots:
    void setParseData(const QByteArray &data);

    void parseUrls();
    QStringList findText(const QString &searchText);

private:
    QByteArray  m_data;
};

#endif // TEXTPARSER_H
