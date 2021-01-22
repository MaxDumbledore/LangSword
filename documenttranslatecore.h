#ifndef DOCUMENTTRANSLATECORE_H
#define DOCUMENTTRANSLATECORE_H

#include <QFile>
#include <QObject>
#include "translatecore.h"

class DocumentTranslateCore : public TranslateCore
{
    Q_OBJECT
public:
    DocumentTranslateCore(QString _from=LanguageConstants::defaultSourceCode, QString _to=LanguageConstants::defaultTargetCode);
    const static QString prefixUrl;

    void translate(QString) override;

protected slots:
    void callback(QNetworkReply *reply) override;
};

#endif // DOCUMENTTRANSLATECORE_H
