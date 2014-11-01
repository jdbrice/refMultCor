#include "PlotQA.h"

#include "TLine.h"
#include "TMath.h"
#include "TF1.h"
#include "TProfile.h"

PlotQA::PlotQA( XmlConfig * config, string np ) : HistoAnalyzer( config, np ){

	logger->setClassSpace( "PlotQA" );
	logger->info( __FUNCTION__ ) << endl;

	logger->info(__FUNCTION__) << "Making Z Vertex Report from " << np+"refMultZ.Reporter" << endl;
	rpZ = new Reporter( cfg, np+"refMultZ.Reporter." );

	vector<string> children = cfg->childrenOf( np+"timePeriods" );
	for ( int i =0; i < children.size(); i++ ){
		ConfigRange * cr = new ConfigRange( cfg, children[ i ] );
		periods.push_back( cr );
		logger->info( __FUNCTION__ ) << " Initializing Run range " <<
	}


}

PlotQA::~PlotQA(){

	delete rpZ;

}

void PlotQA::make(){

	gStyle->SetOptStat( 0 );

	logger->info( __FUNCTION__ ) << "{" << endl;
	vector<string> hNames = cfg->getStringVector( nodePath + "makeMeanHistos" );

	book->cd();

	logger->info( __FUNCTION__ ) << "Found " << ts( (int)hNames.size() ) << " Histograms" << endl;
	for ( int i = 0; i < hNames.size(); i++ ){
	
		TH2D * h2 = (TH2D*)inFile->Get( hNames[ i ].c_str() );
		logger->info( __FUNCTION__ ) << "Processing " << hNames[ i ] << endl;


		/*TH1D * h1 = meanSliceX( h2 );
		mirrorOverY( h1 );
		if ( "refMultZ" != hNames[ i ] )
			translateX( h1, -150 );
		
		h1->Draw( "pel" );
		h1->SetMarkerStyle( 7 );
		//gPad->SetLogz();*/
		
		reporter->newPage();
		if ( "bbc" == hNames[ i ] ){
			//TH1D* bbc = drawWithAcceptanceBands( h2, 5 );
			TProfile * bbc = h2->ProfileX();
			book->add( "bbc", bbc );
			bbc = h2->ProfileY();
			book->add( "bbcY", bbc );
		} else if ( "ptPrimary" == hNames[ i ] ){
			//TH1D * pt = drawWithAcceptanceBands( h2, 3);
			//book->add( "pt", pt );
		} else if ( "refMult" == hNames[ i ] ){
			//TH1D * rm = drawWithAcceptanceBands( h2, 4);
			//book->add( "refMult", rm );
		}

		reporter->savePage();

	}

	TH2D * h2 = (TH2D*)inFile->Get( "events" );
	TH1D * h1 = entrySliceX( h2 );
	book->add( "nEvents", h1 );
	reporter->newPage();
	
	book->style( "nEvents" )->set( nodePath+"style.profile" )->draw(); 
	reporter->savePage();

	//makeZVertexProjections();

	logger->info( __FUNCTION__ ) << "}" << endl;

}

void PlotQA::drawWithAcceptanceBands( TH1D* h1, double nSig ){
	/*
	double m = h2->GetMean(2);
	double rms = h2->GetRMS(2) * nSig;
	logger->info(__FUNCTION__) << "Sig = " << h2->GetRMS(2) << endl;

	h1->GetYaxis()->SetRangeUser( m - rms * 1.5, m + rms * 1.5  );

	TLine * c = new TLine( 0, m, 1200, m );
	TLine * ub = new TLine( 0, m+rms, 1200, m+rms );
	TLine * lb = new TLine( 0, m-rms, 1200, m-rms );

	c->SetLineColor( kBlue );
	lb->SetLineColor( kRed );
	ub->SetLineColor( kRed );

	c->Draw();
	lb->Draw();
	ub->Draw();
	*/
}

