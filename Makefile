OBJ= token.o lexer.o templatestack.o link.o Symboltable.o hash.o microc.o parser.o hashTable.o
OPTS= -g -c -Wall -Werror

microc: $(OBJ) 
	g++ -o microc $(OBJ) 

lextest: lexer.o token.o lextest.o
	g++ -o lextest lexer.o token.o lextest.o

lextest.o: lextest.cc
	g++ $(OPTS) lextest.cc

token.o: token.cc token.h
	g++ $(OPTS) token.cc

lexer.o: lexer.cc lexer.h
	g++ $(OPTS) lexer.cc

microc.o: microc.cc token.h lexer.h parser.h 
	g++ $(OPTS) microc.cc

parser.o: parser.cc parser.h token.h lexer.h
	g++ $(OPTS) parser.cc 

hashTable.o: hashTable.cc hashTable.h hash.h link.h
	g++ $(OPTS) hashTable.cc

hash.o: hash.cc hash.h
	g++ $(OPTS) hash.cc

link.o: link.cc link.h
	g++ $(OPTS) link.cc

Symboltable.o: Symboltable.cc Symboltable.h
	g++ $(OPTS) Symboltable.cc

templatestack.o: templatestack.cc templatestack.h
	g++ $(OPTS) templatestack.cc

clean:
	rm -f *.o *~ *.gch
