#ifndef TRANSLATECORE_H
#define TRANSLATECORE_H

//#include <QObject>
#include <QString>
#include <random>

struct CurlMem
{
    char *response;
    size_t size;
};

class TranslateCore
{
public:

    TranslateCore(QString _from="en", QString _to="zh");
    QString translate(QString content);

    QString getFrom() const;
    void setFrom(const QString &value);

    QString getTo() const;
    void setTo(const QString &value);

private:
    const static char prefixURL[],appId[],secretKey[];

    QString from,to;

    std::random_device rd;
    std::default_random_engine rng;
    std::uniform_int_distribution<int> myRand;

    static size_t cb(void *data, size_t size, size_t nmemb, void *userp);
    QString getResultFromJson(QByteArray s);
};

#endif // TRANSLATECORE_H
