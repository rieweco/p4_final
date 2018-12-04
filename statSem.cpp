#include <string>
#include "statSem.h"

using namespace std;

StatSem::StatSem(const Node *parseTree) : parseTree(parseTree) {
    scopeStack->push_back(new Semantics(0));
}

StatSem::~StatSem() = default;

const string StatSem::runCheck() {
    string checkResult = checkSem(parseTree);
    cout << "Semantics Check Completed Scuccessfully\n";
    return checkResult;
}

const string StatSem::checkSem(const Node *node) {
    string result = "";
    if(node->isNonTerminal()) {
	const vector<Node *> &childNodes = node->getChildren();
	if(node->getNonTerminalIdentifier() == PROGRAM) {
	    if(childNodes.size() == 2) {
		//void
		//<vars>
		cout << childNodes.at(0)->getValue();
		cout << childNodes.at(1)->getValue();
		checkSem(childNodes.at(0));
		
		cout << "inside program\n";
		//<block>
		checkSem(childNodes.at(1));

		result += "Semantics Check Successful!\n";
	    }
            else {
		result += "Semantics Check Failed!\n";
		cerr << "Semantics Error:: PROGRAM node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == BLOCK) {
	    if(childNodes.size() == 4) {
		//start
		//<vars>
		checkSem(childNodes.at(1));
		//<stats>
		checkSem(childNodes.at(2));
		//stop
	    }
	    else {
		cerr << "Semantics Error:: BLOCK node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == VARS) {
	    if(childNodes.empty()) {
		//do nothing. empty node
	    }
            else if(childNodes.size() == 5) {
		//var
		//Identifier
		if(inCurrentScope(childNodes.at(1)->getValue())) {
		    vector<string> *varNames = scopeStack->at(scopeStack->size() - 1)->getVarNames();
		    varNames->push_back(childNodes.at(1)->getValue());
		    scopeStack->at(scopeStack->size() - 1)->setVarNames(varNames);

		}
		else if(inCurrentScope(childNodes.at(1)->getValue())) {
		    vector<string> *varNames = scopeStack->at(scopeStack->size() - 1)->getVarNames();
		    varNames->push_back(childNodes.at(1)->getValue());
		    scopeStack->at(scopeStack->size() - 1)->setVarNames(varNames);
		}
		else {
		    cerr << "Semantics Error:: cannot create variable: " + childNodes.at(1)->getValue() + " because it has been defined in current scope previously\n";
		    exit(-1);
		}
		
		if(isUnique(childNodes.at(1)->getValue())) {
		    variables->push_back(childNodes.at(1)->getValue());
		}
		//:
		//Integer
		//<vars>
		checkSem(childNodes.at(4));
	    }
	    else {
		cerr << "Semantics Error:: VARS node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == EXPR) {
	    if(childNodes.size() == 1) {
	   	//<A>
            }
	    else if(childNodes.size() == 3 && childNodes.at(1)->getValue() == "/") {
		//<A>
		checkSem(childNodes.at(0));
		//<expr>
		checkSem(childNodes.at(2));
	    }
	    else if(childNodes.size() == 3 && childNodes.at(1)->getValue() == "*") {
		//<A>
		checkSem(childNodes.at(0));
		//<expr>
		checkSem(childNodes.at(2));
	    }
	    else {
		cerr << "Semantics Error:: EXPR node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == A) {
	    if(childNodes.size() == 1) {
	   	//<M>
            }
	    else if(childNodes.size() == 3 && childNodes.at(1)->getValue() == "+") {
		//<M>
		checkSem(childNodes.at(0));
		//<A>
		checkSem(childNodes.at(2));
	    }
	    else if(childNodes.size() == 3 && childNodes.at(1)->getValue() == "-") {
		//<M>
		checkSem(childNodes.at(0));
		//<A>
		checkSem(childNodes.at(2));
	    }
	    else {
		cerr << "Semantics Error:: A node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == M) {
	    if(childNodes.size() == 2) {
		//-
		//<M>
		checkSem(childNodes.at(1));
	    }
	    else if(childNodes.size() == 1) {
		//<R>
		checkSem(childNodes.at(0));
	    }
	    else {
		cerr << "Semenatics Error:: M node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == R) {
	    if(childNodes.size() == 3) {
		//(
		//<expr>
		checkSem(childNodes.at(1));
		//)
	    }
	    else if(childNodes.size() == 1 && isalpha(childNodes.at(0)->getValue()[0])) {
		//Identifier
		string declaredVar = checkTempVar(childNodes.at(0)->getValue());
  	    }
	    else if(childNodes.size() == 1 && isdigit(childNodes.at(0)->getValue()[0])) {
		//Integer
	    }
	    else {
		cerr << "Semantics Error:: R node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == STATS) {
	    if(childNodes.size() == 2) {
		//<stat>
		checkSem(childNodes.at(0));
		//<mStat>
		checkSem(childNodes.at(1));
	    }
	    else {
		cerr << "Semantics Error:: STATS node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == MSTAT) {
	    if(childNodes.empty()) {
		//do nothing. empty node
	    }
	    else if(childNodes.size() == 2) {
		//<stat>
		checkSem(childNodes.at(0));
		//<mStat>
		checkSem(childNodes.at(1));
	    }
	    else {
		cerr << "Semantics Error:: MSTAT node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == STAT) {
	    if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "in") {
		//<in>
		checkSem(childNodes.at(0));
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "out") {
		//<out>
		checkSem(childNodes.at(0));
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "block") {
		//<block>
		checkSem(childNodes.at(0));
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "if") {
		//<if>
		checkSem(childNodes.at(0));
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "loop") {
		//<loop>
		checkSem(childNodes.at(0));
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "assign") {
		//<assign>
		checkSem(childNodes.at(0));
	    }
	    else {
		cerr << "Semantics Error:: STAT node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == IN) {
	    if(childNodes.size() == 3) {
		//scan
		//Identifier
		string declaredVar = checkTempVar(childNodes.at(1)->getValue());
		//.
	    }
	    else {
		cerr << "Semantics Error:: IN node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == OUT) {
	    if(childNodes.size() == 5) {
		//out
		//[
		//<expr>
		checkSem(childNodes.at(2));
		//]
		//.
	    } 
            else {
		cerr << "Semantics Error:: OUT node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == IF) {
	    if(childNodes.size() == 7) {
		//if
		//(
		//<expr>
		checkSem(childNodes.at(2));
		//<RO>
		checkSem(childNodes.at(3));
		//<expr>
		checkSem(childNodes.at(4));
		//)
		//<stat>
		checkSem(childNodes.at(6));
	    }
	    else {
		cerr << "Semantics Error:: IF node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == LOOP) {
	    if(childNodes.size() == 7) {
		//loop
		//(
		//<expr>
		checkSem(childNodes.at(2));
		//<RO>
		checkSem(childNodes.at(3));
		//<expr>
		checkSem(childNodes.at(4));
		//)
		//<stat>
		checkSem(childNodes.at(6));
	    }
	    else {
		cerr << "Semantics Error:: LOOP node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == ASSIGN) {
	    if(childNodes.size() == 5) {
		//let
		//Identifier
		string declaredVar = checkTempVar(childNodes.at(1)->getValue());
		//=
		//<expr>
		checkSem(childNodes.at(3));
		//.
	    }
	    else {
		cerr << "Semantics Error:: ASSIGN node incorrect\n";
	    }
	}
	else if(node->getNonTerminalIdentifier() == RO) {
	    if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "<") {
		//<
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == ">") {
		//>
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "=") {
		//=
	    }
	    else if(childNodes.size() == 2 && childNodes.at(0)->getValue() == "<") {
		//< =
	    }
	    else if(childNodes.size() == 2 && childNodes.at(0)->getValue() == ">") {
		//> =
	    }
	    else if(childNodes.size() == 2 && childNodes.at(0)->getValue() == "=") {
		//= =
	    }
	    else {
		cerr << "Semantics Error:: RO node incorrect\n";
	    }
	}
	else {
	    cerr << "Semantics Error:: Non Terminal node: " + to_string(node->getNonTerminalIdentifier()) + " DNE in BNF definition rules.\n";
	}
    } 
    return result;

}

const string StatSem::checkTempVar(const string &varName) const {
    bool exists = false;
    int semanticIndex = scopeStack->size() -1;
    string tempVarName;

    while(semanticIndex >= 0 && !exists) {
   	for(int i = 0; i < scopeStack->at(semanticIndex)->getVarNames()->size(); i++) {
	    if(scopeStack->at(semanticIndex)->getVarNames()->at(i) == varName) {
		exists = true;
		tempVarName = scopeStack->at(semanticIndex)->getVarNames()->at(i);
	    }
	}
	semanticIndex++;
    }
    if(exists) {
	return tempVarName;
    }
    else {
	cerr << "Semantics Error:: Read Var: " + varName + " not found in scope stack\n";
	exit(-1);
    } 
}

const bool StatSem::isUnique(const string &varName) {
    for(int i = 0; i < variables->size(); i++) {
	if(varName == variables->at(i)) {
	   return false;
	}
    }
    return true;
}

const bool StatSem::inCurrentScope(const std::string &varName) {
    for(int i = 0; i < scopeStack->at(scopeStack->size() - 1)->getVarNames()->size(); i++) {
	if(varName == scopeStack->at(scopeStack->size() - 1)->getVarNames()->at(i)) {
	    return false;
	}
    }
    return true;
}


