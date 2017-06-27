#include <cstdlib>
#include <iostream>
#include <fstream>

#include <TApplication.h>
#include <MainFrame.h>
#include <SelectionBox.h>

#include <TStyle.h>
#include <TH1F.h>
#include <TGaxis.h>
#include <TLine.h>
#include <TFrame.h>

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
MainFrame::MainFrame(const TGWindow *p, UInt_t w, UInt_t h)
      : TGMainFrame(p, w, h){

  fSelectionBox = 0;

   SetCleanup(kDeepCleanup);
   SetWMPosition(500,0);
   SetWMSize(1200,700);

   AddTopMenu();
   SetTheFrame();
   AddHistoCanvas();
   AddButtons();
   AddNumbersDialog();
   SetWindowName("guiAnalysis");
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
void MainFrame::setHistoManager(HistoManager *aHistoManager) {
  fHistoManager = aHistoManager;
  if(fCanvas) fHistoManager->drawHistos(fCanvas, fSelectedHistos);
  if(fEntryDialog) fEntryDialog->initialize(fHistoManager);

  MapSubwindows();
  Resize();
  MapWindow();
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
   fMenuFile->AddEntry("S&ave as...", M_FILE_SAVEAS);
   fMenuFile->AddSeparator();
   fMenuFile->AddEntry("E&xit", M_FILE_EXIT);
   fMenuFile->Connect("Activated(Int_t)", "MainFrame", this,"HandleMenu(Int_t)");

   fMenuHelp = new TGPopupMenu(fClient->GetRoot());
   fMenuHelp->AddEntry("&Contents", M_HELP_CONTENTS);
   fMenuHelp->AddEntry("&Search...", M_HELP_SEARCH);
   fMenuHelp->AddSeparator();
   fMenuHelp->AddEntry("&About", M_HELP_ABOUT);
   fMenuHelp->Connect("Activated(Int_t)", "MainFrame", this,"HandleMenu(Int_t)");

   fMenuBar = new TGMenuBar(fMenuDock, 1, 1, kHorizontalFrame);
   fMenuBar->AddPopup("&File", fMenuFile, menuBarItemLayout);
   fMenuBar->AddPopup("&Help", fMenuHelp, menuBarHelpLayout);

   fMenuDock->AddFrame(fMenuBar, menuBarLayout);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::SetTheFrame(){

   fFrame = new TGCompositeFrame(this,400,400,kSunkenFrame);
   TGTableLayout* tlo = new TGTableLayout(fFrame, 6, 12, 1);
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
   fTCanvasLayout = new TGTableLayoutHints(0,8,0,6,
                                 kLHintsExpandX|kLHintsExpandY |
                                 kLHintsShrinkX|kLHintsShrinkY |
                                 kLHintsFillX|kLHintsFillY);
   fFrame->AddFrame(recanvas,fTCanvasLayout);

   fCanvas = recanvas->GetCanvas();
   fCanvas->MoveOpaque(kFALSE);
   fCanvas->Divide(3,3);
   fCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
                              "MainFrame", this,
                              "HandleEmbeddedCanvas(Int_t,Int_t,Int_t,TObject*)");

   gStyle->SetOptStat(0);
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::AddButtons(){

   // The shape buttons
   const char* shape_button_name[] = {
       "Load thresholds", "Save thresholds", "Select histos", " ", " ", "Exit"
   };

   unsigned int button_id[6] = {M_FILE_OPEN, M_FILE_SAVEAS, M_BUTTON_SEL_HIST, 11, 12, M_FILE_EXIT};

   UInt_t ind;
   for (ind = 0; ind < 6; ++ind) {
      TGTextButton* button = new TGTextButton(fFrame,shape_button_name[ind],button_id[ind]);
      TGTableLayoutHints *tloh = new TGTableLayoutHints(8,9,ind, ind+1,
                                    kLHintsExpandX|kLHintsExpandY |
                                    kLHintsShrinkX|kLHintsShrinkY |
                                    kLHintsFillX|kLHintsFillY);
      fFrame->AddFrame(button,tloh);
      button->Resize(50,button->GetDefaultHeight());
      button->Connect("Clicked()","MainFrame",this,"DoButton()");
   }
 }
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::AddNumbersDialog(){

   fEntryDialog = new EntryDialog(fFrame, this);
   TGTableLayoutHints *tloh = new TGTableLayoutHints(9,12,0,6,
                                    kLHintsExpandX|kLHintsExpandY |
                                    kLHintsShrinkX|kLHintsShrinkY |
                                    kLHintsFillX|kLHintsFillY);
   this->Connect("CutChanged(Int_t, Bool_t, Float_t, Int_t, Int_t)", "EntryDialog",fEntryDialog,
               "HandleCutChanged(Int_t, Bool_t, Float_t, Int_t, Int_t)");
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
void MainFrame::CutChanged(Int_t iCut, Bool_t isLow, Float_t value,
                           Int_t nDataEvents, Int_t nSecondaryEvents){

EmitVA("CutChanged(Int_t, Bool_t, Float_t, Int_t, Int_t)", 5, iCut, isLow, value, nDataEvents, nSecondaryEvents);

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Bool_t MainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t){
   // Handle messages send to the MainFrame object. E.g. all menu button
   // messages.

   return kTRUE;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Bool_t MainFrame::ProcessMessage(Long_t msg){

   unsigned int nHistos = fHistoManager->getNumberOfHistos();
   for(unsigned int iHisto=0;iHisto<nHistos;++iHisto){
     float theCut = fEntryDialog->getLowCut(iHisto)->GetNumber();
     TH1F *aHisto = fHistoManager->getGuiPrimaryHisto(iHisto)->getHisto();
     int binNumber = aHisto->FindBin(theCut);
     if(binNumber>0) --binNumber;
     fHistoManager->getGuiPrimaryHisto(iHisto)->setCutLow(binNumber);
     fHistoManager->getGuiSecondaryHisto(iHisto)->setCutLow(binNumber);

     theCut = fEntryDialog->getHighCut(iHisto)->GetNumber();
     binNumber = aHisto->FindBin(theCut);
     if(binNumber>0) --binNumber;
     fHistoManager->getGuiPrimaryHisto(iHisto)->setCutHigh(binNumber);
     fHistoManager->getGuiSecondaryHisto(iHisto)->setCutHigh(binNumber);
   }

   TH1F *aHisto = fHistoManager->getGuiPrimaryHisto(0)->getHisto();
   int nDataEvents = aHisto->Integral(0,aHisto->GetNbinsX()+1);

   aHisto = fHistoManager->getGuiSecondaryHisto(0)->getHisto();
   int nSecondaryEvents = aHisto->Integral(0,aHisto->GetNbinsX()+1);

   fEntryDialog->updateEventNumbers(nDataEvents, nSecondaryEvents);
   fHistoManager->updateHistos();
   fHistoManager->drawHistos(fCanvas, fSelectedHistos);
   fCanvas->Update();

   return kTRUE;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::HandleEmbeddedCanvas(Int_t event, Int_t x, Int_t y,
                                      TObject *sel){

  ///Enums defined in Buttons.h ROOT file.
  if(event == kButton1) fIgnoreCursor=!fIgnoreCursor;
  if(event == kButton2) fCutSide*=-1;
  if(event == kMouseMotion && !fIgnoreCursor){

     TObject *select = gPad->GetSelected();
     if(select && std::string(select->GetName())=="TFrame"){

    TVirtualPad *aCurrentPad = gPad->GetSelectedPad();
    aCurrentPad->cd();
    int padNumber = aCurrentPad->GetNumber();
    if(padNumber==1 || padNumber>=fSelectedHistos.size()) return;

    int hIndex = fSelectedHistos[padNumber-1];

     float localX = aCurrentPad->AbsPixeltoX(x);
     TH1F *aHisto = fHistoManager->getGuiPrimaryHisto(hIndex)->getHisto();
     int binNumber = aHisto->FindBin(localX);

     if(fCutSide==-1){
     fHistoManager->getGuiPrimaryHisto(hIndex)->setCutLow(binNumber);
     fHistoManager->getGuiSecondaryHisto(hIndex)->setCutLow(binNumber);
   }
   if(fCutSide==+1){
      fHistoManager->getGuiPrimaryHisto(hIndex)->setCutHigh(binNumber);
     fHistoManager->getGuiSecondaryHisto(hIndex)->setCutHigh(binNumber);
   }

     bool isLow = (fCutSide==-1);
     fHistoManager->updateHistos();
     fHistoManager->drawHistos(fCanvas, fSelectedHistos);
     int nDataEvents = aHisto->Integral(0,aHisto->GetNbinsX()+1);
     float cutValue = aHisto->GetXaxis()->GetBinLowEdge(binNumber+1);
     if(!isLow) cutValue = aHisto->GetXaxis()->GetBinUpEdge(binNumber);

     aHisto = fHistoManager->getGuiSecondaryHisto(hIndex)->getHisto();
     int nSecondaryEvents = aHisto->Integral(0,aHisto->GetNbinsX()+1);
     CutChanged(hIndex, isLow, cutValue, nDataEvents, nSecondaryEvents);
     fCanvas->Update();
   }
 }
}
////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::SaveCuts(const std::string & filePath) const{

 if(!filePath.size()) return;
 std::fstream outputFile(filePath.c_str(), std::ios::out);

unsigned int nHistos = fHistoManager->getNumberOfHistos();
for(unsigned int iHisto=0;iHisto<nHistos;++iHisto){
     float theCut = fEntryDialog->getLowCut(iHisto)->GetNumber();
     TH1F *aHisto = fHistoManager->getGuiPrimaryHisto(iHisto)->getHisto();
     int binNumber = aHisto->FindBin(theCut);
     if(binNumber>0) --binNumber;

     outputFile<<binNumber<<" ";

     theCut = fEntryDialog->getHighCut(iHisto)->GetNumber();
     binNumber = aHisto->FindBin(theCut);
     if(binNumber>0) --binNumber;

     outputFile<<binNumber<<std::endl;
   }
   outputFile.close();
}
////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::LoadCuts(const std::string & filePath){

 if(!filePath.size()) return;
  std::fstream inputFile(filePath.c_str(), std::ios::in);

   int binNumberLow, binNumberHigh;
unsigned int nHistos = fHistoManager->getNumberOfHistos();
for(unsigned int iHisto=0;iHisto<nHistos;++iHisto){
     inputFile>>binNumberLow>>binNumberHigh;

     fHistoManager->getGuiPrimaryHisto(iHisto)->setCutLow(binNumberLow);
     fHistoManager->getGuiSecondaryHisto(iHisto)->setCutLow(binNumberLow);

     fHistoManager->getGuiPrimaryHisto(iHisto)->setCutHigh(binNumberHigh);
     fHistoManager->getGuiSecondaryHisto(iHisto)->setCutHigh(binNumberHigh);

     if(iHisto==nHistos-1) fHistoManager->updateHistos();

     TH1F *aHisto = fHistoManager->getGuiPrimaryHisto(iHisto)->getHisto();
     int nDataEvents = aHisto->Integral(0,aHisto->GetNbinsX()+1);
     float cutValueLow = aHisto->GetXaxis()->GetBinLowEdge(binNumberLow+1);
     float cutValueHigh = aHisto->GetXaxis()->GetBinUpEdge(binNumberHigh);

     aHisto = fHistoManager->getGuiSecondaryHisto(iHisto)->getHisto();
     int nSecondaryEvents = aHisto->Integral(0,aHisto->GetNbinsX()+1);

     CutChanged(iHisto, true, cutValueLow, nDataEvents, nSecondaryEvents);
     CutChanged(iHisto, false, cutValueHigh, nDataEvents, nSecondaryEvents);
   }
   inputFile.close();

   fHistoManager->drawHistos(fCanvas, fSelectedHistos);
   fCanvas->Update();
}
////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::HandleMenu(Int_t id){

  const char *filetypes[] = {
   "Cuts files",    "*.dat",
   "All files",     "*",
    0,               0
};

  switch (id) {
      case M_FILE_OPEN:
         {
            TGFileInfo fi;
            fi.fFileTypes = filetypes;
            fi.fIniDir    = StrDup(".");
            new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fi);
            std::string fileName;
            if(fi.fFilename) fileName.append(fi.fFilename);
            LoadCuts(fileName);
         }
         break;

      case M_FILE_SAVEAS:
      {
            TGFileInfo fi;
            fi.fFileTypes = filetypes;
            fi.fIniDir    = StrDup(".");
            fi.fFilename   = StrDup("selections.dat");
            new TGFileDialog(gClient->GetRoot(), this, kFDSave, &fi);
            std::string fileName;
            if(fi.fFilename) fileName.append(fi.fFilename);
            SaveCuts(fileName);
      }
        break;

      case M_FILE_EXIT:
         CloseWindow();   // terminate theApp no need to use SendCloseMessage()
         break;
       }
}
////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::HandleHistoSelect(Long_t msg){

    std::vector<std::string> hNames = fHistoManager->getHistoNames();
    fSelectedHistos.clear();

    const TList *aList = fSelectionBox->GetSelected();
    for(const auto&& obj: *aList){
    std::string hName(((TGTextLBEntry*)obj)->GetText()->GetString());
    for(unsigned int hIndex=0;hIndex<hNames.size();++hIndex){
      if(hName==hNames[hIndex]) fSelectedHistos.push_back(hIndex);
      if(fSelectedHistos.size()==9) break;
    }
 }
 fHistoManager->drawHistos(fCanvas, fSelectedHistos);
}
////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MainFrame::DoButton(){
 TGButton* button = (TGButton*)gTQSender;
   UInt_t button_id = button->WidgetId();

   switch (button_id) {
      case M_FILE_OPEN:
        HandleMenu(button_id);
        break;
      case M_FILE_SAVEAS:
        HandleMenu(button_id);
        break;
      case M_FILE_EXIT:
        HandleMenu(button_id);
        break;
      case M_BUTTON_SEL_HIST:
          fSelectionBox = new SelectionBox(gClient->GetRoot(), this, 400, 200);
          fSelectionBox->Initialize(fHistoManager->getHistoNames());
        break;
      }
 }
////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
