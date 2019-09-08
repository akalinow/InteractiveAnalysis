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
#include <mutex>
#include<DataSource.h>
#include <HistoCreator.h>
using namespace std;

/**
 * Executes task specified in * task
 * Fills histos with data from creator->data
 * from between [start, end[
 */
void fillBins(Task* task){
    size_t nHistos = task->histos.size();
    // Iterates through specified range of data
    for(size_t i=task->start; i<task->end; i++){
        std::vector<unsigned int>& row = task->creator->data[i];
        // If all values fit between set cuts adds values to certain bins
        int l = 0;
        for (int k = 0; k < nHistos; ++k){
            if(row[k] < task->creator->cutsLow[k] or row[k]>= task->creator->cutsHigh[k]) l = nHistos;
        }
        for(;l<nHistos;++l){
            task->histos[l][row[l]]++;
        }
    }
}
/**
 * Function to be executed by histogram-creating threads
 * Tries to acquire &Task from tasks and execute fillBins on it
 * Returns if task.shutdown is true
 */
void executeTasks(HistoCreator* creator){
    while(true){ // works until receives task with shutdown==true
        std::unique_lock<mutex> lock(creator->tasksMutex);
        while(creator->tasks.empty()){
            creator->tasksNotEmpty.wait(lock); // waits for task to be sent
        }
        // removes first element from tasks queue and executes it
        Task* task = creator->tasks.front();
        creator->tasks.pop();
        lock.unlock();
        if(task->shutdown){
            return; // returns if task is set to shutdown
        }
        fillBins(task);
        lock.lock();
        creator->tasksActive--;
        if(creator->tasksActive==0){
            creator->allTasksDone.notify_one(); // notifies main thread if there are no active tasks
        }
        lock.unlock();
    }
}
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
    // Reads data from file into data vector
    IDataSource *ids= PreloadContainer::get().getDataSource(hc.myDataFile.c_str());
	for (int i = 0; i < hc.numOfEvents; ++i){
        vector<unsigned int> val(hc.vec.size(),0);
        for (int k = 0; k < hc.vec.size(); ++k){
            val[k]=ids->read( hc.vec[k].bytes);
        }
        data.push_back(val);
    }
     // Sets data-analyzing threads
    for(int i=0; i<numOfThreads; i++){
        threads.emplace_back(executeTasks, this);
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
    // Sets data-analyzing threads
    for(int i=0; i<numOfThreads; i++){
        threads.emplace_back(executeTasks,this);
    }
}
/**
 * Explicit destructor for HistoCreator
 * Joins all threads
 */
HistoCreator::~HistoCreator(){
    Task task;
    task.shutdown = true;
    std::unique_lock<mutex> lock(tasksMutex);
    for(auto&& t: threads){
        tasks.push(&task); // Sends shutdown tasks to all related threads
        tasksNotEmpty.notify_one();
    }
    lock.unlock();
    for(auto&& t: threads){
        t.join(); // Joins all related threads
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
 * Analyzes data and refills histos
 * based on currently set cuts
 */
void HistoCreator::createHistos() {
    // Writes 0 to all bins
    writeZeros();

    // Splits work into tasks and sends them to threads
    size_t numOfEvents = hc.numOfEvents;
    vector<Task> taskVector;
    std::unique_lock<mutex> lock(tasksMutex);
    for(int i=0; i<numOfThreads; i++){
        taskVector.emplace_back();
        Task* task = &taskVector.back();
        task->start = (i*numOfEvents)/numOfThreads;
        task->end = ((i+1)*numOfEvents)/numOfThreads;
        task->histos = histos;
        task->creator = this;
        tasks.push(task);
        tasksNotEmpty.notify_one();
        tasksActive++;
    }
    // Waits for all threads to finish tasks
    while(tasksActive!=0){
        allTasksDone.wait(lock);
    }
    // Joins results for all threads
    for(auto&& task: taskVector){
        for(size_t j=0; j<histos.size(); j++){
            for(size_t k=0; k<histos[j].size(); k++){
                histos[j][k] += task.histos[j][k];
             }
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
