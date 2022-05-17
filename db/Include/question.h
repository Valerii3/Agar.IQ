#ifndef QUESTION_H
#define QUESTION_H

#include "answers.h"

class Question {
    std::string question;
    int correct_answer;

public:
    Question(int bits, int operand_counts, std::string operands);

    std::string get_question() const;

   int get_answer();
};

#endif // QUESTION_H
