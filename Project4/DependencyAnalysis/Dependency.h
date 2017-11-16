#pragma once
/////////////////////////////////////////////////////////////////////
//  Dependency.h   - Analyze the graph and get dependency          //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr3, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This package provides the function to analyze the dependency and generate graph.

Public Interface:
=================
void CreateGraph();                                                   //create and initialze new graph
void DetectDependency(std::string key, std::string filepath);          //detect the dependency from the NoSql Database          
std::string PersistXML(std::string);                                   //store the dependencies to xml
graph& GetGraph() { return g; }                                       //print all the keys in the unordered map.

Build Process:
==============
Required files
- graph.h, NoSql.h

Maintenance History:
====================
ver 1.0 : 05 Mar 2017
- First release

*/
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>
#include "../NoSqlDb/NoSql.h"
#include "../Graph/Graph.h"
#include <vector>

using namespace GraphLib;
typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;
class DependencyAnalysis
{
public:
	DependencyAnalysis(NoSqlDb *db_) : db(db_) {};
	void CreateGraph();
	void DetectDependency(std::string key, std::string filepath);
	
	std::string PersistXML(std::string);
	bool GraphContains(std::string key);
	graph& GetGraph() { return g; }

private:
	NoSqlDb *db;
	graph g;
	std::string DependencyEdge(std::string dependpath, std::string filepath);//add edge between the nodes
};
//find the dependency and then call function to create edges
void DependencyAnalysis::DetectDependency(std::string key, std::string filepath)
{
	if (db->isExist(key))
	{
		Element cur = db->GetValue(key);
		if (cur.type() == "global_function" || cur.type() == "global_Variable" || cur.type()=="alias" || cur.type()=="typedef"||cur.type()=="enum_Variable")
		{
			for (std::string path : cur.Get_Overload())
			{
				if (path == filepath) return;
			}

			for (std::string path : cur.Get_Overload())
			{
				DependencyEdge(path, filepath);
			}
		}
		else
		{
			if(cur.path()!= filepath) DependencyEdge(cur.path(), filepath);
		}
	}
}
//add edge between the nodes
std::string DependencyAnalysis::DependencyEdge(std::string dependpath, std::string filepath)
{
	
	std::vector<vertex> temp = g.GetAdj();
	vertex *a = nullptr;
	for (vertex &v : temp) 
	{
		if (v.value() == dependpath)
		{
			a = &v;
			break;
		}
	}
	vertex *b = nullptr;
	for (vertex &k : temp) 
	{
		if (k.value() == filepath)
		{
			b = &k;
			break;
		}
	}
	if (a != nullptr && b != nullptr)
	{
		vertex &c = *a;
		vertex &d = *b;

		g.addEdge(a->value() , d, c);
		return "";
	}
	return "";
}



//create nodes from type table, every path/package can be a node.
void DependencyAnalysis::CreateGraph()
{
	std::vector <Element> values = db->IterateMap();
	for (Element elem : values)
	{
		if (elem.type() != "global_function" && elem.type() != "global_Variable" && elem.type() != "typedef" && elem.type()!="enum_Variable" && elem.type() != "alias")
		{
			if (!GraphContains(elem.path()))
			{
				vertex temp(elem.path());
				g.addVertex(temp);
			}
		}
		else
		{
			for (std::string path : elem.Get_Overload())
			{
				if (!GraphContains(path))
				{
					vertex temp(path);
					g.addVertex(temp);
				}
			}
		}
	}
}
//give a key, check whether the graph contains this node or not
bool DependencyAnalysis::GraphContains(std::string key)
{
	for (graph::iterator ptr = g.begin(); ptr != g.end(); ptr++)
	{
		std::string &v = ptr->value();
		if (v == key) return true;
	}
	return false;
}

//transform graph to xml
std::string DependencyAnalysis::PersistXML(std::string path)
{
	std::string str = g.toXml(path);
	std::ofstream out(path);
	if (out.good())
	{
		out << str;
		out.close();
	}
	return str;
}
