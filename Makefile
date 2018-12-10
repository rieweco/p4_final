#       MAKEFILE
CFLAGS = -g -o
GCC = g++ $(CFLAGS)

all: comp

comp:     main.cpp node.cpp parser.cpp scanner.cpp utility.cpp semantics.cpp statSem.cpp token.cpp nonTerminalBank.cpp codeGen.cpp
	$(GCC) $(CFLAGS) comp main.cpp node.cpp parser.cpp scanner.cpp utility.cpp semantics.cpp statSem.cpp token.cpp nonTerminalBank.cpp codeGen.cpp -std=c++11

clean:
	rm -f *.o *.asm a.out comp
