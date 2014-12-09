EXEC   = mst-fof

OPTIMIZE =  -O2 -Wno-write-strings


OBJS   = main.o kdtree2.o spanning_tree.o write_spanning_tree.o rng.o timer.o assign_peak.o load_particles.o comparison_functions.o routines.o rotation.o construct_fof_catalogue.o write_fof_catalogues.o

CXX    = g++
CC     = gcc

INCL   = kdtree2.hpp spanning_tree.hpp spanning_tree_data_types.hpp write_spanning_tree.hpp rng.h timer.h assign_peak.hpp load_particles.hpp comparison_functions.hpp routines.hpp rotation.hpp construct_fof_catalogue.hpp write_fof_catalogues.hpp

LIBS   = -lm -lgsl -lgslcblas


CFLAGS = $(OPTIMIZE)
CXXFLAGS = $(OPTIMIZE)

$(EXEC): $(OBJS) 
	 $(CXX) $(OBJS) $(LIBS) -o $(EXEC)   
         

$(OBJS): $(INCL) 

.PHONY : clean

clean:
	 rm -f $(OBJS) $(EXEC)

