PROGNAME := production_pi
OUTDIR := build/objs
TARGET := $(PROGNAME)
LIBDIR := ../lib
INCLUDE := -I$(LIBDIR)
SRCS := $(wildcard *.cpp) $(wildcard $(LIBDIR)/*.cpp)
LIBS := -lzmq
OBJS := $(addprefix $(OUTDIR)/,$(patsubst %.cpp,%.o,$(SRCS)))

CXX := g++
CXXFLAGS := -Wall -std=c++17 -O2

UNAME := ${shell uname}
ifeq ($(UNAME), Linux)
CXXFLAGS := $(CXXFLAGS) -DENABLE_SPI
endif

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

$(OUTDIR)/%.o:%.cpp
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(OPT) -o $@ -c $<

clean:
	rm -rf build $(TARGET)
