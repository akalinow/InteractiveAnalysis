#include <cstdlib>
#include <iostream>

#include <TROOT.h>
#include <TApplication.h>
#include <TVirtualX.h>
#include <TVirtualPadEditor.h>
#include <TGResourcePool.h>
#include <TGListBox.h>
#include <TGListTree.h>
#include <TGFSContainer.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGIcon.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGMsgBox.h>
#include <TGMenu.h>
#include <TGCanvas.h>
#include <TGComboBox.h>
#include <TGTab.h>
#include <TGSlider.h>
#include <TGDoubleSlider.h>
#include <TGFileDialog.h>
#include <TGTextEdit.h>
#include <TGShutter.h>
#include <TGProgressBar.h>
#include <TGColorSelect.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TEnv.h>
#include <TFile.h>
#include <TKey.h>
#include <TGDockableFrame.h>
#include <TGFontDialog.h>

#include <TCanvas.h>
#include <TGTableLayout.h>
#include <EntryDialog.h>

enum ETestCommandIdentifiers {
   M_FILE_OPEN,
   M_FILE_SAVE,
   M_FILE_SAVEAS,
   M_FILE_PRINT,
   M_FILE_PRINTSETUP,
   M_FILE_EXIT,

   M_TEST_DLG,
   M_TEST_MSGBOX,
   M_TEST_SLIDER,
   M_TEST_SHUTTER,
   M_TEST_DIRLIST,
   M_TEST_FILELIST,
   M_TEST_PROGRESS,
   M_TEST_NUMBERENTRY,
   M_TEST_FONTDIALOG,
   M_TEST_NEWMENU,

   M_VIEW_ENBL_DOCK,
   M_VIEW_ENBL_HIDE,
   M_VIEW_DOCK,
   M_VIEW_UNDOCK,

   M_HELP_CONTENTS,
   M_HELP_SEARCH,
   M_HELP_ABOUT,

   M_CASCADE_1,
   M_CASCADE_2,
   M_CASCADE_3,

   M_NEW_REMOVEMENU,

   VId1,
   HId1,
   VId2,
   HId2,

   VSId1,
   HSId1,
   VSId2,
   HSId2,

   ColorSel
};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
class HistoFrame : public TGCompositeFrame {

private:
   TGCanvas *fCanvas;

public:
   HistoFrame(const TGWindow *p);
   virtual ~HistoFrame() { }

   void SetCanvas(TGCanvas *canvas) { fCanvas = canvas; }
   Bool_t HandleButton(Event_t *event);
   Bool_t HandleMotion(Event_t *event);
};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
class MainFrame : public TGMainFrame {

private:

   TGDockableFrame    *fMenuDock;

   TGCompositeFrame   *fFrame;
   TGCanvas           *fCanvasWindow;
   HistoFrame          *fContainer;

   TGMenuBar          *fMenuBar;
   TGPopupMenu        *fMenuFile, *fMenuHelp;

   TGLayoutHints      *fMenuBarLayout, *fMenuBarItemLayout, *fMenuBarHelpLayout;
   TGLayoutHints      *fFrameLayout;
   TGTableLayoutHints *fTCanvasLayout;

   EntryDialog *fEntryDialog;

public:
   MainFrame(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~MainFrame();

   virtual void CloseWindow();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);
};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
HistoFrame::HistoFrame(const TGWindow *p) :
   TGCompositeFrame(p, 10, 10, kHorizontalFrame, GetWhitePixel())
{
   // Create tile view container. Used to show colormap.

   fCanvas = 0;
   SetLayoutManager(new TGTileLayout(this, 8));

   // Handle only buttons 4 and 5 used by the wheel mouse to scroll
  gVirtualX->GrabPointer(fId, kNoEventMask, kNone, kNone);

  gVirtualX->GrabButton(fId, kButton1, kAnyModifier,
                         kButtonPressMask | kButtonReleaseMask,
                         kNone, kNone);

  gVirtualX->GrabButton(fId, kButton3, kAnyModifier,
                         kButtonPressMask | kButtonReleaseMask,
                         kNone, kNone);

   gVirtualX->GrabButton(fId, kButton4, kAnyModifier,
                         kButtonPressMask | kButtonReleaseMask,
                         kNone, kNone);
   gVirtualX->GrabButton(fId, kButton5, kAnyModifier,
                         kButtonPressMask | kButtonReleaseMask,
                         kNone, kNone);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Bool_t HistoFrame::HandleMotion(Event_t *event){

   std::cout<<"HistoFrame::HandleMotion "<<event->fCode<<std::endl;

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Bool_t HistoFrame::HandleButton(Event_t *event){


   std::cout<<"HistoFrame::HandleButton "<<event->fCode<<std::endl;
   std::cout<<"height: "<<fCanvas->GetContainer()->GetHeight()
            <<" fId: "<<fId
            <<" TGFrame: "<<GetHeight()<<std::endl;

   return kTRUE;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
MainFrame::MainFrame(const TGWindow *p, UInt_t w, UInt_t h)
      : TGMainFrame(p, w, h){

   SetCleanup(kDeepCleanup);
   SetWMPosition(500,0);
   SetWMSize(1000,700);

   fMenuDock = new TGDockableFrame(this);
   AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));
   fMenuDock->SetWindowName("GuiTest Menu");

   fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
   fMenuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
   fMenuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);

   fMenuFile = new TGPopupMenu(fClient->GetRoot());
   fMenuFile->AddEntry("&Open...", M_FILE_OPEN);
   fMenuFile->AddEntry("&Save", M_FILE_SAVE);
   fMenuFile->AddEntry("S&ave as...", M_FILE_SAVEAS);
   fMenuFile->AddEntry("&Close", -1);
   fMenuFile->AddSeparator();
   fMenuFile->AddEntry("&Print", M_FILE_PRINT);
   fMenuFile->AddEntry("P&rint setup...", M_FILE_PRINTSETUP);
   fMenuFile->AddSeparator();
   fMenuFile->AddEntry("E&xit", M_FILE_EXIT);

   fMenuFile->DisableEntry(M_FILE_SAVEAS);
   fMenuFile->HideEntry(M_FILE_PRINT);

   fMenuHelp = new TGPopupMenu(fClient->GetRoot());
   fMenuHelp->AddEntry("&Contents", M_HELP_CONTENTS);
   fMenuHelp->AddEntry("&Search...", M_HELP_SEARCH);
   fMenuHelp->AddSeparator();
   fMenuHelp->AddEntry("&About", M_HELP_ABOUT);

   // Menu button messages are handled by the main frame (i.e. "this")
   // ProcessMessage() method.
   fMenuFile->Associate(this);
   fMenuHelp->Associate(this);

   fMenuBar = new TGMenuBar(fMenuDock, 1, 1, kHorizontalFrame);
   fMenuBar->AddPopup("&File", fMenuFile, fMenuBarItemLayout);
   fMenuBar->AddPopup("&Help", fMenuHelp, fMenuBarHelpLayout);

   fMenuDock->AddFrame(fMenuBar, fMenuBarLayout);

   fFrame = new TGCompositeFrame(this,400,400,kSunkenFrame);
   TGTableLayout* tlo = new TGTableLayout(fFrame, 6, 6, 1);
   fFrame->SetLayoutManager(tlo);
   fFrameLayout = new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                          kLHintsExpandX|kLHintsExpandY);
   AddFrame(fFrame,fFrameLayout);

