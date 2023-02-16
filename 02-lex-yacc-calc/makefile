CC = gcc
CFLAGS = -g
CLIBS = -ly -lfl
LEXC = flex
LEXFLAGS = -l
YACCC = bison
YACCFLAGS = -dv
MKDIR = mkdir -p

calc: out/calc.tab.o out/lex.yy.o out/lib/hashtable.o out/lib/hashset.o
	$(CC) $(CFLAGS) out/*.o out/lib/*.o $(CLIBS) -o calc

out/lib/hashset.o: out/lib/ src/lib/hashset.c
	$(CC) $(CFLAGS) -c src/lib/hashset.c -o out/lib/hashset.o

out/lib/hashtable.o: out/lib/ src/lib/hashtable.c
	$(CC) $(CFLAGS) -c src/lib/hashtable.c -o out/lib/hashtable.o

out/calc.tab.o: calc.tab.c out/
	$(CC) $(CFLAGS) -c calc.tab.c -o out/calc.tab.o

out/lex.yy.o: lex.yy.c out/
	$(CC) $(CFLAGS) -c lex.yy.c -o out/lex.yy.o

calc.tab.c: src/calc.y
	$(YACCC) $(YACCFLAGS) src/calc.y

lex.yy.c: src/calc.l
	$(LEXC) $(LEXFLAGS) src/calc.l

out/lib/: out/
	$(MKDIR) out/lib/

out/:
	$(MKDIR) out/

clean:
	rm -rf out/* *.output lex.yy.* calc.tab.* calc
