/***
* @file MyHistogramWrapper.cpp
 @brief Implementation of MyHistogramWrapper
 @author   kzawisto
 @created:   2015-11-04
 ****/

#include <MyHistogramWrapper.h>

#include<cmath>
#include<cstdlib>
#include<iostream>

#include <HistoCreator.h>
#include <TH1F.h>

MyHistogramWrapper::MyHistogramWrapper(std::string title, int n, float xmin, float  xmax){

  theHisto = new TH1F(title.c_str(),title.c_str(),n,xmin,xmax);

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
MyHistogramWrapper::~MyHistogramWrapper(){

  delete theHisto;

  }
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MyHistogramWrapper::setHisto()
{

    for(int i =0;i<bins->size();++i) theHisto->SetBinContent(i+1,(*bins)[i]);

}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MyHistogramWrapper::setCutLow(int b) {
  //if(b==cutLow) return;
	cutLow= b;
	creator->cutsLow[histoId] = b;
	creator->createHistos();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
void MyHistogramWrapper::setCutHigh(int b) {
  //if(b==cutHigh) return;
	cutHigh= b;
	creator->cutsHigh[histoId] = b;
	creator->createHistos();
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
float MyHistogramWrapper::getFCutLow() const {
  if(!theHisto) return -999;
  return theHisto->GetXaxis()->GetBinUpEdge(cutLow);
};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
float MyHistogramWrapper::getFCutHigh() const {
  if(!theHisto) return -999;
  return theHisto->GetXaxis()->GetBinLowEdge(cutHigh);
};
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////