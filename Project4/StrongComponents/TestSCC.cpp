
/////////////////////////////////////////////////////////////////////
//  TestSSC.cpp   - Create a graph and try to find ssc             //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr2, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
#include "StrongComponents.h"
#ifdef TESTSCC
int main()
{
	graph g;
	vertex v1("v1");
	vertex v2("v2");
	vertex v3("v3");
	vertex v4("v4");
	vertex v5("v5");
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
	g.addEdge("e6", v3, v1);
	StrongComponents<std::string, std::string> test(&g);
	test.FindSSC();
	test.SccToXml("");
	return 0;
}
#endif