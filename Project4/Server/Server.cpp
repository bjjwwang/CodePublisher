/////////////////////////////////////////////////////////////////////
// HttpMessage.cpp - Define the sockets and actions in server      //
// ver 1.2                                                         //
/////////////////////////////////////////////////////////////////////
#include<iostream>
#include "Server.h"
#include <fstream>
#include <algorithm>
#include <cstdio>

void Server::operator()(Socket socket)
{
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		msgQ->enQ(msg);
	}
}

void Server::connecttoClient(std::string port)
{
	std::cout << "\nServer connect to client:" + port;
	try
	{
		while (!si->connect("localhost", std::stoi(port)))
		{
			Show::write("\n Server waiting to connect");
			::Sleep(100);
		}
	}
	catch (std::exception& exc)
	{
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
	}
}

void Server::serverStartListen(int port) {
	try {
		sl = new SocketListener(port, Socket::IP6);
		si = new SocketConnecter();
		sl->start(*this);
		while (true)
		{
			HttpMessage msg = msgQ->deQ();
		}
	}
	catch (std::exception& exc)
	{
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
	}
}
HttpMessage Server::ServerMakeMessage(size_t mode, const std::string& msgBody, const EndPoint& ep)
{
	HttpMessage msg;
	EndPoint myEndPoint = "localhost:8080";
	HttpMessage::Attribute attrib;
	msg.clear();
	msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
	msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));
	//if send file, so we do not need to add conetent-length
	if(mode!=14) msg.addAttribute(HttpMessage::attribute("content-length", Converter<size_t>::toString(msgBody.size())));
	msg.addBody(msgBody);
	switch (mode) {
	case 10://Server : return all categories
		msg.addAttribute(HttpMessage::attribute("mode", "10"));
		msg.addBody(msgBody);
		break;
	case 11://Server : return HTMl name
		msg.addAttribute(HttpMessage::attribute("mode", "11"));
		msg.addBody(msgBody);
		break;
	case 12://Server : return all files names in category
		msg.addAttribute(HttpMessage::attribute("mode", "12"));
		msg.addBody(msgBody);
		break;
	case 13://Server : return all files names with no parents
		msg.addAttribute(HttpMessage::attribute("mode", "13"));
		msg.addBody(msgBody);
		break;
	case 14://Server : send files
		msg.addAttribute(HttpMessage::attribute("mode", "14"));
		msg.addBody(msgBody);//empty
		break;
	case 15://Server: disconnect
	default:
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
		break;
	}
	return msg;
}


void Server::sendMessage(HttpMessage& msg) {
	std::string msgString = msg.toString();
	si->send(msgString.size(), (Socket::byte*)msgString.c_str());
}

