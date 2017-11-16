#pragma once
/////////////////////////////////////////////////////////////////////
//  DBElement.h   - define the value of NoSql and basic operation  //
//  ver 1.1                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr2, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This package provides design of value of NoSql and basic operation

Public Interface:
=================
std::string show();                                     //print the metadata
bool Add_Overload(std::string a);                       //add one overload path (for global functions and variables)
bool Remove_Overload(std::string a);                    //remove one overload path (for global functions and variables)
std::vector<std::string> Get_Overload();                //return the vector

Maintenance History:
====================
ver 1.1 : 05 Mar 2017
- Jiawei change the design of NoSql for Project 2.

ver 1.0 : 06 Feb 2017
- Prof. Fawcett's Pr 1 help code

*/
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>


class Element
{
public:
	
	std::string& name() { return name_; }  //class name,function name or variable's anme
	std::string& type() { return type_; }  //type can be Class, Global_Function, Global_Variable, Using, Typedef etc.
	std::string& path() { return path_; } //the path of this item
	std::string& nspace() { return nspace_; } //the namespace of this item.

	std::string show();   //print the metadata
	bool Add_Overload(std::string a); //add one child
	bool Remove_Overload(std::string a); //remove one child
	std::vector<std::string> Get_Overload(); //return the vector
	void Set_Overload(std::vector<std::string> overload_) { overload = overload_; } //set the overload




protected:
	std::vector<std::string> overload; //overload
	std::string name_;  //class name,function name or variable's anme
	std::string type_;  //type can be Class, Global_Function, Global_Variable, Using, Typedef etc.
	std::string path_;  //the path of this item
	std::string nspace_; //the namespace of this item.

};

bool Element::Add_Overload(std::string a)
{
	overload.push_back(a);
	return true;
}

bool Element::Remove_Overload(std::string a)
{
	bool remove = false;
	std::vector<std::string>::iterator it = overload.begin();
	for (; it != overload.end(); it++)
	{
		if (*it == a)
		{
			remove = true;
			overload.erase(it); //iterate the map and erase the child
			break;
		}
	}
	return remove;
}

std::vector<std::string> Element::Get_Overload()
{
	return overload;
}


std::string Element::show()
{
	// show overload when you've implemented them

	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "type" << " : " << type_;
	out << "\n    " << std::setw(8) << "path" << " : " << path_;
	out << "\n    " << std::setw(8) << "namespace" << " : " << nspace_;
	for (std::string a : overload)
	{
		out << "\n    " << std::setw(8) << "Global Function Overload" << " : " << a;
	}

	out << "\n";
	return out.str();
}