/*
   fCanvasWindow = new TGCanvas(this, w, h);
   fContainer = new HistoFrame(fCanvasWindow->GetViewPort());
   fContainer->SetCanvas(fCanvasWindow);
   fCanvasWindow->SetContainer(fContainer);

   // use hierarchical cleaning for container
   fContainer->SetCleanup(kDeepCleanup);
/*
   for (int i=0; i < 8; ++i)
      fCanvasWindow->AddFrame(new TGFrame(fCanvasWindow->GetContainer(),
                              32, 32, 0, TColor::RGB2Pixel(0,0,(50*i+1)&255)),
                              new TGLayoutHints(kLHintsExpandY | kLHintsRight));
*/
   //fFrame->AddFrame(fCanvasWindow, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,0, 0, 2, 2));

    // The Canvas
   TRootEmbeddedCanvas* recanvas = new TRootEmbeddedCanvas("Shapes",fFrame,700,700);
   fTCanvasLayout = new TGTableLayoutHints(0,3,0,6,
                                 kLHintsExpandX|kLHintsExpandY |
                                 kLHintsShrinkX|kLHintsShrinkY |
                                 kLHintsFillX|kLHintsFillY);
   fFrame->AddFrame(recanvas,fTCanvasLayout);

   TCanvas *fCanvas = recanvas->GetCanvas();
   fCanvas->Draw();
   TH1F *hTest = new TH1F("hTest","",10,0,1);
   hTest->Draw();

   // The shape buttons
   const char* shape_button_name[] = {
       "Ellipse", "Box", "Triangle", "Arrow", "Zoom Out", "Zoom In", "Close"
   };
   UInt_t ind;
   for (ind = 0; ind < 6; ++ind) {
      TGTextButton* button = new TGTextButton(fFrame,shape_button_name[ind],ind);
      TGTableLayoutHints *tloh = new TGTableLayoutHints(3,4,ind, ind+1,
                                    kLHintsExpandX|kLHintsExpandY |
                                    kLHintsShrinkX|kLHintsShrinkY |
                                    kLHintsFillX|kLHintsFillY);
      fFrame->AddFrame(button,tloh);
      button->Resize(100,button->GetDefaultHeight());
      //button->Connect("Clicked()","Viewer",this,"DoButton()");
   }

   fEntryDialog = new EntryDialog(fFrame);
   TGTableLayoutHints *tloh = new TGTableLayoutHints(4,6,0,6,
                                    kLHintsExpandX|kLHintsExpandY |
                                    kLHintsShrinkX|kLHintsShrinkY |
                                    kLHintsFillX|kLHintsFillY);
   fFrame->AddFrame(fEntryDialog,tloh);

   SetWindowName("guiAnalysis");
   MapSubwindows();

   // we need to use GetDefault...() to initialize the layout algorithm...
   Resize();   // resize to default size
   MapWindow();
   Print();
 }

MainFrame::~MainFrame()
{
   // Delete all created widgets.
   delete fMenuFile;
   delete fMenuHelp;
   delete fMenuBar;
}

void MainFrame::CloseWindow()
{
   // Got close message for this MainFrame. Terminate the application
   // or returns from the TApplication event loop (depending on the
   // argument specified in TApplication::Run()).

   gApplication->Terminate(0);
}

Bool_t MainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t)
{
   // Handle messages send to the MainFrame object. E.g. all menu button
   // messages.

   std::cout<<"msg: "<<msg<<" parm1 "<<parm1<<std::endl;

   return kTRUE;
}


//---- Main program ------------------------------------------------------------
int main(int argc, char **argv){
   TApplication theApp("App", &argc, argv);

   MainFrame mainWindow(gClient->GetRoot(), 50, 500);
   theApp.Run();

   return 0;
}
