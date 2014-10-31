#ifndef EVENT_QA_H
#define EVENT_QA_H 

/**
 * JDB 
 */

#include "Utils.h"
#include "XmlConfig.h"
#include "Logger.h"
#include "LoggerConfig.h"
#include "HistoBook.h"
#include "ConfigRange.h"
#include "ConfigPoint.h"
#include "Reporter.h"
using namespace jdb;

/**
 * ROOT
 */
#include "TChain.h"

/**
 * STD
 */
#include <map>
#include <vector>

/**
 * Local
 */
#include "PicoDataStore.h"
#include "TreeAnalyzer.h"

class EventQA : public TreeAnalyzer
{
protected:

	PicoDataStore * pico;

	vector<int> runs;
	map<int, int> runMap;

	/**
	 * Event cuts
	 */
	ConfigRange *cutVertexZ;
	ConfigRange *cutVertexR;
	ConfigPoint *cutVertexROffset;
	ConfigRange *cutTofMatch;

	static int nRuns;
	static int runList[];
	vector<int> rList;
	


public:
	EventQA( XmlConfig * config, string nodePath );
	~EventQA();

	virtual void make(){
		TreeAnalyzer::make();
	}

protected:

	/**
	 * Before the event loop starts - for subclass init
	 */
	virtual void preEventLoop();

	/**
	 * After the event loop starts - for subclass reporting
	 */
	virtual void postEventLoop(){}

	/**
	 * Analyze a track in the current Event
	 * @param	iTrack 	- Track index 
	 */
	virtual void analyzeTrack( Int_t iTrack );

	/**
	 * Analyze Event
	 */
	virtual void analyzeEvent( );

	/**
	 * Performs event based cuts
	 * @return 	True 	- Keep Event 
	 *          False 	- Reject Event
	 */
	virtual bool keepEvent();

	/**
	 * Performs track based cuts
	 * @return 	True 	- Keep Track 
	 *          False 	- Reject Track
	 */
	virtual bool keepTrack( Int_t iTrack );

	int runIndex( UInt_t runId ){
		auto it = std::find( rList.begin(), rList.end(), runId );
		if (it == rList.end()){
			return -1;
		}
		
		return (int)(it - rList.begin());
	}
	


};


#endif