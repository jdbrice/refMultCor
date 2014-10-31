#include "PlotQA.h"

#include "TLine.h"

PlotQA::PlotQA( XmlConfig * config, string np ) : HistogramAnalyzer( config, np ){

	lg->setClassSpace( "PlotQA" );
	lg->info( __FUNCTION__ ) << endl;

}

PlotQA::~PlotQA(){

}

void PlotQA::make(){

	lg->info( __FUNCTION__ ) << "{" << endl;
	vector<string> hNames = cfg->getStringVector( nodePath + "makeMeanHistos" );

	book->cd();

	lg->info( __FUNCTION__ ) << "Found " << ts( (int)hNames.size() ) << " Histograms" << endl;
	for ( int i = 0; i < hNames.size(); i++ ){
	
		TH2D * h2 = (TH2D*)inFile->Get( hNames[ i ].c_str() );
		
		lg->info( __FUNCTION__ ) << "Processing " << hNames[ i ] << endl;


		TH1D * h1 = meanSliceX( h2 );
		reporter->newPage();
		h1->Draw( "pel" );
		//gPad->SetLogz();
		

		if ( "bbc" == hNames[ i ] ){

			h1->GetYaxis()->SetRangeUser( -1000, 5000 );
			h1->GetXaxis()->SetRangeUser( 0, 1000 );
			double m = h1->GetMean();
			double rms = h1->GetRMS() * 5;
			TLine * c = new TLine( 0, m, 1000, m );
			TLine * ub = new TLine( 0, m+rms, 1000, m+rms );
			TLine * lb = new TLine( 0, m-rms, 1000, m-rms );
			c->Draw();
			lb->Draw();
			ub->Draw();

		}

		reporter->savePage();

	}

	lg->info( __FUNCTION__ ) << "}" << endl;

}


TH1D* PlotQA::meanSliceX( TH2D* h2 ){

	TH1D * h1;
	int nBinsX = h2->GetNbinsX();

	h1 = new TH1D( ((string)"_"+(string)h2->GetTitle()).c_str(), h2->GetTitle(), nBinsX, 0, nBinsX );
	for ( int i = 1; i <= nBinsX; i++ ){

		// project then mean
		TH1D* py = h2->ProjectionY( "py", i, i );
		double m = py->GetMean();
		double me = py->GetMeanError();
		h1->SetBinContent( i, m );
		h1->SetBinError( i, me );

	}
	book->add( ((string)"_"+(string)h2->GetTitle()), h1 );



	return h1;
}







