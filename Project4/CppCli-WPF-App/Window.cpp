///////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                //
//          - Demo for CSE 687 Project #4                                //
// ver 4.0                                                               //
// Jiawei Wang, CSE687 - Object Oriented Design, Spring 2015             //
///////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo(String^ Client_Port,String^ IISPort, String^ Client_Repo)
{
  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr_ = pObjFact->createSendr();
  pRecvr_ = pObjFact->createRecvr();
  pClient_ = pObjFact->createClient();
  pChann_ = pObjFact->createMockChannel(pSendr_, pRecvr_,pClient_);
  pClient_->startClient(toStdString(Client_Port), toStdString(IISPort), toStdString(Client_Repo));
  pChann_->start();
  pClient_->setSender(pSendr_);
  delete pObjFact;
  recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
  recvThread->Start();
  this->Loaded +=  gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing += gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  hDemo->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DemoThePr4);
  hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
  hCategory->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::ShowAllCategory);
  hShowCategory->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::ShowFilesInCategory);
  hShowNoParent->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::ShowNoParentInCategory);


  hUploadItemButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UploadFile);
  hDownload->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DownloadFile);
  hDelete->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::DeleteFile);
  hLazyDownload->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::LazyDownloadFile);
  hIIS->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::StartIIS);
  hDisplayhtml->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::ShowAllHTML);
  hAnalysis->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::SendAnalyze);

  // set Window properties

  this->Title = "WPF C++/CLI Demo";
  this->Width = 800;
  this->Height = 600;
  // attach dock panel to Window

  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);

  // setup Window controls and views
  setUpTabControl();
  setUpStatusBar();
  setUpDemoView();
  setUpFileListView();
  setUpRepoView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
  delete pChann_;
  delete pSendr_;
  delete pRecvr_;
}

void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  //status->FontWeight = FontWeights::Bold;
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  hDemoTab->Header = "Demo";
  hFileListTab->Header = "File List";
  hRepoTab->Header = "Repository";
  hTabControl->Items->Add(hDemoTab);
  hTabControl->Items->Add(hFileListTab);
  hTabControl->Items->Add(hRepoTab);
}

void WPFCppCliDemo::setUpDemoView()
{
  Console::Write("\n  setting up Demo Tab view");
  hDemoGrid->Margin = Thickness(20);
  hDemoTab->Content = hDemoGrid;
  hDemo->Content = "One Click To Demo";
  hDemo->Height = 60;
  hDemo->Width = 240;
  hDemo->BorderThickness = Thickness(2);
  hDemo->BorderBrush = Brushes::Black;
  hDemoGrid->Children->Add(hDemo);
}

std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}



String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

void WPFCppCliDemo::addText(String^ msg)
{
	hRepoBox->Items->Clear();
	for each(String^ item in msg->Split(':'))
	{
	   if(!item->Equals(""))
	   hRepoBox->Items->Add(item);
	}
}

void WPFCppCliDemo::getMessage()
{
  // recvThread runs this function

  while (true)
  {
   //std::cout << "\n  receive thread calling getMessage()";
    std::string msg = pRecvr_->getMessage();
    String^ sMsg = toSystemString(msg);
    array<String^>^ args = gcnew array<String^>(1);
    args[0] = sMsg;

    Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
    Dispatcher->Invoke(act, args);  // must call addText on main UI thread
  }
}

