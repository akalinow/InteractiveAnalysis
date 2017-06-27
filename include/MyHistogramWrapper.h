/****
@file MyHistogramWrapper.h
 @brief Responsible for direct communication between the GUI and HistoCreator
 @author   kzawisto
 @created:   2015-11-04
 ***/
/** *************************************************************
 @file      MyHistogramWrapper.h
 @brief   Responsible for direct communication between the GUI and HistoCreator
 @author    kzawisto
 @created:   2015-10-29

 **************************************************************/
#ifndef MYHISTOGRAM_H
#define MYHISTOGRAM_H
#include<vector>
#include<string>


using namespace std;


class HistoCreator;
class TH1F;

/// This class handles all the communication between the GUI and DataIO framework
class MyHistogramWrapper
{

    public:
    ///constructor, sets bin range and the num of bins
        MyHistogramWrapper(std::string title, int n, float xmin, float  xmax);
        virtual ~MyHistogramWrapper();

        void setId(int aId) { histoId = aId;};

        void setCreator(HistoCreator * aCreator) {creator = aCreator;};

        void setBins(vector<unsigned int> *aBins) {bins = aBins;};

        void fillGauss(int n, int center, float height);
        void setHisto();
        void setCutLow(int b);
        void setCutHigh(int b);
        void update();

        int getICutLow() const {return cutLow;};
        int getICutHigh() const {return cutHigh;};

        float getFCutLow() const;
        float getFCutHigh() const;

        TH1F *getHisto() const { return theHisto;};

    private:

    ///id of a histogram in creator
    int histoId;
    int cutLow=0, cutHigh=0;

    ///pointer to vector owned by creator
    vector<unsigned int> * bins;
    HistoCreator * creator;
    TH1F *theHisto;
};


#endif // MYHISTOGRAM_H
