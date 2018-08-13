.PHONY: clean leakCheck allocs build

COMPILER = g++ -std=c++14 -Wall -Wextra -Wpedantic -g

Executable = line

Objects = Hasher FileRecursiveIterator FilePathIterator String

LibCrypto = -I./ssl/include ./ssl/lib/libcrypto.a

define Object
$(2).o: $(1)/$(2)/$(2).cc $(addsuffix .o,$(3))
	$(COMPILER) -c $$< $(LibCrypto)
endef

line: $(addsuffix .o,$(Objects)) ./main.cc
	$(COMPILER) -o $(Executable) $^ $(LibCrypto)

build: line

core = ./core

$(eval $(call Object,$(core),String))

$(eval $(call Object,$(core),FilePathIterator,String))

$(eval $(call Object,$(core),FileRecursiveIterator,String))

$(eval $(call Object,$(core),Hasher))

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