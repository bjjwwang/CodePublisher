#pragma once
/////////////////////////////////////////////////////////////////////
// Server.h - Define the sockets and actions in server             //
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
*  This Package define the data structures and functions that client may use.
*  WPF and mockchannel will call this class
*
*  Public Interface:
HttpMessage ServerMakeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);//make message according to message mode
void operator()(Socket socket);      //start server and run dead while loop
void serverStartListen(int port);    //start server
void connecttoClient(std::string port); //connect back to client
void sendMessage(HttpMessage& msg);  //send message to client
bool sendFile(const std::string& fqname);  //send file to client
HttpMessage readMessage(Socket& socket); //read message from socket 


*  Required Files:
*  ---------------
*  - HttpMessage.h"
*  - Sockets.h
*  - FileSystem.h
*  - Logger.h
*  - Utilities.h
*  Maintanence History:
*  --------------------
*  ver 1.2 : 02 May 2017
*  - change some function
*  ver 1.0 : 24 Apr 2017
*  - first release
*
*/
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../FileSystem2/FileSystem.h"
#include "../Logger2/Logger.h"
#include "../Utilities2/Utilities.h"

#include <string>
#include <iostream>
#include <thread>
#include <stdio.h>      /* printf */
#include <stdlib.h> 



using Show = StaticLogger<1>;
using namespace Utilities;
///////////////////////////////////////////
//this class is for server
////////////////////////////////////////////
class Server {
public:
	using EndPoint = std::string;
	Server(std::string repo, std::string html) { repoPath = repo; htmlPath = html; ss = new SocketSystem(); msgQ = new BlockingQueue<HttpMessage>(); }
	HttpMessage ServerMakeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);//make message according to message mode
	void operator()(Socket socket);
	void serverStartListen(int port);//start server and run dead while loop
	void connecttoClient(std::string port);//connect back to client
	void sendMessage(HttpMessage& msg);  //send message to client
	bool sendFile(const std::string& fqname); //send file to client
	HttpMessage readMessage(Socket& socket);// will use functions in the process
	void receiveFile(const std::string& filename, size_t fileSize, std::string catagory, Socket& socket);//receive files from socket


private:
	BlockingQueue<HttpMessage>* msgQ;
	SocketSystem *ss;
	SocketConnecter *si;
	SocketListener * sl;
	std::string repoPath; //The path of Repository
	std::string htmlPath; //The path of html
	std::vector<std::string> forlazy; //it will be used in lazy download to keep track of file
	void HandleMessage(HttpMessage msg, std::string client_port, int mode); //will be called in read message to process the message
	void ExecuteTypeAnalysis();//server do type analysis 
	void returnAllCategories(std::string porter);  // send all categories to client
	void returnHTMLnames(std::string porter);  // send html files name to client
	void returnAllfilesInCategory(std::string catagory, std::string porter);  //send all file names in specified category to client
	void returnAllfilesWithNoParent(std::string catagory, std::string porter); //send the file names of no parents in specified category
	void returnDownloadFile(const std::string& msgBody);  //send file to client (download)
	void returnLazyRequest(std::string tempfile);  //send files to client (lazy download)
	void deletefile(std::string, std::string);
	void SendJSCSS();
};