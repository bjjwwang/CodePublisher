#pragma once
/////////////////////////////////////////////////////////////////////
// Execute.h - Output the result and meet the requirements         //
// ver 1.1                                                         //
//-----------------------------------------------------------------//
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr3, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  Output the Webpages, Typetable, Dependency, Strong Components, XML and so on
*  to meet the requirement 1 - 9;
*
*  The web pages will be stored in the Test_Case folder
*
*  the default arguments will be " ..\\Test_Case *.cpp *.h *.cs a.cpp b.cpp "
*  the first is folder , second is pattern *cpp *h, and third is the default open files(a.cpp.html and b.cpp.html)
*  besides, the important package in project 3 is ConvertHtml.h, Executive.h, Executive.cpp, Execute.h
*
*  Required Files:
*  ---------------
*  - Executive.h, Executive.cpp, ConvertHtml.h
*  - Parser.h, Parser.cpp, ActionsAndRules.h, ActionsAndRules.cpp
*  - ConfigureParser.h, ConfigureParser.cpp
*  - ScopeStack.h, ScopeStack.cpp, AbstrSynTree.h, AbstrSynTree.cpp
*  - ITokenCollection.h, SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
*  - IFileMgr.h, FileMgr.h, FileMgr.cpp, FileSystem.h, FileSystem.cpp
*  - Logger.h, Logger.cpp, Utilities.h, Utilities.cpp

Maintenance History:
====================
ver 1.1 : 04 Apr 2017
- Change the req1 - req 9 to project 3, and add Pr3() to execute functions in ConvertHtml.h

ver 1.0 : 06 Mar 2017
- First Release
*/
#include "Executive.h"
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <exception>
#include <iomanip>
#include <chrono>
#include <ctime>

#include <windows.h>

#include "../ConvertHtml/ConvertHtml.h"

#include "../CodeAnalyze/Parser.h"
#include "../CodeAnalyze/FileSystem.h"
#include "../CodeAnalyze/FileMgr.h"
#include "../CodeAnalyze/ActionsAndRules.h"
#include "../CodeAnalyze/ConfigureParser.h"
#include "../CodeAnalyze/AbstrSynTree.h"
#include "../CodeAnalyze/Logger.h"
#include "../CodeAnalyze/Utilities.h"

using namespace CodeAnalysis;

class Req {
public:
	Req(CodeAnalysisExecutive* ex) { exec = ex; }
	void req1();
	void req2();
	void req3();
	void req4();
	void req5();
	void req6();
	void req7();
	void req8();
	void req9();

	void Pr3(const std::string& htmlpath);
private:
	CodeAnalysisExecutive* exec;


};
//state that I meet the requirement 1
void Req::req1()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Project 3" << " " << "Demonstration" << endl;
	out << "\t=================================================================================================" << endl;
	

	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 1" <<" "  << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	std::cout << "\n use Visual Studio 2015 and its C++ Windows console projects, as provided in the ECS computer labs.";
}
//state that I meet the requirement 2
void Req::req2()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 2" << " " << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	std::cout << "\n use the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management1"<< endl;
}
//state that I meet the requirement 3
void Req::req3()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 3" << " " << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	std::cout << "\n  provide a Publisher program that provides for creation of web pages each of which captures the content of a single C++ source code file, e.g., *.h or *.cpp. ";
}
//state that I meet the requirement 4
void Req::req4()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 4" << " " << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	std::cout << "provide the facility to expand or collapse class bodies, methods, and global functions using JavaScript and CSS properties.";
	std::cout << "In every html files, you can see the tag <span> and <div> and implement the button to expand and collapse.";
	//DisplayPr2::displayTypeTable(exec->GetTable());
}
//state that I meet the requirement 5, and display all the web pages I generated.
void Req::req5()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 5" << " " << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	cout << "The pages we generated: ";
	for (std::string page : exec->cppHeaderFiles())
		cout << "\n\t---" << page << ".html";
	for (std::string page : exec->cppImplemFiles())
		cout << "\n\t---" << page << ".html";
	for (std::string page : exec->csharpFiles())
		cout << "\n\t---" << page << ".html";

	//DisplayPr2::displayDependency(exec->GetDependency(), "C://test");
	// here is for the css print and js print
}
//state that I meet the requirement 6, and display the css js files we refer
void Req::req6()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 6" << " " << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	cout << "embed in each web page's <head> section links to the style sheet and JavaScript file. ";
	cout << "\nthe .css file is ../ConvertHtml/csstemplate.css and ../ConvertHtml/jstemplate.js";
	//DisplayPr2::displayStrongComponents(exec->GetSCC()->GetResult());
}
//state that I meet the requirement 7
void Req::req7()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 7" << " " << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	cout << "\t All the dependencies links have been printed in the .htmls.";

}

//state that I meet the requirement 8, and display all the files that are analyzed and output.
void Req::req8()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 8" << " " << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	std::cout << "\n  The folder we analyze is ." << exec->Getpath();
	std::cout << "\n  The file we analyze is (.h):";
	for (std::string file : exec->cppHeaderFiles())
	{
		std::cout << " \n-- " << file;
	}
	std::cout << "\n  The file we analyze is (.cpp):";
	for (std::string file : exec->cppImplemFiles())
	{
		std::cout << " \n-- " << file;
	}
	std::cout << "\n  The file we analyze is (.cs):";
	for (std::string file : exec->csharpFiles())
	{
		std::cout << " \n-- " << file;
	}
	std::cout << "\n  The file we open is:";
	std::cout << "\n" + exec->Getpath() + "\\welcome" + ".html";
	for (std::string file : exec->GetOpenFiles())
	{
		std::cout << "\n" + exec->Getpath() + "\\" + file + ".html";
	}

}
//state that I meet the requirement 9, and display all the important files in project 3.
void Req::req9()
{
	std::ostringstream out;
	out << "\n\n\t\t\t\t" << std::setw(20) << "Requirement 9" << " " << endl;
	out << "\t-------------------------------------------------------------------------------------------------" << endl;
	cout << out.str();
	std::cout << "\n   the important packages is ConvertHtml.h, Executive.cpp Execute.h Executive.h";
	std::cout << "";
}
//Generate the webpages and automatically open the web browsers.
void Req::Pr3(const std::string& htmlpath)
{
	typedef std::pair<int, std::string> Edge;  // graph index of target vertex, edge type
	typedef Vertex<std::string, std::string> vertex;
	if (!FileSystem::Directory::exists(htmlpath))
		FileSystem::Directory::create(htmlpath);

	convert startpage(htmlpath+"\\index.html");
	startpage.welcomepage(exec->GetSCC()->GetResult());
	for (vertex v : exec->GetDependency()->GetGraph().GetAdj())
	{
		std::string curfile = v.value();
		convert curhtml(curfile, htmlpath + "\\"+FileSystem::Path::getName(curfile) + ".html", htmlpath);
		std::vector<std::string> dep; //print title and dependency
		for (Edge e : v.getEdge())
		{
			dep.push_back(e.second);
		}
		curhtml.markDependency(dep);
		curhtml.addhead(); //add js and css to .html
		curhtml.addDependency(dep);
		curhtml.readhtml();
	}

}