///////////////////////////////////////////////////////////////
// Graph.cpp - Graph Library                                 //
// Ver 1.6                                                   //
// Language:    Visual C++ 2012                              //
// Platform:    Lenova ThinkPad E420s, Windows 7             //
// Application: Help for CSE687 - OOD, Pr#1, Spring 2013     //
// Author:      Jim Fawcett, CST 4-187, Syracuse University  //
//              (315) 443-3948, jfawcett@twcny.rr.com        //
///////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include "Graph.h"
using namespace GraphLib;

typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;

void showVert(Vertex<std::string, std::string>& v)
{
	std::cout << "\n  " << v.id();
}

template<typename V, typename E>
void TshowVert(Vertex<V, E>& v)
{
	std::cout << "\n  " << v.id();
}


int main()
{
	std::cout << "\n  Testing Graph Library";
	std::cout << "\n =======================\n";
	try
	{
		std::cout << "\n  Constructing Graph instance";
		std::cout << "\n -----------------------------";
		graph g;
		vertex v1("v1");
		vertex v2("v2");
		vertex v3("v3");
		vertex v4("v4");
		vertex v5("v5", 50);
		g.addVertex(v2);
		g.addVertex(v1);
		g.addVertex(v3);
		g.addVertex(v4);
		g.addVertex(v5);
		g.addEdge("e1", v1, v2);
		g.addEdge("e2", v1, v3);
		g.addEdge("e3", v2, v3);
		g.addEdge("e4", v4, v3);
		g.addEdge("e5", v5, v2);

		display::show(g);
		
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
	}
	std::cout << "\n\n";
	return 0;
}


