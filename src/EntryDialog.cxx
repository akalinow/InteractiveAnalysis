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

#include "EntryDialog.h"
#include "MainFrame.h"
#include "HistoManager.h"

/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
EntryDialog::EntryDialog(const TGWindow * p, MainFrame * aFrame)
 : TGCompositeFrame(p, 10, 10, kHorizontalFrame), theMainFrame(aFrame){
   // build widgets

   // use hierarchical cleani
   SetCleanup(kDeepCleanup);

   TGGC myGC = *fClient->GetResourcePool()->GetFrameGC();
   TGFont *myfont = fClient->GetFont("-adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1");
   if (myfont) myGC.SetFont(myfont->GetFontHandle());


   fF1 = new TGVerticalFrame(this, 200, 300);
   fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2);
   AddFrame(fF1, fL1);
   fL2 = new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2);
   fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2);
   fL4 = new TGLayoutHints(kLHintsRight, 2, 2, 2, 2);

   fGframe = new TGGroupFrame(fF1, "Number of DATA events:");
   fDataLabel = new TGLabel(fGframe, "No input.");
   fGframe->AddFrame(fDataLabel, new TGLayoutHints(kLHintsTop | kLHintsLeft,
                                               5, 5, 5, 5));
   fF1->AddFrame(fGframe, new TGLayoutHints(kLHintsExpandX, 2, 2, 1, 1));

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
EntryDialog::~EntryDialog(){
   // dtor
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void EntryDialog::initialize(HistoManager * aHistoManager){

   fGframe1 = new TGGroupFrame(fF1, "Event selections:");

 for(unsigned int iHisto=0;iHisto<4;++iHisto){
     TH1F *aHisto = aHistoManager->getGuiPrimaryHisto(iHisto)->getHisto();
     std::string hName(aHisto->GetName());
     float lowX = aHisto->GetBinLowEdge(0);
     float highX = aHisto->GetBinLowEdge(aHisto->GetNbinsX());
     float step = aHisto->GetBinWidth(1);
     addHistoCutsFrame(hName, lowX, highX, step);
   }
      fF1->AddFrame(fGframe1, new TGLayoutHints(kLHintsExpandX, 2, 2, 1, 1));
      fF1->Layout();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void EntryDialog::addHistoCutsFrame(const std::string &hName, float lowX, float highX, float step){

      TGGC myGC = *fClient->GetResourcePool()->GetFrameGC();
      TGFont *myfont = fClient->GetFont("-adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1");
      if (myfont) myGC.SetFont(myfont->GetFontHandle());

      TGHorizontalFrame *aHorizontalFrame = new TGHorizontalFrame(fGframe1, 200, 30);
      fF.push_back(aHorizontalFrame);
      fGframe1->AddFrame(aHorizontalFrame, fL2);

      TGNumberEntry *aNumberEntry = new TGNumberEntry(aHorizontalFrame,lowX,5,0, TGNumberFormat::EStyle::kNESRealTwo);
      aNumberEntry->Connect("ValueSet(Long_t)","MainFrame",theMainFrame,"ProcessMessage(Long_t)");
      aNumberEntry->Associate(this);
      fLowCuts.push_back(aNumberEntry);
      aHorizontalFrame->AddFrame(aNumberEntry, fL2);

      TGLabel *aLabel = new TGLabel(aHorizontalFrame, ("<"+hName+"<").c_str(), myGC(), myfont->GetFontStruct());
      fLabel.push_back(aLabel);
      aHorizontalFrame->AddFrame(aLabel, fL2);

      aNumberEntry = new TGNumberEntry(aHorizontalFrame,highX,5, 0, TGNumberFormat::EStyle::kNESRealTwo);
      aNumberEntry->Connect("ValueSet(Long_t)","MainFrame",theMainFrame,"ProcessMessage(Long_t)");
      aNumberEntry->Associate(this);
      fHighCuts.push_back(aNumberEntry);
      aHorizontalFrame->AddFrame(aNumberEntry, fL2);


}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void EntryDialog::HandleCutChanged(Int_t iCut, Bool_t isLow, Float_t value, Int_t nDataEvents){

  if(isLow) fLowCuts[iCut]->SetNumber(value);
  else fHighCuts[iCut]->SetNumber(value);

  fDataLabel->SetText(Form("%ld",nDataEvents));

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
Bool_t EntryDialog::ProcessMessage(Long_t msg, Long_t parm1, Long_t /*parm2*/){
   switch (GET_MSG(msg)) {
   case kC_COMMAND:
      {
         switch (GET_SUBMSG(msg)) {
         case kCM_BUTTON:
            {
               switch (parm1) {
                  // exit button
               case 1:
                  {
                     std::cout<<"EntryDialog::ProcessMessage(): msg: "<<msg<<std::endl;
                     break;
                  }
                  // set button
               case 2:
                  {
                     break;
                  }
               }
               break;
            }
         }
         break;
      }
   }
   return kTRUE;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
