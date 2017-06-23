#include <cstdlib>
#include <iostream>

#include <TApplication.h>
#include <MainFrame.h>

#include <TStyle.h>
#include <TH1F.h>
#include <TGaxis.h>
#include <TLine.h>
#include <TFrame.h>

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
MainFrame::MainFrame(const TGWindow *p, UInt_t w, UInt_t h)
      : TGMainFrame(p, w, h){

   SetCleanup(kDeepCleanup);
   SetWMPosition(500,0);
   SetWMSize(1000,700);

   AddTopMenu();
   SetTheFrame();
   AddHistoCanvas();
   AddButtons();
   AddNumbersDialog();
   SetWindowName("guiAnalysis");
   MapSubwindows();

   Resize();
   MapWindow();
 }
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
MainFrame::~MainFrame(){
   // Delete all created widgets.
   delete fMenuFile;
   delete fMenuHelp;
   delete fMenuBar;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::AddTopMenu(){

  fMenuDock = new TGDockableFrame(this);
   AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));
   fMenuDock->SetWindowName("GuiTest Menu");

   TGLayoutHints * menuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
   TGLayoutHints * menuBarItemLayout = new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 4, 0, 0);
   TGLayoutHints * menuBarHelpLayout = new TGLayoutHints(kLHintsTop | kLHintsRight);

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
   fMenuBar->AddPopup("&File", fMenuFile, menuBarItemLayout);
   fMenuBar->AddPopup("&Help", fMenuHelp, menuBarHelpLayout);

   fMenuDock->AddFrame(fMenuBar, menuBarLayout);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::SetTheFrame(){

   fFrame = new TGCompositeFrame(this,400,400,kSunkenFrame);
   TGTableLayout* tlo = new TGTableLayout(fFrame, 6, 6, 1);
   fFrame->SetLayoutManager(tlo);
   fFrameLayout = new TGLayoutHints(kLHintsTop|kLHintsLeft|
                                          kLHintsExpandX|kLHintsExpandY);
   AddFrame(fFrame,fFrameLayout);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::AddHistoCanvas(){

    // The Canvas
   TRootEmbeddedCanvas* recanvas = new TRootEmbeddedCanvas("Histograms",fFrame,700,700);
   fTCanvasLayout = new TGTableLayoutHints(0,4,0,6,
                                 kLHintsExpandX|kLHintsExpandY |
                                 kLHintsShrinkX|kLHintsShrinkY |
                                 kLHintsFillX|kLHintsFillY);
   fFrame->AddFrame(recanvas,fTCanvasLayout);

   TCanvas *fCanvas = recanvas->GetCanvas();
   fCanvas->MoveOpaque(kFALSE);
   fCanvas->Divide(3,3);
   //TVirtualPad *fPad = fCanvas->GetPad(1);
   fCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
                              "MainFrame", this,
                              "HandleEmbeddedCanvas(Int_t,Int_t,Int_t,TObject*)");

   TH1F *hTest = new TH1F("hTest","",10,-1,1);
   TH1F *hTest1 = new TH1F("hTest1","",10,-1,1);

   hTest->FillRandom("gaus",100);
   hTest1->FillRandom("gaus",1000);
   gStyle->SetOptStat(0);

   fCanvas->cd(1);
   hTest->DrawCopy();

   gPad->Update();
   Float_t rightmax = 1.1*hTest1->GetMaximum();
   Float_t scale = gPad->GetUymax()/rightmax;
   hTest1->SetLineColor(kRed);
   hTest1->Scale(scale);
   hTest1->DrawCopy("same");

   //draw an axis on the right side
   TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
         gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
   axis->SetLineColor(kRed);
   axis->SetLabelColor(kRed);
   axis->Draw();

   fCanvas->cd(2);
   hTest1->DrawCopy();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::AddButtons(){

   // The shape buttons
   const char* shape_button_name[] = {
       "Ellipse", "Box", "Triangle", "Arrow", "Zoom Out", "Zoom In", "Close"
   };
   UInt_t ind;
   for (ind = 0; ind < 6; ++ind) {
      TGTextButton* button = new TGTextButton(fFrame,shape_button_name[ind],ind);
      TGTableLayoutHints *tloh = new TGTableLayoutHints(4,5,ind, ind+1,
                                    kLHintsExpandX|kLHintsExpandY |
                                    kLHintsShrinkX|kLHintsShrinkY |
                                    kLHintsFillX|kLHintsFillY);
      fFrame->AddFrame(button,tloh);
      button->Resize(50,button->GetDefaultHeight());
      //button->Connect("Clicked()","Viewer",this,"DoButton()");
   }
 }
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::AddNumbersDialog(){

   fEntryDialog = new EntryDialog(fFrame);
   TGTableLayoutHints *tloh = new TGTableLayoutHints(5,6,0,6,
                                    kLHintsExpandX|kLHintsExpandY |
                                    kLHintsShrinkX|kLHintsShrinkY |
                                    kLHintsFillX|kLHintsFillY);
   fFrame->AddFrame(fEntryDialog,tloh);
 }
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::CloseWindow(){
   // Got close message for this MainFrame. Terminate the application
   // or returns from the TApplication event loop (depending on the
   // argument specified in TApplication::Run()).

   gApplication->Terminate(0);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Bool_t MainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t){
   // Handle messages send to the MainFrame object. E.g. all menu button
   // messages.

   std::cout<<"msg: "<<msg<<" parm1 "<<parm1<<std::endl;

   return kTRUE;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::HandleEmbeddedCanvas(Int_t event, Int_t x, Int_t y,
                                      TObject *sel)
{
  ///Enums defined in Buttons.h ROOT file.
  if(event == kMouseMotion){
     TObject *select = gPad->GetSelected();
     if(select && std::string(select->GetName())=="TFrame"){

    TVirtualPad *aCurrentPad = gPad->GetSelectedPad();
    aCurrentPad->cd();
    int padNumber = aCurrentPad->GetNumber();
    if(padNumber==1) return;

     float localX = aCurrentPad->AbsPixeltoX(x);
     gVirtualX->DrawLine(localX,0,localX,1);
     /*
     TLine aLine(0,0,0,0);
     aLine.SetLineColor(2);
     aLine.DrawLine(localX,0,localX,1);
     aLine.SetLineWidth(0);
     */
     //aCurrentPad->Update();
     std::cout<<" localX = "<<localX<<std::endl;
   }
 }
  if(event == kButton2Double) std::cout<<"event: "<<event<<" x = "<<x<<" y ="<<y<<std::endl;

}
////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
