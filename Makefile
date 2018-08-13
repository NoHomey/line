.PHONY: clean leakCheck allocs build

COMPILER = g++ -std=c++14 -Wall -Wextra -Wpedantic -g

Executable = line

Objects = FileRecursiveIterator FilePathIterator String

define Object
$(2).o: $(addsuffix .o,$(3)) $(1)/$(2)/$(2).cc
	$(COMPILER) -c $$<
endef

line: $(addsuffix .o,$(Objects)) ./main.cc
	$(COMPILER) -o $(Executable) $^

build: line

core = ./core

$(eval $(call Object,$(core),String))

$(eval $(call Object,$(core),FilePathIterator,String))

$(eval $(call Object,$(core),FileRecursiveIterator,String))

clean:
	rm -f *.o ./$(Executable)

leakCheck: line
	valgrind ./$(Executable)

allocs: line
	valgrind --tool=massif --time-unit=B --massif-out-file=massif.out ./$(Executable)
	ms_print massif.out > allocs.txt
	rm massif.out