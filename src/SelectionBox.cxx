#include <SelectionBox.h>


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void SelectionBox::DoSelect()
{
   fSelected->Clear();

   if (fListBox->GetMultipleSelections()) {
      Printf("Selected entries are:\n");
      fListBox->GetSelectedEntries(fSelected);
      fSelected->ls();
   } else {
      Printf("Selected entries is: %d\n", fListBox->GetSelected());
   }

   Emit("DoSelect()");
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void SelectionBox::DoExit(){

   fMain->CloseWindow();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
SelectionBox::SelectionBox(const TGWindow *p, const TGWindow *main, UInt_t w,
                           UInt_t h, UInt_t options){

   fMain = new TGTransientFrame(p, main, w, h, options);
   fMain->Connect("CloseWindow()", "SelectionBox", this, "DoExit()");
   fMain->DontCallClose(); // to avoid double deletions.
   fMain->SetCleanup(kDeepCleanup);
   fMain->Resize(300,500);

   fFrame = new TGHorizontalFrame(fMain, 60, 20, kFixedWidth);
   fListBox = new TGListBox(fFrame, 89);
   fListBox->SetMultipleSelections(true);
   fListBox->Resize(300,500);
   fSelected = new TList;

   fFrame->AddFrame(fListBox, new TGLayoutHints(kLHintsTop | kLHintsLeft |
                                        kLHintsExpandX | kLHintsExpandY,
                                        5, 5, 5, 5));

   TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain, 250, 20, kFixedWidth);
   TGTextButton *show = new TGTextButton(hframe, "&Set");
   show->Connect("Pressed()", "SelectionBox", this, "DoSelect()");
   hframe->AddFrame(show, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));
   TGTextButton *exit = new TGTextButton(hframe, "&Exit ");
   exit->Connect("Pressed()", "SelectionBox", this, "DoExit()");
   hframe->AddFrame(exit, new TGLayoutHints(kLHintsExpandX, 5, 5, 3, 4));
   fMain->AddFrame(hframe, new TGLayoutHints(kLHintsBottom | kLHintsExpandX, 2, 2, 5, 1));

   // Set a name to the main frame
  // position relative to the parent's window
   TGLayoutHints *fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX,
                           2, 2, 2, 2);

   fMain->AddFrame(fFrame, fL1);
   fMain->CenterOnParent();
   fMain->SetWindowName("Histogram selection");
   fMain->MapWindow();

   fMain->MapSubwindows();
   fMain->Resize();

   this->Connect("DoSelect()","MainFrame",fMain,"HandleHistoSelect()");

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
SelectionBox::~SelectionBox()
{
   if (fSelected) {
      fSelected->Delete();
      delete fSelected;
   }
   fMain->DeleteWindow();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void SelectionBox::HandleButtons()
{
   // Handle check button.
   Int_t id;
   TGButton *btn = (TGButton *) gTQSender;
   id = btn->WidgetId();

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void SelectionBox::Initialize(const std::vector<std::string> & hNames){

  for(unsigned int iHisto=0;iHisto<hNames.size();++iHisto){
    fListBox->AddEntry(hNames[iHisto].c_str(), iHisto+1);
  }
  fListBox->Select(1);
  fMain->Layout();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
const TList* SelectionBox::GetSelected() const{

  return fSelected;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
