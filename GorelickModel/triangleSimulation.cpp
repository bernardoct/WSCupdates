/////////////////////////////////////////////
///  Research Triangle Water Supply Model ///
////////////  For use with MOEA /////////////


/*  COMMENTS INCLUDED BY DAVID GORELICK,
    BEGINNING 11-09-2015 TO HELP UNDERSTAND  */

#include "global.h"
#include "Simulation.h"
#include "moeaframework.h"
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
	return;
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

void calculationWrapperFunction(double *xreal, double *obj, double *constr)
{
	simulation.calculation(xreal, obj, constr);
}
	// THIS IS DIFFERENT BECAUSE I ADDED AN OUTPUT.  NEED TO UNCOMMENT THIS WHEN OPTIMIZING
	// DONT FORGET ABOUT THIS
	// everything related to using the moea is commented out to let me create some outputs

int main (int argc, char *argv[])
{

// -----------------------------------------------------------------------------------------------------------
// 				READ COMMAND-LINE PARAMETERS AND INITIALIZE PROGRAM
//------------------------------------------------------------------------------------------------------------

	int opt;
	//int seed = (int)time(NULL);
	int seed = 1;
	int numRealizations;
	int nDeeplyUncertainSets;
	int offset;
	int fromRDM;

	while ((opt = getopt(argc, argv, "r:u:t:c:b:s:o:h")) != -1)
	{
		switch (opt)
		{
			case 'r':
				numRealizations = atoi(optarg);
				break;
					// r has to be <= t (below)
			case 'u':
				nDeeplyUncertainSets = atoi(optarg);
				break;
			case 't':
				simulation.numRecords = atoi(optarg);
				break;
					// number of synthetic records to read in 
			case 'c':
				simulation.formulation = atoi(optarg);
				break;
					// David (11-14-2016)
					// the following formulations to be used:
					// 0 - No releases (but includes transfers, restrictions, standard infrastructure pathways)
					// 1 - Releases through spot or option contracts (but no contract negotiations)
					// 2 - Releases (with contract negotiations)
					// 3 - Releases with Joint Lake Michie Expansion available as infrastructure option
					 
			case 'b':
				simulation.borgToggle = atoi(optarg);
				break;
			case 's':
				seed = atoi(optarg);
				break;
			case 'o':
				offset = atoi(optarg);
					// must be set to 0 when running the model 
					// add -o 0 to the testParallel.sh file 
				break;
			case 'h':
				usage(argc, argv);
				break;
			default:
				usage(argc, argv);
				break;
		}
	}

	if (numRealizations == 0)
	{
		cerr << "Error! Number of realizations not given." << endl;
		exit(-1);
	}



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
	
	simulation.numIntervals = 50;
		// the number of discrete volume increments
		// used for insurance payouts and for 
		// setting the "GUIDE curve" for raleigh
		// and durham water supply for release determination
	simulation.discountrate = 0.05;
		// discount rate on present value calculations 
	simulation.numContractRiskYears = 20;
		// the number of years of baseline ROF that will be stored
		// for use in release contract determination 
	simulation.availableJLallocation = 0.69;
		// the fraction of Jordan Lake water supply pool
		// that can be divided among Cary, OWASA, Durham, and Raleigh.
		// may be less than 1 (100%) because of allocation
		// to Chatham County communities.
		
	// if (simulation.formulation > 0)
		// // in zero formulation, no releases 
	// {
		// simulation.allowReleases = 1;
			// // a logical to determine whether releases will occur
			// // SET TO 0 SO RELEASES NEVER OCCUR 
	// }
	// else 
	// {
		// simulation.allowReleases = 0;
	// }
		// EVERY INSTANCE OF allowReleases WAS REPLACED WITH formulation > 0 
	
	//seed random number generator
	// NOTE: for model, the seed stays the same. For Borg, the seed comes from command line argument.
	srand(1);

	//variables for interfacing with algorithm
	int c_num_dec = 80;
	double *c_xreal;
	general_1d_allocate(c_xreal, c_num_dec);
        // c_xreal is decision vars
        // c_num_dec is number of dec vars
		// MAY 2016: ADD NEW VARIABLES TO THIS 
		// NOW 64 VARIABLES 
	
	simulation.setNumDecisions(c_num_dec);

	// Import historical demand and inflow datasets
	//cout << "import data files" << endl;
	
	simulation.runHistoric = true;
		// determines whether to use fake historic streamflows
		// in place of synthetic flows 
		// these flows still won't be read until the fixRDMFactors function runs 
	
	simulation.directoryName = "./inputfiles/";
	simulation.historicFlowPath = "./historicflows/";
	simulation.fakesynthFlowPath = "./historicfakesyntheticflows/";
	simulation.syntheticFlowPath = "./syntheticflows/";
	simulation.evaporationPath = "./evapfiles/";
	simulation.oldstochPath = "./oldstochfiles/";
	simulation.demanddataPath = "./demandfiles/";
		// set some subfolder input paths 
	
	simulation.importDataFiles();

	// Set water prices and consumer elasticity,

	//cout << "write data lists" << endl;
	
	simulation.writeDataLists();

	// 'Whiten' demand and inflow data, generate joint PDFs,
	//cout << "precondition data" << endl;
	// (Here pass in 1's because we don't want to scale the demand yet).
	
	
	simulation.preconditionData(1.0, 1.0, true);

	// Weighted average water prices and consumer surplus losses
	// these functions create water use distribution matricies that show the breakdown of water use into different price tiers/elasticity types as restrictions are implemented
	// output from these functions is used by the simulation.calc_Water_Surcharge functions inside the MOEA to determine:

	// 1 - the average revenue each utility aquires per MG of water sold (based on month and restriction stage)
	// 2 - the consumer surplus loss per MG of water sold that would occur during prescribed use restrictions and price increases (based on month and restriction stage)
	// 3 - total reduction in water use from a given combination of restrictions and price increases

	//cout << "calc water prices" << endl;
	
	
	simulation.calculateWaterPrices();
	
	simulation.calculateWaterSurcharges();
		// not used 

	//Generates synthetic streamflows using the autocorrelated bootstrap technique.
	//Streamflows records have weekly values with a length of 52*(terminateYear)
	//there are a number of streamflow records generated equal to (numRealizations)
	
	//cout << "generate streamflows" << endl;
	
	
	simulation.generateStreamflows();

	//Reservoir risk of failure uses historical streamflow records to determine the probability that reservoir storage will drop below
	//20% of storage capacity within a period of 52 weeks for a given storage level, week of the year, and average consumer demand
	//Risk of failure is used in determining triggers for water transfers

	//Function not needed - data is inputted from file
	//simulation.createRiskOfFailure();
	
	simulation.use_RDM_ext = false;
		// determines how to read synthetic flows
		// if true, read from folder extension bernardo uses 
	simulation.printDetailedOutput = false;
		// determines whether to write all the output csvs that I want 
		
	//simulation.spotPricing = true;
		// determines whether releases are controlled with spot agreements
		// or with an option contract 
	//simulation.tieredSpotPricing = false;
		// says whether or not spot pricing is tiered or 
		// a flat rate
	// THESE ARE ADDRESSED IN THE SIMULATION SCRIPT 
	
	if (simulation.formulation > 2)
	{
		simulation.sharedLM = true;
	}
	else
	{
		simulation.sharedLM = false;
	}
		// an indicator as to whether Lake Michie expansion is shared
		// between Raleigh and Durham to allow Raleigh some capacity
		// in LM 
	simulation.indepReleaseAlloc = false;
		// this determines whether release allocation into FL conservation pool
		// is based on the input parameter file or based on the current week's
		// supply pool to water quality pool ratio 
	simulation.numRealizationsTOREAD = numRealizations;
		// read all realizations in current mode
		// sets the number of realizations for which data
		// will be added to output files 


	double *c_obj = NULL, *c_constr = NULL;
	int c_num_obj;
	int c_num_constr;

	if (simulation.borgToggle < 3)
	{
		c_num_obj = 6;
			//Number of objective variables

		// JDH 11/12: Turning off constraints for now (below here, only for parallel version)
		
		c_num_constr = 0;
		general_1d_allocate(c_obj, c_num_obj);
		//general_1d_allocate(c_constr, c_num_constr);

		simulation.initializeFormulation(c_num_obj, c_num_constr); 
			// number of decisions, objectives, constraints

		// Interface with Borg-MS (parallel)
		#ifdef PARALLEL

			// BORG_Debug_on();
			// BORG_Algorithm_ms_max_time(0.008);
			// BORG_Algorithm_output_aerovis();

			char runtime[256];
			char outputFilename[256];
			FILE* outputFile = NULL;

			BORG_Algorithm_ms_startup(&argc, &argv);
			BORG_Algorithm_ms_max_evaluations(5);
			BORG_Algorithm_output_frequency(1);
			BORG_Problem problem = BORG_Problem_create(c_num_dec, c_num_obj, c_num_constr, calculationWrapperFunction);

			// Set all the parameter bounds and epsilons
			setProblemDefinition(problem, simulation.formulation);

			// This is set up to run only one seed at a time.

			sprintf(runtime, "./output/O%d_F%d/CBorg_NCTriangle_O%d_F%d_S%d.runtime", simulation.borgToggle, simulation.formulation, simulation.borgToggle, simulation.formulation, seed);
			sprintf(outputFilename, "./output/O%d_F%d/CBorg_NCTriangle_O%d_F%d_S%d.set", simulation.borgToggle, simulation.formulation, simulation.borgToggle, simulation.formulation, seed);
			BORG_Algorithm_output_runtime(runtime);

			BORG_Random_seed(seed);
			BORG_Archive result = BORG_Algorithm_ms_run(problem);
				// segmentation fault here?

			// If this is the master node, print out the final archive
			if (result != NULL) {
				outputFile = fopen(outputFilename, "w");
				if (!outputFile) {
					BORG_Debug("Unable to open final output file\n");
				}
				BORG_Archive_print(result, outputFile);
				BORG_Archive_destroy(result);
				fclose(outputFile);
			}

			BORG_Algorithm_ms_shutdown();
			BORG_Problem_destroy(problem);

		#else // Interface with MOEA Framework

			MOEA_Init(c_num_obj, c_num_constr); // pass number of objectives and formulation to MOEA

			while (MOEA_Next_solution() == MOEA_SUCCESS)
			{
				MOEA_Read_doubles(c_num_dec, c_xreal);//Input decision variables
				simulation.calculation(c_xreal, c_obj, c_constr); // Run simulation iteration
				MOEA_Write(c_obj, c_constr);//write new objective functions
			}

		#endif
	}
	else // If running from parameter input file (no constraints here)
        // RUNNING FROM AN INPUT FILE... now just looking at
        // optimized outputs, not actually optimizing here
	{
		int c_num_obj = 25;
		int nRDM = 1;
			// Read a certain number of RDM parameter sets 
			// when running in simulation mode without uncertain factors
			// set this to 1, other wise bernardo was using 1000
		int numSolutions = 10000;
			// when pulling parameter inputs from the paramterinputfile.csv, make sure this
			// is equal to the number of rows in the file
			// THIS WAS SET TO 444 FOR BERNARDO TO RUN ON BW
		general_1d_allocate(c_obj, c_num_obj);
		simulation.initializeFormulation(c_num_obj, 0);
		simulation.directoryName = "./inputfiles/";
		
		//readFile(simulation.parameterInput, "./CBorg_NCTriangle_O0_F2_S1epsilon.set", numSolutions, c_num_dec);
			// use this one for Bernardo's input file 
		readFile(simulation.parameterInput, "./inputfiles/paramterInputFile.csv", numSolutions, c_num_dec);
			// these are david's input parameters 
		readFile(simulation.RDMInput, "./inputfiles/lhs_samples_final.csv", nRDM, simulation.num_rdm_factors);
			// Read the random samples of the 13 uncertain parameters. This will be a 13 x numRealizations matrix.  
			// ADDED BY BERNARDO
			// num_rdm_factors is set to 30 in the Simulation header file
			// the first value of the first line of this file will determine 

		// Set up the output stream for objective values
		MPI_Init(NULL,NULL);
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            // gives a value to rank, based on the number of processors being used
            // each processor doing the same thing, but each has diff rank
		int rank2 = rank + offset;
			// when not running RDM, offset = 0

		//int rdmNumber = rank2 / numSolutions;
		int rdmNumber = 0;
			// set to 0 when not running with RDM loop
		//int solutionNumber = rank2 % numSolutions;
		int solutionNumber = rank2;
			// the solution is equal to the rank
			// without the RDM loop 
		
		//cout << rdmNumber << " " << solutionNumber << endl;
		
		//int solutionNumber = rank2;
		simulation.solutionNumber = rank;
			// set this to rank when running without uncertain factors
			// set as solutionNumber otherwise
		
		//simulation.nDeeplyUncertainSets = 4;

		std::string filename1 = "output/simulationOutput";
		std::string filename2 = ".csv";
		std::string completeFilename;
		std::stringstream sstm;
		sstm << filename1 << solutionNumber << "_" << rdmNumber << filename2;
		completeFilename = sstm.str();
		
		ofstream out1;
		openFile(out1, completeFilename);
			// prepare the output file that 
			// contains input params and objective values 
		
		double calculation_time;
		
		// RDM LOOP
		//for (int i = 0; i < nDeeplyUncertainSets; i++)
		//{
 		    //cout << "Setting up solution number " << solutionNumber << " RDM number " << rdmNumber << " rank number " << rank2 << endl;
			time_t start = time(0);
			
			simulation.fixRDMFactors(rdmNumber);
			simulation.correlateDemandVariations(1.0); 
				// 1.0 reflects no scaling
			
			//cout << "Solution " << solutionNumber << " rdmNumber " << rdmNumber << " took " << difftime( time(0), start) << " seconds to set up data." << endl;

 	        simulation.solutionNumber = rank;
            // cout << "Calculating solution number " << solutionNumber << " RDM number " << rdmNumber << endl;
			
			calculation_time = simulation.calculation(c_xreal, c_obj, c_constr);
			
            //cout << "Solution number " << solutionNumber << " RDM number " << rdmNumber << " took " << calculation_time << " seconds." << endl;
			
			for (int x = 0; x< c_num_dec; x++)
			{
				out1 << simulation.parameterInput[simulation.solutionNumber][x] << ",";
			}
			for (int x = 0; x < c_num_obj; x++)
			{
				out1 << c_obj[x] << ",";
			}
			out1 << endl;
		//}
		
		MPI_Finalize();
		out1.close();
	}

	zap(c_obj);
	zap(c_xreal);
	zap(c_constr);

	return 0;
}
