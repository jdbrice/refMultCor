

#include "XmlConfig.h"
using namespace jdb;


#include <iostream>
#include "EventQA.h"
#include "PlotQA.h"

#include <exception>

int main( int argc, char* argv[] ) {

	if ( argc >= 2 ){

		try{
			XmlConfig config( argv[ 1 ] );
			//config.report();

			string job = config.getString( "jobType" );

			if ( "EventQA" == job ){
				EventQA eqa( &config, "EventQA."  );

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