bool Server::sendFile(const std::string& filename) {
	std::string fqname;
	if (FileSystem::Path::getExt(filename) == "html" || FileSystem::Path::getExt(filename) == "js" || FileSystem::Path::getExt(filename) == "css")
		fqname = htmlPath+"\\" + filename; //we download html file, so the default folder is Result instead of Repository
	else
		fqname = repoPath + "\\" + filename;  //if it is .cpp files, it should be the RepoPath.
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;
	HttpMessage msg = ServerMakeMessage(14, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", FileSystem::Path::getName(filename)));
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
//read message from socket and take action
HttpMessage Server::readMessage(Socket& socket)
{
	HttpMessage msg;
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
	std::string client_port = StringHelper::split(msg.findValue("fromAddr"),':')[1];
	size_t pos = msg.findAttribute("content-length");
	size_t numBytes = 0;
	if (mode == 1)
	{
		size_t contentSize;
		std::string cata = msg.findValue("category");
		std::string filename = msg.findValue("file");
		std::string sizeString = msg.findValue("content-length");
		if (sizeString != "")
			contentSize = Converter<size_t>::toValue(sizeString);
		else
			return msg;
		receiveFile(filename, contentSize, cata, socket);
	}
	else
	{
		numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
		Socket::byte* buffer = new Socket::byte[numBytes + 1];
		socket.recv(numBytes, buffer);
		buffer[numBytes] = '\0';
		std::string msgBody(buffer);
		msg.addBody(msgBody);
		delete[] buffer;
	}
	HandleMessage(msg, client_port, mode);
	// If client is done, connection breaks and recvString returns empty string
	if (msg.attributes().size() == 0){
		return msg;
	}
	return msg;
}
//
void Server::HandleMessage(HttpMessage msg, std::string client_port,int mode)
{
	connecttoClient(client_port);
	switch (mode)
	{
	case 1:
		std::cout << "\nServer received the File:" + msg.findValue("file");
		break;
	case 2:
		std::cout << "\nServer will start analysis";
		ExecuteTypeAnalysis();
		break;
	case 3:
		std::cout << "\nServer will return all categories";
		returnAllCategories(client_port);
		break;
	case 4: 
		std::cout << "\nServer will return all html filenames";
		returnHTMLnames(client_port);
		break;
	case 5: 
		std::cout << "\nServer will return all files in specified category";
		returnAllfilesInCategory(msg.findValue("category"), client_port);
		break;
	case 6: 
		std::cout << "\nServer will return all files which has no parent";
		returnAllfilesWithNoParent(msg.findValue("category"), client_port);
		break;
	case 7: 
		if (FileSystem::Path::getExt(msg.findValue("file")) == "html") {
			std::cout << "\nServer will send css, js, and html to client";
			SendJSCSS();
		}returnDownloadFile(msg.findValue("file"));
		break;
	case 8: 
		std::cout << "\n(Lazy Download)Server will send css, js, and html to client";
		SendJSCSS(); forlazy.clear();
		returnLazyRequest(msg.findValue("file"));
		break;
	case 9:
		std::cout << "\nServer will delete file";
		deletefile(msg.findValue("file"), msg.findValue("category"));
		break;
	default:
		break;
	}
}
//send js and css to client
void Server::SendJSCSS()
{
	sendFile("jstemplate.js"); //send js and css to client
	sendFile("csstemplate.css");
}
//client send file to server, and server receive the file
void Server::receiveFile(const std::string& filename, size_t fileSize, std::string catagory, Socket& socket) {
	//need to create catagory in repository
	if (!FileSystem::Directory::exists(repoPath+"/" + catagory))
	{
		FileSystem::Directory::create(repoPath + "/" + catagory);
	}
	std::string fqname = repoPath + "/" + catagory + "/" + FileSystem::Path::getName(filename);
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
//Call Pr3 in the shell.
void Server::ExecuteTypeAnalysis() {
	std::string shell = "start ../Debug/Executive.exe ../Repository/ *.cpp *.h *.cs " + htmlPath;
	//for(std::string file: FileSystem::Directory::getFiles(htmlPath,"*.html"))
	//{
	//	FileSystem::File::remove(htmlPath + "/" + file);
	//}
	//std::string shell = "start Debug/Executive.exe Repository/ *.cpp *.h *.cs " + htmlPath;
	int i = system(shell.c_str());
}
//return all the categories  to client
void Server::returnAllCategories(std::string porter)
{
	std::vector<std::string> directories = FileSystem::Directory::getDirectories(repoPath);
	std::string msgbody = "";
	for (std::string folder : directories) 
	{
		if (folder == "." || folder == "..") continue;
			msgbody += folder + ":";
	}
	sendMessage(ServerMakeMessage(10, msgbody, porter));
}
//return all the html files to client
void Server::returnHTMLnames(std::string porter)
{
	std::string msgbody = "";
	for (std::string file : FileSystem::Directory::getFiles(htmlPath,"*.html"))
	{
		msgbody += FileSystem::Path::getName(file) + ":"; // use : to split the files
	} 
	HttpMessage tempmsg = ServerMakeMessage(11, msgbody, porter);
	sendMessage(tempmsg);
}
//return all the files to client
void Server::returnAllfilesInCategory(std::string catagory, std::string porter) {

	std::string msgbody = "";
	for (std::string file : FileSystem::Directory::getFiles(repoPath + "/" + catagory))
	{
		msgbody += FileSystem::Path::getName(file) + ":";
	}
	HttpMessage tempmsg = ServerMakeMessage(12, msgbody, porter);
	HttpMessage::Attribute attrib = HttpMessage::attribute("category", catagory);
	tempmsg.addAttribute(attrib);
	sendMessage(tempmsg);

}
//get dependencies and send to client
void Server::returnAllfilesWithNoParent(std::string category, std::string porter) {
	std::string msgbody = "";
	for (std::string elem : FileSystem::Directory::getFiles(repoPath+"/" + category))
	{
		std::string filename = htmlPath+"/" + FileSystem::Path::getName(elem)+".html";
		std::ifstream in(filename); 
		std::string line;
		if (in)
		{
			getline(in, line);
		}
		else {
			break;
		}
		in.close();
		if (line.size() == 7)//line = "<!---->"
			msgbody += FileSystem::Path::getName(elem) + ":";
	}
	HttpMessage tempmsg = ServerMakeMessage(13, msgbody, porter);
	HttpMessage::Attribute attrib = HttpMessage::attribute("category", category);
	tempmsg.addAttribute(attrib);
	sendMessage(tempmsg);

}
//recursively get dependencies and send to client
void Server::returnLazyRequest(std::string tempfile) {
	returnDownloadFile(tempfile);
	forlazy.push_back(tempfile);
	std::ifstream in(htmlPath+"/"+tempfile);
	std::string line;
	if (in) 
	{
		getline(in, line);
	}
	else {
		return;
	}
	in.close();
	std::string fileset = line.substr(4, line.size() - 8);
	std::vector<std::string> dep = StringHelper::split(fileset,':');
	for (std::string elem : dep)
	{
		if ((std::find(forlazy.begin(), forlazy.end(), elem)) == forlazy.end())
			returnLazyRequest(elem);
	}
}
//delete file we can delete categoies, code file or html file
void Server::deletefile(std::string file, std::string category)
{
	bool isfile = false;
	std::string filename;
	if (FileSystem::Path::getExt(category) == "html") {
		isfile = true;
		filename = htmlPath+"\\" + category;
	}
	else if(file==""){
		filename = repoPath + "\\" +category;
	}
	else
	{
		isfile = true;
		filename = repoPath + "\\" + category + "\\" + file;
	}

	if(isfile) FileSystem::File::remove(filename);
	else
	{
		for (std::string elem : FileSystem::Directory::getFiles(filename, "*.*"))
		{
			FileSystem::File::remove(filename+"\\"+elem);
		}
	}
	FileSystem::Directory::remove(filename);
}
void Server::returnDownloadFile(const std::string& msgBody)
{
	sendFile(msgBody);
}

#ifdef TEST_SERVER

int main(int argc,char* argv[]) //Three Arguments 1 is port , 2 is repo path, 3 is the html path,
{
	int port = std::atoi(argv[1]);
	std::string repo = FileSystem::Path::getFullFileSpec(argv[2]);
	std::string html = FileSystem::Path::getFullFileSpec(argv[3]);
	Server se(repo,html);
	se.serverStartListen(port);
}
#endif