#include "Question.h"

Question::Question(const QSettings &settings)
    : selection(-1)
{
    question = settings.value("question").toString();
    options = settings.value("options").toStringList();
    answer = settings.value("answer").toInt();
}

Question::~Question()
{

}

