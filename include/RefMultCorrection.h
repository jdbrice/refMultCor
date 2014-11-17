#ifndef REF_MULT_CORRECTION
#define REF_MULT_CORRECTION


/**
 * JDB
 */
#include "XmlConfig.h"
#include "ConfigRange.h"
#include "Logger.h"

/**
 * ROOT
 */
#include "TMath.h"
#include "TRandom.h"

class RefMultCorrection
{
protected:

	Logger * logger;
	XmlConfig * cfg;

	int year;
	double energy;
	ConfigRange * zVertexRange;
	ConfigRange * runRange;
	vector<double> zParameters;



public:

	RefMultCorrection(string paramFile ){

		cout <<"Making logger" << endl;
		logger = new Logger();
		logger->setClassSpace( "RefMultCorrection" );
		logger->info( __FUNCTION__ ) << endl;
		cfg = new XmlConfig( paramFile.c_str() );

		year 		= cfg->getInt( "year" );
		energy 		= cfg->getDouble( "energy" );
		zVertexRange= new ConfigRange( cfg, "zVertex" );
		runRange	= new ConfigRange( cfg, "runRange" );
		zParameters = cfg->getDoubleVector( "zParameters" );

		logger->info(__FUNCTION__) << "Year : " << year << endl;
		logger->info(__FUNCTION__) << "Energy : " << energy << endl;
		logger->info(__FUNCTION__) << "Z Vertex : " << zVertexRange->toString() << endl;
		logger->info(__FUNCTION__) << "Run Range : " << runRange->toString() << endl;
		

	}
	~RefMultCorrection();


	int refMult( int rawRefMult, double z ){

		double rmZ = zPolEval( z );
		if ( rmZ > 0 && zParameters.size() >= 1 ){

			const double center = zParameters[ 0 ];
			double corRefMult = center / rmZ;


			return corRefMult;
		}

		logger->warn( __FUNCTION__ ) << "Ref Mult not corrected " << endl;
		return rawRefMult;
	}

	double zPolEval( double z ){
		if ( zParameters.size() <= 0 ){
			logger->warn(__FUNCTION__) << "No Z Parameters " << endl;
			return 0;
		}

		double r = 0;
		for ( int i = 0; i < zParameters.size(); i++ ){
			r += zParameters[ i ] * TMath::Power( z, i );
		}
		return r;
	}
	
};


#endif