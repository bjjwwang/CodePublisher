/////////////////////////////////////////////////////////////////////
//  TestDP.cpp - test the Dependency Class						   //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr2, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
#include "Dependency.h"
#include "../TypeTable/TypeTable.h"
#ifdef TESTDP
int main()
{
	NoSqlDb *db = new NoSqlDb();
	TypeTable *tt = new TypeTable(db);
	tt->InsertType("class", "ClassA", "C://Test/a.cpp");
	tt->InsertType("class", "ClassB", "C://Test/b.cpp");

	DependencyAnalysis test(tt->GetDb());
	test.CreateGraph();
	test.DetectDependency("FuncA", "C://Test/UA.cpp");
	test.DetectDependency("ClassA", "C://Test/UA.cpp");
	test.DetectDependency("ClassB", "C://Test/UA.cpp");

	std::cout << "Test Dependency Analysis, here we have two type in DB, FuncA and ClassA. So we detect three times" << std::endl;
	std::cout<<"Dependency size: "<< test.GetGraph().size();
	return 0;
}
#endif