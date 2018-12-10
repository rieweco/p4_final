//main.cpp

#include <iostream>
#include <fstream>
#include "parser.h"
#include "statSem.h"
#include "codeGen.h"

using namespace std;

//vars
static string NO_FILE_EXTENSION = "out";
static string FILE_IN_EXTENSION = ".fs182";
static string ASM_FILE_EXTENSION = ".asm";

//functions
string getKBData();
string getFileData(string &fileName);
void parseProgram(string &program, string &fileName);
string trim(string &toBeTrimmed);

int main(int argc, char *argv[]) {
	int numArgs = argc - 1;
	
	if (numArgs == 0) {	
    		string kbData = getKBData();
    		parseProgram(kbData, NO_FILE_EXTENSION);
	}
	else if (numArgs == 1) {
		string fileName = argv[1];
		string extendedFileName = fileName + FILE_IN_EXTENSION;
		string fileData = getFileData(extendedFileName);
		parseProgram(fileData,fileName);	
	}
	else {
		cout << "incorrect # of args! 0 args for kb input, 1 arg for file input." << endl;
		exit(1);
	}

	return 0;
}

string getKBData() {
    string data;
    string line;

    getline(cin, line);
    while (!cin.eof()) {
        if (data.empty()) {
            data = line;
        } else {
            data += "\n" + line;
        }
        getline(cin, line);
    }

    return data;
}

string getFileData(string &extendedFileName) {
    	ifstream inputFile(extendedFileName);

    	if (inputFile) {
        	string fileData;
        	string fileLine;

        	while (!inputFile.eof()) {
            		getline(inputFile, fileLine);
            		if (fileData.empty()) {
                		fileData = fileLine;
            		} else {
                		fileData += "\n" + fileLine;
            		}
        	}	

        	inputFile.close();

        	return fileData;
    	} else {
        	cout << "file: " << inputFile << " not found!" << endl;
        	exit(1);
   	}
}

void parseProgram(string &program, string &fileName) {
    Parser *parser = new Parser(program);
    Node *tree = parser->parse();
    //uncomment to print tree for P2
    //cout << tree->toString();

    StatSem *statSem = new StatSem(tree);
    const string check = statSem->runCheck();
    cout << check << endl;

    string codeGenFileName = fileName + ASM_FILE_EXTENSION;
    CodeGen *codeGen = new CodeGen(tree, codeGenFileName); 
       
}










