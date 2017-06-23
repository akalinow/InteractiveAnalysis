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

  std::cout<<"title: "<<title<<std::endl;
  theHisto = new TH1F(title.c_str(),title.c_str(),n,xmin,xmax);

}

MyHistogramWrapper::~MyHistogramWrapper(){

  delete theHisto;

  }

void MyHistogramWrapper::setHisto()
{

    for(int i =0;i< bins->size();++i) theHisto->SetBinContent(i+1,(*bins)[i]);

}

/**Sets lower cut and triggers histogram recreation
 *
 */
void MyHistogramWrapper::setCutLow(int b) {
	cutLow= b;
	creator->cutsLow[histoId] = b;
	creator->createHistos();
}

/**Sets higher cut and triggers histogram recreation
 *
 */
void MyHistogramWrapper::setCutHigh(int b) {
	cutHigh= b;
		creator->cutsHigh[histoId] = b;
	creator->createHistos();
}
