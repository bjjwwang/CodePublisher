#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 3.4                                                        //
//                                                                 //
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr2, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module builds and configures parsers.  It builds the parser
parts and configures them with application specific rules and actions.

Public Interface:
=================
ConfigParseForCodeAnal config;
config.Build();
config.Attach(someFileName);

Build Process:
==============
Required files
- ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
ActionsAndRules.h, ActionsAndRules.cpp,
SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv Project1HelpS06.sln
- cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 3.3 : 06 May 2017
- add enum analyze

ver 3.2 : 29 Oct 2016
- added check for Byte Order Mark (BOM) in attach(...)
ver 3.1 : 27 Aug 16
- added default rule and action to configuration
ver 3.0 : 23 Aug 16
- major changes for CodeAnalysis application
ver 2.1 : 19 Feb 16
- Added PrintFunction action to FunctionDefinition rule
ver 2.0 : 01 Jun 11
- Major revisions to begin building a strong code analyzer
ver 1.1 : 01 Feb 06
- cosmetic changes to ConfigureParser.cpp
ver 1.0 : 12 Jan 06
- first release
*/

#include <fstream>
#include "Parser.h"
#include "ActionsAndRules.h"
#include "SemiExp.h"
#include "Tokenizer.h"

namespace CodeAnalysis
{
	///////////////////////////////////////////////////////////////
	// build parser that writes its output to console

	class ConfigParseForCodeAnal : IBuilder
	{
	public:
		ConfigParseForCodeAnal() : pIn(nullptr) {};
		~ConfigParseForCodeAnal();
		bool Attach(const std::string& name, bool isFile = true);
		Parser* Build();
		Parser* Build2();

	private:
		// Builder must hold onto all the pieces

		std::ifstream* pIn;
		Scanner::Toker* pToker;
		Scanner::SemiExp* pSemi;
		Parser* pParser;
		Repository* pRepo;

		// add Rules and Actions

		BeginScope* pBeginScope = nullptr;
		HandleBeginScope* pHandleBeginScope = nullptr;

		EndScope* pEndScope = nullptr;
		HandleEndScope* pHandleEndScope = nullptr;

		PreprocStatement* pPreprocStatement = nullptr;
		HandlePreprocStatement* pHandlePreprocStatement = nullptr;

		NamespaceDefinition* pNamespaceDefinition = nullptr;
		HandleNamespaceDefinition* pHandleNamespaceDefinition = nullptr;

		ClassDefinition* pClassDefinition = nullptr;
		HandleClassDefinition* pHandleClassDefinition = nullptr;

		StructDefinition* pStructDefinition = nullptr;
		HandleStructDefinition* pHandleStructDefinition = nullptr;

		EnumDefinition* pEnumDefinition = nullptr;
		HandleEnumDefinition* pHandleEnumDefinition = nullptr;

		CppFunctionDefinition* pCppFunctionDefinition = nullptr;
		HandleCppFunctionDefinition* pHandleCppFunctionDefinition = nullptr;
		//PrintFunction* pPrintFunction;

		CSharpFunctionDefinition* pCSharpFunctionDefinition = nullptr;
		HandleCSharpFunctionDefinition* pHandleCSharpFunctionDefinition = nullptr;

		ControlDefinition* pControlDefinition = nullptr;
		HandleControlDefinition* pHandleControlDefinition = nullptr;

		CppDeclaration* pCppDeclaration = nullptr;
		HandleCppDeclaration* pHandleCppDeclaration = nullptr;

		CSharpDeclaration* pCSharpDeclaration = nullptr;
		HandleCSharpDeclaration* pHandleCSharpDeclaration = nullptr;

		CppExecutable* pCppExecutable = nullptr;
		HandleCppExecutable* pHandleCppExecutable = nullptr;

		CSharpExecutable* pCSharpExecutable = nullptr;
		HandleCSharpExecutable* pHandleCSharpExecutable = nullptr;

		Default* pDefault = nullptr;
		HandleDefault* pHandleDefault = nullptr;

		// prohibit copies and assignments

		ConfigParseForCodeAnal(const ConfigParseForCodeAnal&) = delete;
		ConfigParseForCodeAnal& operator=(const ConfigParseForCodeAnal&) = delete;
	};
}
#endif
