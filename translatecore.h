#ifndef TRANSLATECORE_H
#define TRANSLATECORE_H

#include <QNetworkReply>
#include <QObject>
#include <random>
#include <languageconstants.h>

class TranslateCore : public QObject
{
    Q_OBJECT
public:
    TranslateCore(QString _from=LanguageConstants::defaultSourceCode, QString _to=LanguageConstants::defaultTargetCode);

    const static QString appId;
    const static QString secretKey;

    QString getFrom() const;
    void setFrom(const QString &value);

    QString getTo() const;
    void setTo(const QString &value);

    virtual void translate(QString)=0;

protected:
    static std::random_device rd;
    static std::default_random_engine rng;
    static std::uniform_int_distribution<int> myRand;
    QNetworkAccessManager *naManager;

    QString from,to;
protected slots:
    virtual void callback(QNetworkReply *reply);
};

#endif // TRANSLATECORE_H
