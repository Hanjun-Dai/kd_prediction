include $(HOME)/Workspace/cpp/graphnn/core_makefile
   
all: build/kernel_mean_field build/kernel_loopy_bp

CXXFLAGS += -I./include 

build/%: src/%.cpp include/* $(gnn_objs) $(gnn_include)
	$(dir_guard)
	$(CXX) $(CXXFLAGS) -I$(RDBASE)/Code -o $@ $(filter %.cpp %.o %.cuo, $^) $(LDFLAGS) -L$(RDBASE)/lib -lGraphMol -lRDGeneral -lSmilesParse -lDepictor -lSmilesParse -lFileParsers -lFingerprints

clean:
	rm build/*
