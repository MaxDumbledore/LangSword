#ifndef LANGUAGEMAP_H
#define LANGUAGEMAP_H

#include <QMap>
#include <QObject>

class LanguageConstants: public QObject
{
    //Q_OBJECT
public:
    static const QString defaultSourceLanguage,defaultTargetLanguage,langAuto;
    static const QMap<QString,QString> & getLanguageMap();
    static const QList<QString> & getSourceLanguages();
    static const QList<QString> & getTargetLanguages();
private:
    static QMap<QString,QString> languageToCode;
    static QList<QString> sourceLanguages;
    static QList<QString> targetLanguages;
};

#endif // LANGUAGEMAP_H
