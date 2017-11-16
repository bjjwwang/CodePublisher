/////////////////////////////////////////////////////////////////////
//  TestTA.cpp   - Create a ASTTree and Do Type Analysis Demo      //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr2, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
#include "TypeAnalysis.h"
#include "../CodeAnalyze/AbstrSynTree.h"
#ifdef TESTTA
int main()
{
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

	TypeAnalysis test;
	test.DoAnalysis(pX);
	TypeTable *tb = test.GetTable();
	NoSqlDb *db = tb->GetDb();

	std::cout << "\n Elements in TypeTable" << std::endl;
	for (Element elem : db->IterateMap())
	{
		std::cout <<" "<< elem.name() << " " << elem.type()<<" "<<elem.path() << std::endl;
	}
	return 0;
}
#endif