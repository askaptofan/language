
#include "lexer.h"
#include "parser.h"
// Семантический анализатор
class SemanticAnalyzer {
public:
    // Принимает строку с выражением
    explicit SemanticAnalyzer(string str) : lexer(str), currentToken(lexer.getNextToken()) {}

    // Выполнить семантический анализ и вычислить значение выражения
    double evaluate() {
        double result = expression();
        endToken();
        return result;
    }

    // Присвоить значение переменной
    void assign(string varName, double value) {
        variables[varName] = value;
    }
    void endToken() {
        eat(END);
    }


private:
    Lexer lexer;
    Token currentToken;

    // Сообщение об ошибке
    void error() {
        throw runtime_error("Invalid syntax");
    }

    // Проверить, что текущий токен имеет ожидаемый тип, и перейти к следующему токену
    void eat(TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        }
        else {
            error();
        }
    }

    // Обработать число
    double number() {
        double result = currentToken.value;
        eat(NUMBER);
        return result;
    }

    // Обработать переменную
    double variable() {
        double result = 0.0;
        string varName = currentToken.variable;
        eat(VARIABLE);

        // Проверяем, объявлена ли переменная
        if (variables.count(varName) == 0) {
            error();
        }

        // Получаем значение переменной
        result = variables[varName];
        return result;
    }

    // Обработать выражение в скобках
    double parenExpr() {
        eat(LPAREN);
        double result = expression();
        eat(RPAREN);
        return result;
    }

    // Обработать функцию sqrt
    double sqrtExpr() {
        eat(SQRT);
        eat(LPAREN);
        double result = expression();
        eat(RPAREN);
        return sqrt(result);
    }

    // Обработать фактор
    double factor() {
        switch (currentToken.type) {
        case NUMBER:
            return number();
        case VARIABLE:
            return variable();
        case LPAREN:
            return parenExpr();
        case SQRT:
            return sqrtExpr();
        default:
            error();
            return 0.0;
        }
    }
    // Обработать сложение и вычитание
    double term() {
        double result = factor();

        while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE) {
            if (currentToken.type == MULTIPLY) {
                eat(MULTIPLY);
                result *= factor();
            }
            else if (currentToken.type == DIVIDE) {
                eat(DIVIDE);
                double divisor = factor();

                // Проверяем деление на 0
                if (divisor == 0) {
                    throw runtime_error("Division by zero");
                }

                result /= divisor;
            }
        }

        return result;
    }

    // Обработать возведение в степень
    double power() {
        double result = term();

        while (currentToken.type == POWER) {
            eat(POWER);
            result = pow(result, term());
        }

        return result;
    }

    // Обработать сложение и вычитание
    double expression() {
        double result = power();
        while (currentToken.type == PLUS || currentToken.type == MINUS) {
            if (currentToken.type == PLUS) {
                eat(PLUS);
                result += power();
            }
            else if (currentToken.type == MINUS) {
                eat(MINUS);
                result -= power();
            }
        }

        // Обрабатываем токен END
        endToken();

        return result;

    // Словарь для хранения переменных и их значений
    map<string, double> variables;
};
