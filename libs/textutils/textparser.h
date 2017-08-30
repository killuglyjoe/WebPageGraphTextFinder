#ifndef TEXTPARSER_H
#define TEXTPARSER_H

#include "basetextparser.h"
/**
 * @brief The TextParser class provides any text search routines
 * which return a list of positions matched text occurances
 */
class TextParser : public BaseTextParser
{
    Q_OBJECT
public:
    explicit TextParser(QObject *parent = 0);
    ~TextParser();

    void parse();

protected:
    void moveToBaseThread(QThread *baseThread);

private:
};

#endif // TEXTPARSER_H
