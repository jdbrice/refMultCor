

#include "XmlConfig.h"
#include "Logger.h"
using namespace jdb;


#include <iostream>
#include "EventQA.h"
#include "PlotQA.h"


#include <exception>

int main( int argc, char* argv[] ) {

	Logger::setGlobalLogLevel( Logger::llWarn );
	if ( argc >= 2 ){

		string fileList = "";
		string jobPrefix = "";

		try{
			XmlConfig config( argv[ 1 ] );
			

			if ( argc >= 3)
				fileList = (string) argv[ 2 ];
				
			if ( argc >= 4)
				jobPrefix = (string) argv[ 3 ];

			string job = config.getString( "jobType" );

			if ( "EventQA" == job ){
				EventQA eqa( &config, "EventQA.", fileList, jobPrefix );
				eqa.make();  
			} else if ( "PlotQA" == job ){
				PlotQA pqa( &config, "PlotQA." );
				pqa.make();
			}

		} catch ( exception &e ){
			cout << e.what() << endl;
		}

	}
	return 0;
}
