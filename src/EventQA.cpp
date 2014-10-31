
#include "EventQA.h"
#include "ChainLoader.h"
#include "RefMultPicoDst.h"
#include "TMath.h"

/**
 * Constructor
 */
EventQA::EventQA( XmlConfig * config, string np) : TreeAnalyzer( config, np ){

    /**
     * Setup the event cuts
     */
    cutVertexZ = new ConfigRange( cfg, np + "eventCuts.vertexZ", -200, 200 );
    cutVertexR = new ConfigRange( cfg, np + "eventCuts.vertexR", 0, 10 );
    cutVertexROffset = new ConfigPoint( cfg, np + "eventCuts.vertexROffset", 0.0, 0.0 );


    pico = new RefMultPicoDst( chain  );
}
/**
 * Destructor
 */
EventQA::~EventQA(){

	/** 
	 * DELETE CONFIGRNGES ETC.!!!
	 */
	delete cutVertexZ;
	delete cutVertexR;
	delete cutVertexROffset;
}



void EventQA::preEventLoop(){
	TreeAnalyzer::preEventLoop();

}

void EventQA::analyzeEvent() {

	UInt_t run = pico->eventRunId();

	int ri = runMap[ run ];

	double vX = pico->eventVertexX();
	double vY = pico->eventVertexY();

	book->fill( "events", ri, 1 );
	book->fill( "bbc", ri, pico->eventBBC() );
	book->fill( "nGlobal", ri, pico->eventNumGlobal() );
	book->fill( "nPrimary", ri, pico->eventNumPrimary() );
	book->fill( "vtxX", ri, vX );
	book->fill( "vtxY", ri, vY );
	book->fill( "vtxZ", ri, pico->eventVertexZ() );
	
	double vR = TMath::Sqrt( vX*vX + vY*vY );
	book->fill( "vtxR", ri, vR );

	book->fill( "tofMult", ri, pico->eventTofMult() );
	book->fill( "refMult", ri, pico->eventRefMult() );
	book->fill( "refMultZ", pico->eventVertexZ(), pico->eventRefMult() );


	/**
	 * Analyze the tracks
	 */
	Int_t nTracks = pico->eventNumTracks();

	for ( Int_t iTrack = 0; iTrack < nTracks; iTrack ++ ){

		if ( !keepTrack( iTrack ) )
			continue;
		
		analyzeTrack( iTrack );	

	}

}


void EventQA::analyzeTrack( Int_t iTrack ){

	UInt_t run = pico->eventRunId();
	int ri = runMap[ run ];
	book->fill( "ptPrimary", ri, pico->trackPt( iTrack ) );
	book->fill( "etaPrimary", ri, pico->trackEta( iTrack ) );
	book->fill( "pPrimary", ri, pico->trackP( iTrack ) );
	book->fill( "chargePrimary", ri, pico->trackCharge( iTrack ) );

}

bool EventQA::keepEvent(){

	double z = pico->eventVertexZ();
	double x = pico->eventVertexX() + cutVertexROffset->x;
	double y = pico->eventVertexY() + cutVertexROffset->y;
	double r = TMath::Sqrt( x*x + y*y );
	
	if ( z < cutVertexZ->min || z > cutVertexZ->max )
		return false;
	if ( r < cutVertexR->min || r > cutVertexR->max )
		return false;
	
	

	return true;
}

bool EventQA::keepTrack( Int_t iTrack ){

	return true;
}






