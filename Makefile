LLVM_CONFIG=`llvm-config --cxxflags --ldflags --libs --system-libs`

all: parser

src/parser.cpp: parser.y
	bison -d -o $@ $^

src/parser.hpp: src/parser.cpp

src/tokens.cpp: tokens.l src/parser.hpp
	flex -o $@ $^

parser: src/parser.cpp src/codegen.cpp src/tokens.cpp main.cpp
	g++ -O2 -Ilib $^ $(LLVM_CONFIG) -std=c++14 -o build/$@

debug: src/parser.cpp src/codegen.cpp src/tokens.cpp main.cpp
	g++ -DDEBUG -O2 -Ilib $^ $(LLVM_CONFIG) -std=c++14 -o build/$@