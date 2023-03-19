
#include "lexer.h"
#include "parser.h"
// ������������� ����������
class SemanticAnalyzer {
public:
    // ��������� ������ � ����������
    explicit SemanticAnalyzer(string str) : lexer(str), currentToken(lexer.getNextToken()) {}

    // ��������� ������������� ������ � ��������� �������� ���������
    double evaluate() {
        double result = expression();
        endToken();
        return result;
    }

    // ��������� �������� ����������
    void assign(string varName, double value) {
        variables[varName] = value;
    }
    void endToken() {
        eat(END);
    }


private:
    Lexer lexer;
    Token currentToken;

    // ��������� �� ������
    void error() {
        throw runtime_error("Invalid syntax");
    }

    // ���������, ��� ������� ����� ����� ��������� ���, � ������� � ���������� ������
    void eat(TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        }
        else {
            error();
        }
    }

    // ���������� �����
    double number() {
        double result = currentToken.value;
        eat(NUMBER);
        return result;
    }

    // ���������� ����������
    double variable() {
        double result = 0.0;
        string varName = currentToken.variable;
        eat(VARIABLE);

        // ���������, ��������� �� ����������
        if (variables.count(varName) == 0) {
            error();
        }

        // �������� �������� ����������
        result = variables[varName];
        return result;
    }

    // ���������� ��������� � �������
    double parenExpr() {
        eat(LPAREN);
        double result = expression();
        eat(RPAREN);
        return result;
    }

    // ���������� ������� sqrt
    double sqrtExpr() {
        eat(SQRT);
        eat(LPAREN);
        double result = expression();
        eat(RPAREN);
        return sqrt(result);
    }

    // ���������� ������
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
    // ���������� �������� � ���������
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

                // ��������� ������� �� 0
                if (divisor == 0) {
                    throw runtime_error("Division by zero");
                }

                result /= divisor;
            }
        }

        return result;
    }

    // ���������� ���������� � �������
    double power() {
        double result = term();

        while (currentToken.type == POWER) {
            eat(POWER);
            result = pow(result, term());
        }

        return result;
    }

    // ���������� �������� � ���������
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

        // ������������ ����� END
        endToken();

        return result;

    // ������� ��� �������� ���������� � �� ��������
    map<string, double> variables;
};
