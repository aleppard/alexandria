UNAME_S:=$(shell uname -s)

TARGET=transpiler
CXX=clang
CFLAGS=-std=c++14 -I/usr/local/opt/llvm/include/ -L/usr/local/opt/llvm/lib
FLAGS=`/usr/local/opt/llvm/bin/llvm-config --libs all` -lclangFrontend -lclangSerialization -lclangDriver -lclangTooling -lclangParse -lclangSema -lclangAnalysis -lclangRewriteFrontend -lclangRewrite -lclangEdit -lclangAST -lclangLex -lclangBasic -lclangASTMatchers -lcurses -lz -lstdc++ $(CFLAGS)

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CXX) $(TARGET).cpp -o $(TARGET) $(FLAGS)

