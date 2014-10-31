#include "PlotQA.h"

#include "TLine.h"
#include "TMath.h"
#include "TF1.h"

PlotQA::PlotQA( XmlConfig * config, string np ) : HistoAnalyzer( config, np ){

	lg->setClassSpace( "PlotQA" );
	lg->info( __FUNCTION__ ) << endl;

	lg->info(__FUNCTION__) << "Making Z Vertex Report from " << np+"refMultZ.Reporter" << endl;
	rpZ = new Reporter( cfg, np+"refMultZ.Reporter." );

}

PlotQA::~PlotQA(){

	delete rpZ;

}

void PlotQA::make(){

	gStyle->SetOptStat( 0 );

	lg->info( __FUNCTION__ ) << "{" << endl;
	vector<string> hNames = cfg->getStringVector( nodePath + "makeMeanHistos" );

	book->cd();

	lg->info( __FUNCTION__ ) << "Found " << ts( (int)hNames.size() ) << " Histograms" << endl;
	for ( int i = 0; i < hNames.size(); i++ ){
	
		TH2D * h2 = (TH2D*)inFile->Get( hNames[ i ].c_str() );
		h2->GetXaxis()->SetRange( 200, 1200 );
		
		lg->info( __FUNCTION__ ) << "Processing " << hNames[ i ] << endl;


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
	
		} else if ( "ptPrimary" == hNames[ i ] ){
			TH1D * pt = drawWithAcceptanceBands( h2, 3);
		} else if ( "refMult" == hNames[ i ] ){
			TH1D * rm = drawWithAcceptanceBands( h2, 4);
		}

		reporter->savePage();

	}

	TH2D * h2 = (TH2D*)inFile->Get( "events" );
	TH1D * h1 = entrySliceX( h2 );
	reporter->newPage();
	h1->Draw( "pel" );
	h1->SetMarkerStyle( 7 );
	reporter->savePage();

	//makeZVertexProjections();

	lg->info( __FUNCTION__ ) << "}" << endl;

}

TH1D* PlotQA::drawWithAcceptanceBands( TH2D* h2, double nSig ){

	TH1D * h1 = meanSliceX( h2 );
	//mirrorOverY( h1 );

	h1->Draw( "pel" );
	h1->SetMarkerStyle( 9 );
	
	//h1->GetXaxis()->SetRangeUser( 0, 1200 );
	
	double m = h2->GetMean(2);
	double rms = h2->GetRMS(2) * nSig;
	lg->info(__FUNCTION__) << "Sig = " << h2->GetRMS(2) << endl;

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

	return h1;

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
	book->add( ((string)"_"+(string)h2->GetTitle()), h1 );



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
	book->add( ((string)"_"+(string)h2->GetTitle()), h1 );

	return h1;

}

void PlotQA::mirrorOverY( TH1D * hIn ){

	TH1D * h1 = (TH1D*)hIn->Clone( "tmp" );
	int nBinsX = hIn->GetNbinsX();
	for ( int i = 1; i <= nBinsX; i++ ){
		hIn->SetBinContent( (nBinsX - i + 1), h1->GetBinContent( i ) );
		hIn->SetBinError( (nBinsX - i + 1), h1->GetBinError( i ) );
	}
	book->add( ((string)"_"+(string)h1->GetTitle()), h1 );
}

void PlotQA::translateX( TH1D * hIn, int x ){

	TH1D * h1 = (TH1D*)hIn->Clone( "tmp" );
	int nBinsX = hIn->GetNbinsX();
	for ( int i = 1; i <= nBinsX; i++ ){
		hIn->SetBinContent( i, 0 );
		hIn->SetBinError( i, 0 );
	}
	for ( int i = 1; i <= nBinsX; i++ ){
		if ( i + x < 0 || i + x > nBinsX )
			continue;
		hIn->SetBinContent( i + x, h1->GetBinContent( i ) );
		hIn->SetBinError( i + x, h1->GetBinError( i ) );
	}
	book->add( ((string)"_"+(string)h1->GetTitle()), h1 );
}



