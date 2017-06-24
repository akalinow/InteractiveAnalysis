#include <cstdlib>
#include <iostream>

#include <TROOT.h>
#include <TApplication.h>
#include <MainFrame.h>


#include <vector>
#include <HistoManager.h>
#include <MyHistogramWrapper.h>
#include <TH1F.h>

int main(int argc, char **argv){
   TApplication theApp("App", &argc, argv);

  ///Container for histoCreators
	HistoManager hc("config.json");

   if(hc.isGood()){
     hc.createLogicalHistos();
     hc.buildGuiHistos();
   }

   MainFrame mainWindow(gClient->GetRoot(),0, 0);
   mainWindow.setHistoManager(&hc);
   
   theApp.Run();

   return 0;
}
