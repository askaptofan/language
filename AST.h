#pragma once
#include 'lexer.h'
#include 'parser.h'

class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual double evaluate() = 0;
};

class NumberNode : public ASTNode {
public:
    NumberNode(double value) : value(value) {}
    double evaluate() { return value; }
private:
    double value;
};

class VariableNode : public ASTNode {
public:
    VariableNode(string name, unordered_map<string, double>& variables) : name(name), variables(variables) {}
    double evaluate() { return variables[name]; }
private:
    string name;
    unordered_map<string, double>& variables;
};

class BinaryOpNode : public ASTNode {
public:
    BinaryOpNode(ASTNode* left, TokenType op, ASTNode* right) : left(left), op(op), right(right) {}
    ~BinaryOpNode() {
        delete left;
        delete right;
    }
    double evaluate() {
        double leftValue = left->evaluate();
        double rightValue = right->evaluate();
        switch (op) {
        case PLUS:
            return leftValue + rightValue;
        case MINUS:
            return leftValue - rightValue;
        case MULTIPLY:
            return leftValue * rightValue;
        case DIVIDE:
            if (rightValue == 0) {
                cerr << "Division by zero" << endl;
                exit(1);
            }
            return leftValue / rightValue;
        case POWER:
            return pow(leftValue, rightValue);
        default:
            cerr << "Invalid operator" << endl;
            exit(1);
        }
    }
private:
    ASTNode* left;
    TokenType op;
    ASTNode* right;
};