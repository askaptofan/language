#include 'AST.h'
#include 'lexer.h'

class Parser {
public:
    Parser(string str) : lexer(str), currentToken(lexer.getNextToken()) {}

    // разбор выражения
    ASTNode* parseExpression() {
        ASTNode* left = parseTerm();

        while (currentToken.type == PLUS || currentToken.type == MINUS) {
            Token op = currentToken;
            eat(currentToken.type);
            ASTNode* right = parseTerm();
            left = new BinaryOpNode(left, op.type, right);
        }

        return left;
    }

private:
    Lexer lexer;
    Token currentToken;

    // получить следующий токен и проверить, соответствует ли он ожидаемому типу
    void eat(TokenType type) {
        if (currentToken.type == type) {
            currentToken = lexer.getNextToken();
        }
        else {
            cerr << "Syntax error" << endl;
            exit(1);
        }
    }

    // разбор множителей
    ASTNode* parseTerm() {
        ASTNode* left = parseFactor();

        while (currentToken.type == MULTIPLY || currentToken.type == DIVIDE || currentToken.type == POWER) {
            Token op = currentToken;
            eat(currentToken.type);
            ASTNode* right = parseFactor();
            left = new BinaryOpNode(left, op.type, right);
        }

        return left;
    }

    // разбор факторов
    ASTNode* parseFactor() {
        Token token = currentToken;

        if (token.type == NUMBER) {
            eat(NUMBER);
            return new NumberNode(token.value);
        }
        else if (token.type == VARIABLE) {
            eat(VARIABLE);
            return new VariableNode(token.variable, variables);
        }
        else if (token.type == SQRT) {
            eat(SQRT);
            eat(LPAREN);
            ASTNode* arg = parseExpression();
            eat(RPAREN);
            return new UnaryOpNode(SQRT, arg);
        }
        else if (token.type == PLUS) {
            eat(PLUS);
            return parseFactor();
        }
        else if (token.type == MINUS) {
            eat(MINUS);
            return new UnaryOpNode(NEG, parseFactor());
        }
        else if (token.type == LPAREN) {
            eat(LPAREN);
            ASTNode* result = parseExpression();
            eat(RPAREN);
            return result;
        }
        else {
            cerr << "Syntax error" << endl;
            exit(1);
        }
    }

    // выполнить операцию
    double applyOp(Token op, double left, double right) {
        switch (op.type) {
        case PLUS:
            return left + right;
        case MINUS:
            return left - right;
        case MULTIPLY:
            return left * right;
        case DIVIDE:
            if (right == 0) {
                cerr << "Division by zero" << endl;
                exit(1);
            }
            return left / right;
        case POWER:
            return pow(left, right);
        default:
            cerr << "Invalid operator" << endl;
            exit(1);
        }
    }
};