#ifndef STATSEM_H
#define STATSEM_H

#include "semantics.h"
#include "node.h"

class StatSem {
private:
    const Node *parseTree;
    std::vector<std::string> *variables = new std::vector<std::string>();
    std::vector<Semantics *> *scopeStack = new std::vector<Semantics *>();
    int numTempVars = 0;

public:
    explicit StatSem(const Node *parseTree);
    ~StatSem();
    const std::string runCheck();
    const std::string checkSem(const Node *node);
    const std::string checkTempVar(const std::string &varName) const;
    const bool isUnique(const std::string &readString);
    const bool inCurrentScope(const std::string &varName);

};

#endif
