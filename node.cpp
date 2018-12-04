#include "node.h"

using namespace std;

Node::Node(const string value, const bool nonTerminal, const int level, const NONTERMINALBANK nonTerminalIdentifier) : value(value),
                                                        nonTerminal(nonTerminal),
                                                        level(level),
							nonTerminalIdentifier(nonTerminalIdentifier) {
}

Node::~Node() = default;

const int Node::getLevel() const {
    return level;
}

const NONTERMINALBANK Node::getNonTerminalIdentifier() const {
    return nonTerminalIdentifier;
}

const string &Node::getValue() const {
    return value;
}

const bool Node::isNonTerminal() const {
    return nonTerminal;
}

const vector<Node *> &Node::getChildren() const {
    return children;
}

const void Node::setChildren(const vector<Node *> &children) {
    Node::children = children;
}

const string Node::toString() const {
    string output;

    output += toStringMe();
    output += toStringChildren();

    return output;
}

const string Node::toStringMe() const {
    string indent = "   ";
    string output;

    for (int i = 0; i < getLevel(); i++) {
        output += indent + indent;
    }

    if (nonTerminal) {
        output += "<" + value + ">\n";
    } else {
        output += value + "\n";
    }

    return output;
}

const string Node::toStringChildren() const {
    string output = "";

    for (Node *node : getChildren()) {
        output += node->toString();
    }

    return output;
}
