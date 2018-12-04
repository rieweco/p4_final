#include <iostream>
#include <cstdarg>
#include <vector>
#include "parser.h"

using namespace std;

//vars
int VA_LIST_TERMINATOR = -1;
bool NONTERMINAL = true;
bool TERMINAL_NODE = false;

//functions
Token *token = nullptr;

Parser::Parser(string &rawData) : rawData(rawData) {
    	scanner = new Scanner(rawData);
}

Parser::~Parser() = default;

string &Parser::getRawData() {
    	return rawData;
}

void Parser::setRawData(string &rawData) {
    	Parser::rawData = rawData;
}

//<program>
Node *Parser::parse_program(int level) {
    	Node *node = new Node("program", NONTERMINAL, level, PROGRAM);
	node = consumeTerminal(node);
	
	node = consumeNonTerminal(node, parse_vars(level + 1));
   	node = consumeNonTerminal(node, parse_block(level + 1));

    	return node;
}

//<block>
Node *Parser::parse_block(int level) {
    	Node *node = new Node("block", NONTERMINAL, level, BLOCK);

    	if (currentToken() == KEYWORD_START) {
        	node = consumeTerminal(node);

        	node = consumeNonTerminal(node, parse_vars(level + 1));
        	node = consumeNonTerminal(node, parse_stats(level + 1));

        	if (currentToken() == KEYWORD_STOP) {
           		node = consumeTerminal(node);

            	return node;
        	} 
		else {
            		reportError(KEYWORD_STOP, VA_LIST_TERMINATOR);
        	}
    	} else {
        	reportError(KEYWORD_START, VA_LIST_TERMINATOR);
    	}
}

//<vars>
Node *Parser::parse_vars(int level) {
    	Node *node = new Node("vars", NONTERMINAL, level, VARS);

    	if (currentToken() == KEYWORD_VAR) {
       		node = consumeTerminal(node);

        	if (currentToken() == IDENTIFIER) {
 	        	node = consumeTerminal(node);
		
			if(currentToken() == OPERATOR_COLON) {
				node = consumeTerminal(node);
			
				if(currentToken() == INTEGER) {
					node = consumeTerminal(node);
	
        	    			node = consumeNonTerminal(node, parse_vars(level + 1));
	
        	    			return node;
				} else {
					reportError(INTEGER, VA_LIST_TERMINATOR);
				}
			}else {
				reportError(OPERATOR_COLON, VA_LIST_TERMINATOR);
			}
        	} else {
           		 reportError(IDENTIFIER, VA_LIST_TERMINATOR);
		}
    	} else {
        	return node;
    	}
}

//<expr>
Node *Parser::parse_expr(int level) {
    	Node *node = new Node("expr", NONTERMINAL, level, EXPR);

    	node = consumeNonTerminal(node, parse_A(level + 1));

    	if(currentToken() == OPERATOR_FORWARD_SLASH) {
		node = consumeTerminal(node);
		
		node = consumeNonTerminal(node, parse_expr(level + 1));
	} else if(currentToken() == OPERATOR_ASTERISK) {
		node = consumeTerminal(node);
	
		node = consumeNonTerminal(node, parse_expr(level + 1));
	} else {

	        return node;
    	}
}

//<A>
Node *Parser::parse_A(int level) {
	Node *node = new Node("A", NONTERMINAL, level, A);
	
	node = consumeNonTerminal(node, parse_M(level + 1));
	
	if(currentToken() == OPERATOR_PLUS) {
		node = consumeTerminal(node);
		
		node = consumeNonTerminal(node, parse_A(level + 1));
		
		return node;
	} else if(currentToken() == OPERATOR_MINUS) {
		node = consumeTerminal(node);
		
		node = consumeNonTerminal(node, parse_A(level + 1));
	
		return node;
	} else {
		
		return node;
	}
}

