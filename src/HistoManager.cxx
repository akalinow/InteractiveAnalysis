/**
 @file HistoManager.cpp
 @brief Implementation of container for HistoCreators
 @author   kzawisto
 @created:   2015-11-04
 */

#include <MyHistogramWrapper.h>
#include "HistoManager.h"
#include"HistoCreator.h"
#include<cstdlib>
#include<sys/stat.h>
#include<fstream>
#include<iostream>

#include <TCanvas.h>
#include <TH1F.h>
#include <TAxis.h>
#include <TGaxis.h>


/// to test whether config_json exists
bool fileExists(const std::string & name){
	ifstream f(name.c_str());
	bool ret=(f.good());
	f.close();
	return ret;

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
HistoManager::HistoManager(string json_path) {
		ptree pt;

		if(! fileExists(json_path)){
			cout<<"ERROR: No such file: "<<json_path<<"\n";
			cout<<"Current working dir is"<<endl;
			system("pwd");
			good = false;
		}
		else{
			good = true;
		read_json(json_path,pt);
		for(auto & p : pt){
			if(p.first.substr(0,3) =="dat")
			creators.push_back(new HistoCreator(p.second));
		}
		}
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
HistoManager::~HistoManager() {
	for(int i =0;i < creators.size();++i) delete	creators[i];
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::createLogicalHistos() {
	for(auto  c : creators) c->createHistos();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::buildGuiHistos() {

	for(unsigned int iCreator=0;iCreator<2 && iCreator<creators.size();++iCreator){
		HistoCreator* aCreator = creators[iCreator];
		auto it =aCreator->histos.begin();
		int i=0;
		for(auto &h: aCreator->hc.vec){
      MyHistogramWrapper *aHisto = new MyHistogramWrapper(h.name,h.bins,h.min,h.max);
      aHisto->setId(i);
			aHisto->setCreator(aCreator);
			aHisto->setBins(&(*it));
			if(iCreator==0) guiHistosPrimary.push_back(aHisto);
			if(iCreator==1) guiHistosSecondary.push_back(aHisto);
			it++;
			i++;
		}
	}
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
MyHistogramWrapper* HistoManager::getGuiPrimaryHisto( unsigned int iHisto){

if(iHisto<guiHistosPrimary.size()) return guiHistosPrimary[iHisto];
else return 0;

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
MyHistogramWrapper* HistoManager::getGuiSecondaryHisto( unsigned int iHisto){

if(iHisto<guiHistosSecondary.size()) return guiHistosSecondary[iHisto];
else return 0;

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::drawHistos(TCanvas *aCanvas){

drawPrimaryHistos(aCanvas);
drawSecondaryHistos(aCanvas);

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::drawPrimaryHistos(TCanvas *aCanvas){

  for(auto &aHisto :guiHistosPrimary) aHisto->setHisto();
  for(unsigned int iPad=0;iPad<9;++iPad){
		if(iPad<guiHistosPrimary.size()) {
			aCanvas->cd(iPad+1);
			guiHistosPrimary[iPad]->getHisto()->Draw();
		}
	}
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::drawSecondaryHistos(TCanvas *aCanvas){

  for(auto &aHisto :guiHistosSecondary) aHisto->setHisto();
  for(unsigned int iPad=0;iPad<9;++iPad){
		if(iPad<guiHistosSecondary.size()) {
			aCanvas->cd(iPad+1);
			gPad->Update();
      TH1F *aHisto = guiHistosSecondary[iPad]->getHisto();
   		Float_t rightmax = 1.1*aHisto->GetMaximum();
			if(rightmax<1) rightmax = 1.0;
   		Float_t scale = gPad->GetUymax()/rightmax;			
   		aHisto->SetLineColor(kRed);
   		aHisto->Scale(scale);
   		aHisto->DrawCopy("same hist");
			aHisto->Scale(1.0/scale);

   //draw an axis on the right side
   TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
         gPad->GetUxmax(), gPad->GetUymax(),0,rightmax,510,"+L");
   axis->SetLineColor(kRed);
   axis->SetLabelColor(kRed);
   axis->Draw();
	}
}
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
