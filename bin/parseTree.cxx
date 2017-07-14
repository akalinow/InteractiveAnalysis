/*
 * ParseTree.cpp
 *
 *  Created on: Oct 27, 2015
 *      Author: krystian892
 */

#include <HistoCreator.h>
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include "HistoConfig.h"
#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

int main(int argc, char **argv){

 std::string configFileName;
  if(argc<1){
     std::cout<<" Usage: parseTree config.json"<<std::endl;
    return 0;
   }
  else {
    configFileName.append(argv[1]);
    std::cout<<"configFileName = "<<configFileName<<std::endl;
  }

	ptree pt;
	read_json(configFileName.c_str(),pt);
	for(auto & p : pt){
		cout<<"\n"<<p.first;
		HistoCreator data(p.second);
		data.runTests();
	}
	return 0;
}


