/**
 @file    HistoCreator.h
 @brief Loads multiplexed binary file for the GUI or processes ROOT tree to generate one
 @author   kzawisto
 @created:   2015-10-29
 **************************************************************/

#ifndef INCLUDE_HISTOCREATOR_H_
#define INCLUDE_HISTOCREATOR_H_
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>
#include"HistoConfig.h"
using std::string;
using std::vector;

///Task specifies range of data to be evaluated to create histograms
struct Task {
    class HistoCreator* creator; // HistoCreator sending task
    std::vector< std::vector<unsigned int> > histos; // storage of evaluated data
    size_t start=0, end=0; // [start,end[ is range of creator->data to be evaluated
    bool shutdown=false; // if shutdown==true thread returns and is deactivated
};


///Creates histos from single IDataSource
class HistoCreator {
	string configPath;

public:
    const int numOfThreads=4; //number of threads on which histogram data is evaluated
	vector<unsigned int> cutsLow,cutsHigh;
	HistogramConfig hc;
	vector< vector<unsigned int> > histos;
    vector< vector<unsigned int> > data; //data used to draw histograms
    std::queue<Task*> tasks; //queue of tasks to be executed by threads
    std::atomic<int> tasksActive{0}; //number of currently active tasks
    std::mutex tasksMutex; //mutex locking tasks queue and tasksActive counter
    std::condition_variable allTasksDone; //notified when tasksActive is set to 0
    std::condition_variable tasksNotEmpty; //notified when task is being pushed to tasks queue

    vector<std::thread> threads; //vector of threads working on assigned tasks

	HistoCreator(string configPath );
	HistoCreator(ptree pt );
    ~HistoCreator();
	void processTree();
	void createHistos();
	void writeZeros();
	void runTests();

	static const unsigned int maxNumberOfVariables = 64;

};

#endif /* INCLUDE_HISTOCREATOR_H_ */
