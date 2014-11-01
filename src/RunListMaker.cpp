#include "RunListMaker.h"
#include "RefMultPicoDst.h"

RunListMaker::RunListMaker( XmlConfig * config, string np, string fl, string jp ) : TreeAnalyzer( config, np, fl, jp ){

	pico = new RefMultPicoDst( chain );

}

RunListMaker::~RunListMaker(){

}

void RunListMaker::analyzeEvent(){

	int runId = pico->eventRunId();

	runMap[ runId ]++;

}

void RunListMaker::postEventLoop(){

	
	for ( auto it = runMap.begin(); it != runMap.end(); ++it ){
		cout << it->first << ", " << endl;
	}

}