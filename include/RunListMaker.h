#ifndef RUN_LIST_MAKER_H
#define RUN_LIST_MAKER_H

#include "TreeAnalyzer.h"
#include "PicoDataStore.h"

/**
 * STD
 */
#include <map>

class RunListMaker : public TreeAnalyzer
{

protected:

	std::map< int, int > runMap;
	PicoDataStore * pico;

public:
	RunListMaker( XmlConfig * config, string np, string fileList, string prefix );
	~RunListMaker();

protected:
	virtual void analyzeEvent();
	virtual void postEventLoop();
};

#endif