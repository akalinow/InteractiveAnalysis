/**
 @file    DataSource.h
 @brief Loading data from binary file for the GUI or testing purposes.
 @author   kzawisto
 @created:   2015-10-29
 **************************************************************/

#ifndef INCLUDE_DATASOURCE_H_
#define INCLUDE_DATASOURCE_H_
#include <fstream>
#include <string>
#include <map>
using namespace std;
using std::fstream;
using std::map;
///Abstract DataSource, used by HistoCreator to load multiplexed data
class IDataSource{
public:

	IDataSource(){}
	virtual const unsigned int & read(const unsigned int & bytes)=0;

  virtual void skipData(const unsigned int & count) {};

	virtual bool good(){return true;}
	virtual ~IDataSource(){};
};
///Regular DataSource, loads from multiplexed binary file each time.
class DataSource :public IDataSource{
	fstream file1;
	unsigned int readValue = 0;
public:
	DataSource(string path);
	virtual const unsigned int & read(const unsigned int & bytes);
	virtual ~DataSource();
};
struct DataBuffer;
///Singleton caching facility for RAM-based PreloadedDataSource's
class PreloadContainer{
	map<string, long int> lengths;
	map<string, DataBuffer*> buffers;
	long int getLength(string path);
public:
	static PreloadContainer& get(){ static PreloadContainer inst; return inst;}
	DataBuffer * getDataBuffer(string path);
	IDataSource * getDataSource(string path);
};
///Cached DataSource, data stored in RAM during program run.
class PreloadedDataSource : public IDataSource{
	DataBuffer * buffer;
	long int iter;
	unsigned int readValue = 0;

public:
	PreloadedDataSource(DataBuffer * buf);
	virtual const unsigned int & read(const unsigned int & bytes);
	virtual void skipData(const unsigned int & count);
	virtual ~PreloadedDataSource();
};
#endif /* INCLUDE_DATASOURCE_H_ */
