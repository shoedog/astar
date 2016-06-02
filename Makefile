CXX = g++
CXXFLAGS = -std=c++0x

SRCS = tspOPT.cpp

tsp: ${SRCS} 
	${CXX} ${CXXFLAGS} ${SRCS} -o tsp


