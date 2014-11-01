#ifndef PLOT_QA_H
#define PLOT_QA_H

#include "ConfigRange.h"

#include "HistoAnalyzer.h"


#include "TH1D.h"
#include "TH2D.h"

class PlotQA : public HistoAnalyzer {
// Protected member properties
protected:

	Reporter * rpZ;

	vector<ConfigRange*> periods;

	vector<int> badRuns;

// public member methods
public:
	PlotQA( XmlConfig * config, string nodePath );
	~PlotQA();

	virtual void make();

// protected member methods
protected:

	//static UInt_t runList[];
	
	TH1D* meanSliceX( TH2D * h2 );
	TH1D* entrySliceX( TH2D * h2 );
	TH1* isolate( TH1* h, int x1, int x2 );

	void drawWithAcceptanceBands( TH1D* h, double nSig );

	/**
	 * For zVertex 
	 */
	void makeZVertexProjections();
	static double fTail( Double_t * x, Double_t * par);
};

#endif