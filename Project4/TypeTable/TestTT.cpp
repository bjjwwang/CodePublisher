/////////////////////////////////////////////////////////////////////
//  TestTT.cpp   - Create a database and test TestTable            //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr2, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
#include "TypeTable.h"
#ifdef TESTTT
int main()
{
	NoSqlDb *db = new NoSqlDb();
	TypeTable *tt = new TypeTable(db);
	tt->InsertType("Global_Function", "FuncA", "C://Test/a.cpp");

	Element elem1;
	elem1.name() = "Class3";
	elem1.type() = "elem1";
	elem1.path() = "C:/test";
	elem1.nspace() = "CodeAnalyze";
	elem1.Add_Overload("C:/test/woca.cpp");
	tt->InsertType(elem1);

	std::cout << "\n Elements in TypeTable" << std::endl;
	for (Element elem : db->IterateMap())
	{
		std::cout << elem.name() << " " << elem.path()<<std::endl;
	}
	return 0;
}
#endif