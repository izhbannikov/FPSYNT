CXX = g++
#CFLAGS = -Wall -g `pkg-config --cflags --libs gtk+-2.0` -L/usr/local/lib
CFLAGS = -Wall -g -lpthread

all:   Sif.o Converter.o Statistics.o GraphLevel.o GraphNode.o  Graph.o Worker.o Branch_p.o Branch.o FNode.o FTree.o GraphTree.o GTreeLevel.o  main.o fpsynt 
		
					
fpsynt :   main.o  
	$(CXX) $(CFLAGS) -o fpsynt main.o GTreeLevel.o GraphTree.o FTree.o FNode.o Branch.o Graph.o GraphNode.o GraphLevel.o Branch_p.o Worker.o Statistics.o Converter.o Sif.o -lpthread

main.o :  
	$(CXX) $(CFLAGS)  -c main.cpp 
		
GTreeLevel.o :
	$(CXX) $(CFLAGS)  -c GTreeLevel.cpp 
	
GraphTree.o :
	$(CXX) $(CFLAGS)  -c GraphTree.cpp 
	
FTree.o : 
	$(CXX) $(CFLAGS) -c FTree.cpp
	
FNode.o :
	$(CXX) $(CFLAGS) -c FNode.cpp
	
Branch.o : Graph.h
	$(CXX) $(CFLAGS) -c Branch.cpp
	
	
Graph.o : 
	$(CXX) $(CFLAGS) -c Graph.cpp 
	
GraphNode.o :
	$(CXX) $(CFLAGS)  -c GraphNode.cpp 

GraphLevel.o :
	$(CXX) $(CFLAGS) -c GraphLevel.cpp
	
Branch_p.o : 
	$(CXX) $(CFLAGS) -c Branch_p.cpp
	
Worker.o :
	$(CXX) $(CFLAGS)   -c Worker.cpp 
	
Statistics.o :
	$(CXX) -Wall -g -c Statistics.cpp
	
Converter.o : 
	$(CXX) -Wall -g -c Converter.cpp 
	
Sif.o :
	$(CXX) $(CFLAGS) -c Sif.cpp

clean:
	rm -rf fpsynt *.o
		
