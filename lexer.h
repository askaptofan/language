#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <stack>
#include <cmath>

using namespace std;

// Токены
enum TokenType {
    INVALID,
    NUMBER,
    VARIABLE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,
    SQRT,
    ASSIGN,
    LPAREN,
    RPAREN,
    END
};

// Структура токена
struct Token {
    TokenType type;  // тип токена
    double value;    // значение токена
    string variable; // название переменной (если это переменная)
};

// Лексический анализатор
class Lexer {
public:
    Lexer(string str) : input(str), pos(0) {}

    // получить следующий токен
    Token getNextToken() {
        skipWhiteSpace();

        if (pos >= input.size()) {
            return Token{ END, 0.0, "" };
        }

        if (isdigit(input[pos])) {
            return getNumber();
        }

        if (isalpha(input[pos]) || input[pos] == '_') {
            return getVariable();
        }

        switch (input[pos]) {
        case '+':
            ++pos;
            return Token{ PLUS, 0.0, "" };
        case '-':
            ++pos;
            return Token{ MINUS, 0.0, "" };
        case '*':
            ++pos;
            return Token{ MULTIPLY, 0.0, "" };
        case '/':
            ++pos;
            return Token{ DIVIDE, 0.0, "" };
        case '^':
            ++pos;
            return Token{ POWER, 0.0, "" };
        case '=':
            ++pos;
            return Token{ ASSIGN, 0.0, "" };
        case '(':
            ++pos;
            return Token{ LPAREN, 0.0, "" };
        case ')':
            ++pos;
            return Token{ RPAREN, 0.0, "" };
        case '\0':
            ++pos;
            return Token(END);
        default:
            ++pos;
            return Token{ INVALID, 0.0, "" };
        }
    }

private:
    string input;
    size_t pos;

    // получить число
    Token getNumber() {
        double number = 0;
        int decimalPlaces = 0;

        while (pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) {
            if (input[pos] == '.') {
                decimalPlaces = 1;
            }
            else {
                number *= 10;
                number += input[pos] - '0';
                decimalPlaces *= 10;
            }

            ++pos;
        }

        return Token{ NUMBER, number / decimalPlaces, "" };
    }

    // получить переменную
    Token getVariable() {
        string variable = "";

        while (pos < input.size() && (isalnum(input[pos]) || input[pos] == '_')) {
            variable += input[pos];
            ++pos;
        }

        return Token{ VARIABLE, 0.0, variable };
    }

    // пропустить пробельные символы
    void skipWhiteSpace() {
        while (pos < input.size() && isspace(input[pos])) {
            ++pos;
        }
    }
};
