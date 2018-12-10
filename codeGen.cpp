#include "codeGen.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

CodeGen::CodeGen(const Node *parseTree, string &fileName) : parseTree(parseTree) {
    filename = fileName;
    var = 0;
    label = 0;
    generateCode(parseTree);
    cout << "Code Gen Complete!" << endl;
}

CodeGen::~CodeGen() = default;

void CodeGen::generateCode(const Node *node) {
    if(node->isNonTerminal()) {
    	const vector<Node *> &childNodes = node->getChildren();
    	if(node->getNonTerminalIdentifier() == PROGRAM) {
	    //vars
	    generateCode(childNodes.at(0));
 	    //block
	    generateCode(childNodes.at(1));
	    printToTarget("STOP");
	    int limit = CodeGen::var;
	    for(int i = 0; i < limit; i++) {
		string temp = "T" + to_string(i) + " 0";
		printToTarget(temp);
	    }
    	}
        else if(node->getNonTerminalIdentifier() == BLOCK) {
	    //vars
	    generateCode(childNodes.at(1));
	    //stats
	    generateCode(childNodes.at(2));		
    	}	
    	else if(node->getNonTerminalIdentifier() == VARS) {
	    if(childNodes.empty()) {
	    }
	    else {
	    	printToTarget("LOAD " + childNodes.at(3)->getValue());
	    	printToTarget("STORE " + childNodes.at(1)->getValue());
	    	generateCode(childNodes.at(4));	
	    }
    	}
    	else if(node->getNonTerminalIdentifier() == EXPR) {
	    if(childNodes.size() == 1) {
	        //<A>
	        generateCode(childNodes.at(0));
	    }
	    else if(childNodes.size() == 3 && childNodes.at(1)->getValue() == "/") {
	    	//<A> / <expr>
	    	generateCode(childNodes.at(2));
	   	string temp = "T" + getVar();
	    	printToTarget("STORE " + temp);
	   	generateCode(childNodes.at(0));
	    	printToTarget("DIV " +  temp);
	    }
	    else if(childNodes.size() == 3 && childNodes.at(1)->getValue() == "*") {
	        //<A> * <expr>
                generateCode(childNodes.at(2));
	    	string temp = "T" + getVar();
	    	printToTarget("STORE " + temp);
	    	generateCode(childNodes.at(0));
	    	printToTarget("MULT " + temp);
	    }
    	}
      	else if(node->getNonTerminalIdentifier() == A) {
	     if(childNodes.size() == 1) {
	        //<M>
	    	generateCode(childNodes.at(0));
	     }
	     else if(childNodes.size() == 3 && childNodes.at(1)->getValue() == "+") {
	  	//<M> + <A>
	    	generateCode(childNodes.at(2));
	    	string temp = "T" + getVar();
	    	printToTarget("STORE " + temp);
	    	generateCode(childNodes.at(0));
	    	printToTarget("ADD " + temp);
	    }
	    else if(childNodes.size() == 3 && childNodes.at(1)->getValue() == "-") {
	    	//<M> - <A>
	    	generateCode(childNodes.at(2));
	    	string temp = "T" + getVar();
	    	printToTarget("STORE " + temp);
	    	generateCode(childNodes.at(0));
	    	printToTarget("SUB " + temp);
	    }
    	}
    	else if(node->getNonTerminalIdentifier() == M) {
	    if(childNodes.size() == 2) {
	    	//-<M>
	    	generateCode(childNodes.at(1));
	    	printToTarget("MULT -1");
	    }
	    else if(childNodes.size() == 1) {
	    	//<R>
	    	generateCode(childNodes.at(0));
	    }
        }
    	else if(node->getNonTerminalIdentifier() == R) {
	    if(childNodes.size() == 3) {
	    	//(<expr>)
	    	generateCode(childNodes.at(1));
	    }
	    else if(childNodes.size() == 1 && isalpha(childNodes.at(0)->getValue()[0])) {
	    	//Identifier
	    	printToTarget("LOAD " + childNodes.at(0)->getValue());
	    }
	    else if(childNodes.size() == 1 && isdigit(childNodes.at(0)->getValue()[0])) {
	    	//Integer
	    	printToTarget("LOAD " + childNodes.at(0)->getValue());
	    }
    	}
    	else if(node->getNonTerminalIdentifier() == STATS) {
	    generateCode(childNodes.at(0));
	    generateCode(childNodes.at(1));
    	}
    	else if(node->getNonTerminalIdentifier() == MSTAT) {
	    if(childNodes.empty()) {
	    }
	    else if(childNodes.size() == 2) {
	    	generateCode(childNodes.at(0));
	    	generateCode(childNodes.at(1));
	    }
        }
    	else if(node->getNonTerminalIdentifier() == STAT) {
	    generateCode(childNodes.at(0));
    	}
        else if(node->getNonTerminalIdentifier() == IN) {
	    printToTarget("READ " + childNodes.at(1)->getValue());
    	}
    	else if(node->getNonTerminalIdentifier() == OUT) {

	    generateCode(childNodes.at(2));
	    string temp = "T" + getVar();
	    printToTarget("STORE " + temp);
	    printToTarget("WRITE " + temp);
    	}
    	else if(node->getNonTerminalIdentifier() == IF) {
	    generateCode(childNodes.at(4));
	    string temp = "T" + getVar();
	    printToTarget("STORE " + temp);
	    generateCode(childNodes.at(2));
	    printToTarget("SUB " + temp);
	    generateCode(childNodes.at(3));
	    string tLabel = "L" + getLabel();
	    int rO = getRO();
	    if(rO == 0) {
	    	cout << "Relational Operator Not Changed!" << endl;
	    	exit(-1);
	    }
	    else if(rO == 1) {
	    	printToTarget("BRNEG " + tLabel);
	    }
	    else if(rO == 2) {
	    	printToTarget("BRZNEG " + tLabel);
	    }
	    else if(rO == 3) {
	    	printToTarget("BRPOS " + tLabel);
	    }
	    else if(rO == 4) {
	    	printToTarget("BRZPOS " + tLabel);
	    }
	    else if(rO == 5) {
	    	printToTarget("BRZERO " + tLabel);
	    }
	    generateCode(childNodes.at(6));
	    printToTarget(tLabel + ": NOOP");
    	}
    	else if(node->getNonTerminalIdentifier() == LOOP) {

	    string inLabel = "L" + getLabel();
	    string outLabel = "L" + getLabel();
	    string temp = "T" + getVar();
	    printToTarget(inLabel + ": ");
	    generateCode(childNodes.at(4));
	    printToTarget("STORE " + temp);
	    generateCode(childNodes.at(2));
	    printToTarget("SUB " + temp);
	    generateCode(childNodes.at(3));
	    int rO = getRO();
	    if(rO == 0) {
	    	cout << "Relational Operator Not Changed!" << endl;
	    	exit(-1);
	    }
	    else if(rO == 1) {
	    	printToTarget("BRNEG " + outLabel);
	    }
	    else if(rO == 2) {
	    	printToTarget("BRZNEG " + outLabel);
	    }
	    else if(rO == 3) {
	    	printToTarget("BRPOS " + outLabel);
	    }
	    else if(rO == 4) {
	    	printToTarget("BRZPOS " + outLabel);
	    }
	    else if(rO == 5) {
	    	printToTarget("BRZERO " + outLabel);
	    }
	    generateCode(childNodes.at(6));
	    printToTarget("BR " + inLabel);
	    printToTarget(outLabel + ": NOOP");
    	}
    	else if(node->getNonTerminalIdentifier() == ASSIGN) {

	    generateCode(childNodes.at(3));
	    printToTarget("STORE " + childNodes.at(1)->getValue());
    	}
    	else if(node->getNonTerminalIdentifier() == RO) {
	
	    if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "<") {
	    	setRO(3);
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == ">") {
	    	setRO(2);
	    }
	    else if(childNodes.size() == 1 && childNodes.at(0)->getValue() == "=") {
	    	setRO(3);
	    }
	    else if(childNodes.size() == 2 && childNodes.at(0)->getValue() == "<") {
	    	setRO(4);
	    }
	    else if(childNodes.size() == 2 && childNodes.at(0)->getValue() == ">") {
	    	setRO(1);
	    }
	    else if(childNodes.size() == 2 && childNodes.at(0)->getValue() == "=") {
	    	setRO(5);
	    }
    	}
    }
}


void CodeGen::printToTarget(string output) {
    ofstream outputFile;
    outputFile.open(filename, std::ios_base::app);
    outputFile << output << endl;
    outputFile.close();
}

void CodeGen::setVar(int newVar) {
    CodeGen::var = newVar;
}

void CodeGen::setLabel(int newLabel) {
    CodeGen::label = newLabel;
}

string CodeGen::getVar() {
    string number = to_string(CodeGen::var);
    int temp = CodeGen::var;
    temp++;
    setVar(temp);
    return number;
}

string CodeGen::getLabel() {
    string number = to_string(CodeGen::label);
    int temp = CodeGen::label;
    temp++;
    setLabel(temp);
    return number;
}

int CodeGen::getRO() {
    return CodeGen::ro;
}

void CodeGen::setRO(int rel) {
    CodeGen::ro = rel;
}


