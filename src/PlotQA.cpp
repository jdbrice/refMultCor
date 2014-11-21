#include "PlotQA.h"
#include "EventQA.h"

#include "RefMultCorrection.h"

PlotQA::PlotQA( XmlConfig * config, string np ) : HistoAnalyzer( config, np ){

	//
	logger->setClassSpace( "PlotQA" );
	logger->info( __FUNCTION__ ) << endl;

	logger->info(__FUNCTION__) << "Making Z Vertex Report from " << np+"refMultZ.Reporter" << endl;
	rpZ = new Reporter( cfg, np+"refMultZ.Reporter." );

	vector<string> children = cfg->childrenOf( np+"timePeriods" );
	for ( int i =0; i < children.size(); i++ ){
		ConfigRange * cr = new ConfigRange( cfg, children[ i ] );
		period.push_back( cr );
		logger->info( __FUNCTION__ ) << " Initializing Run range [ " << i << " ] = " << cr->toString() << endl;
	}

	

}

PlotQA::~PlotQA(){

	delete rpZ;

}

void PlotQA::make(){

	gStyle->SetOptStat( 0 );

	makeRunByRun();
	makeRefMultVersus();
	makeZVertexProjections();

	//reportBadRuns();

}

void PlotQA::makeRunByRun(){
	logger->info( __FUNCTION__ ) << "{" << endl;
	vector<string> hNames = cfg->getStringVector( nodePath + "runByRun" );

	book->cd();

	TH1D * hEvents = entrySliceX((TH2D*)inFile->Get( "events" ));
	book->add( "nEvents", hEvents );
	
	findBadRunsFromEvents( hEvents, cfg->getInt( nodePath+"badRunCuts.events:min" ) );


	logger->info( __FUNCTION__ ) << "Found " << ts( (int)hNames.size() ) << " Histograms" << endl;

	/**
	 * Find all the bad runs
	 */
	for ( int i = 0; i < hNames.size(); i++ ){
	
		TH2D * h2 = (TH2D*)inFile->Get( hNames[ i ].c_str() );
		logger->info( __FUNCTION__ ) << "Processing " << hNames[ i ] << endl;
		
		TProfile * hP = (TProfile*)h2->ProfileX()->Clone( hNames[ i ].c_str() );
		double sigma = cfg->getDouble( nodePath+"badRunCuts."+hNames[i]+":sigma", -1 );
		if ( sigma > 0  ){
			for ( int j = 0; j < period.size(); j++ ){	
				findBadRuns( hP, sigma, period[ j ] );
			}
		}

	}

	/**
	 * Events
	 */
	reporter->newPage();
	book->style( "nEvents" )->set( nodePath+"style.allRunByRun" )->set("domain", 1, nRuns+5 )->draw();
	TH1D * bRuns = badRunsHist( (TH1D*)book->get( "nEvents" ), "badRuns_nEvents" );
	book->add( "badRuns_nEvents", bRuns );
	book->style( "badRuns_nEvents" )->set( nodePath+"style.bad" )->draw();
	reporter->savePage();
	reporter->saveImage( "img/Events.pdf" );
	/**
	 * Draw everything
	 */
	for ( int i = 0; i < hNames.size(); i++ ){
	
		TH2D * h2 = (TH2D*)inFile->Get( hNames[ i ].c_str() );
		logger->info( __FUNCTION__ ) << "Processing " << hNames[ i ] << endl;

		
		reporter->newPage();

		/**
		 * Make the Profile and save it in the book
		 */
		TProfile * hP = (TProfile*)h2->ProfileX()->Clone( hNames[ i ].c_str() );
		book->add( hNames[ i ], hP );

		string stylePath = nodePath+"style."+hNames[ i ];
		if ( cfg->nodeExists( stylePath ) )
			book->style( hNames[ i ] )->set( stylePath ); 


		book->style( hNames[ i ] )->set(nodePath+"style.allRunByRun" )->draw();
		double sigma = cfg->getDouble( nodePath+"badRunCuts."+hNames[i]+":sigma", -1 );
		if ( sigma > 0 ){
			for ( int j = 0; j < period.size(); j++ )
				drawWithAcceptanceBands( hP, sigma, period[ j ] );
		}
		
		TH1D * bRuns = badRunsHist( hP, "badRuns_"+hNames[ i ] );
		book->add( "badRuns_"+hNames[ i ], bRuns );
		book->style( "badRuns_"+hNames[ i ] )->set( nodePath+"style.bad" )->draw();

		reporter->savePage();
		reporter->saveImage( "img/" + hNames[ i ] + ".pdf" );

	}

	

	logger->info( __FUNCTION__ ) << "}" << endl;
}

