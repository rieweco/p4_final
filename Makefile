#       MAKEFILE
CFLAGS = -g -Wall -o
GCC = g++ $(CFLAGS)

all: frontEnd

frontEnd:     main.cpp node.cpp parser.cpp scanner.cpp utility.cpp semantics.cpp statSem.cpp token.cpp nonTerminalBank.cpp
	$(GCC) $(CFLAGS) frontEnd main.cpp node.cpp parser.cpp scanner.cpp utility.cpp semantics.cpp statSem.cpp token.cpp nonTerminalBank.cpp -std=c++11

clean:
	rm -f *.o a.out frontEnd
