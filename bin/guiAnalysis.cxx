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

  std::string configFileName;
  if(argc<1){
     std::cout<<" Usage: guiAnalysis config.json"<<std::endl;
    return 0;
   }
  else {
    configFileName.append(argv[1]);
    std::cout<<"configFileName = "<<configFileName<<std::endl;
  }

   TApplication theApp("App", &argc, argv);

  ///Container for histoCreators
	HistoManager hc(configFileName);

   if(hc.isGood()){
     hc.createLogicalHistos();
     hc.buildGuiHistos();
   }

   MainFrame mainWindow(gClient->GetRoot(),0, 0);
   mainWindow.setHistoManager(&hc);

   theApp.Run();

   return 0;
}
