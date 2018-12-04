#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <string>
#include <vector>

class Semantics {
private:
    const int depth;
    std::vector<std::string> *varNames = new std::vector<std::string>();
    std::vector<std::string> *tempNames = new std::vector<std::string>();
    int blockIndex = 0;

public:
    Semantics(const int depth);
    ~Semantics();

    std::vector<std::string> *getVarNames() const;
    void setVarNames(std::vector<std::string> *varNames);
    std::vector<std::string> *getTempNames() const;
    void setTempNames(std::vector<std::string> *tempNames);
    const int getDepth() const;
    const std::string toString() const;
    const int getBlockIndex() const;
    void setBlockIndex(int newIndex);

};

#endif
