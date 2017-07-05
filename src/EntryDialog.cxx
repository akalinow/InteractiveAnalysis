#include <cstdlib>
#include <iostream>

#include <TGResourcePool.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGCanvas.h>
#include <TGTableLayout.h>
#include <TGFontDialog.h>

#include <TCanvas.h>
#include <TH1.h>

#include <EntryDialog.h>
#include <MainFrame.h>
#include <HistoManager.h>
#include <ScrollFrame.h>

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


   fF1 = new TGVerticalFrame(this, 300, 500);
   fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY |
                                    kLHintsShrinkX|kLHintsShrinkY |
                                    kLHintsFillX|kLHintsFillY, 2, 2, 2, 2);
   AddFrame(fF1, fL1);
   fL2 = new TGLayoutHints(kLHintsLeft, 2, 2, 2, 2);
   fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2);
   fL4 = new TGLayoutHints(kLHintsRight, 2, 2, 2, 2);

   fGframeDATA = new TGGroupFrame(fF1, "Number of DATA events:");
   fDataLabelDATA = new TGLabel(fGframeDATA, "No input.");
   fGframeDATA->AddFrame(fDataLabelDATA, new TGLayoutHints(kLHintsTop | kLHintsLeft,
                                               5, 5, 5, 5));
   fF1->AddFrame(fGframeDATA, new TGLayoutHints(kLHintsExpandX, 2, 2, 1, 1));

   fGframeMC = new TGGroupFrame(fF1, "Number of MC events:");
   fDataLabelMC = new TGLabel(fGframeMC, "No input.");
   fGframeMC->AddFrame(fDataLabelMC, new TGLayoutHints(kLHintsTop | kLHintsLeft,
                                               5, 5, 5, 5));
   fF1->AddFrame(fGframeMC, new TGLayoutHints(kLHintsExpandX, 2, 2, 1, 1));

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
EntryDialog::~EntryDialog(){
   // dtor
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void EntryDialog::initialize(HistoManager * aHistoManager){

   fGCanvasCuts = new TGCanvas(fF1, 500, 520);
   fContainer = new TGCompositeFrame(fGCanvasCuts->GetViewPort(), kVerticalFrame);
   fGCanvasCuts->SetContainer(fContainer);

 for(unsigned int iHisto=0;iHisto<aHistoManager->getNumberOfHistos();++iHisto){
     TH1F *aHisto = aHistoManager->getGuiPrimaryHisto(iHisto)->getHisto();
     std::string hName(aHisto->GetName());
     float lowX = aHisto->GetBinLowEdge(0);
     float highX = aHisto->GetBinLowEdge(aHisto->GetNbinsX());
     float step = aHisto->GetBinWidth(1);
     lowX = -999;
     highX = 999;
     addHistoCutsFrame(hName, lowX, highX, step);
   }
      fF1->AddFrame(fGCanvasCuts, new TGLayoutHints(kLHintsExpandX, 2, 2, 1, 1));
      fF1->Layout();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void EntryDialog::addHistoCutsFrame(const std::string &hName, float lowX, float highX, float step){

      TGGC myGC = *fClient->GetResourcePool()->GetFrameGC();
      TGFont *myfont = fClient->GetFont("-adobe-helvetica-bold-r-*-*-12-*-*-*-*-*-iso8859-1");
      if (myfont) myGC.SetFont(myfont->GetFontHandle());

      TGHorizontalFrame *aHorizontalFrame = new TGHorizontalFrame(fGCanvasCuts->GetContainer(), 200, 30);
      fF.push_back(aHorizontalFrame);
      fContainer->AddFrame(aHorizontalFrame, fL2);

      TGNumberEntry *aNumberEntry = new TGNumberEntry(aHorizontalFrame,lowX,5,0, TGNumberFormat::EStyle::kNESRealTwo);
      aNumberEntry->Connect("ValueSet(Long_t)","MainFrame",theMainFrame,"ProcessMessage(Long_t)");
      aNumberEntry->Associate(this);
      fLowCuts.push_back(aNumberEntry);
      aHorizontalFrame->AddFrame(aNumberEntry, fL2);

      TGLabel *aLabel = new TGLabel(aHorizontalFrame, ("<="+hName+"<").c_str(), myGC(), myfont->GetFontStruct());
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
void EntryDialog::HandleCutChanged(Int_t iCut, Bool_t isLow, Float_t value, Int_t nDataEvents, Int_t nSecondaryEvents){

  if(isLow) fLowCuts[iCut]->SetNumber(value);
  else fHighCuts[iCut]->SetNumber(value);

  updateEventNumbers(nDataEvents, nSecondaryEvents);

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void EntryDialog::updateEventNumbers(unsigned int nDataEvents,
                                     unsigned int nSecondaryEvents){

  fDataLabelDATA->SetText(Form("%ld",nDataEvents));
  if(nSecondaryEvents<(unsigned int)(-1)) fDataLabelMC->SetText(Form("%ld",nSecondaryEvents));
  fGCanvasCuts->Layout();
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