void PlotQA::makeRefMultVersus(){

	logger->info( __FUNCTION__ ) << "{" << endl;
	vector<string> hNames = cfg->getStringVector( nodePath + "refMultVersus" );

	book->cd();

	logger->info( __FUNCTION__ ) << "Found " << ts( (int)hNames.size() ) << " Histograms" << endl;

	for ( int i = 0; i < hNames.size(); i++ ){
	
		TH2D * h2 = (TH2D*)inFile->Get( hNames[ i ].c_str() );
		if ( !h2 )
			continue;
		logger->info( __FUNCTION__ ) << "Processing " << hNames[ i ] << endl;

		reporter->newPage();

		TProfile * hP = (TProfile*)h2->ProfileX()->Clone( hNames[ i ].c_str() );
		book->add( hNames[ i ], hP );

		string stylePath = nodePath+"style."+hNames[ i ];
		if ( cfg->nodeExists( stylePath ) )
			book->style( hNames[ i ] )->set( stylePath ); 

		book->style( hNames[ i ] )->set(nodePath+"style.allRefMultVersus" )->draw();

		reporter->savePage();
		reporter->saveImage( "img/" + hNames[ i ] + ".pdf" );

	}

	logger->info( __FUNCTION__ ) << "}" << endl;

}

TH1D* PlotQA::badRunsHist( TH1D* hIn, string name ){

	TH1D* h = (TH1D*)hIn->Clone( name.c_str() );
	for ( int i = 1; i < nRuns; i++ ){
		if ( !badRuns[ i ] ){
			h->SetBinContent( i, 0 );
			h->SetBinError( i, 0 );
		} else {
			
		}
	}
	//h->SetMarkerColor( kRed );

	return h;

}

void PlotQA::drawWithAcceptanceBands( TProfile* pr, double nSig, ConfigRange * cr ){
	

	pr->GetXaxis()->SetRange( cr->min, cr->max );
	double m = meanForPeriod( pr, cr );
	double rms = rmsForPeriod(pr, cr) * nSig;
	logger->info(__FUNCTION__) << "Sig = " << pr->GetRMS(2) << endl;

	pr->GetXaxis()->SetRange( 1, nRuns );
	//pr->GetYaxis()->SetRangeUser( m - rms * 1.5, m + rms * 1.5  );

	if ( nSig > 0 ){
		int x1 = cr->min;
		int x2 = cr->max;
		TLine * c = new TLine( x1, m, x2, m );
		TLine * ub = new TLine( x1, m+rms, x2, m+rms );
		TLine * lb = new TLine( x1, m-rms, x2, m-rms );

		c->SetLineColor( kBlue );
		c->SetLineWidth( 3 );
		lb->SetLineColor( kRed );
		lb->SetLineWidth( 3 );
		ub->SetLineColor( kRed );
		ub->SetLineWidth( 3 );

		c->Draw();
		lb->Draw();
		ub->Draw();	
	}
	
}

void PlotQA::findBadRuns( TProfile* pr, double nSig, ConfigRange * cr ) {

	pr->GetXaxis()->SetRange( cr->min, cr->max );
	double m = meanForPeriod( pr, cr );
	double rms = rmsForPeriod(pr, cr) * nSig;

	pr->GetXaxis()->SetRange( 1, nRuns );
	logger->info( __FUNCTION__ ) << " Range = ( " << (m - rms) << ", " << (m+rms)<< " ) " << endl; 
	for ( int i = cr->min; i < cr->max; i++ ){

		double val = pr->GetBinContent( i );
		if ( val < m - rms || val > m + rms ){
			logger->info( __FUNCTION__ ) << "[ " << i << " ] = " << val << endl; 
			logger->info(__FUNCTION__) << " found bad run in " << pr->GetTitle() << " run # " << i << endl;
			badRuns[ i ] = true;
		}
	}
}

void PlotQA::findBadRunsFromEvents( TH1D *h, double nEvents ){

	for ( int i = 1; i <= nRuns; i++ ){
		double val = h->GetBinContent( i );
		if ( val < nEvents ){
			logger->info(__FUNCTION__) << " found bad run in " << h->GetTitle() << " run # " << i << endl;
			badRuns[ i ] = true;
		}
	}

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
		h1->SetBinContent( i, m );
	}
	return h1;

}

TH1* PlotQA::isolate( TH1 * hIn, int x1, int x2 ){

	TH1 * h1 = (TH1*)hIn->Clone( "tmp" );

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
		double bc = hIn->GetBinContent( i );
		//cout << " BC = " << bc << endl;
		h1->SetBinContent( i, bc );
		//cout << " BC = " << bc << endl;
		h1->SetBinError( i, hIn->GetBinError( i ) );
	}

	return h1;
}



