#ifndef MainFrame_H
#define MainFrame_H

#include <TGDockableFrame.h>
#include <TGFrame.h>
#include <TGCanvas.h>
#include <TGLayout.h>
#include <TGTableLayout.h>
#include <TGMenu.h>
#include <TRootEmbeddedCanvas.h>

#include <RQ_OBJECT.h>
#include <TCanvas.h>
#include <TObject.h>
#include <TClass.h>
#include <EntryDialog.h>
#include <HistoManager.h>

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
class MainFrame : public TGMainFrame {

  RQ_OBJECT("MainFrame")

public:
   MainFrame(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~MainFrame();
   virtual void CloseWindow();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

///Process message from EntryDialog
    virtual Bool_t ProcessMessage(Long_t msg);

   void HandleEmbeddedCanvas(Int_t event, Int_t x, Int_t y, TObject *sel);

   void setHistoManager(HistoManager *aHistoManager);

   void CutChanged(Int_t iCut, Bool_t isLow, Float_t value, Int_t nDataEvents);

private:

   void AddTopMenu();
   void SetTheFrame();
   void AddHistoCanvas();
   void AddButtons();
   void AddNumbersDialog();

   TGDockableFrame    *fMenuDock;

   TGCompositeFrame   *fFrame;
   TGCanvas           *fCanvasWindow;
   TCanvas            *fCanvas;

   TGMenuBar          *fMenuBar;
   TGPopupMenu        *fMenuFile, *fMenuHelp;

   TGLayoutHints      *fFrameLayout;
   TGTableLayoutHints *fTCanvasLayout;

   EntryDialog *fEntryDialog;

   HistoManager *fHistoManager;

   int fCutSide = -1;
   bool fIgnoreCursor = true;



};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#endif
