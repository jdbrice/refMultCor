
#include "EventQA.h"
#include "ChainLoader.h"
#include "RefMultPicoDst.h"
#include "TMath.h"

/**
 * Constructor
 */
EventQA::EventQA( XmlConfig * config, string np, string fl, string jp) : TreeAnalyzer( config, np, fl, jp ){

    /**
     * Setup the event cuts
     */
    cutVertexZ = new ConfigRange( cfg, np + "eventCuts.vertexZ", -200, 200 );
    cutVertexR = new ConfigRange( cfg, np + "eventCuts.vertexR", 0, 10 );
    cutVertexROffset = new ConfigPoint( cfg, np + "eventCuts.vertexROffset", 0.0, 0.0 );
    /**
     * Setup track cuts
     */
    cutEta = new ConfigRange( cfg, np + "trackCuts.eta", -10, 10 );
    

    pico = new RefMultPicoDst( chain  );

    logger->setClassSpace( "EventQA" );

    rList.assign( runList, runList + nRuns );

    badRuns = cfg->getIntVector( np+"badRuns" );

    logger->info(__FUNCTION__) << "Found " << badRuns.size() << " Bad Runs " << endl;

    /**
     * Setup correction parameters
     */
    correctZ = config->getBool( np+"correctRefMult:zVertex", false );
    correctMCG = config->getBool( np+"correctRefMult:mcg", false );
    rmc = new RefMultCorrection( config->getString( np + "RMCParams" ) );

    vector<string> children = cfg->childrenOf( np+"timePeriods" );
	for ( int i =0; i < children.size(); i++ ){
		ConfigRange * cr = new ConfigRange( cfg, children[ i ] );
		period.push_back( cr );
		logger->info( __FUNCTION__ ) << " Initializing Run range [ " << i << " ] = " << cr->toString() << endl;
	}
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

	delete cutEta;
}



void EventQA::preEventLoop(){
	TreeAnalyzer::preEventLoop();
	logger->info( __FUNCTION__ ) << endl;

	for ( int i = 0; i < period.size(); i++ ){
		book->clone( "refMultZ", "refMultZ_" + ts(i) );
		book->clone( "refMultBBC", "refMultBBC_" + ts(i) );
		book->clone( "refMultZDC", "refMultZDC_" + ts(i) );
		book->clone( "refMultTOF", "refMultTOF_" + ts(i) );
	}
	
}

void EventQA::analyzeEvent() {
	
	UInt_t run = pico->eventRunId();

	int ri = runIndex( run );
	bool br = badRun( run );
	if ( ri < 0 || ri > nRuns || br ){
		return;
	}

	double vX = pico->eventVertexX();
	double vY = pico->eventVertexY();
	double vZ = pico->eventVertexZ();

	book->fill( "events", ri, 1 );
	book->fill( "eventsZ", vZ);
	book->fill( "bbc", ri, pico->eventBBC() );
	book->fill( "zdc", ri, pico->eventZDC() );
	book->fill( "nGlobal", ri, pico->eventNumGlobal() );
	book->fill( "nPrimary", ri, pico->eventNumPrimary() );
	book->fill( "nTofMatch", ri, pico->eventNumTofMatched() );
	book->fill( "vtxX", ri, vX );
	book->fill( "vtxY", ri, vY );
	book->fill( "vtxZ", ri, vZ );
	
	double vR = TMath::Sqrt( vX*vX + vY*vY );
	book->fill( "vtxR", ri, vR );


	int refMult = pico->eventRefMult();
	if ( correctZ ){
		refMult = rmc->refMult( refMult, vZ );
	}


	book->fill( "tofMult", ri, pico->eventTofMult() );
	book->fill( "refMult", ri, refMult );


	book->fill( "refMultZ", vZ, refMult );
	book->fill( "refMultBBC", pico->eventBBC(), refMult );
	book->fill( "refMultZDC", pico->eventZDC(), refMult );
	book->fill( "refMultTOF", pico->eventTofMult(), refMult );

	int pi = periodIndex( ri );
	if ( pi >= 0 ){
		book->fill( "refMultZ_" + ts(pi), vZ, refMult );
		book->fill( "refMultBBC_" + ts(pi), pico->eventBBC(), refMult );
		book->fill( "refMultZDC_" + ts(pi), pico->eventZDC(), refMult );
		book->fill( "refMultTOF_" + ts(pi), pico->eventTofMult(), refMult );	
	}
	


	/**
	 * Analyze the tracks
	 */
	Int_t nTracks = pico->eventNumPrimary();

	for ( Int_t iTrack = 0; iTrack < nTracks; iTrack ++ ){

		if ( !keepTrack( iTrack ) )
			continue;
		
		analyzeTrack( iTrack );	

	}

}