void WPFCppCliDemo::setUpFileListView()
{
  Console::Write("\n  setting up FileList view");
  hFileListGrid->Margin = Thickness(20);
  hFileListTab->Content = hFileListGrid;
  RowDefinition^ hRow1Def = gcnew RowDefinition();
  hFileListGrid->RowDefinitions->Add(hRow1Def);
  Border^ hBorder1 = gcnew Border();
  hBorder1->BorderThickness = Thickness(1);hBorder1->BorderBrush = Brushes::Black;
  hListBox->SelectionMode = SelectionMode::Multiple;
  hBorder1->Child = hListBox;
  hFileListGrid->SetRow(hBorder1, 0);hFileListGrid->SetColumn(hBorder1, 0);
  hFileListGrid->SetColumnSpan(hBorder1, 2);hFileListGrid->Children->Add(hBorder1);
  RowDefinition^ hRow2Def = gcnew RowDefinition();
  hRow2Def->Height = GridLength(45);
  RowDefinition^ hRow2Def2 = gcnew RowDefinition();
  hRow2Def2->Height = GridLength(45);
  hFileListGrid->RowDefinitions->Add(hRow2Def);hFileListGrid->RowDefinitions->Add(hRow2Def2);
  ColumnDefinition^ hCol1Def = gcnew ColumnDefinition();
  hCol1Def->Width = GridLength(300);
  ColumnDefinition^ hCol2Def = gcnew ColumnDefinition();
  hFileListGrid->ColumnDefinitions->Add(hCol1Def);
  hFileListGrid->ColumnDefinitions->Add(hCol2Def);
  hFolderBrowseButton->Content = "Browse Directory";
  hFolderBrowseButton->Height = 30;hFolderBrowseButton->Width = 120;
  hFolderBrowseButton->BorderThickness = Thickness(2);hFolderBrowseButton->BorderBrush = Brushes::Black;
  hFileListGrid->SetRow(hFolderBrowseButton, 1);hFileListGrid->SetColumn(hFolderBrowseButton, 1);
 // hFileListGrid->SetColumnSpan(hFolderBrowseButton, 2);
  hFileListGrid->Children->Add(hFolderBrowseButton);
  GuideCategory->Text = "Input Category here";
  GuideCategory->Height = 30;GuideCategory->Width = 120;
  hFileListGrid->SetRow(GuideCategory, 1);hFileListGrid->SetColumn(GuideCategory, 0);
  hFileListGrid->Children->Add(GuideCategory);
  //Category box
  Category->Height = 30;Category->Width = 120;
  hFileListGrid->SetRow(Category, 2);hFileListGrid->SetColumn(Category, 0);
  hFileListGrid->Children->Add(Category);
  // Upload selected items button.
  hUploadItemButton->Content = "Upload Selected Items";
  hUploadItemButton->Height = 30;hUploadItemButton->Width = 120;
  hUploadItemButton->BorderThickness = Thickness(2);
  hUploadItemButton->BorderBrush = Brushes::Black;
  hFileListGrid->SetRow(hUploadItemButton,2);
  hFileListGrid->SetColumn(hUploadItemButton,1);
  hFileListGrid->Children->Add(hUploadItemButton);
  hFolderBrowserDialog->ShowNewFolderButton = false;
  hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

void WPFCppCliDemo::setUpRepoView(){
	Console::Write("\n  setting up RepoList view");
	hRepoGrid->Margin = Thickness(20);
	hRepoTab->Content = hRepoGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hRepoGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hRepoBox->SelectionMode = SelectionMode::Multiple;
	hBorder1->Child = hRepoBox;
	hRepoGrid->SetRow(hBorder1, 0);hRepoGrid->SetColumn(hBorder1, 0);
	hRepoGrid->SetColumnSpan(hBorder1,3);
	hRepoGrid->Children->Add(hBorder1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(50);
	RowDefinition^ hRow3Def = gcnew RowDefinition();
	hRow3Def->Height = GridLength(50);
	RowDefinition^ hRow4Def = gcnew RowDefinition();
	hRow4Def->Height = GridLength(50);
	hRepoGrid->RowDefinitions->Add(hRow2Def);
	hRepoGrid->RowDefinitions->Add(hRow3Def);
	hRepoGrid->RowDefinitions->Add(hRow4Def);

	ColumnDefinition^ hCol1Def = gcnew ColumnDefinition();
	hCol1Def->Width = GridLength(300);
	ColumnDefinition^ hCol2Def = gcnew ColumnDefinition();
	hCol2Def->Width = GridLength(200);
	ColumnDefinition^ hCol3Def = gcnew ColumnDefinition();
	hRepoGrid->ColumnDefinitions->Add(hCol1Def);
	hRepoGrid->ColumnDefinitions->Add(hCol2Def);
	hRepoGrid->ColumnDefinitions->Add(hCol3Def);

	hCategory->Content = "Show All Categories";
	hCategory->Height = 30;hCategory->Width = 150;
	hCategory->BorderThickness = Thickness(2);
	hCategory->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hCategory, 1);hRepoGrid->SetColumn(hCategory, 0);
	hRepoGrid->Children->Add(hCategory);

	hShowCategory->Content = "Show Content in Category";
	hShowCategory->Height = 30;hShowCategory->Width = 150;
	hShowCategory->BorderThickness = Thickness(2);
	hShowCategory->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hShowCategory, 1);hRepoGrid->SetColumn(hShowCategory, 1);
	hRepoGrid->Children->Add(hShowCategory);
	setUpRepoButton();	
}
void WPFCppCliDemo::setUpRepoButton()
{
	hShowNoParent->Content = "Show no Parent in Category";
	hShowNoParent->Height = 30;hShowNoParent->Width = 150;
	hShowNoParent->BorderThickness = Thickness(2);hShowNoParent->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hShowNoParent, 1);hRepoGrid->SetColumn(hShowNoParent, 2);
	hRepoGrid->Children->Add(hShowNoParent);

	hDisplayhtml->Content = "Display HTML";
	hDisplayhtml->Height = 30;hDisplayhtml->Width = 150;
	hDisplayhtml->BorderThickness = Thickness(2);hDisplayhtml->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hDisplayhtml, 2);hRepoGrid->SetColumn(hDisplayhtml, 1);
	hRepoGrid->Children->Add(hDisplayhtml);

	hAnalysis->Content = "Analyze the Repository";
	hAnalysis->Height = 30;hAnalysis->Width = 150;
	hAnalysis->BorderThickness = Thickness(2);hAnalysis->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hAnalysis, 2);hRepoGrid->SetColumn(hAnalysis, 2);
	hRepoGrid->Children->Add(hAnalysis);

	hDelete->Content = "Delete";
	hDelete->Height = 30;hDelete->Width = 150;
	hDelete->BorderThickness = Thickness(2);hDelete->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hDelete, 2);hRepoGrid->SetColumn(hDelete, 0);
	hRepoGrid->Children->Add(hDelete);

	hDownload->Content = "Download";
	hDownload->Height = 30;hDownload->Width = 150;
	hDownload->BorderThickness = Thickness(2);hDownload->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hDownload, 3);hRepoGrid->SetColumn(hDownload, 0);
	hRepoGrid->Children->Add(hDownload);

	hLazyDownload->Content = "Lazy Download";
	hLazyDownload->Height = 30;hLazyDownload->Width = 150;
	hLazyDownload->BorderThickness = Thickness(2);hLazyDownload->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hLazyDownload, 3);hRepoGrid->SetColumn(hLazyDownload, 1);
	hRepoGrid->Children->Add(hLazyDownload);

	hIIS->Content = "IIS Server";
	hIIS->Height = 30;hIIS->Width = 150;
	hIIS->BorderThickness = Thickness(2);hIIS->BorderBrush = Brushes::Black;
	hRepoGrid->SetRow(hIIS, 3);hRepoGrid->SetColumn(hIIS, 2);
	hRepoGrid->Children->Add(hIIS);
}
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
  std::cout << "\n  Browsing for folder";
  hListBox->Items->Clear();
  System::Windows::Forms::DialogResult result;
  result = hFolderBrowserDialog->ShowDialog();
  if (result == System::Windows::Forms::DialogResult::OK)
  {
    String^ path = hFolderBrowserDialog->SelectedPath;
    std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
    array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
    for (int i = 0; i < files->Length; ++i)
      hListBox->Items->Add(files[i]);
    array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
    for (int i = 0; i < dirs->Length; ++i)
      hListBox->Items->Add(L"<> " + dirs[i]);
  }
}
void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
}
void WPFCppCliDemo::DemoThePr4(Object^ sender, RoutedEventArgs^ args)
{
	pClient_->OneClickToDemo();
}
void WPFCppCliDemo::ShowAllCategory(Object^ sender, RoutedEventArgs^ args) {
	hRepoBox->SelectionMode = SelectionMode::Single;
	temp_category = ""; IfInNoparent = false;
	pClient_->requestAllCategory();
	//hRepoBox->Items->Clear();

}
void WPFCppCliDemo::ShowFilesInCategory(Object^ sender, RoutedEventArgs^ args) {
	hRepoBox->SelectionMode = SelectionMode::Multiple;
	if (hRepoBox->SelectedIndex == -1) return;
	temp_category = hRepoBox->SelectedItem->ToString(); IfInNoparent = false;
	pClient_->requestAllfileInCategory(toStdString(hRepoBox->SelectedItem->ToString()));
	//hRepoBox->Items->Clear();
}
void WPFCppCliDemo::ShowNoParentInCategory(Object^ sender, RoutedEventArgs^ args) {
	hRepoBox->SelectionMode = SelectionMode::Multiple;
	if (hRepoBox->SelectedIndex == -1) return;
	temp_category = hRepoBox->SelectedItem->ToString(); IfInNoparent = true;
	pClient_->requestNoParentsInCategory(toStdString(hRepoBox->SelectedItem->ToString()));
	//hRepoBox->Items->Clear();
}
void WPFCppCliDemo::StartIIS(Object^ sender, RoutedEventArgs^ args)
{
	pClient_->startIIS();
}
void WPFCppCliDemo::DownloadFile(Object^ sender, RoutedEventArgs^ args) {
	//std::string msgBody;
	if (hRepoBox->SelectedIndex == -1) return;
	for each(String^ item in hRepoBox->SelectedItems)
	{
		pClient_->download(toStdString(temp_category+"\\"+item));
	}
}
void WPFCppCliDemo::LazyDownloadFile(Object^ sender, RoutedEventArgs^ args) {
	if (hRepoBox->SelectedIndex == -1) return;
	for each(String^ item in hRepoBox->SelectedItems)
	{
		pClient_->lazydownload(toStdString(item));
	}
}
void WPFCppCliDemo::ShowAllHTML(Object^ sender, RoutedEventArgs^ args) {
	hRepoBox->SelectionMode = SelectionMode::Multiple;
	temp_category = ""; IfInNoparent = false;
	pClient_->requestAllHTMLfileNames();
	//hRepoBox->Items->Clear();
}
void WPFCppCliDemo::UploadFile(Object^ sender, RoutedEventArgs^ args) {
	std::string cata = toStdString(Category->Text->ToString());
	for each(String^ item in hListBox->SelectedItems)
	{
		pClient_->sendfile(toStdString(item), cata);
	}
}
void WPFCppCliDemo::DeleteFile(Object^ sender, RoutedEventArgs^ args)
{
	if (hRepoBox->SelectedIndex == -1) return;
	if (hRepoBox->SelectionMode == SelectionMode::Single) {  //delete category
		String^ elem = hRepoBox->SelectedItem->ToString();
		if (!temp_category->Equals(""))
			pClient_->deletefile(toStdString(hRepoBox->SelectedItem->ToString()), toStdString(temp_category));
		else
			pClient_->deletefile("", toStdString(elem));
		hRepoBox->Items->Remove(elem);
	}
	else //delete html or files in category
	{
		for each (String^ elem in hRepoBox->SelectedItems)
		{
			if (!temp_category->Equals(""))
				pClient_->deletefile(toStdString(elem), toStdString(temp_category));
			else
				pClient_->deletefile("", toStdString(elem));
			if (elem->Contains(".html")) pClient_->requestAllHTMLfileNames(); //it is html mode, so we should refresh thml
			else if (IfInNoparent->Equals(false))
				pClient_->requestAllfileInCategory(toStdString(temp_category));  //it is file mode, so we should refresh the files in category or no parents
			else
				pClient_->requestNoParentsInCategory(toStdString(temp_category));
		}
	}
}
void WPFCppCliDemo::SendAnalyze(Object^ sender, RoutedEventArgs^ args) {

	pClient_->requestTypeAnalysis();
	hRepoBox->Items->Clear();
}
void WPFCppCliDemo::Uploadfiles(Object^ sender, RoutedEventArgs^ args){
	std::string category = toStdString(Category->Text);
	for each(String^ item in hRepoBox->SelectedItems)
	{
		pClient_->sendfile(toStdString(item),category);
	}
}

#ifdef TESTWPF
[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
  Console::WriteLine(L"\n Starting WPFCppCliDemo");
  Application^ app = gcnew Application();
  app->Run(gcnew WPFCppCliDemo(args[0],args[1],args[2]));
  Console::WriteLine(L"\n\n");
}
#endif