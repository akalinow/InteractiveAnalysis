#ifndef EntryDialog_H
#define EntryDialog_H

#include <TGFrame.h>
#include <TGLayout.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGButton.h>
#include <TGCanvas.h>
#include <TGListTree.h>

class MainFrame;
class HistoManager;

class EntryDialog : public TGCompositeFrame {

public:
   EntryDialog(const TGWindow *p, MainFrame *aFrame);

   void initialize(HistoManager *aManager);

   virtual ~EntryDialog();

   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

   void HandleCutChanged(Int_t iCut, Bool_t isLow, Float_t value, Int_t nDataEvents);

   const TGNumberEntry * getLowCut(unsigned int iCut) const {return fLowCuts[iCut];};

   const TGNumberEntry * getHighCut(unsigned int iCut) const {return fHighCuts[iCut];};


private:

  void addHistoCutsFrame(const std::string &hName, float lowX, float highX, float step);

   MainFrame *theMainFrame;

   TGVerticalFrame      *fF1;
   std::vector<TGHorizontalFrame*>   fF;
   std::vector<TGLabel*> fLabel;
   std::vector<TGNumberEntry*> fLowCuts;
   std::vector<TGNumberEntry*> fHighCuts;

   TGLayoutHints        *fL1, *fL2, *fL3, *fL4;

   TGGroupFrame *fGframeDATA, *fGframeMC;// *fGframeCuts;
   TGLabel *fDataLabelDATA, *fDataLabelMC;

   TGCanvas *fGCanvasCuts;
   TGCompositeFrame  *fContainer;

  std::vector<std::string> histoNames;
  std::vector<float> histoLowRanges, histoHighRanges;
};

#endif