//<M>
Node *Parser::parse_M(int level) {
    	Node *node = new Node("M", NONTERMINAL, level, M);

    	if (currentToken() == OPERATOR_MINUS) {
        	node = consumeTerminal(node);

        	node = consumeNonTerminal(node, parse_M(level + 1));

       	 	return node;
    	} else {
        	node = consumeNonTerminal(node, parse_R(level + 1));

        	return node;
   	}
}

//<R>
Node *Parser::parse_R(int level) {
    	Node *node = new Node("R", NONTERMINAL, level, R);

    	if (currentToken() == DELIMITER_LEFT_PARENTHESIS) {
        	node = consumeTerminal(node);

        	node = consumeNonTerminal(node, parse_expr(level + 1));

        	if (currentToken() == DELIMITER_RIGHT_PARENTHESIS) {
            		node = consumeTerminal(node);

            		return node;
        	} else {
            		reportError(DELIMITER_RIGHT_PARENTHESIS, VA_LIST_TERMINATOR);
        	}
    	} else if (currentToken() == IDENTIFIER) {
        	node = consumeTerminal(node);

        	return node;
    	} else if (currentToken() == INTEGER) {
        	node = consumeTerminal(node);

        	return node;
    	} else {
        	reportError(DELIMITER_LEFT_PARENTHESIS, IDENTIFIER, INTEGER, VA_LIST_TERMINATOR);
    	}
}

//<stats>
Node *Parser::parse_stats(int level) {
    	Node *node = new Node("stats", NONTERMINAL, level, STATS);

    	node = consumeNonTerminal(node, parse_stat(level + 1));
    	node = consumeNonTerminal(node, parse_mStat(level + 1));

    	return node;
}

//<mStat>
Node *Parser::parse_mStat(int level) {
    	Node *node = new Node("mStat", NONTERMINAL, level, MSTAT);

    	if (currentToken() == KEYWORD_SCAN || currentToken() == KEYWORD_OUT || currentToken() == KEYWORD_START || currentToken() == KEYWORD_IF || currentToken() == KEYWORD_LOOP || currentToken() == KEYWORD_LET) {
        	node = consumeNonTerminal(node, parse_stat(level + 1));
        	node = consumeNonTerminal(node, parse_mStat(level + 1));

        	return node;
    	} else {
        	return node;
    	}	
}

//<stat>
Node *Parser::parse_stat(int level) {
    	Node *node = new Node("stat", NONTERMINAL, level, STAT);

    	if (currentToken() == KEYWORD_SCAN) {
        	node = consumeNonTerminal(node, parse_in(level + 1));

        	return node;
    	} else if (currentToken() == KEYWORD_OUT) {
        	node = consumeNonTerminal(node, parse_out(level + 1));

        	return node;
    	} else if (currentToken() == KEYWORD_START) {
        	node = consumeNonTerminal(node, parse_block(level + 1));

        	return node;
    	} else if (currentToken() == KEYWORD_IF) {
        	node = consumeNonTerminal(node, parse_if(level + 1));

        	return node;
    	} else if (currentToken() == KEYWORD_LOOP) {
        	node = consumeNonTerminal(node, parse_loop(level + 1));

        	return node;
    	} else if (currentToken() == KEYWORD_LET) {
        	node = consumeNonTerminal(node, parse_assign(level + 1));

        	return node;
    	} else {
        	reportError(KEYWORD_SCAN, KEYWORD_OUT, KEYWORD_START, KEYWORD_IF, KEYWORD_LOOP, KEYWORD_LET, VA_LIST_TERMINATOR);
    	}
}

//<in>
Node *Parser::parse_in(int level) {
    	Node *node = new Node("in", NONTERMINAL, level, IN);

    	if (currentToken() == KEYWORD_SCAN) {
        	node = consumeTerminal(node);

        	if (currentToken() == IDENTIFIER) {
            		node = consumeTerminal(node);

            		if (currentToken() == DELIMITER_PERIOD) {
                		node = consumeTerminal(node);

                		return node;
            		} else {
                		reportError(DELIMITER_PERIOD, VA_LIST_TERMINATOR);
            		}
        	} else {
            		reportError(IDENTIFIER, VA_LIST_TERMINATOR);
        	}
    	} else {
        	reportError(KEYWORD_SCAN, VA_LIST_TERMINATOR);
    	}
}

