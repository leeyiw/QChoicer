#ifndef QUESTION_H
#define QUESTION_H

#include <QList>
#include <QSettings>
#include <QString>
#include <QStringList>

class Question
{
public:
    Question(const QSettings &settings);
    ~Question();

    QString question;
    QStringList options;
    int answer;
    int selection;
};

#endif // QUESTION_H
