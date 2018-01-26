/**
 @file    HistoCreator.cpp
 @brief HistoCreator implementation
 @author   kzawisto
 @created:   2015-10-29
 **************************************************************/

#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include<fstream>
#include<iostream>
#include<chrono>
#include<DataSource.h>
#include <HistoCreator.h>
using namespace std;
/**
 * Unused, takes configuration from json at _configPath.
 */
HistoCreator::HistoCreator(string _configPath) :
		configPath(_configPath), hc(_configPath) {
	for (int i = 0; i < hc.vec.size(); ++i) {
		histos.push_back(vector<unsigned int>(hc.vec[i].bins));
		cutsLow.push_back(0);
		cutsHigh.push_back(hc.vec[i].bins);
	}

    IDataSource *ids= PreloadContainer::get().getDataSource(hc.myDataFile.c_str());
	for (int i = 0; i < hc.numOfEvents; ++i){
        vector<unsigned int> val(hc.vec.size(),0);
        for (int k = 0; k < hc.vec.size(); ++k){
            val[k]=ids->read( hc.vec[k].bytes);
        }
        data.push_back(val);
    }

}
/**
 * Constructor takes configuration from boost::program_options ptree, loaded from json_config
 */
HistoCreator::HistoCreator(ptree pt) :hc(pt)  {

	for (int i = 0; i < hc.vec.size(); ++i) {
			histos.push_back(vector<unsigned int>(hc.vec[i].bins));
			cutsLow.push_back(0);
				cutsHigh.push_back(hc.vec[i].bins);
		}

    // Reads data from file into data vector
    IDataSource *ids= PreloadContainer::get().getDataSource(hc.myDataFile.c_str());
	for (int i = 0; i < hc.numOfEvents; ++i){
        vector<unsigned int> val(hc.vec.size(),0);
        for (int k = 0; k < hc.vec.size(); ++k){
            val[k]=ids->read( hc.vec[k].bytes);
        }
        data.push_back(val);
    }

}
/**
 * Loads ROOT file from hc.rootDataFile path and saves multiplexed binary file to hc.myDataFile path/
 */
void HistoCreator::processTree() {
	TFile * file = new TFile( hc.rootDataFile.c_str());
	TTree* tree = (TTree*) file->Get(hc.treeName.c_str());
	fstream file1(hc.myDataFile.c_str(), fstream::out | fstream::binary);
	float val[HistoCreator::maxNumberOfVariables];
	for (int i = 0; i < HistoCreator::maxNumberOfVariables; ++i) val[i] = 0;
	tree->SetBranchAddress(hc.branchName.c_str(), val);
	for (int i = 0; i <  hc.numOfEvents; ++i) {
		tree->GetEntry(i);
		for (int k = 0; k < hc.vec.size(); ++k) {
			unsigned int j = hc.getBin(k, val[k]);
			file1.write((char*) &j, hc.vec[k].bytes);
		}
	}
	file1.close();

}

/**
 * Clears and refills bins
 */
void HistoCreator::createHistos() {
	writeZeros();

    for (auto&&  val : data){
        int l = 0;
        for (int k = 0; k < val.size(); ++k){
            if(val[k] < cutsLow[k] or val[k]>= cutsHigh[k]) l = val.size();
        }
        for(;l<val.size();++l){
            histos[l][val[l]]++;
        }
	}
}
/**
 * Writes zeros to all histograms' bins
 */
void HistoCreator::writeZeros() {
		for (auto &vec: histos) for(auto &v:vec) v =0;
}
/**
 * Automatic testing function.
 * Loads data from ROOT tree, then processes the ROOT file again saving it to multiplexed binary file, then
 * loads multiplexed data with createHistos() and checks whether got the same bin values in all the histograms
 */
void HistoCreator::runTests(){

	for(auto h:histos) for(int i =0;i<h.size();++i) h[i]=0;

	vector< vector<unsigned int> > histos_copy;
	for (int i = 0; i < hc.vec.size(); ++i) {
			histos_copy.push_back(vector<unsigned int>(hc.vec[i].bins));
	}
	TFile * file = new TFile(hc.rootDataFile.c_str());
	TTree* tree = (TTree*) file->Get(hc.treeName.c_str());
	float val[HistoCreator::maxNumberOfVariables];
	for (int i = 0; i < HistoCreator::maxNumberOfVariables; ++i) val[i] = 0;
	tree->SetBranchAddress(hc.branchName.c_str(), val);
	for (int i = 0; i <  hc.numOfEvents; ++i) {
			tree->GetEntry(i);
			for (int k = 0; k < hc.vec.size(); ++k) {
						unsigned int j = hc.getBin(k, val[k]);
						histos_copy[k][j]++;
			}
	}
	processTree();
	createHistos();
	int errors=0;
	for(int i =0;i < histos.size();++i)
		for(int j =0; j<histos[i].size();++j){
			errors+= histos[i][j]!= histos_copy[i][j];
		}
	cout<<"Errors: "<<errors<<".";
	if(errors == 0) cout<<" Everything is fine."<<std::endl;
	else cerr<<"What a terrible failure";
}
