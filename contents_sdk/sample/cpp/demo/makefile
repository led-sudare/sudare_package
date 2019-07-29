CXX			:= g++
CXXFLAGS	:= -Wall -std=c++11

TARGET		:= sample
PROJ_ROOT	:= $(realpath .)
INCLUDES	:= 
SRCS		:= main.cpp
LIBS		:= -lsudare
OBJS		:= $(SRCS:.cpp=.o)

### Rules ######################################################################

.PHONY: all
all: $(TARGET)

.PHONY: make
make: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	$(RM) $(TARGET) *.o

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $<
