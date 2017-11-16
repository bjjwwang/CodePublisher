/////////////////////////////////////////////////////////////////////
// Client.cpp - Define the sockets and actions in server      //
// ver 1.2                                                         //
/////////////////////////////////////////////////////////////////////
#include "Client.h"
#include<iostream>
#include <shellapi.h>
#include <windows.h>
#include <iomanip>
#include <chrono>
#include <ctime>

void Client::ClientStartListen() {
	std::cout << "Client started listen\n";
	try {
		sl = new SocketListener(std::stoi(port), Socket::IP6);
		std::cout << "The Client Port is " + port << std::endl;
		msgQ = new BlockingQueue<HttpMessage>();
		sl->start(*this);
		while (true)
		{
			HttpMessage msg = msgQ->deQ();
		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}

}

void Client::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		msgQ->enQ(msg);
	}
}

HttpMessage Client::ClientMakeMessage(size_t n, const std::string& msgBody, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:" + port;

	msg.clear();
	msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));
	msg.addBody(msgBody);
	if (n == 1)
	{
		//if we send file, we do not need to add the attribute
		msg.addAttribute(HttpMessage::attribute("mode", "1"));
		msg.addBody(msgBody);//empty
	}
	else if(n>=2 && n<=9)
	{
		//if we send message, we should add content-length to the HttpMessage
		attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(msgBody.size()));
		msg.addAttribute(attrib);
		msg.addAttribute(HttpMessage::attribute("mode", std::to_string(n)));
		msg.addBody(msgBody);//empty
	}
	else
	{
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}

