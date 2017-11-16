
/////////////////////////////////////////////////////////////////////
// testNosql.cpp - Test the functions in NoSql.h                   //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////
/*
* This is the main function to call functions in NoSql.h
*
* Package operations:
* Main Function to test the Persist Class
* Create a DB and Persist the DB
*/
#include "NoSql.h"
#include <iostream>
using StrData = std::string;
using intData = int;
using namespace std;
#ifdef TESTNOSQL
int main()
{
	mutex lock;
	std::cout << "\n  Demonstrating NoSql Helper Code";
	std::cout << "\n =================================\n";

	std::cout << "\n  Creating and saving NoSqlDb elements with string data";
	std::cout << "\n -------------------------------------------------------\n";
	//std::cout << NoSqlDb<StrData>::utc2str();
	NoSqlDb db;
	Element elem1;
	elem1.name() = "Class3";
	elem1.type() = "elem1";
	elem1.path() = "C:/test";
	elem1.nspace() = "CodeAnalyze";
	elem1.Add_Overload("C:/test/woca.cpp");
	db.save(elem1.name(), elem1);

	Element res = db.GetValue("Class3");
	std::cout<<res.show();

	return 0;
}
#endif