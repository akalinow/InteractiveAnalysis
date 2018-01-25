#ifndef MainFrame_H
#define MainFrame_H

#include <TGDockableFrame.h>
#include <TGFrame.h>
#include <TGCanvas.h>
#include <TGLayout.h>
#include <TGTableLayout.h>
#include <TGMenu.h>
#include <TRootEmbeddedCanvas.h>
#include <TGFileDialog.h>

#include <RQ_OBJECT.h>
#include <TCanvas.h>
#include <TObject.h>
#include <TClass.h>
#include <TLine.h>
#include <TArrow.h>

#include <EntryDialog.h>
#include <HistoManager.h>
#include <SelectionBox.h>

enum ETestCommandIdentifiers {
   M_FILE_OPEN,
   M_FILE_SAVE,
   M_FILE_SAVEAS,
   M_FILE_PRINT,
   M_FILE_PRINTSETUP,
   M_FILE_EXIT,

   M_HELP_CONTENTS,
   M_HELP_SEARCH,
   M_HELP_ABOUT,

   M_BUTTON_SEL_HIST,
   M_BUTTON_RESET,
   M_BUTTON_SHOW_LEGEND
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

   void HandleHistoSelect(Long_t);

   void setHistoManager(HistoManager *aHistoManager);

   void CutChanged(Int_t iCut, Bool_t isLow, Float_t value,
                   Int_t nDataEvents, Int_t nSecondaryEvents);

   void HandleMenu(Int_t);

   void DoButton();

   void CloseLegend();

private:

   void AddTopMenu();
   void SetTheFrame();
   void AddHistoCanvas();
   void AddButtons();
   void AddNumbersDialog();

   void SaveCuts(const std::string & filePath) const;
   void LoadCuts(const std::string & filePath);
   void ResetCuts();
   void ShowLegend();
   void SetCursorTheme();

   void DrawCutMarker(unsigned int iPad, float localX);

   TGDockableFrame    *fMenuDock;

   TGCompositeFrame   *fFrame;
   TGCanvas           *fCanvasWindow;
   TCanvas            *fCanvas;

   TGMenuBar          *fMenuBar;
   TGPopupMenu        *fMenuFile, *fMenuHelp;

   TGLayoutHints      *fFrameLayout;
   TGTableLayoutHints *fTCanvasLayout;

   TGTransientFrame *fLegendMain;

   EntryDialog *fEntryDialog;

   HistoManager *fHistoManager;
   SelectionBox *fSelectionBox;

   TArrow *fArrow;
   TLine *fLine;

   int fCutSide = -1;
   bool fIgnoreCursor = true;
   std::vector<int> fSelectedHistos = {0,1,2,3,4,5,6,7,8};



};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
#endif
