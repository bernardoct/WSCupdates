/////////////////////////////////////////////
///  Research Triangle Water Supply Model ///
////////////  For use with MOEA /////////////


/*  COMMENTS INCLUDED BY DAVID GORELICK,
    BEGINNING 11-09-2015 TO HELP UNDERSTAND  */

#include "global.h"
#include "Simulation.h"
#include <time.h>
    // these are "directives" read by the C++ preprocessor.
    // they don't require a semicolon because they are not statements.
    // each instructs for a header file to be included.

#ifdef PARALLEL
	#include <mpi.h>
	#include "borgProblemDefinition.h"
	#include "cborg/borgms.h"
#endif

using namespace std;

    // tells C++ that we are using the standard namespace, so
    // instead of typing "std::cerr" we just type "cerr"

void usage(int argc, char* argv[])
{
	cerr << "Usage: " << argv[0] << " [OPTIONS]" << endl;
	cerr << "-r <realizations> \t Number of realizations. REQUIRED." << endl;
        // debug with small number... "-r1" to do 1 simulation
	cerr << "-t <total streamflow> \t Number of streamflow records in distribution. REQUIRED"<<endl;
        // generally equal to number of realizations
	cerr << "-c <formulation> \t Constraint options.  REQUIRED."<<endl;
        // specific formulation
	cerr << "-b <BORG Interface toggle> \t BORG interface options or write output to file.  REQUIRED." <<endl;
        // which mode are we running?
	cerr << "-s <seed> \t Seed. (optional)." << endl;
	cerr << "-h Help (this screen)." << endl;

	exit(-1);
}
//Definition for run types:
// -m batch: batch method, for connection to MOEA
// -m interactive: input decision variables at prompt
// -rX: run model simulations using X number of realizations (different streamflow records)
// -tX: total number of streamflow records to characterize the distribution from r number of streamflow realizations are drawn
// -cX: user defined scenario X
// -bX: toggle MOEA on/off
//		X = 0;  on, objectives minimize the maximum individual utility objective
//		X = 1;  on, objectives are additive
//		X = 1; off - requires decision variable file


// Global simulation object
Simulation simulation;

//void calculationWrapperFunction(double *xreal, double *obj, double *constr)
//{
//	simulation.calculation(xreal, obj, constr);
//}
	// THIS IS DIFFERENT BECAUSE I ADDED AN OUTPUT.  NEED TO UNCOMMENT THIS WHEN OPTIMIZING
	// DONT FORGET ABOUT THIS
	// everything related to using the moea is commented out to let me create some outputs

int main (int argc, char *argv[])
{

// -----------------------------------------------------------------------------------------------------------
// 				READ COMMAND-LINE PARAMETERS AND INITIALIZE PROGRAM
//------------------------------------------------------------------------------------------------------------

	int numRealizations = 1000;
	int nDeeplyUncertainSets = 3;

	//set defaults
	simulation.setNumRealizations(numRealizations);

	simulation.startYear = 2010;
	simulation.endYear = 2060;
	simulation.currentYear = 2015;
	
	int startYear = simulation.startYear;
	int endYear = simulation.endYear;
	int currentYear = simulation.currentYear;

	int terminateYear = endYear-startYear + 1;
	int startSimulationYear = currentYear - startYear + 1;
	simulation.setNumYears(terminateYear);
	simulation.setStartYear(startSimulationYear);
	
	//seed random number generator
	// NOTE: for model, the seed stays the same. For Borg, the seed comes from command line argument.
	srand(1);

	// Import historical demand and inflow datasets
	//cout << "import data files" << endl;

		// determines whether to use fake historic streamflows
		// in place of synthetic flows 
		// these flows still won't be read until the fixRDMFactors function runs
	string data_dir = string(argv[1]);
	int rdm = atoi(argv[2]);
	simulation.directoryName = data_dir; //"./inputfiles/";
	simulation.historicFlowPath = data_dir + "historical_inflows/";
	simulation.demandDataPath = data_dir + "demand_files/";
	simulation.synthFlowsPath = data_dir + "rdm_" + to_string(rdm) + "/inflows/";
	simulation.demandOutputPath = data_dir + "rdm_" + to_string(rdm) + "/demands/";
		// set some subfolder input paths 
	
	simulation.importDataFiles();

	// Set water prices and consumer elasticity,

	//cout << "write data lists" << endl;
	
	simulation.writeDataLists();

	// 'Whiten' demand and inflow data, generate joint PDFs,
	//cout << "precondition data" << endl;
	// (Here pass in 1's because we don't want to scale the demand yet).
	
	
	simulation.preconditionData(1.0, 1.0, true);

	double *c_xreal = nullptr, *c_obj = nullptr, *c_constr = nullptr;

//		simulation.directoryName = "./inputfiles/";
		
		simulation.use_RDM_ext = true;
		simulation.numRealizations = 1000;
		simulation.terminateYear = 51;

		//int rdmNumber = rank2 / numSolutions;
		int rdmNumber = 5;
		
		// RDM LOOP
//		for (int i = 0; i < nDeeplyUncertainSets; i++)
//		{
    simulation.fixRDMFactors();
			simulation.correlateDemandVariations(1.0); 
				// 1.0 reflects no scaling
			
			auto calculation_time = simulation.calculation(c_xreal, c_obj, c_constr);

//		}


	return 0;
}
