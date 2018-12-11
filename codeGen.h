#ifndef CODEGEN_H
#define CODEGEN_H

#include "semantics.h"
#include "node.h"
#include <string>
#include <stack>

using namespace std;

class CodeGen {
private:
    const Node *parseTree;
    string filename;
    int var;
    int label;
    int ro;
    int isLoop;
    string loopLabel;
    stack <string> globalStack;

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
    void setLoop(int loop);
    int getLoop();
    void setLoopLabel(string lLabel);
    string getLoopLabel();
    void printToTarget(string output);
    void pushStack(string gVar);
    string popStack();
    int getGlobalVarSize();
};



#endif
