#pragma once
/////////////////////////////////////////////////////////////////////
//  Display.h - display dependency, typetable, strong components   //
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
This package provides functions to display everything that is required
by the project 2. 

Public Interface:
=================
static void displayASTTree(CodeAnalysis::ASTNode *);              //print AST Tree
static void displayTypeTable(TypeTable*);                         //print TypeTable
static void displayDependency(DependencyAnalysis*, std::string);  //print Dependency 
static void displayStrongComponents(std::vector<std::vector<Vertex<std::string, std::string>>> res);  //print strong components

Build Process:
==============
Required files
- AbstrSynTree.h, TypeTable.h, AbstrSyntree.cpp, StrongComponents.h, Utilities.h

Maintenance History:
====================
ver 1.0 : 06 Mar 2017
- first release

*/
#include<iostream>
#include<vector>
#include "../CodeAnalyze/AbstrSynTree.h"
#include "../TypeTable/TypeTable.h"
#include "../StrongComponents/StrongComponents.h"
#include "../CodeAnalyze/Utilities.h"
using namespace std;
class DisplayPr2 {
public:
	static void displayASTTree(CodeAnalysis::ASTNode *);
	static void displayTypeTable(TypeTable*);
	static void displayDependency(DependencyAnalysis*, std::string);
	static void displayStrongComponents(std::vector<std::vector<Vertex<std::string, std::string>>> res);

	static void displaytitle(std::string);
	static std::string empty(int a)
	{
		string res = "";
		while (a != 0) { res += " "; a--; }
		return res;
	}
	static std::string trim(std::string);
};
/*
void DisplayPr2::displayASTTree(CodeAnalysis::ASTNode* ast)
{
	displaytitle("AST Tree");
	static std::string path;
	static size_t indentLevel = 0;
	std::ostringstream out;
	out  << std::string(2 * indentLevel, ' ') << ast->show();
	std::cout << out.str();
	auto iter = ast->children_.begin();
	++indentLevel;
	while (iter != ast->children_.end())
	{
		TreeWalk(*iter,false);
		++iter;
	}
	--indentLevel;
	
}*/

void DisplayPr2::displayTypeTable(TypeTable* tt)
{
	displaytitle("Type Table");
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\t" << empty(5) << "Name" << empty(20) << "Type" << empty(20) << "Namespace" << empty(20) << "Package" << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	NoSqlDb* db = tt->GetDb();
	std::vector<Element> values = db->IterateMap();
	for (Element elem : values)
	{
		if(elem.type()=="class"|| elem.type()=="struct")
			out << "\t" << empty(5)  << elem.name()<<empty(20 - elem.name().length())
				<< elem.type() << empty(25 - elem.type().length())
				<< elem.nspace() << empty(25 - elem.nspace().length())
				<< trim(elem.path()) << endl;
		else
			for (std::string path : elem.Get_Overload())
			{
				out << "\t" << empty(5) << elem.name() << empty(20 - elem.name().length())
					<< elem.type() << empty(25 - elem.type().length())
					<< elem.nspace() << empty(25 - elem.nspace().length())
					<< trim(path) << endl;
			}
	}
	cout << out.str();
}
std::string DisplayPr2::trim(std::string path)
{
	std::string s = path;
	std::string delimiter = "\\";

	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		//std::cout << token << std::endl;
		s.erase(0, pos + delimiter.length());
	}
	//std::cout << s << std::endl;
	return s;
}

void DisplayPr2::displaytitle(std::string title)
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n\n\t\t\t\t" << std::setw(20) << "Project 2"<<" "<< title << endl;
	out << "\t=================================================================================================" << endl;
	cout << out.str();
}
void DisplayPr2::displayDependency(DependencyAnalysis* da,std::string path)
{
	std::ostringstream out;
	displaytitle("Dependency Analysis");
	graph& g = da->GetGraph();
	GraphLib::Display<std::string,std::string>::show(g);
}

void DisplayPr2::displayStrongComponents(std::vector<std::vector<Vertex<std::string, std::string>>> res)
{
	displaytitle("Strong Components");
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	for (std::vector<Vertex<std::string, std::string>> component: res)
	{
		out << "\tComponents:";
		for (Vertex<std::string, std::string> node : component)
		{
			out << "\t" << trim(node.value());
		}
		out << "\n";
	}
	std::cout << out.str();
}