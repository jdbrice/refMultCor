#include "VtxSeed.h"
#include "TMath.h"

VtxSeed::VtxSeed( XmlConfig * config, string np, string fl, string jp) : TreeAnalyzer( config, np, fl, jp ){


	tm = new TreeMap( chain );

}

VtxSeed::~VtxSeed(){


}



void VtxSeed::make() {
	TreeAnalyzer::make();
}

void VtxSeed::preEventLoop(){
	book->cd("preCuts");
	TreeAnalyzer::preEventLoop();

	book->cd("postCuts");
	TreeAnalyzer::preEventLoop();
}


bool VtxSeed::keepEvent(){


	double yOffset = -0.90;

	double vX = tm->get( "x" );
	double vY = tm->get( "y" ) - yOffset;
	double vR = TMath::Sqrt( vX*vX + vY*vY );

	//if ( vR > 1.0 )
	//	return false;
	//	
	if ( tm->get( "tmatch" ) < 2 )
		return false;


	return true;
}


void VtxSeed::analyzeEvent() {


	book->cd("postCuts");
	fillEventHists();



}

void VtxSeed::fillEventHists(){
	book->fill( "vtxX", tm->get( "x" ) );
	book->fill( "vtxY", tm->get( "y" ) );
	book->fill( "vtxZ", tm->get( "z" ) );
	book->fill( "tofMatch", tm->get( "tmatch" ) );
	double errorR = TMath::Sqrt( tm->get( "ex" )*tm->get( "ex" ) + tm->get( "ey" )*tm->get( "ey" ) );
	book->fill( "errorR", errorR );

	book->fill( "vtxXZ", tm->get( "z" ), tm->get( "x" ) );
	book->fill( "vtxYZ", tm->get( "z" ), tm->get( "y" ) );

	book->fill( "vtxXMult", tm->get( "mult" ), tm->get( "x" ) );
	book->fill( "vtxYMult", tm->get( "mult" ), tm->get( "y" ) );

	book->fill( "errorXY", tm->get( "ex" ), tm->get( "ey" ) );
}



void VtxSeed::analyzeEventBeforeCuts() {

	book->cd("preCuts");
	fillEventHists();


}


void VtxSeed::postEventLoop(){

	book->cd( "postCuts" );
	gStyle->SetOptStat( 0 );

	vector<string> histos = cfg->childrenOf( nodePath + "histograms" );
	for ( int i = 0; i < histos.size(); i++ ){

		string hName = cfg->getString( histos[ i ] + ":name" );

		reporter->newPage();

		if ( book->is2D( hName ) )
			book->style( hName )->set( nodePath + "style.all2D" );
		if ( book->is1D( hName ) )
			book->style( hName )->set( nodePath + "style.all1D" );
		book->style( hName )->set( nodePath + "style." + hName )->draw();

		reporter->savePage();

	}


}




