/**
 @file    HistoContainer.h
 @brief Wraps HistoCreators and provides data to the GUI
 @author   kzawisto
 @created:   2015-11-04
 **************************************************************/
#ifndef INCLUDE_HISTOCONTAINER_H_
#define INCLUDE_HISTOCONTAINER_H_
#include<string>
#include <vector>
#include<memory>

using namespace std;
#include <boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>

#include"MyHistogramWrapper.h"
class HistoCreator;
using namespace boost::property_tree;

/// Proxy between GUI and DataIO.so
class HistoContainer {
	public:

		///constructor, loads config from json and constructs HistoCreators
		HistoContainer(string json_path);

		///Get rid of Creators
		~HistoContainer();

		bool isGood() const { return good;};

		const vector<MyHistogramWrapper*> & getGuiHistos() const {return guiHistos;}

		void buildGuiHistos();

		/// Reset and fill bins with experimental data, loaded from file/RAM cache
		void createLogicalHistos();


	private:

    ///HistoCreators, construct histograms, store bins to display
		vector< HistoCreator* > creators;

    vector<MyHistogramWrapper*> guiHistos;

			/// Whether histograms were loaded successufy and can be drawn
		bool good;
};

#endif /* INCLUDE_HISTOCONTAINER_H_ */
