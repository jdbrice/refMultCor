#ifndef PLOT_QA_H
#define PLOT_QA_H

#include "ConfigRange.h"

#include "HistoAnalyzer.h"


#include "TH1D.h"
#include "TH2D.h"
#include "TLine.h"
#include "TMath.h"
#include "TF1.h"
#include "TProfile.h"

class PlotQA : public HistoAnalyzer {
// Protected member properties
protected:

	Reporter * rpZ;

	vector<ConfigRange*> period;

	map<int, bool> badRuns;
	const static int nRuns = 733;

// public member methods
public:
	PlotQA( XmlConfig * config, string nodePath );
	~PlotQA();

	virtual void make();

// protected member methods
protected:


	void makeRunByRun();
	void makeRefMultVersus();
	//static UInt_t runList[];
	
	TH1D* meanSliceX( TH2D * h2 );
	TH1D* entrySliceX( TH2D * h2 );
	TH1* isolate( TH1* h, int x1, int x2 );

	TH1D* badRunsHist( TH1D* h, string name );
	void drawWithAcceptanceBands( TProfile* h, double nSig, ConfigRange* cr );
	void findBadRuns( TProfile* h, double nSig, ConfigRange* cr );
	void findBadRunsFromEvents( TH1D* h, double nEvents );

	double meanForPeriod( TProfile * h, ConfigRange * cr );
	double rmsForPeriod( TProfile * h, ConfigRange * cr );

	void reportBadRuns();

	/**
	 * For zVertex 
	 */
	void makeZVertexProjections();
	static double fTail( Double_t * x, Double_t * par);
};

#endif