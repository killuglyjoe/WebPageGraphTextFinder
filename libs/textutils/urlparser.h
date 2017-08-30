#ifndef URLPARSER_H
#define URLPARSER_H

#include "basetextparser.h"
/**
 * @brief The UrlParser class helper
 * that searchs only hyperlinks in text
 * and return a list of links
 */
class UrlParser : public BaseTextParser
{
public:
    explicit UrlParser(QObject *parent = 0);
    ~UrlParser();

public:
    void parse();

protected:
    void moveToBaseThread(QThread *baseThread);
};

#endif // URLPARSER_H