void PlotQA::makeZVertexProjections(){
	logger->setLogLevel( Logger::llInfo  );

	gStyle->SetOptFit( 1 );

	double fX1 = cfg->getDouble( nodePath + "refMultZ.fit:x1", 100 );
	double fX2 = cfg->getDouble( nodePath + "refMultZ.fit:x2", 300 );

	TH2D * h2 = (TH2D*)inFile->Get( "refMultZ_1" );
	TAxis * x = h2->GetXaxis();
	vector<double> ranges = cfg->getDoubleVector( nodePath + "refMultZ.ranges" );

	logger->info(__FUNCTION__) << " Histogram range : ( " << ranges[0]+5 << ", " << ranges[ranges.size()-1]-5 << endl;
	book->make1D( "fRes", "zVertex", ranges.size()-1, ranges[0], ranges[ranges.size()-1] );

	for ( int i = 0; i < ranges.size() - 1; i++ ){
		int x1 = x->FindBin( ranges[ i ] );
		int x2 = x->FindBin( ranges[ i+1 ] );
		logger->info(__FUNCTION__) << "Project ( " << ranges[ i ] << " cm, " << ranges[ i+1] << "cm ) -> ( " << x1 << ", " << x2 << " ) " << endl;


		string name = ("refMult_" + dts( ranges[i] ));
		TH1D * h1 = h2->ProjectionY( name.c_str(), x1, x2 );
		book->add( name, h1 );

		h1->SetTitle( ( dts( ranges[i] ) + " cm < z < " + dts( ranges[i+1] ) + " cm" ).c_str() );
		rpZ->newPage();
		
		book->style( name )->set( nodePath+"style.allZProjections" )->draw();
		
		TF1 * f1 = new TF1( "fTail", fTail, fX1, fX2, 3 );
		f1->SetParameters( 10000, .01, 300 );
		f1->SetLineWidth( 7 );
		//f1->Draw();
		book->get( name )->Fit( f1, "RQ" );
		book->get( name )->Fit( f1, "RQ" );
		book->get( name )->Fit( f1, "RQ" );
		book->get( "fRes" )->SetBinContent( (i+1), f1->GetParameter( 2 ) );
		double h = f1->GetParameter( 2 );
		logger->info( __FUNCTION__ ) << "Step : " << i << endl;

		TLine * hLine = new TLine( h, 0, h, f1->Eval( h )  );
		hLine->SetLineColor( kRed );
		hLine->SetLineStyle( kDashed );
		hLine->SetLineWidth( 7 );
		hLine->Draw();

		rpZ->savePage();
		rpZ->saveImage( "img/Z/" + name + ".pdf" );

	}

	rpZ->newPage();
		TF1* p6 = new TF1( "pol6", "pol6", -65, 65 );
		book->get("fRes")->Fit( p6, "QN" );
		book->style( "fRes" )->set( nodePath+"style.zFit" )->draw();
		p6->Draw("SAME");
	cout << "zParams : " ;
	for ( int i = 0; i < 7; i++ ){
		cout << p6->GetParameter( i );
		if ( i != 6 )
			cout << ", ";
	}
	cout << endl;
	rpZ->savePage();
	rpZ->saveImage( "img/Z/maxRefMult.pdf" );
	

}


double PlotQA::fTail( Double_t * x, Double_t * par ){

	double x0 = x[0];
	double A = par[ 0 ];
	double sig = par[ 1 ];
	double h = par[ 2 ];

	return A * TMath::Erf( -sig * ( x0 - h ) ) + A;

}

double PlotQA::meanForPeriod( TProfile * h, ConfigRange * cr ){

	double total = 0;
	for ( int i = cr->min; i < cr->max; i++ ){
		total += h->GetBinContent( i );
	}
	return ( total / ( cr->max - cr->min) );
}

double PlotQA::rmsForPeriod( TProfile * h, ConfigRange * cr ){

	double m = meanForPeriod( h, cr );
	double total = 0;
	for ( int i = cr->min; i < cr->max; i++ ){
		double bc = h->GetBinContent( i );
		total += ( m - bc )*( m - bc );
	}
	return TMath::Sqrt( total / ( cr->max - cr->min) );
}


void PlotQA::reportBadRuns(){

	cout << " badRuns = { ";
	for ( int i = 1; i <= nRuns; i++ ){
		if ( badRuns[ i ] ){
			cout << EventQA::runList[ i - 1 ];
			if ( i != nRuns )
				cout <<", ";
			cout << endl; 

		}
	}
	cout <<" }" << endl;


}