/**
 * Makes a 1D histo from a 2D where each entry is the mean of a slice of the 2D
 * @param  h2 input 2D
 * @return    1d histo
 */
TH1D* PlotQA::meanSliceX( TH2D* h2 ){

	TH1D * h1;
	int nBinsX = h2->GetNbinsX();

	TAxis *x = h2->GetXaxis();
	double x1, x2;
	h1 = new TH1D( ((string)"_"+(string)h2->GetTitle()).c_str(), ("<"+(string)h2->GetTitle()+">").c_str(), nBinsX, x->GetXmin(), x->GetXmax() );
	for ( int i = 1; i <= nBinsX; i++ ){

		// project then mean
		TH1D* py = h2->ProjectionY( "py", i, i );
		
		double m = py->GetMean();
		double me = py->GetMeanError();
		
		h1->SetBinContent( i, m );
		h1->SetBinError( i, me );

		if ( 0 >= m ){
		} else {
			//cout << runList[ i ] << ", " << endl;
		}

	}

	return h1;
}


TH1D* PlotQA::entrySliceX( TH2D * h2 ){

	TH1D * h1;
	int nBinsX = h2->GetNbinsX();

	h1 = new TH1D( ((string)"_"+(string)h2->GetTitle()).c_str(), h2->GetTitle(), nBinsX, 0, nBinsX );
	for ( int i = 1; i <= nBinsX; i++ ){

		// project then mean
		TH1D* py = h2->ProjectionY( "py", i, i );
		double m = py->GetEntries();
		h1->SetBinContent( (nBinsX - i + 1), m );
	}
	return h1;

}

TH1* PlotQA::isolate( TH1 * hIn, int x1, int x2 ){

	TH1D * h1 = (TH1D*)hIn->Clone( "tmp" );

	int nBinsX = hIn->GetNbinsX();

	if( x1 > x2  )
		return h1;
	if ( x1 < 1 )
		x1 = 1;
	if ( x2 > nBinsX )
		x2 = nBinsX;

	for ( int i = 1; i <= nBinsX; i++ ){
		h1->SetBinContent( i, 0 );
		h1->SetBinError( i, 0 );
	}
	for ( int i = x1; i <= x2; i++ ){
		h1->SetBinContent( i, hIn->GetBinContent( i ) );
		h1->SetBinError( i, hIn->GetBinError( i ) );
	}

	return h1;
}



void PlotQA::makeZVertexProjections(){

	TH2D * h2 = (TH2D*)inFile->Get( "refMultZ" );
	TAxis * x = h2->GetXaxis();
	vector<double> ranges = cfg->getDoubleVector( nodePath + "refMultZ.ranges" );

	for ( int i = 0; i < ranges.size() - 1; i++ ){
		int x1 = x->FindBin( ranges[ i ] );
		int x2 = x->FindBin( ranges[ i+1 ] );
		logger->info(__FUNCTION__) << "Project ( " << ranges[ i ] << ", " << ranges[ i+1] << " ) -> ( " << x1 << ", " << x2 << " ) " << endl;


		TH1D * h1 = h2->ProjectionY( ("refMult_" + ts( ranges[i] )).c_str(), x1, x2 );
		h1->SetTitle( ( dts( ranges[i] ) + " < z < " + dts( ranges[i+1] ) ).c_str() );
		rpZ->newPage();
		gPad->SetLogy();
		h1->Draw( "pel" );
		h1->SetMarkerStyle( 7 );
		rpZ->savePage();

	}

	TF1 * f1 = new TF1( "fTail", fTail, 0, 1000, 3 );
	f1->Draw();

}


double PlotQA::fTail( Double_t * x, Double_t * par ){

	double x0 = x[0];
	double A = par[ 0 ];
	double sig = par[ 1 ];
	double h = par[ 2 ];

	return A * TMath::Erf( -sig * ( x0 - h ) ) + A;

}







