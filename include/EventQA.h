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
	vector<int> cutTriggers;
	


public:
	EventQA( XmlConfig * config, string nodePath );
	~EventQA();

	void make() {
		eventLoop();
	}

protected:

	/**
	 * Loops the tree events and calculates the non-linear
	 * recentering for use with unbinned methods
	 */
	void eventLoop();

	void makeRunMap();

	/**
	 * Before the event loop starts - for subclass init
	 */
	virtual void preLoop();

	/**
	 * After the event loop starts - for subclass reporting
	 */
	virtual void postLoop(){}

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
	virtual bool eventCut();

	/**
	 * Performs track based cuts
	 * @return 	True 	- Keep Track 
	 *          False 	- Reject Track
	 */
	virtual bool trackCut( Int_t iTrack );
	


};


#endif