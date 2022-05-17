#include "../Include/question.h"

Question::Question(int bits, int operand_counts, std::string operands) {
    int p = pow(10, bits);
    int ans = 0;
    int a = rand() % p;
    question = std::to_string(a) + ' ';
    int prevOp = 1;
    for (int i = 0; i < operand_counts; i++) {
        int op = rand() % operands.size();
        int b = rand() % p;
        question = question + operands[op] + ' ';
        question.append(std::to_string(b) + ' ');
        if (operands[op] == '+') {
            ans += prevOp * a;
            a = b;
            prevOp = 1;
        } else if (operands[op] == '-') {
            ans += prevOp * a;
            a = b;
            prevOp = -1;
        } else if (operands[op] == '*') {
            a *= b;
        }
    }
    ans += prevOp * a;
    correct_answer = ans;
    question += "= ";
}

std::string Question::get_question() const {
    return question;
}

int Question::get_answer() {
    return correct_answer;
}

