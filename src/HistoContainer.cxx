/**
 @file HistoContainer.cpp
 @brief Implementation of container for HistoCreators
 @author   kzawisto
 @created:   2015-11-04
 */

#include <MyHistogramWrapper.h>
#include "HistoContainer.h"
#include"HistoCreator.h"
#include<cstdlib>
#include<sys/stat.h>
#include<fstream>
#include<iostream>

/// to test whether config_json exists
bool fileExists(const std::string & name){
	ifstream f(name.c_str());
	bool ret=(f.good());
	f.close();
	return ret;

}
HistoContainer::HistoContainer(string json_path) {
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

HistoContainer::~HistoContainer() {
	for(int i =0;i < creators.size();++i)
		delete	creators[i];
}

void HistoContainer::createLogicalHistos() {
	for(auto  c : creators) c->createHistos();
}

void HistoContainer::buildGuiHistos() {

	for(auto  c : creators){
		auto it =c->histos.begin();
		int i=0;
		for(auto &h: c->hc.vec){
      MyHistogramWrapper *aHisto = new MyHistogramWrapper(h.name,h.bins,h.min,h.max);
      aHisto->setId(i);
			aHisto->setCreator(&(*c));
			aHisto->setBins(&(*it));
			guiHistos.push_back(aHisto);
			it++;
			i++;
		}
	}
}
