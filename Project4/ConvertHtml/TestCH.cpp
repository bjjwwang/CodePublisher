/////////////////////////////////////////////////////////////////////
//  TestCH.cpp   - Generate a html page and test functions in ConvertHtml.h///
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr3, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
#include<iostream>
#include"ConvertHtml.h"
#ifdef TEST_CONVERT
int main()
{
	convert aa("..\\test.h", "..\\test.html","..\\");
	std::vector<std::string> dep;
	dep.push_back("hello1.cpp");
	dep.push_back("hello2.cpp");
	aa.addhead();
	aa.addDependency(dep);
	aa.readhtml();

}
#endif