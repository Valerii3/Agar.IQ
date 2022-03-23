#include "question.h"

Question::Question(int bits, int operandCounts, std::string operands) {
    int p = pow(10, bits);
    int ans = rand() % p;
    question = std::to_string(ans) + ' ';
    for (int i = 0; i < operandCounts; i++) {
        int op = rand() % operands.size();
        int num = rand() % p;
        if (operands[op] == '+') {
            ans += num;
        } else if (operands[op] == '-') {
            ans -= num;
        } else if (operands[op] == '*') {
            ans *= num;
        }
        question = question + operands[op] + ' ';
        question.append(std::to_string(num) + ' ');
    }
    answer = Answer(ans);
    question += "= ";
}

std::string Question::getQuestion() const {
    return question;
}

Answer *Question::getAnswer() {
    return &answer;
}

