CC = gcc
CFLAGS = -g -fprofile-arcs -ftest-coverage
CHECKFLAGS = $(shell pkg-config --cflags --libs check)
CHECKLIBS = $(shell pkg-config --libs check)
MKDIR = mkdir -p
EXECUTABLE = ./test
LCOVFLAGS = -c --directory ../out/ --output-file coverage.info
GENHTMLFLAGS = --output-directory ../coverage/

all: $(EXECUTABLE) coverage_report run

$(EXECUTABLE): out/lib/ out/lib/check_libs.o ../out/lib/hashtable.o ../out/lib/hashset.o
	$(CC) $(CFLAGS) $(CHECKFLAGS) -o $(EXECUTABLE) out/**/*.o  ../out/**/*.o $(CHECKLIBS)

out/lib/check_libs.o: out/lib/ src/lib/check_libs.c
	$(CC) $(CFLAGS) -c src/lib/check_libs.c -o out/lib/check_hashtable.o

../out/lib/hashset.o: ../out/lib/ ../src/lib/hashset.c
	$(CC) $(CFLAGS) -c ../src/lib/hashset.c -o ../out/lib/hashset.o

../out/lib/hashtable.o: ../out/lib/ ../src/lib/hashtable.c
	$(CC) $(CFLAGS) -c ../src/lib/hashtable.c -o ../out/lib/hashtable.o

out/lib/: out/
	$(MKDIR) out/lib/

out/:
	$(MKDIR) out/

../out/lib/: ../out/
	$(MKDIR) ../out/lib/

../out/:
	$(MKDIR) ../out/

../coverage/:
	$(MKDIR) ../coverage/

.PHONY : clean
clean:
	rm -rf ./out/* ../out/* ./test ../**/*.gcov ../**/*.gcda ./**/*.gcno ./coverage.info

.PHONY : run
run: $(EXECUTABLE)
	$(EXECUTABLE)

.PHONY : coverage_report
coverage_report: lcov_coverage ../coverage/
	genhtml coverage.info $(GENHTMLFLAGS)

.PHONY: lcov_coverage
lcov_coverage: gcov_coverage
	lcov $(LCOVFLAGS)

.PHONY : gcov_coverage
gcov_coverage: run
	gcov ../src/**/*.o
