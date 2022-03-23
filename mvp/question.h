#ifndef QUESTION_H
#define QUESTION_H

#include "answers.h"

class Question {
    std::string question;
    Answer answer;

public:
    Question(int bits, int operandCounts, std::string operands);

    std::string getQuestion() const;

    Answer *getAnswer();
};

#endif // QUESTION_H
