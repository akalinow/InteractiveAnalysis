#ifndef EntryDialog_H
#define EntryDialog_H

#include <TGFrame.h>
#include <TGLayout.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TGButton.h>

class EntryDialog : public TGCompositeFrame {

private:
   TGVerticalFrame      *fF1;
   TGVerticalFrame      *fF2;
   TGHorizontalFrame    *fF[13];
   TGLayoutHints        *fL1;
   TGLayoutHints        *fL2;
   TGLayoutHints        *fL3;
   TGLabel              *fLabel[13];
   TGNumberEntry        *fNumericEntries[13];
   TGNumberEntry        *fLimits[2];
   TGCheckButton        *fLowerLimit;
   TGCheckButton        *fUpperLimit;
   TGCheckButton        *fPositive;
   TGCheckButton        *fNonNegative;
   TGButton             *fSetButton;
   TGButton             *fExitButton;

   static const char *const numlabel[13];
   static const Double_t numinit[13];

public:
   EntryDialog(const TGWindow *p);
   virtual ~EntryDialog();

   void SetLimits();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);
};

#endif
