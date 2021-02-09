#ifndef COMMONTRANSLATECORE_H
#define COMMONTRANSLATECORE_H

#include <QObject>
#include "translatecore.h"

class CommonTranslateCore : public TranslateCore
{
    Q_OBJECT
public:
    CommonTranslateCore(QString _from=LanguageConstants::defaultSourceCode, QString _to=LanguageConstants::defaultTargetCode);
    const static QString prefixUrl;

    void translate(QString content) override;

public slots:
    void changeSingleLineMode();

protected:
    bool singleLineMode;

protected slots:
    void callback(QNetworkReply *reply) override;

signals:
    void resultReceived(QString result);
};

#endif // COMMONTRANSLATECORE_H