void EventQA::analyzeTrack( Int_t iTrack ){

	UInt_t run = pico->eventRunId();
	int ri = runIndex( run );
	book->fill( "ptPrimary", ri, pico->trackPt( iTrack ) );
	book->fill( "etaPrimary", ri, pico->trackEta( iTrack ) );
	book->fill( "pPrimary", ri, pico->trackP( iTrack ) );

	if ( pico->trackP( iTrack ) < 2.0 ){
		book->fill( "qxPrimary", ri, pico->trackPx( iTrack ) );
		book->fill( "qyPrimary", ri, pico->trackPy( iTrack ) );
	}

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
	double eta = pico->trackEta( iTrack );
	
	if ( eta < cutEta->min || eta > cutEta->max )
		return false;

	return true;
}


int EventQA::nRuns = 731;
int EventQA::runList[] = {
15046106, 
15047015, 
15047019, 
15047021, 
15047024, 
15047027, 
15047028, 
15047030, 
15047039, 
15047040, 
15047041, 
15047050, 
15047052, 
15047053, 
15047056, 
15047065, 
15047068, 
15047069, 
15047074, 
15047075, 
15047082, 
15047086, 
15047087, 
15047096, 
15047097, 
15047098, 
15047100, 
15047102, 
15047104, 
15047106, 
15048003, 
15048004, 
15048012, 
15048013, 
15048014, 
15048016, 
15048018, 
15048019, 
15048020, 
15048021, 
15048023, 
15048024, 
15048025, 
15048026, 
15048028, 
15048029, 
15048030, 
15048031, 
15048033, 
15048034, 
15048074, 
15048075, 
15048076, 
15048077, 
15048078, 
15048079, 
15048080, 
15048081, 
15048082, 
15048083, 
15048084, 
15048085, 
15048086, 
15048088, 
15048089, 
15048091, 
15048093, 
15048094, 
15048095, 
15048096, 
15048097, 
15048098, 
15049002, 
15049003, 
15049009, 
15049013, 
15049014, 
15049015, 
15049017, 
15049018, 
15049030, 
15049031, 
15049032, 
15049033, 
15049037, 
15049038, 
15049039, 
15049040, 
15049074, 
15049077, 
15049084, 
15049085, 
15049086, 
15049088, 
15049092, 
15049093, 
15049094, 
15049096, 
15049097, 
15050001, 
15050002, 
15050003, 
15050006, 
15050012, 
15050013, 
15050014, 
15050015, 
15050016, 
15051133, 
15051134, 
15051137, 
15051141, 
15051144, 
15051146, 
15051147, 
15051148, 
15051149, 
15051156, 
15051157, 
15051159, 
15051160, 
15052001, 
15052004, 
15052005, 
15052007, 
15052008, 
15052009, 
15052010, 
15052014, 
15052015, 
15052016, 
15052017, 
15052018, 
15052019, 
15052020, 
15052021, 
15052022, 
15052023, 
15052024, 
15052025, 
15052026, 
15052040, 
15052041, 
15052042, 
15052043, 
15052060, 
15052061, 
15052062, 
15052063, 
15052064, 
15052065, 
15052066, 
15052067, 
15052068, 
15052069, 
15052070, 
15052073, 
15052074, 
15052075, 
15053001, 
15053002, 
15053003, 
15053005, 
15053006, 
15053007, 
15053008, 
15053011, 
15053014, 
15053015, 
15053016, 
15053019, 
15053020, 
15053021, 
15053022, 
15053023, 
15053024, 
15053025, 
15053026, 
15053027, 
15053028, 
15053029, 
15053034, 
15053049, 
15053050, 
15053052, 
15053053, 
15053054, 
15053056, 
15053057, 
15053058, 
15053059, 
15053060, 
15053062, 
15053064, 
15053065, 
15053067, 
15054001, 
15054002, 
15054003, 
15054004, 
15054005, 
15054006, 
15054007, 
15054008, 
15054009, 
15054025, 
15054026, 
15054028, 
15054029, 
15054031, 
15054037, 
15054042, 
15054043, 
15054044, 
15054046, 
15054047, 
15054048, 
15054049, 
15054050, 
15054051, 
15054052, 
15054053, 
15054054, 
15055001, 
15055002, 
15055003, 
15055004, 
15055005, 
15055006, 
15055007, 
15055008, 
15055009, 
15055011, 
15055012, 
15055013, 
15055014, 
15055015, 
15055016, 
15055017, 
15055018, 
15055019, 
15055020, 
15055021, 
15055131, 
15055133, 
15055134, 
15055135, 
15055136, 
15055137, 
15055138, 
15055139, 
15055140, 
15055141, 
15056001, 
15056002, 
15056003, 
15056004, 
15056005, 
15056006, 
15056007, 
15056008, 
15056009, 
15056013, 
15056014, 
15056015, 
15056016, 
15056017, 
15056018, 
15056020, 
15056021, 
15056022, 
15056023, 
15056024, 
15056025, 
15056026, 
15056028, 
15056036, 
15056037, 
15056038, 
15056039, 
15056113, 
15056114, 
15056116, 
15056119, 
15056123, 
15056124, 
15056125, 
15057001, 
15057002, 
15057003, 
15057004, 
15057005, 
15057006, 
15057007, 
15057008, 
15057010, 
15057012, 
15057015, 
15057016, 
15057017, 
15057018, 
15057019, 
15057020, 
15057021, 
15057022, 
15057023, 
15057029, 
15057030, 
15057031, 
15057048, 
15057049, 
15057050, 
15057051, 
15057052, 
15057053, 
15057054, 
15057056, 
15057057, 
15057058, 
15057059, 
15057060, 
15057061, 
15057062, 
15057063, 
15058001, 
15058002, 
15058003, 
15058004, 
15058005, 
15058007, 
15058008, 
15058009, 
15058010, 
15058011, 
15058012, 
15058013, 
15058014, 
15058015, 
15058016, 
15058017, 
15058018, 
15058019, 
15058020, 
15058021, 
15058022, 
15058024, 
15058025, 
15058032, 
15058033, 
15058043, 
15058044, 
15058045, 
15058048, 
15058049, 
15058050, 
15058051, 
15058052, 
15058053, 
15058054, 
15058055, 
15059001, 
15059002, 
15059003, 
15059005, 
15059006, 
15059007, 
15059009, 
15059010, 
15059011, 
15059013, 
15059014, 
15059015, 
15059016, 
15059017, 
15059018, 
15059019, 
15059020, 
15059021, 
15059024, 
15059025, 
15059026, 
15059027, 
15059028, 
15059029, 
15059033, 
15059037, 
15059040, 
15059041, 
15059042, 
15059055, 
15059059, 
15059060, 
15059061, 
15059063, 
15059064, 
15059068, 
15059074, 
15059076, 
15059081, 
15059082, 
15059086, 
15059087, 
15059090, 
15060001, 
15060006, 
15060007, 
15060009, 
15060011, 
15060012, 
15060014, 
15060015, 
15060016, 
15060017, 
15060018, 
15060019, 
15060020, 
15060021, 
15060022, 
15060023, 
15060027, 
15060029, 
15060031, 
15060032, 
15060033, 
15060035, 
15060036, 
15060037, 
15060044, 
15060045, 
15060046, 
15060047, 
15060048, 
15060049, 
15060050, 
15060051, 
15060052, 
15060053, 
15060061, 
15060062, 
15060067, 
15060068, 
15060069, 
15060070, 
15060071, 
15061001, 
15061002, 
15061003, 
15061004, 
15061006, 
15061007, 
15061008, 
15061009, 
15061010, 
15061011, 
15061012, 
15061014, 
15061015, 
15061016, 
15061018, 
15061019, 
15061021, 
15061023, 
15061024, 
15061025, 
15061026, 
15061027, 
15061034, 
15061036, 
15061037, 
15061038, 
15061039, 
15061041, 
15061047, 
15061048, 
15061051, 
15061053, 
15061054, 
15061055, 
15061056, 
15061059, 
15061060, 
15061061, 
15061062, 
15061063, 
15061064, 
15062002, 
15062003, 
15062004, 
15062005, 
15062006, 
15062007, 
15062008, 
15062009, 
15062010, 
15062011, 
15062012, 
15062013, 
15062015, 
15062017, 
15062018, 
15062020, 
15062022, 
15062023, 
15062024, 
15062025, 
15062026, 
15062031, 
15062032, 
15062033, 
15062034, 
15062035, 
15062036, 
15062037, 
15062038, 
15062040, 
15062041, 
15062042, 
15062066, 
15062070, 
15062071, 
15062075, 
15062076, 
15062077, 
15063001, 
15063002, 
15063003, 
15063006, 
15063008, 
15063010, 
15063011, 
15063012, 
15063014, 
15063016, 
15063017, 
15063018, 
15063020, 
15063021, 
15063029, 
15063032, 
15063036, 
15063037, 
15063040, 
15063041, 
15063042, 
15063043, 
15063045, 
15063046, 
15063048, 
15063049, 
15063053, 
15063056, 
15063059, 
15063060, 
15063061, 
15063062, 
15063063, 
15063065, 
15063066, 
15063067, 
15064001, 
15064002, 
15064003, 
15064005, 
15064006, 
15064007, 
15064008, 
15064009, 
15064010, 
15064011, 
15065011, 
15065012, 
15065013, 
15065014, 
15065017, 
15065054, 
15065056, 
15065059, 
15065060, 
15065061, 
15066005, 
15066006, 
15066008, 
15066010, 
15066012, 
15066013, 
15066014, 
15066016, 
15066017, 
15066018, 
15066019, 
15066020, 
15066021, 
15066022, 
15066023, 
15066024, 
15066025, 
15066026, 
15066064, 
15066071, 
15066076, 
15066077, 
15066082, 
15066083, 
15066085, 
15066086, 
15066088, 
15067001, 
15067002, 
15067003, 
15067005, 
15067006, 
15067008, 
15067009, 
15067011, 
15067012, 
15067013, 
15067014, 
15067015, 
15067016, 
15067018, 
15067019, 
15067020, 
15067022, 
15067023, 
15067024, 
15067025, 
15067026, 
15067027, 
15067032, 
15067033, 
15067034, 
15067035, 
15067036, 
15067037, 
15067038, 
15067040, 
15067041, 
15067042, 
15067043, 
15067044, 
15067046, 
15067048, 
15068001, 
15068002, 
15068003, 
15068004, 
15068005, 
15068006, 
15068007, 
15068009, 
15068010, 
15068012, 
15068013, 
15068014, 
15068016, 
15068018, 
15068022, 
15068023, 
15068024, 
15068025, 
15068026, 
15068027, 
15068028, 
15068031, 
15068032, 
15068033, 
15068034, 
15068035, 
15068036, 
15068037, 
15068038, 
15068039, 
15068040, 
15068041, 
15068042, 
15068043, 
15068044, 
15068045, 
15068046, 
15068047, 
15068048, 
15069002, 
15069003, 
15069004, 
15069005, 
15069006, 
15069007, 
15069008, 
15069009, 
15069010, 
15069011, 
15069012, 
15069013, 
15069014, 
15069015, 
15069016, 
15069017, 
15069018, 
15069019, 
15069020, 
15069021, 
15069022, 
15069023, 
15069030, 
15069031, 
15069034, 
15069035, 
15069036, 
15069038, 
15069040, 
15069042, 
15069043, 
15069044, 
15069045, 
15069046, 
15069047, 
15069048, 
15069049, 
15069050, 
15069051, 
15070001, 
15070002, 
15070013, 
15070014, 
15070015, 
15070016, 
15070017, 
15070018, 
15070019, 
15070020, 
15070021
};