void PlotQA::makeZVertexProjections(){

	TH2D * h2 = (TH2D*)inFile->Get( "refMultZ" );
	TAxis * x = h2->GetXaxis();
	vector<double> ranges = cfg->getDoubleVector( nodePath + "refMultZ.ranges" );

	for ( int i = 0; i < ranges.size() - 1; i++ ){
		int x1 = x->FindBin( ranges[ i ] );
		int x2 = x->FindBin( ranges[ i+1 ] );
		lg->info(__FUNCTION__) << "Project ( " << ranges[ i ] << ", " << ranges[ i+1] << " ) -> ( " << x1 << ", " << x2 << " ) " << endl;


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
/*
UInt_t PlotQA::runList[] = {
15070021,
15070020,
15070019,
15070018,
15070017,
15070016,
15070015,
15070014,
15070013,
15070012,
15070009,
15070007,
15070006,
15070005,
15070004,
15070003,
15070002,
15070001,
15069051,
15069050,
15069049,
15069048,
15069047,
15069046,
15069045,
15069044,
15069043,
15069042,
15069041,
15069040,
15069039,
15069038,
15069037,
15069036,
15069035,
15069034,
15069033,
15069032,
15069031,
15069030,
15069023,
15069022,
15069021,
15069020,
15069019,
15069018,
15069017,
15069016,
15069015,
15069014,
15069013,
15069012,
15069011,
15069010,
15069009,
15069008,
15069007,
15069006,
15069005,
15069004,
15069003,
15069002,
15069001,
15068049,
15068048,
15068047,
15068046,
15068045,
15068044,
15068043,
15068042,
15068041,
15068040,
15068039,
15068038,
15068037,
15068036,
15068035,
15068034,
15068033,
15068032,
15068031,
15068030,
15068029,
15068028,
15068027,
15068026,
15068025,
15068024,
15068023,
15068022,
15068018,
15068016,
15068014,
15068013,
15068012,
15068010,
15068009,
15068008,
15068007,
15068006,
15068005,
15068004,
15068003,
15068002,
15068001,
15067049,
15067048,
15067047,
15067046,
15067045,
15067044,
15067043,
15067042,
15067041,
15067040,
15067039,
15067038,
15067037,
15067036,
15067035,
15067034,
15067033,
15067032,
15067027,
15067026,
15067025,
15067024,
15067023,
15067022,
15067020,
15067019,
15067018,
15067016,
15067015,
15067014,
15067013,
15067012,
15067011,
15067009,
15067008,
15067006,
15067005,
15067003,
15067002,
15067001,
15066088,
15066086,
15066085,
15066083,
15066082,
15066077,
15066076,
15066075,
15066074,
15066073,
15066072,
15066071,
15066070,
15066069,
15066068,
15066067,
15066066,
15066065,
15066064,
15066026,
15066025,
15066024,
15066023,
15066022,
15066021,
15066020,
15066019,
15066018,
15066017,
15066016,
15066014,
15066013,
15066012,
15066010,
15066008,
15066007,
15066006,
15066005,
15065061,
15065060,
15065059,
15065058,
15065057,
15065056,
15065054,
15065053,
15065052,
15065051,
15065050,
15065049,
15065048,
15065046,
15065041,
15065040,
15065039,
15065038,
15065037,
15065036,
15065035,
15065034,
15065033,
15065032,
15065031,
15065030,
15065029,
15065028,
15065023,
15065022,
15065019,
15065017,
15065015,
15065014,
15065013,
15065012,
15065011,
15064011,
15064010,
15064009,
15064008,
15064007,
15064006,
15064005,
15064003,
15064002,
15064001,
15063067,
15063066,
15063065,
15063064,
15063063,
15063062,
15063061,
15063060,
15063059,
15063058,
15063057,
15063056,
15063053,
15063051,
15063050,
15063049,
15063048,
15063047,
15063046,
15063045,
15063043,
15063042,
15063041,
15063040,
15063039,
15063038,
15063037,
15063036,
15063035,
15063034,
15063032,
15063031,
15063030,
15063029,
15063021,
15063020,
15063018,
15063017,
15063016,
15063014,
15063013,
15063012,
15063011,
15063010,
15063009,
15063008,
15063006,
15063004,
15063003,
15063002,
15063001,
15062077,
15062076,
15062075,
15062074,
15062073,
15062072,
15062071,
15062070,
15062069,
15062068,
15062067,
15062066,
15062045,
15062044,
15062043,
15062042,
15062041,
15062040,
15062038,
15062037,
15062036,
15062035,
15062034,
15062033,
15062032,
15062031,
15062026,
15062025,
15062024,
15062023,
15062022,
15062021,
15062020,
15062019,
15062018,
15062017,
15062015,
15062014,
15062013,
15062012,
15062011,
15062010,
15062009,
15062008,
15062007,
15062006,
15062005,
15062004,
15062003,
15062002,
15061064,
15061063,
15061062,
15061061,
15061060,
15061059,
15061058,
15061056,
15061055,
15061054,
15061053,
15061052,
15061051,
15061050,
15061049,
15061048,
15061047,
15061046,
15061041,
15061039,
15061038,
15061037,
15061036,
15061034,
15061028,
15061027,
15061026,
15061025,
15061024,
15061023,
15061021,
15061019,
15061018,
15061017,
15061016,
15061015,
15061014,
15061013,
15061012,
15061011,
15061010,
15061009,
15061008,
15061007,
15061006,
15061005,
15061004,
15061003,
15061002,
15061001,
15060071,
15060070,
15060069,
15060068,
15060067,
15060065,
15060064,
15060063,
15060062,
15060061,
15060060,
15060059,
15060057,
15060056,
15060055,
15060054,
15060053,
15060052,
15060051,
15060050,
15060049,
15060048,
15060047,
15060046,
15060045,
15060044,
15060043,
15060037,
15060036,
15060035,
15060033,
15060032,
15060031,
15060029,
15060028,
15060027,
15060025,
15060024,
15060023,
15060022,
15060021,
15060020,
15060019,
15060018,
15060017,
15060016,
15060015,
15060014,
15060012,
15060011,
15060010,
15060009,
15060008,
15060007,
15060006,
15060005,
15060002,
15060001,
15059090,
15059088,
15059087,
15059086,
15059085,
15059084,
15059083,
15059082,
15059081,
15059080,
15059079,
15059078,
15059077,
15059076,
15059074,
15059072,
15059071,
15059070,
15059068,
15059067,
15059066,
15059065,
15059064,
15059063,
15059061,
15059060,
15059059,
15059058,
15059057,
15059056,
15059055,
15059042,
15059041,
15059040,
15059039,
15059038,
15059037,
15059036,
15059035,
15059034,
15059033,
15059029,
15059028,
15059027,
15059026,
15059025,
15059024,
15059022,
15059021,
15059020,
15059019,
15059018,
15059017,
15059016,
15059015,
15059014,
15059013,
15059012,
15059011,
15059010,
15059009,
15059008,
15059007,
15059006,
15059005,
15059004,
15059003,
15059002,
15059001,
15058055,
15058054,
15058053,
15058052,
15058051,
15058050,
15058049,
15058048,
15058047,
15058046,
15058045,
15058044,
15058043,
15058042,
15058041,
15058040,
15058039,
15058033,
15058032,
15058030,
15058029,
15058025,
15058024,
15058022,
15058021,
15058020,
15058019,
15058018,
15058017,
15058016,
15058015,
15058014,
15058013,
15058012,
15058011,
15058010,
15058009,
15058008,
15058007,
15058006,
15058005,
15058004,
15058003,
15058002,
15058001,
15057063,
15057062,
15057061,
15057060,
15057059,
15057058,
15057057,
15057056,
15057055,
15057054,
15057053,
15057052,
15057051,
15057050,
15057049,
15057048,
15057032,
15057031,
15057030,
15057029,
15057023,
15057022,
15057021,
15057020,
15057019,
15057018,
15057017,
15057016,
15057015,
15057014,
15057013,
15057012,
15057011,
15057010,
15057008,
15057007,
15057006,
15057005,
15057004,
15057003,
15057002,
15057001,
15056125,
15056124,
15056123,
15056121,
15056120,
15056119,
15056117,
15056116,
15056114,
15056113,
15056039,
15056038,
15056037,
15056036,
15056035,
15056028,
15056027,
15056026,
15056025,
15056024,
15056023,
15056022,
15056021,
15056020,
15056019,
15056018,
15056017,
15056016,
15056015,
15056014,
15056013,
15056012,
15056011,
15056010,
15056009,
15056008,
15056007,
15056006,
15056005,
15056004,
15056003,
15056002,
15056001,
15055141,
15055140,
15055139,
15055138,
15055137,
15055136,
15055135,
15055134,
15055133,
15055132,
15055131,
15055021,
15055020,
15055019,
15055018,
15055017,
15055016,
15055015,
15055014,
15055013,
15055012,
15055011,
15055010,
15055009,
15055008,
15055007,
15055006,
15055005,
15055004,
15055003,
15055002,
15055001,
15054054,
15054053,
15054052,
15054051,
15054050,
15054049,
15054048,
15054047,
15054046,
15054044,
15054043,
15054042,
15054041,
15054038,
15054037,
15054031,
15054030,
15054029,
15054028,
15054027,
15054026,
15054025,
15054024,
15054023,
15054021,
15054020,
15054019,
15054018,
15054017,
15054016,
15054015,
15054014,
15054013,
15054012,
15054011,
15054010,
15054009,
15054008,
15054007,
15054006,
15054005,
15054004,
15054003,
15054002,
15054001,
15053067,
15053066,
15053065,
15053064,
15053062,
15053060,
15053059,
15053058,
15053057,
15053056,
15053055,
15053054,
15053053,
15053052,
15053050,
15053049,
15053048,
15053047,
15053046,
15053045,
15053044,
15053043,
15053042,
15053041,
15053040,
15053035,
15053034,
15053033,
15053031,
15053030,
15053029,
15053028,
15053027,
15053026,
15053025,
15053024,
15053023,
15053022,
15053021,
15053020,
15053019,
15053018,
15053017,
15053016,
15053015,
15053014,
15053013,
15053012,
15053011,
15053010,
15053009,
15053008,
15053007,
15053006,
15053005,
15053004,
15053003,
15053002,
15053001,
15052075,
15052074,
15052073,
15052070,
15052069,
15052068,
15052067,
15052066,
15052065,
15052064,
15052063,
15052062,
15052061,
15052060,
15052046,
15052045,
15052044,
15052043,
15052042,
15052041,
15052040,
15052026,
15052025,
15052024,
15052023,
15052022,
15052021,
15052020,
15052019,
15052018,
15052017,
15052016,
15052015,
15052014,
15052011,
15052010,
15052009,
15052008,
15052007,
15052006,
15052005,
15052004,
15052001,
15051160,
15051159,
15051157,
15051156,
15051155,
15051149,
15051148,
15051147,
15051146,
15051144,
15051141,
15051137,
15051134,
15051133,
15051132,
15051131,
15051122,
15050016,
15050015,
15050014,
15050013,
15050012,
15050011,
15050010,
15050006,
15050005,
15050004,
15050003,
15050002,
15050001,
15049099,
15049098,
15049097,
15049096,
15049095,
15049094,
15049093,
15049092,
15049091,
15049090,
15049089,
15049088,
15049087,
15049086,
15049085,
15049084,
15049083,
15049078,
15049077,
15049076,
15049075,
15049074,
15049041,
15049040,
15049039,
15049038,
15049037,
15049033,
15049032,
15049031,
15049030,
15049028,
15049027,
15049026,
15049025,
15049024,
15049023,
15049022,
15049021,
15049020,
15049019,
15049018,
15049017,
15049016,
15049015,
15049014,
15049013,
15049010,
15049009,
15049008,
15049007,
15049006,
15049005,
15049004,
15049003,
15049002,
15048098,
15048097,
15048096,
15048095,
15048094,
15048093,
15048092,
15048091,
15048089,
15048088,
15048087,
15048086,
15048085,
15048084,
15048083,
15048082,
15048081,
15048080,
15048079,
15048078,
15048077,
15048076,
15048075,
15048074,
15048073,
15048034,
15048033,
15048032,
15048031,
15048030,
15048029,
15048028,
15048026,
15048025,
15048024,
15048023,
15048022,
15048021,
15048020,
15048019,
15048018,
15048017,
15048016,
15048014,
15048013,
15048012,
15048011,
15048010,
15048009,
15048008,
15048007,
15048006,
15048005,
15048004,
15048003,
15048002,
15047106,
15047104,
15047102,
15047100,
15047098,
15047097,
15047096,
15047093,
15047089,
15047087,
15047086,
15047085,
15047082,
15047075,
15047074,
15047073,
15047072,
15047071,
15047070,
15047069,
15047068,
15047065,
15047064,
15047063,
15047062,
15047061,
15047057,
15047056,
15047053,
15047052,
15047050,
15047047,
15047044,
15047043,
15047042,
15047041,
15047040,
15047039,
15047038,
15047036,
15047035,
15047034,
15047030,
15047029,
15047028,
15047027,
15047026,
15047024,
15047023,
15047021,
15047019,
15047016,
15047015,
15047014,
15047012,
15047010,
15047007,
15047006,
15047005,
15047004,
15046111,
15046110,
15046109,
15046108,
15046107,
15046106,
15046105,
15046104,
15046103,
15046102,
15046101,
15046097,
15046096,
15046094,
15046093,
15046092,
15046091,
15046089,
15046073};
*/
