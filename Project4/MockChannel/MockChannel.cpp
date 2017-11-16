/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Demo for CSE687 Project #4, Spring 2015               //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - MockChannel reads from sendQ and writes to recvQ       
// - IClient call the functions in Client.h and 
//                                                                         //
// Jiawei Wang, CSE687 - Object Oriented Design, Spring 2017               //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include "../Client/Client.h"
#include "Cpp11-BlockingQueue.h"
#include <string>
#include <thread>
#include <iostream>

using BQueue = BlockingQueue < Message >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
  void postMessage(const Message& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

void Sendr::postMessage(const Message& msg)
{
  sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
  Message getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

Message Recvr::getMessage()
{
  return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
  return recvQ_;
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
  MockChannel(ISendr* pSendr, IRecvr* pRecvr,IClient* pIClient);
  void start();
  void stop();
private:
  std::thread thread_;
  ISendr* pISendr_;
  IRecvr* pIRecvr_;
  IClient* pClient_;
  bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr,IClient* pClient) : pISendr_(pSendr), pIRecvr_(pRecvr),pClient_(pClient) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
  std::cout << "\n  MockChannel starting up";
  thread_ = std::thread(
    [this] {
    Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
    Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
    if (pSendr == nullptr || pRecvr == nullptr)
    {
      std::cout << "\n  failed to start Mock Channel\n\n";
      return;
    }
    BQueue& sendQ = pSendr->queue();
    BQueue& recvQ = pRecvr->queue();
    while (!stop_)
    {
      Message msg = sendQ.deQ();  // will block here so send quit message when stopping
      recvQ.enQ(msg);
    }
    std::cout << "\n  Server stopping\n\n";
  });
}

//----< signal server thread to stop >---------------------------------------
//GUIClient is inherited from IClient abstract class, and contains Client* and ISendr* to communicate between Client and WPFGUI
class GUIClient : public IClient
{
public:
	void startClient(std::string port, std::string iisport, std::string repo){ cl = new Client(port,iisport,repo); }
	void OneClickToDemo() { cl->StartDemo(); }
	void sendfile(std::string filename, std::string category) {	cl->sendFile(filename, category);}
	void requestTypeAnalysis() {cl->TypeAnalysis(); }
	void requestAllCategory() { 
		cl->requestAllCategory();
		std::thread t1([this]() {while (true){sd->postMessage(cl->SynQ()->deQ().bodyString());break;}});
		t1.detach();
		return;
	 }
	void requestAllHTMLfileNames() {
		cl->requestAllHTMLfileNames();
		std::thread t1([this]() {while (true) { sd->postMessage(cl->SynQ()->deQ().bodyString()); break; }});
		t1.detach();
		return;
		
	}
	void requestAllfileInCategory(std::string category) {
		 cl->requestAllfileNameInCategory(category);
		 std::thread t1([this]() {while (true) { sd->postMessage(cl->SynQ()->deQ().bodyString()); break; }});
		 t1.detach();
		 return;
		 
	 }
	void requestNoParentsInCategory(std::string category) {
		 cl->requestNoParentsInCategory(category);
		 std::thread t1([this]() {while (true) { sd->postMessage(cl->SynQ()->deQ().bodyString()); break; }});
		 t1.detach();
		 return;
	 }
	void download(std::string filename) { cl->downloadFile(filename); }
	void lazydownload(std::string filename) { cl->lazydownloadFile(filename); }
	void deletefile(std::string filename, std::string category){ cl->deletefile(filename, category); }
	void setSender(ISendr* sendr_) { sd = sendr_; }
	void startIIS(){ cl->startIIS(); }

private:
	Client* cl;
	ISendr* sd;
};


void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }
IRecvr* ObjectFactory::createRecvr() { return new Recvr; }
IClient* ObjectFactory::createClient() { return new GUIClient; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr, IClient* pIClient)
{
	return new MockChannel(pISendr, pIRecvr, pIClient);
}
#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
  pMockChannel->start();
  pSendr->postMessage("Hello World");
  pSendr->postMessage("CSE687 - Object Oriented Design");
  Message msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pSendr->postMessage("stopping");
  msg = pRecvr->getMessage();
  std::cout << "\n  received message = \"" << msg << "\"";
  pMockChannel->stop();
  pSendr->postMessage("quit");
  std::cin.get();
}
#endif