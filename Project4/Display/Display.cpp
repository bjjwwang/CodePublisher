/////////////////////////////////////////////////////////////////////
//  Display.cpp   - Create Different data structures and display them///
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr2, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
#include "Display.h"
using namespace CodeAnalysis;
int main()
{
	DisplayPr2::displaytitle("test1");

	ScopeStack<ASTNode*> stack;
	AbstrSynTree ast(stack);
	ASTNode* pX = new ASTNode("namespace", "X");
	ast.add(pX);                                        // add X scope
	ASTNode* pf1 = new ASTNode("function", "f1");
	ast.add(pf1);                                       // add f1 scope
	ast.pop();                                          // end f1 scope
	ASTNode* pc1 = new ASTNode("class", "Y");
	ast.add(pc1);                                       // add c1 scope
	ast.pop();                                          // end c1 scope

	ASTNode* pf2 = new ASTNode("function", "gf2");
	ast.add(pf2);                                       // add gf2 scope
	ast.pop();                                          // end gf2 scope
	ast.pop();                                          // end X scope
	NoSqlDb *db = new NoSqlDb();
	TypeTable *tt = new TypeTable(db);
	tt->InsertType("Global_Function", "FuncA", "C://Test/a.cpp");
	tt->InsertType("Class", "ClassA", "C://Test/a.cpp");

	DisplayPr2::displayTypeTable(tt);



	return 0;
}