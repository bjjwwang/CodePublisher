#ifndef WINDOW_H
#define WINDOW_H
///////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                  //
//          -   for CSE 687 Project #4                
//  Ver 4.0
//  Language:      Visual C++ 2015, Community                      //
//  Platform:      Dell XPS 13 9350, Windows 10 Home               //
//  Application:   Package for CSE687 Pr4, Sp17                    //
//  Author:        Jiawei Wang, Syracuse University                //
//                 (315) 949-8913 , jwang109@syr.edu              //
///////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates the WPF GUI and define the button, listbox and 
*  various event handlers.
*
*  The window class hosts, in its window, a tab control with three views, 
*  Demo Tab, File Tab and Repository Tab.
*  In Demo Tab, you can do a demo from requiremnt 1 to 11
*  In File List Tab, you select a folder and select files and then upload
*  In Repository Tab, you can access the required functionalities 
*        in reposiotory.
*
*  MockChannel's send queue when server send back the message to client
*  Client can receive it and synchronize it.
*
*  Required Files:
*  ---------------
*  Window.h, Window.cpp, MochChannel.h, MochChannel.cpp,
*  Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App.sln
*  - this builds C++\CLI client application and native mock channel DLL
*
*  Maintenance History:
*  --------------------
*  ver 4.0 : 02 May 2017
*  - Connect to Client Class, and bind the buttons and ListBoxes with 
*    the functions in Client Class
*
*  ver 3.0 : 22 Apr 2016
*  - added support for multi selection of Listbox items.  Implemented by
*    Saurabh Patel.  Thanks Saurabh.
*  ver 2.0 : 15 Apr 2015
*  - completed message passing demo with moch channel
*  - added FileBrowserDialog to show files in a selected directory
*  ver 1.0 : 13 Apr 2015
*  - incomplete demo with GUI but not connected to mock channel
*/
/*
* Create C++/CLI Console Application
* Right-click project > Properties > Common Language Runtime Support > /clr
* Right-click project > Add > References
*   add references to :
*     System
*     System.Windows.Presentation
*     WindowsBase
*     PresentatioCore
*     PresentationFramework
*/

using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   // TextBlock formatting
using namespace System::Windows::Controls;                // TabControl
using namespace System::Windows::Controls::Primitives;    // StatusBar
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>

namespace CppCliWindows
{
  ref class WPFCppCliDemo : Window
  {
    // MockChannel references

    ISendr* pSendr_;
    IRecvr* pRecvr_;
	IClient* pClient_;
    IMockChannel* pChann_;

    // Controls for Window

    DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
    TabItem^ hDemoTab = gcnew TabItem();
    TabItem^ hFileListTab = gcnew TabItem();
	TabItem^ hRepoTab = gcnew TabItem();
	
    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();

    // Controls for SendMessage View

    Grid^ hDemoGrid = gcnew Grid();
	Button^ hDemo = gcnew Button();




    // Controls for FileListView View
    Grid^ hFileListGrid = gcnew Grid();
    Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
    ListBox^ hListBox = gcnew ListBox();
    Button^ hFolderBrowseButton = gcnew Button();
	Button^ hUploadItemButton = gcnew Button();
	TextBox^ Category = gcnew TextBox();
	TextBlock^ GuideCategory = gcnew TextBlock();

    Grid^ hGrid2 = gcnew Grid();


	//Control for Repository View
	Grid^ hRepoGrid = gcnew Grid();
	ListBox^ hRepoBox = gcnew ListBox();
	Button^ hCategory = gcnew Button();
	Button^ hShowCategory = gcnew Button();
	Button^ hShowNoParent = gcnew Button();
	Button^ hAnalysis = gcnew Button();
	Button^ hDisplayhtml = gcnew Button();
	Button^ hDelete = gcnew Button();

	Button^ hDownload = gcnew Button();
	Button^ hLazyDownload = gcnew Button();
	Button^ hIIS = gcnew Button();

    // receive thread

    Thread^ recvThread;

  public:
    WPFCppCliDemo(String^ Client_Port, String^ IISPort, String^ Client_Repo);
    ~WPFCppCliDemo();

    void setUpStatusBar();
    void setUpTabControl();
    void setUpDemoView();
    void setUpFileListView();
	void setUpRepoView();

	void setUpRepoButton();

    void addText(String^ msg);
    void getMessage();

	void DemoThePr4(Object^ sender, RoutedEventArgs^ args); //Demo the project 4

    void browseForFolder(Object^ sender, RoutedEventArgs^ args); //browse the folder
    void OnLoaded(Object^ sender, RoutedEventArgs^ args);  
    void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);

	//fileList view
	void Uploadfiles(Object^ sender, RoutedEventArgs^ args); //upload the files to server


	//repository view
	void ShowAllCategory(Object^ sender, RoutedEventArgs^ args);
	void ShowFilesInCategory(Object^ sender, RoutedEventArgs^ args);
	void ShowNoParentInCategory(Object^ sender, RoutedEventArgs^ args);

	void DownloadFile(Object^ sender, RoutedEventArgs^ args);  //download 
	void UploadFile(Object^ sender, RoutedEventArgs^ args);
	void LazyDownloadFile(Object^ sender, RoutedEventArgs^ args);  //lazy download html from server
	void ShowAllHTML(Object^ sender, RoutedEventArgs^ args); //display html from server
	void DeleteFile(Object^ sender, RoutedEventArgs^ args);  //delete file for server
	void SendAnalyze(Object^ sender, RoutedEventArgs^ args);
	void StartIIS(Object^ sender, RoutedEventArgs^ args);

  private:
    std::string toStdString(String^ pStr);
    String^ toSystemString(std::string& str);
	String^ temp_category ="";
	Boolean^ IfInNoparent = false;
  };
}


#endif
