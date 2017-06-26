/**
 @file    HistoManager.h
 @brief Wraps HistoCreators and provides data to the GUI
 @author   kzawisto
 @created:   2015-11-04
 **************************************************************/
#ifndef INCLUDE_HistoManager_H_
#define INCLUDE_HistoManager_H_
#include<string>
#include <vector>
#include<memory>

using namespace std;
#include <boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>

#include"MyHistogramWrapper.h"
class HistoCreator;
using namespace boost::property_tree;

class TCanvas;

/// Proxy between GUI and DataIO.so
class HistoManager {
	public:

		///constructor, loads config from json and constructs HistoCreators
		HistoManager(string json_path);

		///Get rid of Creators
		~HistoManager();

		bool isGood() const { return good;};

		unsigned int getNumberOfHistos() const { return guiHistosPrimary.size();};

		MyHistogramWrapper* getGuiPrimaryHisto(unsigned int iHisto);

	  MyHistogramWrapper* getGuiSecondaryHisto(unsigned int iHisto);

		std::vector<std::string> getHistoNames() const;

		std::vector<float> getLowCuts() const;

		std::vector<float> getHighCuts() const;

		void drawHistos(TCanvas *aCanvas);

		void buildGuiHistos();

		/// Reset and fill bins with experimental data, loaded from file/RAM cache
		void createLogicalHistos();


	private:

		void drawPrimaryHistos(TCanvas *aCanvas);

		void drawSecondaryHistos(TCanvas *aCanvas);

    ///HistoCreators, construct histograms, store bins to display
		vector< HistoCreator* > creators;

    vector<MyHistogramWrapper*> guiHistosPrimary;
		vector<MyHistogramWrapper*> guiHistosSecondary;

			/// Whether histograms were loaded successufy and can be drawn
		bool good;
};

#endif /* INCLUDE_HistoManager_H_ */