//<out>
Node *Parser::parse_out(int level) {
    	Node *node = new Node("out", NONTERMINAL, level, OUT);

    	if (currentToken() == KEYWORD_OUT) {
        	node = consumeTerminal(node);

		if (currentToken() == DELIMITER_LEFT_SQUARE_BRACKET) {
			node = consumeTerminal(node);
			
			node = consumeNonTerminal(node, parse_expr(level + 1));

			if (currentToken() == DELIMITER_RIGHT_SQUARE_BRACKET) {
				node = consumeTerminal(node);

				if (currentToken() == DELIMITER_PERIOD) {
            				node = consumeTerminal(node);

           				return node;
        			} else {
           				reportError(DELIMITER_PERIOD, VA_LIST_TERMINATOR);
        			}
			} else {
				reportError(DELIMITER_RIGHT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
			}
		} else {
			reportError(DELIMITER_LEFT_SQUARE_BRACKET, VA_LIST_TERMINATOR);
		}
    	} else {
        	reportError(KEYWORD_OUT, VA_LIST_TERMINATOR);
    	}
}

//<if>
Node *Parser::parse_if(int level) {
    	Node *node = new Node("if", NONTERMINAL, level, IF);

    	if (currentToken() == KEYWORD_IF) {
        	node = consumeTerminal(node);

        	if (currentToken() == DELIMITER_LEFT_PARENTHESIS) {
            		node = consumeTerminal(node);

            		node = consumeNonTerminal(node, parse_expr(level + 1));
           		node = consumeNonTerminal(node, parse_RO(level + 1));
            		node = consumeNonTerminal(node, parse_expr(level + 1));

            		if (currentToken() == DELIMITER_RIGHT_PARENTHESIS) {
                		node = consumeTerminal(node);

                		node = consumeNonTerminal(node, parse_stat(level + 1));

                		return node;
            		} else {
                		reportError(DELIMITER_RIGHT_PARENTHESIS, VA_LIST_TERMINATOR);
            		}
        	} else {
           		reportError(DELIMITER_LEFT_PARENTHESIS, VA_LIST_TERMINATOR);
        	}
    	} else {
        	reportError(KEYWORD_IF, VA_LIST_TERMINATOR);
    	}
}

//<loop>
Node *Parser::parse_loop(int level) {
    	Node *node = new Node("loop", NONTERMINAL, level, LOOP);

    	if (currentToken() == KEYWORD_LOOP) {
        	node = consumeTerminal(node);

        	if (currentToken() == DELIMITER_LEFT_PARENTHESIS) {
            		node = consumeTerminal(node);

            		node = consumeNonTerminal(node, parse_expr(level + 1));
            		node = consumeNonTerminal(node, parse_RO(level + 1));
            		node = consumeNonTerminal(node, parse_expr(level + 1));

           		if (currentToken() == DELIMITER_RIGHT_PARENTHESIS) {
                		node = consumeTerminal(node);

               			node = consumeNonTerminal(node, parse_stat(level + 1));

                		return node;
            		} else {
                		reportError(DELIMITER_RIGHT_PARENTHESIS, VA_LIST_TERMINATOR);
            		}
        	} else {
            		reportError(DELIMITER_LEFT_PARENTHESIS, VA_LIST_TERMINATOR);
        	}
    	} else {
        	reportError(KEYWORD_LOOP, VA_LIST_TERMINATOR);
    	}
}
//<assign>
Node *Parser::parse_assign(int level) {
    	Node *node = new Node("assign", NONTERMINAL, level, ASSIGN);

   	if (currentToken() == KEYWORD_LET) {
        	node = consumeTerminal(node);
		
		if (currentToken() == IDENTIFIER) {
			node = consumeTerminal(node);

        		if (currentToken() == OPERATOR_EQUALS) {
            			node = consumeTerminal(node);

            			node = consumeNonTerminal(node, parse_expr(level + 1));

            			if (currentToken() == DELIMITER_PERIOD) {
                			node = consumeTerminal(node);

                			return node;
            			} else {
                			reportError(DELIMITER_PERIOD, VA_LIST_TERMINATOR);
            			}
        		} else {
           	 		reportError(OPERATOR_EQUALS, VA_LIST_TERMINATOR);
        		}
    		} else {
        		reportError(IDENTIFIER, VA_LIST_TERMINATOR);
    		}
	} else {
		reportError(KEYWORD_LET, VA_LIST_TERMINATOR);
	}		
}

//<RO>
Node *Parser::parse_RO(int level) {
    	Node *node = new Node("RO", NONTERMINAL, level, RO);

    	if (currentToken() == OPERATOR_LESS_THAN) {
        	node = consumeTerminal(node);
		
		if (currentToken() == OPERATOR_EQUALS) {
			node = consumeTerminal(node);
			
			return node;
		} else {
			return node;
		}
 
    	} else if (currentToken() == OPERATOR_GREATER_THAN) {
        	node = consumeTerminal(node);
		
		if (currentToken() == OPERATOR_EQUALS) {
			node = consumeTerminal(node);
        	
			return node;
		} else {
			return node;
		}
	
    	} else if (currentToken() == OPERATOR_EQUALS) {
        	node = consumeTerminal(node);

		if (currentToken() == OPERATOR_EQUALS) {
        		node = consumeTerminal(node);

        		return node;
    		} else {
			return node;
		}
	} else {
        	reportError(OPERATOR_LESS_THAN, OPERATOR_GREATER_THAN, OPERATOR_EQUALS, VA_LIST_TERMINATOR);
   	}
}

//<EOF>
Node *Parser::parse() {
    	token = scanner->getNextToken();

    	Node *root = parse_program(ROOT_LEVEL);

   	if (currentToken() == END_OF_FILE) {
        	cout << "Parsing completed successfully.\n";
    	} else {
        	reportError(END_OF_FILE, -1);
    	}

    	return root;
}

//reportError function. This will take in the read token, and the expected token(s)
//using a va_list (variable arguments list) to match expected args from the BNF 
void Parser::reportError(TOKEN_IDENTIFIER expectedToken, ...) {
    	string listOfExpectedTokens;

    	va_list va_list;
    	va_start(va_list, expectedToken);

   	while (expectedToken != VA_LIST_TERMINATOR) {
        	listOfExpectedTokens += "\"" + TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP.at(expectedToken) + "\" or ";
        	expectedToken = static_cast<TOKEN_IDENTIFIER>(va_arg(va_list, int));
    	}	

    	listOfExpectedTokens = listOfExpectedTokens.substr(0, listOfExpectedTokens.length() - 4);

    	cerr << "PARSER ERROR: Token \"" << TOKEN_IDENTIFIER_TO_TOKEN_NAME_MAP.at(currentToken()) << "\" read, but needed " << listOfExpectedTokens << "\" on read line #:  " << scanner->getCurrentLineNumber() << "\".\n";
    	exit(1);
}

//function to get tokenID
TOKEN_IDENTIFIER Parser::currentToken() {
    	return token->getTokenIdentifier();
}

//function to consume terminal node
Node *Parser::consumeTerminal(Node *node) {
    	if (node->getLevel() != ROOT_LEVEL) {
        	vector<Node *> children = node->getChildren();
        	children.push_back(new Node(token->getValue(), TERMINAL_NODE, node->getLevel() + 1, TERMINAL));
       		node->setChildren(children);
    	}

    	token = scanner->getNextToken();

    	return node;
}

//function to consume non termimal node
Node *Parser::consumeNonTerminal(Node *originalNode, Node *nonTerminalNode) {
    	vector<Node *> originalNodesChildren = originalNode->getChildren();
   	originalNodesChildren.push_back(nonTerminalNode);
    	originalNode->setChildren(originalNodesChildren);

    	return originalNode;
}


