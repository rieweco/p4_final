#ifndef CODEGEN_H
#define CODEGEN_H

#include "semantics.h"
#include "node.h"
#include <string>

using namespace std;

class CodeGen {
private:
    const Node *parseTree;
    string filename;
    int var;
    int label;
    int ro;

public:
    explicit CodeGen(const Node *parseTree, string &fileName);
    ~CodeGen();
    void generateCode(const Node *node);
    void setVar(int newVar);
    void setLabel(int newLabel);
    string getVar();
    string getLabel();
    void setRO(int rel);
    int getRO();
    void printToTarget(string output);
};



#endif
