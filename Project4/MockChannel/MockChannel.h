#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H
/////////////////////////////////////////////////////////////////////
// MockChannel.h - Define the functions that WPF will call in Client Class//
// ver 1.2                                                        s //
//-----------------------------------------------------------------//
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr4, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu               //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  This Package define the data structures and functions that communicate
*  between Client and WPF GUI. 

*  Public Interface:
virtual void startClient() = 0;  //set up client, including socket listener and connector
virtual void OneClickToDemo() = 0;  //Click event handler, it will demo
virtual void sendfile(std::string filename,std::string category) =0; //send file to server
virtual void requestTypeAnalysis() = 0;   //request to do type analysis, generating dependencies table and html
virtual void requestAllCategory() = 0;  //request the names of all categories
virtual void requestAllHTMLfileNames()=0;  //request all the html files
virtual void requestAllfileInCategory(std::string catagory) = 0;  //request all the files in specified category
virtual void requestNoParentsInCategory(std::string catagory) = 0;  //request all the no parent file in one category
virtual void download(std::string filename) = 0;  //download one file
virtual void lazydownload(std::string filename) = 0;  //lazy download file
virtual void deletefile(std::string filename, std::string category) = 0;  //delete file in repo, specify filename and category
virtual void setSender(ISendr*) = 0; //set the ISendr pointer, which post message to WPF GUI
virtual void startIIS() = 0;  //click to connect to local IIS server.
*  Required Files:
*  ---------------
*  - HttpMessage.h"
*  - Sockets.h
*  - FileSystem.h
*  - Logger.h
*  - Utilities.h
*  - Cpp11-BlockingQueue.cpp

*  Maintanence History:
  ver 1.2 Jiawei Wang's Project 4 Solution
  ver 1.0 Prof. Fawcett's Demo
*/

#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using Message = std::string;

struct ISendr
{
  virtual void postMessage(const Message& msg) = 0;
};

struct IRecvr
{
  virtual std::string getMessage() = 0;
};

struct IMockChannel
{
public:
  virtual void start() = 0;
  virtual void stop() = 0;
};


struct IClient
{
public:
	virtual void startClient(std::string port, std::string iis_port,std::string client_repo) = 0;  //set up client, including socket listener and connector
	virtual void OneClickToDemo() = 0;  //Click event handler, it will demo
	virtual void sendfile(std::string filename,std::string category) =0; //send file to server 
 	virtual void requestTypeAnalysis() = 0;   //request to do type analysis, generating dependencies table and html
	virtual void requestAllCategory() = 0;  //request the names of all categories
	virtual void requestAllHTMLfileNames()=0;  //request all the html files
	virtual void requestAllfileInCategory(std::string catagory) = 0;  //request all the files in specified category
	virtual void requestNoParentsInCategory(std::string catagory) = 0;  //request all the no parent file in one category
	virtual void download(std::string filename) = 0;  //download one file
 	virtual void lazydownload(std::string filename) = 0;  //lazy download file
	virtual void deletefile(std::string filename, std::string category) = 0;  //delete file in repo, specify filename and category
	virtual void setSender(ISendr*) = 0; //set the ISendr pointer, which post message to WPF GUI
	virtual void startIIS() = 0;  //click to connect to local IIS server.
};

extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
	DLL_DECL IClient* createClient();
    DLL_DECL IMockChannel* createMockChannel(ISendr* pISendr, IRecvr* pIRecvr,IClient* pIClient);
  };
}

#endif


