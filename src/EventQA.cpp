
#include "EventQA.h"
#include "ChainLoader.h"
#include "RefMultPicoDst.h"


/**
 * Constructor
 */
EventQA::EventQA( XmlConfig * config, string np){

	//Set the Root Output Level
	gErrorIgnoreLevel=kSysError;

	// Save Class Members
	cfg = config;
	nodePath = np;
 
	// make the Logger
	lg = LoggerConfig::makeLogger( cfg, np + "Logger" );

	lg->info(__FUNCTION__) << "Got config with nodePath = " << np << endl;
	lg->info( __FUNCTION__) << "Making Chain and PicoDataStore Interface " << endl;
	//Create the chain
	TChain * chain = new TChain( cfg->getString(np+"input.dst:treeName", "tof" ).c_str() );
    ChainLoader::load( chain, cfg->getString( np+"input.dst:url" ).c_str(), cfg->getInt( np+"input.dst:maxFiles" ) );

    pico = new RefMultPicoDst( chain );

    // create the book
    lg->info(__FUNCTION__) << " Creating book "<< endl;
    book = new HistoBook( config->getString( np + "output.data" ), config );

    reporter = new Reporter( config->getString( np + "output.report" ) );

    /**
     * Setup the event cuts
     */
    cutVertexZ = new ConfigRange( cfg, np + "eventCuts.vertexZ", -200, 200 );
    cutVertexR = new ConfigRange( cfg, np + "eventCuts.vertexR", 0, 10 );
    cutVertexROffset = new ConfigPoint( cfg, np + "eventCuts.vertexROffset", 0.0, 0.0 );

    
    cutTriggers = cfg->getIntVector( np + "eventCuts.triggers" );
    vector<string> centralPaths = cfg->childrenOf( np + "centrality" );

}
/**
 * Destructor
 */
EventQA::~EventQA(){

	/** 
	 * DELETE CONFIGRNGES ETC.!!!
	 */
	delete book;
	delete reporter;

	lg->info(__FUNCTION__) << endl;
	delete lg;
}



void EventQA::preLoop(){

}

void EventQA::makeRunMap(){
	
	lg->info(__FUNCTION__) << endl;
	Int_t nEvents = (Int_t)pico->getTree()->GetEntries();
	nEventsToProcess = cfg->getInt( nodePath+"input.dst:nEvents", nEvents );
	lg->info(__FUNCTION__) << "Reading " << nEvents << " events from chain" << endl;

	//TaskProgress tp( "Run Map Loop", nEventsToProcess );

	cout << "<runMap>" ;
	for ( Int_t iEvent = 0; iEvent < nEventsToProcess; iEvent ++ ){
		pico->GetEntry(iEvent);

		//tp.showProgress( iEvent );

		if ( !eventCut( ) )
			continue;
		
		UInt_t run = pico->eventRunId();
    	vector<int>::iterator it = std::find( runs.begin(), runs.end(), run );
    	if ( it == runs.end() ){
    		runMap[ run ] = runs.size();
    		cout << run << ", ";
    		runs.push_back( run  );
    	}



	}
	cout << "</runMap>" ;


}

void EventQA::eventLoop(){
	lg->info(__FUNCTION__) << endl;

	Int_t nEvents = (Int_t)pico->getTree()->GetEntries();
	nEventsToProcess = cfg->getInt( nodePath+"input.dst:nEvents", nEvents );

	lg->info(__FUNCTION__) << "Reading " << nEvents << " events from chain" << endl;


	/**
	 * Make the Histograms
	 */
	book->cd();
	lg->info(__FUNCTION__) << "Making all histograms in : " << nodePath + "histograms" << endl;
	book->makeAll( nodePath + "histograms" );

	preLoop();

	TaskProgress tp( "Event Loop", nEventsToProcess );

	for ( Int_t iEvent = 0; iEvent < nEventsToProcess; iEvent ++ ){
		pico->GetEntry(iEvent);

		tp.showProgress( iEvent );

		
		if ( !eventCut( ) )
			continue;

		analyzeEvent();
		
		Int_t nTracks = pico->eventNumTracks();

		for ( Int_t iTrack = 0; iTrack < nTracks; iTrack ++ ){

			if ( !trackCut( iTrack ) )
				continue;
			
			analyzeTrack( iTrack );	

		}

	}

	postLoop();

	lg->info(__FUNCTION__) << "Complete" << endl;

}

