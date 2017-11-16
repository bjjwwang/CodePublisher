#pragma once
/////////////////////////////////////////////////////////////////////
//  NoSql.h   - define the NoSql and basic operation to NoSql      //
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
This package provides functions to manipulate the NoSql

Public Interface:
=================
bool save(Key key, Element elem);            //save <key,value> to Database
size_t count();
std::vector<Key> IterateKey();                //Get all key
std::vector<Element> IterateMap();              //Get all value

bool isExist(std::string key);                  //Give a key, check whether this key is in the DB
 
bool delElement(Key key);                                   //Delete <key,value>
Element GetValue(Key k);                                    //Get Value By Key
std::vector<std::string> CollectKeys(Key pattern);                //Find keys by pattern
void showchildren(Key key);                                        //show children of a specified key
bool addOverload(std::string K, std::string children);           //add a child to one key
bool deleteOverload(std::string K, std::string children);          //delete a child from one key
bool replaceValue(std::string K, Element replace);              //replace value of one key
bool editType(std::string K, std::string name);                   //give a key, edit the name of value
bool editPath(std::string K, std::string category);                 //give a key, edit the category of value
bool editNSpace(std::string K, std::string description);          //give a key, edit the description of value
void PrintKeys();                                             //print all the keys in the unordered map.

Build Process:
==============
Required files
- DBElement.h

Maintenance History:
====================
ver 1.1 : 05 Mar 2017
- Jiawei change the design of NoSql for Project 2.

ver 1.0 : 06 Feb 2017
- Prof. Fawcett's Pr 1 help code

*/
#define _CRT_SECURE_NO_WARNINGS
#include "DBElement.h"
#include<typeinfo>
#include <vector>
#include <time.h>  
#include<string>
#include <mutex>

class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;


	Keys keys();
	bool save(Key key, Element elem); //save <key,value> to Database
	size_t count();
	std::vector<Key> IterateKey(); //Get all key
	std::vector<Element> IterateMap(); //Get all value
	
	bool isExist(std::string key);

	bool delElement(Key key); //Delete <key,value>
	Element GetValue(Key k); //Get Value By Key
	std::vector<std::string> CollectKeys(Key pattern); //Find keys by pattern
	void showchildren(Key key);  //show children of a specified key
	bool addOverload(std::string K, std::string children); //add a child to one key
	bool deleteOverload(std::string K, std::string children); //delete a child from one key
	bool replaceValue(std::string K, Element replace); //replace value of one key
	bool editType(std::string K, std::string name); //give a key, edit the name of value
	bool editPath(std::string K, std::string category); //give a key, edit the category of value
	bool editNSpace(std::string K, std::string description);//give a key, edit the description of value
	void PrintKeys(); //print all the keys in the unordered map.


private:
	using Item = std::pair<Key, Element>;

	std::unordered_map<Key, Element> store;

};

typename NoSqlDb::Keys NoSqlDb::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}



bool NoSqlDb::save(Key key, Element elem) //save the element to database
{

	if (store.find(key) != store.end()) //if key exist in the database, failure
	{
		return false;
	}
	store[key] = elem;
	//std::cout << "Save the key " << key << std::endl;
	return true;
}

bool NoSqlDb::delElement(Key key)//delete the element from database
{
	if (store.erase(key))
	{
		std::cout << "Delete the key " << key << std::endl;
		return true;
	}
	std::cout << "Failure: The key is not in the map.\n ";
	return false;
}

bool NoSqlDb::isExist(std::string key)
{
	if (store.find(key) != store.end()) {
		return true;
	}
	else return false;
}
size_t NoSqlDb::count()//return the size of database
{
	return store.size();
}
std::vector<std::string> NoSqlDb::IterateKey() //Iterate the map and return all keys
{
	std::unordered_map<Key, Element>::iterator it;
	std::vector<std::string> res;
	for (it = store.begin(); it != store.end(); ++it)
	{
		res.push_back(it->first);
	}
	return res;
}

std::vector<Element> NoSqlDb::IterateMap() //Iterate the mao and return all values
{
	std::unordered_map<Key, Element>::iterator it;
	std::vector<Element> value;
	for (it = store.begin(); it != store.end(); ++it)
	{
		value.push_back(it->second);
	}
	return value;
	//return new std::vector<Element<Data>>;
}

Element NoSqlDb::GetValue(Key K) //give a key, return value.
{
	Element res;
	if (store.find(K) != store.end()) {
		return store[K];
	}
	return res;
}

std::vector<std::string> NoSqlDb::CollectKeys(Key pattern) //give a pattern, return all the keys that finds a substring equal to pattern
{
	std::unordered_map<std::string, Element>::iterator it;

	std::vector<std::string> res;
	for (it = store.begin(); it != store.end(); ++it)
	{
		std::size_t found = it->first.find(pattern);  //string::find() to find the key and push_back
		if (found != std::string::npos) res.push_back(it->first);
	}
	return res;
}

void NoSqlDb::showchildren(Key key) //give a key, display(not return) all the children of that key.
{
	std::cout << "The children of " << key << " is that: " << std::endl;
	std::vector<Key> children = GetValue(key).Get_Overload();
	for (Key a : children)
	{
		std::cout << a << " ";
	}
	std::cout << "\n";
}

bool NoSqlDb::addOverload(std::string K, std::string children) //pass a key and child , add the child to that key.
{
	if (store.find(K) != store.end()) {
		//std::cout << "The Key " << K << " add the child " << children << ".\n";
		store[K].Add_Overload(children); //call Add_Child
		return true;
	}
	return false;
}

bool NoSqlDb::deleteOverload(std::string K, std::string children)//pass a key and child , delete the child to that key.
{
	if (store.find(K) != store.end()) {
		std::cout << "The Key " << K << " remove the child " << children << ".\n";
		store[K].Remove_Overload(children);//call Remove_Child
		return true;
	}
	return false;
}

bool NoSqlDb::replaceValue(std::string K, Element replace) //pass a key and Element<Data>, replace the value of that key.
{
	if (store.find(K) != store.end()) {
		store[K] = replace;
		return true;
	}
	return false;
}

bool NoSqlDb::editType(std::string K, std::string name) //pass a key and name, change the name of that key
{
	if (store.find(K) != store.end()) {
		store[K].type() = name;
		return true;
	}
	return false;
}
bool NoSqlDb::editPath(std::string K, std::string category)//pass a key and category, change the name of that key
{
	if (store.find(K) != store.end()) {
		store[K].path() = category;
		return true;
	}
	return false;
}

bool NoSqlDb::editNSpace(std::string K, std::string description)//pass a key and description, change the name of that key
{
	if (store.find(K) != store.end()) {
		store[K].nspace() = description;
		return true;
	}
	return false;
}

void NoSqlDb::PrintKeys()   //print all keys(not return)
{
	std::cout << "The keys in the DB: ";
	std::unordered_map<Key, Element>::iterator it;
	for (it = store.begin(); it != store.end(); ++it)
	{
		std::cout << it->first << " ";
	}
	std::cout << "\n";
}