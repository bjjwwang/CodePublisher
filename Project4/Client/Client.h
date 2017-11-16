#pragma once
/////////////////////////////////////////////////////////////////////
// Client.h - Define the Class Socket Listerner and Connector, and functions//
// ver 1.2                                                        s //
//-----------------------------------------------------------------//
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr3, Sp17                    //
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
HttpMessage ClientMakeMessage(size_t n, const std::string& msgBody, const EndPoint& ep); //Client Make Message
void ClientStartListen();   //Client Listen to Server
void connectToserver(int port);   //Connect to Server according to the port
void sendMessage(HttpMessage& msg);  //Client Send to Message
bool sendFile(const std::string& fqname,std::string catagory); //Client Send File to Server with specified Category
HttpMessage readMessage(Socket& socket);// Client Read message or files
void TypeAnalysis();//client send type analysis request
BlockingQueue<HttpMessage>* SynQ() { return SynQ_; } //it is the synchronized queue between WPF and Non-CLR Package
void requestAllCategory();    //send request to server to request all the category
void displayAllHTMLfileNames();  //send request to server to request all the filename
void displayAllfileInCatagory(std::string catagory);  //specify the category and return all the files
void displayNoParentsInCatagory(std::string catagory);  //specify the category and return all the non-parent files
void downloadFile(std::string filename);  //send filename to request, server will send back the file
void startIIS();  // connect to http://localhost:8090

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
//#include "../Logger/Cpp11-BlockingQueue.h"

using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client



///////////////////////////////////////////
//this class is created for both client and server
//combine all their functions together because they have many same features 
//
////////////////////////////////////////////
class Client {
public:
	using EndPoint = std::string;
	//void execute(const size_t TimeBetweenMessages, const size_t NumMessages);
	Client() {
		port = "8081";
		iisPort = "8091";
		ss = new SocketSystem();
		si = new SocketConnecter();
		connectToserver(8080);
		std::thread t1([&]() {
			ClientStartListen();
		});
		t1.detach();
	}
	Client(std::string port_, std::string iisport_,std::string repo) {
		port = port_;
		client_repo = repo;
		iisPort = iisport_;
		ss = new SocketSystem();
		si = new SocketConnecter();
		connectToserver(8080);
		std::thread t1([&]() {
			ClientStartListen();
		});
		t1.detach();
	}

	void operator()(Socket socket);
	HttpMessage ClientMakeMessage(size_t n, const std::string& msgBody, const EndPoint& ep); //Client Make Message
	void ClientStartListen();   //Client Listen to Server
	void connectToserver(int port);   //Connect to Server according to the port
	void sendMessage(HttpMessage& msg);  //Client Send to Message
	bool sendFile(const std::string& fqname, std::string catagory); //Client Send File to Server with specified Category
	HttpMessage readMessage(Socket& socket);// Client Read message or files
	void TypeAnalysis();//client send type analysis request
	void acceptFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>* SynQ() { return SynQ_; } //it is the synchronized queue between WPF and Non-CLR Package
	void requestAllCategory();    //send request to server to request all the category
	void requestAllHTMLfileNames();  //send request to server to request all the filename
	void requestAllfileNameInCategory(std::string catagory);  //specify the category and return all the files
	void requestNoParentsInCategory(std::string catagory);  //specify the category and return all the non-parent files
	void downloadFile(std::string filename);  //send filename to request, server will send back the file
	void deletefile(std::string, std::string);
	void lazydownloadFile(std::string filename);
	void execute(const size_t TimeBetweenMessages, const size_t NumMessages);
	void startIIS();
	void StartDemo();//Demo Function
private:
	//std::string mode;
	SocketSystem *ss;
	SocketConnecter *si;
	SocketListener * sl;
	std::string port;
	BlockingQueue<HttpMessage>* msgQ;
	std::string client_repo;
	BlockingQueue<HttpMessage>* SynQ_ = new BlockingQueue<HttpMessage>();
	std::string iisPort;
	void Demo1_3();  
	void Demo5_6();
	void Demo8_9();
};