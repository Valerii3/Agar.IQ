#ifndef QUESTION_H
#define QUESTION_H

#include "answers.h"

class Question {
    std::string question;
    int correct_answer;

public:
    Question(int bits, int operandCounts, std::string operands);

    std::string getQuestion() const;

   int getAnswer();
};

#endif // QUESTION_H
