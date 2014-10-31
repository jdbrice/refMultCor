#ifndef PLOT_QA_H
#define PLOT_QA_H

#include "HistogramAnalyzer.h"
#include "TH1D.h"
#include "TH2D.h"

class PlotQA : public HistogramAnalyzer {
// Protected member properties
protected:


// public member methods
public:
	PlotQA( XmlConfig * config, string nodePath );
	~PlotQA();

	virtual void make();

// protected member methods
protected:

	
	TH1D* meanSliceX( TH2D * h2 );

	
};

#endif