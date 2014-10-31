#ifndef HISTOGRAM_ANALYZER_H
#define HISTOGRAM_ANALYZER_H


/**
 * JDB 
 */
#include "Utils.h"
#include "XmlConfig.h"
#include "Logger.h"
#include "LoggerConfig.h"
#include "HistoBook.h"
#include "ConfigRange.h"
#include "ConfigPoint.h"
#include "Reporter.h"
using namespace jdb;

/**
 * ROOT
 */
#include "TFile.h"


class HistogramAnalyzer
{
// protected properties
protected:
	Logger * lg;
	XmlConfig * cfg;
	string nodePath;

	HistoBook * book;
	Reporter * reporter;
	TFile * inFile;

// public methods
public:
	HistogramAnalyzer( XmlConfig * config, string nodePath );
	HistogramAnalyzer();
	~HistogramAnalyzer();

	virtual void make() {}
	
};


#endif