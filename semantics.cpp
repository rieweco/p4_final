#include "semantics.h"

using namespace std;

Semantics::Semantics(const int depth) : depth(depth) {
}

Semantics::~Semantics() = default;

vector<string> *Semantics::getVarNames() const {
    return varNames;
}

void Semantics::setVarNames(vector<string> *varNames) {
    Semantics::varNames = varNames;
}

vector<string> *Semantics::getTempNames() const {
    return tempNames;
}

void Semantics::setTempNames(vector<string> *tempNames) {
    Semantics::tempNames = tempNames;
}

const int Semantics::getDepth() const {
    return depth;
}

const std::string Semantics::toString() const {
    string depthString = to_string(depth) + "\n";
    for(int i = 0; i < varNames->size(); i++) {
	depthString += "var:: " + varNames->at(i) + ": " + tempNames->at(i) + "\n";
    }

    return depthString;
}

const int Semantics::getBlockIndex() const {
    return blockIndex;
}

void Semantics::setBlockIndex(int newIndex) {
    Semantics::blockIndex = newIndex;
}