void EventQA::analyzeEvent() {

	//vector<UInt_t> trIds = pico->eventTriggerIds();
	//cout << pico->eventTriggerIds().size() << endl;

	UInt_t run = pico->eventRunId();
	vector<int>::iterator it = std::find( runs.begin(), runs.end(), run );
	if ( it == runs.end() ){
		runMap[ run ] = runs.size();
		runs.push_back( run  );
	}

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

}


void EventQA::analyzeTrack( Int_t iTrack ){

	UInt_t run = pico->eventRunId();
	int ri = runMap[ run ];
	book->fill( "ptPrimary", ri, pico->trackPt( iTrack ) );
	book->fill( "etaPrimary", ri, pico->trackEta( iTrack ) );
	book->fill( "pPrimary", ri, pico->trackP( iTrack ) );
	book->fill( "chargePrimary", ri, pico->trackCharge( iTrack ) );

	book->fill( "dcaX", ri, pico->trackDcaX( iTrack ) );
	book->fill( "dcaY", ri, pico->trackDcaY( iTrack ) );
	book->fill( "dcaZ", ri, pico->trackDcaZ( iTrack ) );
	book->fill( "dca", ri, pico->trackDca( iTrack ) );

	if ( pico->trackTofMatch( iTrack ) >= 1 ){
		book->fill( "tof", ri, pico->trackTof(iTrack) );
		book->fill( "pathLength", ri, pico->trackPathLength(iTrack) );
	}

}

bool EventQA::eventCut(){

	// if a trigger list is given then filter,
	// if not don't worry about triggers
	if ( cutTriggers.size() >= 1  ){

		vector<UInt_t> triggerIds = pico->eventTriggerIds();
		bool findTrigger = false;
		for ( int iTrig = 0; iTrig < cutTriggers.size(); iTrig++ ){
			if ( triggerIds.end() != find( triggerIds.begin(), triggerIds.end(), (UInt_t)cutTriggers[ iTrig ] ) ){
				findTrigger = true;
			}
		}
		if ( !findTrigger )
			return false;
	}

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

bool EventQA::trackCut( Int_t iTrack ){


/*
	double beta = pico->trackBeta( iTrack );

	if ( (1.0 / beta) < 0.5 )
		return false; 

	double eta = pico->trackEta( iTrack );

	if ( eta > .5 )
		return false;

	double dcaX = pico->trackDcaX( iTrack );
	double dcaY = pico->trackDcaY( iTrack );
	double dcaZ = pico->trackDcaZ( iTrack );
	double dca = TMath::Sqrt( dcaX*dcaX + dcaY*dcaY + dcaZ*dcaZ );

	int tofMatch = pico->trackTofMatch( iTrack );
	int nHits = pico->trackNHits( iTrack );
	int nHitsDedx = pico->trackNHitsDedx( iTrack );
	int nHitsFit = pico->trackNHitsFit( iTrack );
	int nHitsPossible = pico->trackNHitsPossible( iTrack );
	double fitPoss = ( (double)nHitsFit /  (double)nHitsPossible);

	double yLocal = pico->trackYLocal( iTrack );

	book->fill( "preTofMatch", tofMatch );
	book->fill( "preYLocal", yLocal );
	book->fill( "preDca", dca );
	book->fill( "preNHits", nHits );
	book->fill( "preNHitsDedx", nHitsDedx );
	book->fill( "preNHitsFit", nHitsFit );
	book->fill( "preNHitsPossible", nHitsPossible );
	book->fill( "preNHitsFitOverPossible", fitPoss );
	

	if ( tofMatch < cutTofMatch->min || tofMatch > cutTofMatch->max )
		return false;
	if ( yLocal < cutYLocal->min || yLocal > cutYLocal->max )
		return false;
	if ( dca < cutDca->min || dca > cutDca->max )
		return false;
	if ( nHitsDedx < cutNHitsDedx->min || nHitsDedx > cutNHitsDedx->max )
		return false;
	if ( nHits < cutNHits->min || nHits > cutNHits->max )
		return false;
	if ( fitPoss < cutNHitsFitOverPossible->min || fitPoss > cutNHitsFitOverPossible->max )
		return false; 

	book->fill( "tofMatch", tofMatch );
	book->fill( "yLocal", yLocal );
	book->fill( "dca", dca );
	book->fill( "nHits", nHits );
	book->fill( "nHitsDedx", nHitsDedx );
	book->fill( "nHitsFit", nHitsFit );
	book->fill( "nHitsPossible", nHitsPossible );
	book->fill( "nHitsFitOverPossible", fitPoss );
*/
	return true;
}






