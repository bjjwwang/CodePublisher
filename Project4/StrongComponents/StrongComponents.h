#pragma once
/////////////////////////////////////////////////////////////////////
//  StrongComponents.h   - Find the strong components from the graph//
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr2, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This package provides function to find strong componenets from a graph and output .xml

Public Interface:
=================
std::vector<std::vector<Vertex<V, E>>> FindSSC();        //get strong components, return the 2D vector of vertex
void SccToXml(std::string path);                         //transform strong components to XML
std::vector<std::vector<Vertex<V, E>>> GetResult()       //get the result of strong components

Build Process:
==============
Required files
- Dependency.h

Maintenance History:
====================
ver 1.0 : 06 Mar 2017
- First Release

*/
#include <sstream>
#include <iostream>
#include "../DependencyAnalysis/Dependency.h"

using namespace GraphLib;
template<typename V, typename E>
class StrongComponents {
public:
	StrongComponents(DependencyAnalysis* a)//initalize by dependency pointer
	{
		g = &a->GetGraph(); N = g->size();
	}
	StrongComponents(graph* g_) //initialize by graph pointer
	{
		g = g_; N = g->size();
	}
	std::vector<std::vector<Vertex<V, E>>> FindSSC();
	void tarjan(int i, std::vector<int> tree[]);
	void SccToXml(std::string path);
	std::vector<std::vector<Vertex<V, E>>> GetResult() { return ssc; }

private:
	graph* g;
	int Stop;//the number of elements in the stack
	int cnt;// the number of scc
	int visitNum;// the number of visited node 
	int DFN[10000]; //record the number of step when first visit node u
	int LOW[10000]; 
	bool instack[10000];//whether node u is in stack
	int Stap[10000];//stack  
	int Belong[10000]; 
	int N = 0;// the number of node
	std::vector<std::vector<Vertex<V, E>>> ssc; //SSC is here
};
//find the strong components
template<typename V, typename E> 
std::vector<std::vector<Vertex<V, E>>> StrongComponents<V, E>::FindSSC()
{
	std::vector<std::vector<Vertex<V, E>>> res;
	std::vector<Vertex<V, E>> temp = g->GetAdj();
	std::vector<int> tree[1000];
	for (Vertex<V, E> node : temp)
	{
		for (std::pair<int, E> leaf : node.getEdge())
		{
			tree[node.getid()].push_back(leaf.first); //get id
		}
	}
	Stop = cnt = visitNum = 0;
	memset(DFN, 0, sizeof(DFN));
	for (int i = 0; i < N; i++)
		if (!DFN[i])//if this is not the scc
			tarjan(i, tree);
	return res;
}

//tarjan alogorithm
template<typename V, typename E>
void StrongComponents<V, E>::tarjan(int i, std::vector<int> tree[])
{
	int j;
	DFN[i] = LOW[i] = ++visitNum;
	instack[i] = true;
	Stap[++Stop] = i; 
	for (unsigned k = 0; k < tree[i].size(); k++)
	{
		j = tree[i][k];
		if (!DFN[j]) 
		{
			tarjan(j, tree);
			if (LOW[j] < LOW[i])
				LOW[i] = LOW[j];
		}
		else if (instack[j] && DFN[j] < LOW[i])
			LOW[i] = DFN[j];
	}
	if (DFN[i] == LOW[i])
	{
		cnt++;  
		std::vector<Vertex<V, E>> temp;
		do
		{
			j = Stap[Stop--];
			instack[j] = false;
			Vertex<V, E> t = g->GetVertexById(j);
			temp.push_back(t);
			Belong[j] = cnt;
		} while (j != i);
		ssc.push_back(temp);
		std::cout << std::endl;
	}
}

//transform scc to xml
template<typename V, typename E>
void StrongComponents<V, E>::SccToXml(std::string path)
{
	XmlWriter wtr;
	wtr.indent();
	//wtr.addDeclaration();
	wtr.start("StrongComponents");
	for (std::vector < Vertex<V, E>> vec : ssc)
	{
		wtr.start("Component");
		for (Vertex<V, E> node : vec)
		{
			wtr.start("Path");
			wtr.addBody(node.value());
			wtr.end();
		}
		wtr.end();
	}
	wtr.end();

	std::ofstream out(path, std::ios::app);
	if (out.good())
	{
		out << wtr.xml().c_str();
		out.close();
	}
}