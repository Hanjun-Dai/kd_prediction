dir_guard = @mkdir -p $(@D)

CXX := g++
CXXFLAGS += -Wall -O2 -std=c++11
LDFLAGS += -lm
include_dirs := ./include
CXXFLAGS += $(addprefix -I,$(include_dirs))

all: build/feat_export

build/%: src/%.cpp ./include/*
	$(dir_guard)
	$(CXX) $(CXXFLAGS) -o $@ $(filter %.cpp, %.a $^) $(LDFLAGS)

clean:
	rm -rf build
