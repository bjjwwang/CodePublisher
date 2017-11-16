#pragma once
/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.h   - Analyze the files and insert type into db   //
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
This package will be called by executive and insert them into db

Public Interface:
=================
void DoAnalysis(ASTNode* pGlobalScope);                 //do analyze
void AnalysisNonGlobal(ASTNode* cur, ASTNode *parent);   //analyze the class, global functions 
void AnalysisDecl(ASTNode* cur);                         //analyze global variables, using, typedef

Build Process:
==============
Required files
- AbstrSynTree.h, AbstrSynTree.cpp, Logger.cpp, TypeTable.h

Maintenance History:
====================
ver 1.0 : 05 Mar 2017
- First release

*/
#include "../TypeTable/TypeTable.h"
#include "../CodeAnalyze/AbstrSynTree.h"
using namespace CodeAnalysis;
class TypeAnalysis
{
public:
	void DoAnalysis(ASTNode* pGlobalScope);
	void AnalysisNonGlobal(ASTNode* cur, ASTNode *parent);
	void AnalysisDecl(ASTNode* cur);

	TypeAnalysis();
	TypeTable* GetTable() { return table; }
private:
	TypeTable *table;

};
TypeAnalysis::TypeAnalysis()
{
	table = new TypeTable();
}

//analyze the node in the children of ASTNode
void TypeAnalysis::AnalysisNonGlobal(ASTNode *node2, ASTNode *node)
{
	if (node2->type_ == "class" || node2->type_ == "struct") //if the node is class or struct
	{
		Element elem;
		elem.name() = node2->name_; //get current name;
		elem.nspace() = node->name_;//get parent's namespace;
		elem.type ()= node2->type_;
		elem.path() = node2->path_;
		table->InsertType(elem);
	}
	else if (node2->type_ == "function" || node2->type_ == "enum") //if the node is global function or enum definition.
	{
		Element elem;
		elem.name() = node2->name_; //get current name;
		elem.nspace() = node->name_;//get parent's namespace;
		elem.type() = node2->type_ == "function"?"global_function": "enum_Variable";
		elem.path() = node2->path_;
		table->InsertType(elem);
	}
}
//analyze the decl of ASTNode
void TypeAnalysis::AnalysisDecl(ASTNode* cur)
{
	for (CodeAnalysis::DeclarationNode node: cur->decl_)
	{
		Element elem;
		elem.name() = node.name_; //get current name;
		elem.nspace() = cur->name_;//get parent's namespace;
		if (node.declType_ == DeclType::dataDecl)elem.type() = "global_Variable";
		if (node.declType_ == DeclType::typedefDecl)elem.type() = "typedef";
		if (node.declType_ == DeclType::usingDecl)elem.type() = "alias";
		elem.path() = node.path;
		table->InsertType(elem);
	}
}
//analysis the children node and decl node.
void TypeAnalysis::DoAnalysis(ASTNode* tree)
{
	AnalysisDecl(tree);
	for (ASTNode* node : tree->children_)
	{
		if (node->type_ == "namespace")
		{
			AnalysisDecl(node);
			for (ASTNode* node2 : node->children_)
			{
				AnalysisNonGlobal(node2, node);
			}
		}
		else {
			AnalysisNonGlobal(node, tree);
		} 
	}
	return;
}