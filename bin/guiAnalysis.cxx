#include <cstdlib>
#include <iostream>

#include <TROOT.h>
#include <TApplication.h>
#include <MainFrame.h>


#include <vector>
#include <HistoContainer.h>
#include <MyHistogramWrapper.h>
#include <TH1F.h>

int main(int argc, char **argv){
   TApplication theApp("App", &argc, argv);

  ///Container for histoCreators
	HistoContainer hc("config.json");

   if(hc.isGood()){
     hc.createLogicalHistos();
     hc.buildGuiHistos();
   }

  const vector<MyHistogramWrapper*> &guiHistos =  hc.getGuiHistos();

  for(auto &aHisto :guiHistos) aHisto->setHisto();
	for(auto &aHisto :guiHistos) aHisto->getHisto()->Print();
  for(auto &aHisto :guiHistos) aHisto->setCutLow(5);
  for(auto &aHisto :guiHistos) aHisto->setHisto();
  for(auto &aHisto :guiHistos) aHisto->getHisto()->Print();

   MainFrame mainWindow(gClient->GetRoot(),0, 0);
   theApp.Run();

   return 0;
}
