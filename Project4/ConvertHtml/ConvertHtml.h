#pragma once
/////////////////////////////////////////////////////////////////////
// ConvertHtml.h - Generate Web Pages for C++ and C# files         //
// ver 1.2                                                        //
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
*  This package defines the functions to generate the html pages and
*  assemble javascript and css files.
*  addjs() is to add javascript files (external) into the html code
*  addcss() is to add css files (external) into the html code
*  addDependency() is to add dependencies links to html code
*  readhtml() is to add code into the html code
*
*  Public Interface:
*  convert(std::string infile, std::string outfile) specify the input files and output files
*  void addDependency(std::vector<std::string>); add dependency into the html code
*  void welcomepage(std::vector<std::vector<vertex>>);  print all files(strong components) into files

*  Required Files:
*  ---------------
*  - Graph.h
*
*  Maintanence History:
*  --------------------
*  ver 1.2 : 02 May 2017
*  - change some function
*  ver 1.0 : 04 Apr 2017
*  - first release
*
*/
#include<iostream>
#include<fstream>
#include<stack>
#include<vector>
#include<string>
#include<Windows.h>
#include"../Graph/Graph.h"
#include"../CodeAnalyze/FileSystem.h"
using namespace GraphLib;

class convert {
	typedef Vertex<std::string, std::string> vertex;
public:
	convert(std::string infile, std::string outfile, std::string path)
	{
		filename = infile;
		in = new std::ifstream(infile);
		out = new std::ofstream(outfile);
		outputpath = path;
		std::cout << "Creating " + outfile + "\n";
	};
	convert(std::string outfile) { out = new std::ofstream(outfile); }

	void addhead();
	void readhtml();
	void addDependency(std::vector<std::string>);
	void markDependency(std::vector<std::string>);
	void welcomepage(std::vector<std::vector<vertex>>);
private:
	void addjs();
	void addcss();
	std::string outputpath;   //the output path
	std::string filename; //the input file
	std::ifstream* in;
	std::ofstream* out;
};
//create default page for users, output all the files.
void convert::welcomepage(std::vector<std::vector<vertex>> scc)
{
	int sccnum = 0;
	addhead();
	*out << "<h3>" <<
		"Welcome to Dependency Analysis Report"
		<< "</h3><hr/>";
	for (std::vector<vertex> group : scc)
	{
		
		for (vertex v : group)
		{
			*out << "<a href = \""
				<< FileSystem::Path::getName(v.value()) << ".html"
				<< "\">"
				<< FileSystem::Path::getName(v.value())
				<< "</a>"
				<< "<hr/>";
		}
		*out << "</div>";
	}
	*out << "\n</html>\n";
	out->close();
}
// add javascript src into html code
void convert::addjs()
{
	CopyFile("..\\ConvertHtml\\jstemplate.js", (outputpath+"\\jstemplate.js").c_str(), TRUE);
	*out << "\n<html><script src=\"jstemplate.js\"></script>";
}
// add css src into html code
void convert::addcss() {
	CopyFile("..\\ConvertHtml\\csstemplate.css", (outputpath + "\\csstemplate.css").c_str(), TRUE);
	*out << "<link rel = \"stylesheet\" href=\"csstemplate.css\">";
}
void convert::addhead()
{
	addjs();
	addcss();

}

void convert::markDependency(std::vector<std::string> dep)
{
	*out << "<!--";
	for (std::string elem : dep)
	{
		std::string dp = FileSystem::Path::getName(elem);
		*out << dp <<".html"<< ":";
	}
	*out << "-->\n";
}

// add dependency into html code
void convert::addDependency(std::vector<std::string> dep)
{
	*out << "<h3>" <<
		FileSystem::Path::getName(filename)
		<< "</h3><hr/>"
		<< "<div class = \"indent\">"
		<< "<h4>Dependencies:</h4>"
		<< "<br/>";
	for (std::string elem : dep)
	{
		std::string dp = FileSystem::Path::getName(elem);
		*out << "<a href = \""
			<< dp << ".html"
			<< "\">"
			<< dp
			<< "</a>"
			<< "<hr/>";
	}
	*out << "</div>"
		<< "<hr/>";
}
//write code into html code
void convert::readhtml()
{
	int boxnum = 0;
	if (out->is_open())
	{
		char buffer[256];

		*out << "<pre>\n";
		if (!in->is_open())
		{
			std::cout << "Error opening file";
			return;
		}
		while (!in->eof())
		{
			in->getline(buffer, 256);
			for (char c : buffer)
			{
				if (c == '\0')
				{
					*out << "\n";//print \n if the line ends
					break;
				}
				else if (c == '{')
				{
					// add button into before the { and create ...} which is default hidden.
					*out << "<span onclick = 'show(this, \"" << "box" << boxnum << "\", \"" << "hide" << boxnum << "\")' style = 'background:pink'>-</span>{<div id = 'hide" << boxnum << "' style = 'display:none'>...}</div>";
					*out << "<div id = 'box" << boxnum++ << "' style = 'display:node'>";
				}
				else if (c == '}')
				{
					*out << c << "</div>";
				}
				else if (c == '<') *out << "&lt;";
				else if (c == '>') *out << "&gt;";
				else *out << c;
			}
		}
		in->close();
		*out << "<a href = \""
			<< "index" << ".html" //html that back to welcome page
			<< "\">"
			<< "Back To Index Page"
			<< "</a>";
		*out << "\n</pre></html>\n";
	}
	out->close();
}