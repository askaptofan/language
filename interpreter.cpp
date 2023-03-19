#include <iostream>
#include <string>
#include <unordered_map>
#include "lexer.h"
#include "parser.h"
#include "AST.h"

using namespace std;

unordered_map<string, double> variables;

double evaluate(ASTNode* node) {
    if (dynamic_cast<NumberNode*>(node)) {
        return dynamic_cast<NumberNode*>(node)->evaluate();
    }
    else if (dynamic_cast<VariableNode*>(node)) {
        return dynamic_cast<VariableNode*>(node)->evaluate();
    }
    else if (dynamic_cast<BinaryOpNode*>(node)) {
        return dynamic_cast<BinaryOpNode*>(node)->evaluate();
    }
    else {
        cerr << "Unknown AST node type" << endl;
        exit(1);
    }
}

int main() {
    string input;
    while (getline(cin, input)) {
        TokenStream tokens = tokenize(input);
        ASTNode* ast = parse(tokens);
        if (ast == nullptr) {
            cerr << "Parsing error" << endl;
            continue;
        }
        if (analyze(ast, variables)) {
            double result = evaluate(ast);
            cout << result << endl;
        }
        else {
            cerr << "Semantic analysis error" << endl;
        }
        delete ast;
    }
    return 0;
}