void Client::connectToserver(int port) {

	
	Show::attach(&std::cout);
	Show::start();
	try
	{
		while (!si->connect("localhost", port))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

bool Client::sendFile(const std::string& filename,std::string catagory)
{
	std::string fqname = filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;
	HttpMessage msg = ClientMakeMessage(1, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("category",catagory));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	sendMessage(msg);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		si->send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();

	return true;
}

void Client::sendMessage(HttpMessage& msg) {
	std::string msgString = msg.toString();
	si->send(msgString.size(), (Socket::byte*)msgString.c_str());
}
 
void Client::acceptFile(const std::string& filename, size_t fileSize,Socket& socket) {
	std::cout << "\nClient is downloading " << "\"" + filename + "\"";
	std::string fqname = client_repo+"/" + filename ;
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		Show::write("\n\n  can't open file " + fqname);
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;
		socket.recv(bytesToRead, buffer);
		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);
		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
}
void Client::TypeAnalysis()
{
	HttpMessage msg = ClientMakeMessage(2, "", "localhost::8080");
	sendMessage(msg);
}

void Client::downloadFile(std::string filename)
{
	HttpMessage msg = ClientMakeMessage(7, "", "8080");
	
	msg.addAttribute(HttpMessage::attribute("file", filename));
	sendMessage(msg);
}

void Client::lazydownloadFile(std::string filename)
{
	HttpMessage msg = ClientMakeMessage(8, "", "8080");
	msg.addAttribute(HttpMessage::attribute("file", filename));
	sendMessage(msg);
}
void Client::requestAllCategory()
{
	sendMessage(ClientMakeMessage(3, "", "8080"));
}
void Client::requestAllHTMLfileNames() {
	sendMessage(ClientMakeMessage(4, "", "8080")); 
}
void Client::requestAllfileNameInCategory(std::string catagory) {
	HttpMessage msg = ClientMakeMessage(5, "", "8080");
	msg.addAttribute(HttpMessage::attribute("category", catagory));
	sendMessage(msg); 
}
void Client::deletefile(std::string filename, std::string category)
{
	HttpMessage msg = ClientMakeMessage(9, "", "8080");
	msg.addAttribute(HttpMessage::attribute("category", category));
	if(filename!="")msg.addAttribute(HttpMessage::attribute("file", filename));
	sendMessage(msg); 
}

void Client::requestNoParentsInCategory(std::string catagory)
{
	HttpMessage msg = ClientMakeMessage(6, "", "8080");
	msg.addAttribute(HttpMessage::attribute("category", catagory));
	sendMessage(msg);
}
HttpMessage Client::readMessage(Socket& socket)
{
	HttpMessage msg;
	// read message attributes
	while (true)
	{
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1)
		{
			HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);
		}
		else
		{
			break;
		}
	}
	if (msg.attributes().size() == 0) return msg;
	int mode = std::stoi(msg.findValue("mode"));
	std::string tempcatagory = msg.findValue("category");
	std::string tempfile = msg.findValue("file");
	size_t tempfilesize = std::stoi(msg.findValue("content-length"));

	if (mode>=10 && mode<=13)
	{
		size_t numBytes = 0;
		size_t pos = msg.findAttribute("content-length");
		if (pos < msg.attributes().size())
		{
			numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
			Socket::byte* buffer = new Socket::byte[numBytes + 1];
			socket.recv(numBytes, buffer);
			buffer[numBytes] = '\0';
			std::string msgBody(buffer);
			msg.addBody(msgBody);
			delete[] buffer;
		}
	}
	std::string body = msg.bodyString();
	if(mode >= 10 && mode <= 13) SynQ_->enQ(HttpMessage(msg));

	if(mode== 14)// accept files
		acceptFile(tempfile, tempfilesize,socket);
		
	return msg;
}
//start IIS
void Client::startIIS()
{
	std::string shell = "start http://localhost:" + iisPort;
	int i = system(shell.c_str());
}
void Client::StartDemo()
{
	Demo1_3();
	Demo5_6();
	std::cout << "\n\n------------------------------- Requirement 4----------------------------------";
	std::cout << "\nWe will send request to execute type analysis, this part is Project 3.";
	TypeAnalysis(); Sleep(6000);

	std::cout << "\n\n------------------------------- Requirement 7----------------------------------";
	HttpMessage msg = ClientMakeMessage(1, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", "Demo.cpp"));
	msg.addAttribute(HttpMessage::Attribute("category", "DemoCategory"));
	msg.addAttribute(HttpMessage::Attribute("content-length", "1055"));
	std::cout << "\nThis is one Demo of HTTP Style Message in this project " << msg.toString() <<",and using asynchronous request/response one-way messaging";
	Demo8_9();
	std::cout << "\n\n------------------------------- Requirement 10(Bonus)----------------------------------";
	std::cout << "\n(Bonus) we are going to lazy download 'a.h.html' and its dependencies, we will see client download more than one file";
	lazydownloadFile("a.h.html");
	Sleep(2000);
	std::cout << "\n\n------------------------------- Requirement 11(Bonus)----------------------------------";
	std::cout << "\n(Bonus) Before the IIS Server Demo, we should start IIS Server, the instruction is in 'Project4_ReadMe.docx'";
	startIIS();
	std::cout << "\n Complete!Thanks for watching the Demo.";
}

void Client::execute(const size_t TimeBetweenMessages, const size_t NumMessages)
{
	// send NumMessages messages

	Show::attach(&std::cout);
	Show::start();


	try
	{
		// send a set of messages
		HttpMessage msg;
		for (size_t i = 0; i < NumMessages; ++i)
		{
			std::string msgBody =
				"<msg>Message #" + Converter<size_t>::toString(i + 1) +
				" from client #" + "</msg>";
			msg = ClientMakeMessage(1, msgBody, "localhost:8080");
			/*
			* Sender class will need to accept messages from an input queue
			* and examine the toAddr attribute to see if a new connection
			* is needed.  If so, it would either close the existing connection
			* or save it in a map[url] = socket, then open a new connection.
			*/
			sendMessage(msg);
			Show::write("\n\n  client sent\n" + msg.toIndentedString());
			::Sleep(TimeBetweenMessages);
		}
		//  send all *.cpp files from TestFiles folder

		std::vector<std::string> files = FileSystem::Directory::getFiles("../TestFiles", "*.cpp");


		// shut down server's client handler

		//msg = makeMessage(1, "quit", "toAddr:localhost:8080");
		//sendMessage(msg, *si);
		Show::write("\n\n  client sent\n" + msg.toIndentedString());

		Show::write("\n");
		Show::write("\n  All done folks");
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}


void Client::Demo1_3()
{
	std::cout << "\n\n------------------------------- Requirement 1----------------------------------";
	std::cout << "\nThe Start Projects include WPF-Client, it is the Graphical User Interface(GUI) for client";

	std::cout << "\n\n------------------------------- Requirement 2----------------------------------";
	std::cout << "\nIn this project, we use the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management.";

	std::cout << "\n\n------------------------------- Requirement 3----------------------------------";
	std::cout << "\nThis Project the Server call Project 3, so it provide a Repository program that provides functionality to publish,"
		<< " as linked web pages, the contents of a set of C++ source code files. ";
}
void Client::Demo8_9()
{
	std::cout << "\n\n------------------------------- Requirement 8----------------------------------";
	std::cout << "\n In Req# 5, we show the sending files to Server, and now client download 'Logger.h.html' from Repository, which is based on streams of bytes established with an initial exchange of messages";
	downloadFile("Logger.h.html"); 
	std::cout << "\n\n------------------------------- Requirement 9----------------------------------";
	std::cout << "\nWhen you click Button 'One Click To Demo' in WPF GUI, you can see the automated unit test wuite that demonstrate all of requirements";
}
void Client::Demo5_6(){
	std::cout << "\n\n------------------------------- Requirement 5----------------------------------";
	std::cout << "\nIn Req#5, The Client can\n(1)upload code files to Server and specify a category\n(2)delete code files or category from server"
		<< "\n(3)download all the html files from server\n(4)Display all categories or all files in any category \n(5)Display No Parent code files in category";
	std::cout << "\n\nWe upload all the files in local repository to remote Repository ";

	for (std::string file : FileSystem::Directory::getFiles(client_repo))
		sendFile(client_repo+"/" + file, "Demo_Category");

	requestAllCategory();
	std::thread cate([this]() {while (true) { std::cout << "\nAll the names of categories are " << SynQ()->deQ().bodyString(); break; }});
	cate.join();

	requestAllfileNameInCategory("Demo_Category");
	std::thread t1([this]() {while (true) { std::cout << "\nThe Files in Category 'Demo_Category' are " << SynQ()->deQ().bodyString(); break; }});
	t1.join();
	std::cout << "\nThen we delete Logger.cpp from Demo_Category Then See the Files again ";
	deletefile("Logger.cpp", "Demo_Category");
	requestAllfileNameInCategory("Demo_Category");
	std::thread t2([this]() {while (true) { std::cout << "\nThe Files in Category 'Demo_Category' are " << SynQ()->deQ().bodyString(); break; }});
	t2.join();
	std::cout << "\nThen we search the files which have no parent in Category 'Demo_Category'  ";
	requestNoParentsInCategory("Demo_Category");
	std::thread t3([this]() {while (true) { std::cout << "\nThe No Parents Files in Category 'Demo_Category' are " << SynQ()->deQ().bodyString(); break; }});
	t3.join();
	std::cout << "\nAbove is the demo of Req#5";
	
	std::cout << "\n\n------------------------------- Requirement 6----------------------------------";
	std::cout << "Between Client and Server, there is a message-passing communication system, based on Sockets, used to access the Repository's functionality from another process or machine. "
		<< "In order to access Repository's functionality, we design 9 kinds of messages from Client to Server, they will be put in HttpMessage 'mode' Attribute";
	std::cout << "\n(1)Send Files \n(2)Do Type Analysis(Code Publisher) \n(3)Request All Categories \n(4)Request All Html Files \n(5)Request All Files in One Category \n(6)Request All Files which has no parents"
		<< "\n(7)Request a file and then download \n(8)Request a file and then lazy download \n(9)Specify a category or file, delete";
	std::cout << "\n Above is all the messsage-passing type, which ensure all the functionalities in Repository";

}
#ifdef TEST_CLIENT

int main(int argc,char* argv[]) //two argument, the first is the Client Listen Port, The second is the IIS port
{
	Client cl(argv[1],argv[2],argv[3]);

	Sleep(1000);
	cl.StartDemo();
}
#endif

