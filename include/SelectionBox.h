#ifndef SelectionBox_h
#define SelectionBox_h

#include <TGClient.h>
#include <TGButton.h>
#include <TGListBox.h>
#include <TList.h>
#include <RQ_OBJECT.h>

class SelectionBox {

  RQ_OBJECT("SelectionBox")

private:

   TGTransientFrame    *fMain;
   TGHorizontalFrame   *fFrame;
   TGListBox           *fListBox;
   TList               *fSelected;

public:

   SelectionBox(const TGWindow *p, const TGWindow *main, UInt_t w, UInt_t h,
                UInt_t options = kVerticalFrame);
   virtual ~SelectionBox();

   const TList* GetSelected() const;

   void Initialize(const std::vector<std::string> &);

   void DoExit();
   void DoSelect();
   void HandleButtons();

   ClassDef(SelectionBox, 0)

};
#endif
