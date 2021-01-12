#ifndef TRANSLATECORE_H
#define TRANSLATECORE_H

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
    TranslateCore();
    QString translate(QString content);

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
