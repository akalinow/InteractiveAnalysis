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

	std::string hNameSuffix = "";
	for(unsigned int iCreator=0;iCreator<2 && iCreator<creators.size();++iCreator){
		HistoCreator* aCreator = creators[iCreator];
		auto it =aCreator->histos.begin();
		int i=0;
		if(iCreator==1) hNameSuffix = "Secondary";
		for(auto &h: aCreator->hc.vec){
      MyHistogramWrapper *aHisto = new MyHistogramWrapper((h.name+hNameSuffix).c_str(),h.bins,h.min,h.max);
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
std::vector<std::string> HistoManager::getHistoNames() const {

	std::vector<std::string> hNames;

	for(auto &aHisto :guiHistosPrimary){
	 std::string aName(aHisto->getHisto()->GetName());
	 hNames.push_back(aName);
 }
	return hNames;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
std::vector<float> HistoManager::getLowCuts() const {

	std::vector<float> hCuts;

	for(auto &aHisto :guiHistosPrimary){
	 float aCut = aHisto->getFCutLow();
	 hCuts.push_back(aCut);
 }
	return hCuts;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
std::vector<float> HistoManager::getHighCuts() const{

	std::vector<float> hCuts;

	for(auto &aHisto :guiHistosPrimary){
	 float aCut = aHisto->getFCutHigh();
	 hCuts.push_back(aCut);
 }
	return hCuts;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::updateHistos(){
if(guiHistosPrimary.size()) guiHistosPrimary[0]->update();
if(guiHistosSecondary.size()) guiHistosSecondary[0]->update();

for(auto &aHisto :guiHistosPrimary) aHisto->setHisto();
for(auto &aHisto :guiHistosSecondary) aHisto->setHisto();

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::drawHistos(TCanvas *aCanvas, const std::vector<int> & selectedHistos){

aCanvas->Clear();
aCanvas->Divide(3,3);
drawPrimaryHistos(aCanvas, selectedHistos);
drawSecondaryHistos(aCanvas, selectedHistos);

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::drawPrimaryHistos(TCanvas *aCanvas,
						const std::vector<int> & selectedHistos){

	unsigned int hIndex = 0;

  for(auto &aHisto :guiHistosPrimary) aHisto->setHisto();
  for(unsigned int iPad=0;iPad<9;++iPad){
		if(iPad<guiHistosPrimary.size() && iPad<selectedHistos.size()) {
			aCanvas->cd(iPad+1);
			hIndex = selectedHistos[iPad];
			if(iPad==0){
			guiHistosPrimary[hIndex]->getHisto()->SetMarkerStyle(20);
			guiHistosPrimary[hIndex]->getHisto()->Draw("p");
			}
			else guiHistosPrimary[hIndex]->getHisto()->Draw();
		}
	}
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void HistoManager::drawSecondaryHistos(TCanvas *aCanvas,
const std::vector<int> & selectedHistos){

  unsigned int hIndex = 0;

  for(auto &aHisto :guiHistosSecondary) aHisto->setHisto();
  for(unsigned int iPad=0;iPad<9;++iPad){
		if(iPad<guiHistosSecondary.size() && iPad<selectedHistos.size()) {
			aCanvas->cd(iPad+1);
			gPad->Update();
			hIndex = selectedHistos[iPad];
      TH1F *aHisto = guiHistosSecondary[iPad]->getHisto();
   		Float_t rightmax = 1.1*aHisto->GetMaximum();
			if(rightmax<1) rightmax = 1.0;
   		Float_t scale = gPad->GetUymax()/rightmax;
   		aHisto->SetLineColor(kRed);
			aHisto->SetLineWidth(2);
   		aHisto->Scale(scale);
   		aHisto->DrawCopy("same hist");
			aHisto->Scale(1.0/scale);
			gPad->Update();

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
