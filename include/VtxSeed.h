#ifndef VTX_SEED_H
#define VTX_SEED_H

#include "ConfigRange.h"
#include "TreeAnalyzer.h"
#include "TreeMap.h"
using namespace jdb;

class VtxSeed : public TreeAnalyzer {
// Protected member properties
protected:

	TreeMap * tm;

// public member methods
public:
	VtxSeed( XmlConfig * config, string nodePath, string fl, string jp );
	~VtxSeed();

	virtual void make();

// protected member methods
protected:


	void fillEventHists();

	/**
	 * Before the event loop starts - for subclass init
	 */
	virtual void preEventLoop();

	/**
	 * After the event loop starts - for subclass reporting
	 */
	virtual void postEventLoop();

	
	/**
	 * Analyze Event
	 */
	virtual void analyzeEvent( );
	virtual void analyzeEventBeforeCuts( );

	/**
	 * Performs event based cuts
	 * @return 	True 	- Keep Event 
	 *          False 	- Reject Event
	 */
	virtual bool keepEvent();



};

#endif