#pragma once
/////////////////////////////////////////////////////////////////////
//  TypeTable.h   - Define the operation to insert new type into db //
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
This package provides function to insert type to db or search type from db

Public Interface:
=================
void InsertType(std::string type, std::string name, std::string path); //insert type into database
void InsertType(Element elem);                         //insert element into database
Element SearchType(std::string name);                  //search element by key, return the element

Build Process:
==============
Required files
- NoSql.h, DBElement.h

Maintenance History:
====================
ver 1.0 : 06 Mar 2017
- First Release

*/
#include "../NoSqlDb/NoSql.h"

class TypeTable {
public:
	TypeTable() { db = new NoSqlDb(); }
	TypeTable(NoSqlDb *db_) { db = db_; }
	void InsertType(std::string type, std::string name, std::string path);
	void InsertType(Element elem);
	Element SearchType(std::string name);
	NoSqlDb* GetDb() { return db; }

private:
	NoSqlDb *db;
};

void TypeTable::InsertType(std::string type, std::string name, std::string path)
{
	if (type == "global_function")
	{
		if (db->isExist(name))
		{
			db->addOverload(name,path);
		}
		else
		{
			Element elem;
			elem.name() = name;
			elem.type() = type;
			elem.Add_Overload(path);
			db->save(elem.name(), elem);
		}
		return;
	}
	Element elem;
	elem.name() = name;
	elem.type() = type;
	elem.path() = path;
	db->save(elem.name(), elem);
}
//insert new token into db, and we should divide them into global func, global variable, typedef enum, using alias.
// and struct and class.
void TypeTable::InsertType(Element elem)
{
	if (elem.type() == "global_function"||elem.type() == "global_Variable" || elem.type() == "typedef" || elem.type() == "enum_Variable" || elem.type() == "alias")
	{
		if (elem.name()!="main" && db->isExist(elem.name()))
		{
			db->addOverload(elem.name(), elem.path());
		}
		else
		{
			db->save(elem.name(), elem);
			db->addOverload(elem.name(), elem.path());
		}
		return;
	}
	else {
		db->save(elem.name(), elem);
	}
}
//search the token from the typetable
Element TypeTable::SearchType(std::string name)
{
	Element elem;
	if (db->isExist(name))
	{
		return db->GetValue(name);
	}
	else
	{
		elem.type() = "Not Found";
		return elem;
	}
}