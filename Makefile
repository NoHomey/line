.PHONY: clean leakCheck allocs build

COMPILER = g++ -std=c++14 -Wall -Wextra -Wpedantic -g

Executable = line

CoreObjects = PatternMatcher Hasher FileRecursiveIterator FilePathIterator PathBuilder String

CliObjects = Navigator PathCutter Timestamp FileLineReader FilePathMatcher cliCommonFuncs init log status commit

Objects = $(CliObjects) $(CoreObjects)

IncludeLibCrypto = -I./ssl/include

LibCrypto = $(IncludeLibCrypto) ./ssl/lib/libcrypto.a

define Object
$(2).o: $(1)/$(2)/$(2).cc $(addsuffix .o,$(3))
	$(COMPILER) -c $$< $(IncludeLibCrypto)
endef

line: $(addsuffix .o,$(Objects)) ./main.cc
	$(COMPILER) -o $(Executable) $^ $(LibCrypto)

build: line

core = ./core

cli = ./cli

cliCommon = $(cli)/common

$(eval $(call Object,$(core),String))

$(eval $(call Object,$(core),PathBuilder,String))

$(eval $(call Object,$(core),FilePathIterator,String))

$(eval $(call Object,$(core),FileRecursiveIterator,PathBuilder Hasher))

$(eval $(call Object,$(core),Hasher,String))

$(eval $(call Object,$(core),PatternMatcher,String))

$(eval $(call Object,$(cliCommon),Navigator,String))

$(eval $(call Object,$(cliCommon),PathCutter,String))

$(eval $(call Object,$(cliCommon),FileLineReader,String))

$(eval $(call Object,$(cliCommon),FilePathMatcher,PatternMatcher))

$(eval $(call Object,$(cliCommon),Timestamp))

$(eval $(call Object,$(cli),init,cliCommonFuncs))

$(eval $(call Object,$(cli),log,cliCommonFuncs Timestamp FileLineReader))

$(eval $(call Object,$(cli),status,cliCommonFuncs FileRecursiveIterator Hasher PathCutter FileLineReader))

$(eval $(call Object,$(cli),commit,cliCommonFuncs FileRecursiveIterator Hasher Timestamp PathCutter))

cliCommonFuncs.o: $(cliCommon)/funcs/funcs.cc Navigator.o
	$(COMPILER) -c -o cliCommonFuncs.o $< $(IncludeLibCrypto)

clean:
	rm -f *.o ./$(Executable)

leakCheck: line
	valgrind ./$(Executable)

allocs: line
	valgrind --tool=massif --time-unit=B --massif-out-file=massif.out ./$(Executable)
	ms_print massif.out > allocs.txt
	rm massif.out

libcrypto:
	mkdir ./ssl
	mkdir ./sslsrc
	cd ./sslsrc && wget www.openssl.org/source/openssl-1.0.2o.tar.gz
	cd ./sslsrc && tar xf openssl-1.0.2o.tar.gz
	cd ./sslsrc/openssl-1.0.2o && ./config --prefix=$(CURDIR)/ssl no-threads no-zlib no-shared
	cd ./sslsrc/openssl-1.0.2o && make depend
	cd ./sslsrc/openssl-1.0.2o && make
	cd ./sslsrc/openssl-1.0.2o && make install
	rm -R ./sslsrc