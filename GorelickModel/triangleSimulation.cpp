/////////////////////////////////////////////
///  Research Triangle Water Supply Model ///
////////////  For use with MOEA /////////////


/*  COMMENTS INCLUDED BY DAVID GORELICK,
    BEGINNING 11-09-2015 TO HELP UNDERSTAND  */

#include "global.h"
#include "Simulation.h"
#include "moeaframework.h"
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
	cerr << "-a <alternative> \t alternative to be evaluated (optional)." << endl;
	cerr << "-f <deeply uncertain factors vector> \t number of the first deeply uncertain factor vector to be used for reevaluations (optional)." << endl;
	cerr << "-e <deeply uncertain factors vector> \t number of the last deeply uncertain factor vector to be used for reevaluations (optional)." << endl;
	cerr << "-p <comprehensive output> \t print entire output (infrestructure built, etc) (optional)." << endl;
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

	int opt;
	//int seed = (int)time(NULL);
	int seed = 1;
	int numRealizations;
	int duFactorsFrom = 0;
	int duFactorsTo = 1;
	int alternative = 0;
	int interval = 0;
	simulation.printDetailedOutput = false;

	while ((opt = getopt(argc, argv, "r:t:c:b:s:a:f:e:p:i:h")) != -1)
	{
		switch (opt)
		{
			case 'r':
				numRealizations = atoi(optarg);
				break;
			case 't':
				simulation.numRecords = atoi(optarg);
				break;
			case 'c':
				simulation.formulation = atoi(optarg);
				break;
			case 'b':
				simulation.borgToggle = atoi(optarg);
				break;
			case 's':
				seed = atoi(optarg);
				break;
			case 'a':
				alternative = atoi(optarg);
				break;
			case 'f':
				duFactorsFrom = atoi(optarg);
				break;
			case 'e':
				duFactorsTo = atoi(optarg);
				break;
			case 'p':
				simulation.printDetailedOutput = true;
				break;
			case 'i':
				interval = atoi(optarg);
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

	int startYear = 2010;
	int endYear = 2060;
	int currentYear = 2015;

	int terminateYear = endYear-startYear + 1;
	int startSimulationYear = currentYear - startYear + 1;
	simulation.setNumYears(terminateYear);
	simulation.setStartYear(startSimulationYear);
	
	simulation.numIntervals = 20;
		// the number of discrete volume increments
		// used for insurance payouts and for 
		// setting the "GUIDE curve" for raleigh
		// and durham water supply for release determination
	simulation.availableJLallocation = 1.00;
		// the fraction of Jordan Lake water supply pool
		// that can be divided among Cary, OWASA, Durham, and Raleigh.
		// may be less than 1 (100%) because of allocation
		// to Chatham County communities.
	if (simulation.formulation > 0)
		// in any formulation where treated transfers occur 
	{
		simulation.allowReleases = 0;
			// a logical to determine whether releases will occur
			// SET TO 0 SO RELEASES NEVER OCCUR 
	}
	else 
	{
		simulation.allowReleases = 0;
	}
	
	//seed random number generator
	// NOTE: for model, the seed stays the same. For Borg, the seed comes from command line argument.
	srand(1);

	//variables for interfacing with algorithm
	int c_num_dec = 63;
	double *c_xreal;
	general_1d_allocate(c_xreal, c_num_dec);
        // c_xreal is decision vars
        // c_num_dec is number of dec vars
		// MAY 2016: ADD NEW VARIABLES TO THIS 
		// NOW 63 VARIABLES 
	
	simulation.setNumDecisions(c_num_dec);

	// Import historical demand and inflow datasets
	//cout << "import data files" << endl;
	
	
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


	double *c_obj = NULL, *c_constr = NULL;
	int c_num_obj;
	int c_num_constr;

	if (simulation.borgToggle < 3)
	{

		// c_num_obj = 6;//Number of objective variables

		// // JDH 11/12: Turning off constraints for now (below here, only for parallel version)
		// c_num_constr = 0;
		// general_1d_allocate(c_obj, c_num_obj);
		// //general_1d_allocate(c_constr, c_num_constr);

		// simulation.initializeFormulation(c_num_obj, c_num_constr); // number of decisions, objectives, constraints

		// // Interface with Borg-MS (parallel)
		// #ifdef PARALLEL

			// // BORG_Debug_on();
			// // BORG_Algorithm_ms_max_time(0.008);
			// // BORG_Algorithm_output_aerovis();

			// char runtime[256];
			// char outputFilename[256];
			// FILE* outputFile = NULL;

			// BORG_Algorithm_ms_startup(&argc, &argv);
			// BORG_Algorithm_ms_max_evaluations(10000);
			// BORG_Algorithm_output_frequency(10);
			// BORG_Problem problem = BORG_Problem_create(c_num_dec, c_num_obj, c_num_constr, calculationWrapperFunction);

			// // Set all the parameter bounds and epsilons
			// setProblemDefinition(problem, simulation.formulation);

			// // This is set up to run only one seed at a time.

			// sprintf(runtime, "./output/O%d_F%d/CBorg_NCTriangle_O%d_F%d_S%d.runtime", simulation.borgToggle, simulation.formulation, simulation.borgToggle, simulation.formulation, seed);
			// sprintf(outputFilename, "./output/O%d_F%d/CBorg_NCTriangle_O%d_F%d_S%d.set", simulation.borgToggle, simulation.formulation, simulation.borgToggle, simulation.formulation, seed);
			// BORG_Algorithm_output_runtime(runtime);

			// BORG_Random_seed(seed);
			// BORG_Archive result = BORG_Algorithm_ms_run(problem);

			// // If this is the master node, print out the final archive
			// if (result != NULL) {
				// outputFile = fopen(outputFilename, "w");
				// if (!outputFile) {
					// BORG_Debug("Unable to open final output file\n");
				// }
				// BORG_Archive_print(result, outputFile);
				// BORG_Archive_destroy(result);
				// fclose(outputFile);
			// }

			// BORG_Algorithm_ms_shutdown();
			// BORG_Problem_destroy(problem);

		// #else // Interface with MOEA Framework

			// MOEA_Init(c_num_obj, c_num_constr); // pass number of objectives and formulation to MOEA

			// while (MOEA_Next_solution() == MOEA_SUCCESS)
			// {
				// MOEA_Read_doubles(c_num_dec, c_xreal);//Input decision variables
				// simulation.calculation(c_xreal, c_obj, c_constr); // Run simulation iteration
				// MOEA_Write(c_obj, c_constr);//write new objective functions
			// }

		// #endif

	}
	else // If running from parameter input file (no constraints here)
        // RUNNING FROM AN INPUT FILE... now just looking at
        // optimized outputs, not actually optimizing here
	{
		int c_num_obj = 29;
            // 6 OBJ VALUES are combined from each utility,
            // this can look at them separately
		general_1d_allocate(c_obj, c_num_obj);
		simulation.initializeFormulation(c_num_obj, 0);
		simulation.directoryName = "./inputfiles/";

		// cout << "running simulations" << endl;

		// Read a certain number of parameter sets from a file
		int numSolutions = 32;
		int numDeepFactorCombs = 4;
		std::stringstream rdmstm;
		rdmstm << "RDMSamples.csv";// << duFactorsFrom << "_" << duFactorsTo << ".csv";
		std::string rdmfilename = rdmstm.str();
            // make sure it is num of rows in the following file
		readFile(simulation.parameterInput, "./parameterInputFile2.csv", numSolutions, c_num_dec);
		// Read the random samples of the 13 uncertain parameters. This will be a 13 x numRealizations matrix.  ADDED BY BERNARDO
		readFile(simulation.RDMInput, rdmfilename, numDeepFactorCombs, simulation.num_rdm_factors);

		// Set up the output stream for objective values
		MPI_Init(NULL,NULL);
		int rank;
		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            // gives a value to rank, based on the number of processors being used
            // each processor doing the same thing, but each has diff rank
		int rank2 = rank + 256;
		std::string filename1 = "output/simulationOutput";
		std::string filename2 = ".csv";
		
		simulation.directoryName = "./inputfiles/";
		std::string completeFilename;
		
		std::stringstream sstm;
		
		sstm<< filename1 << (rank + interval) << "_" << duFactorsFrom << "_" << duFactorsTo << filename2;
		
		completeFilename = sstm.str();
		
		ofstream out1;
		
		openFile(out1, completeFilename);
		double calculation_time;
		
		// RDM LOOP
		for (int i = duFactorsFrom; i <= duFactorsTo; i++)
		{
			simulation.fixRDMFactors(i);
			simulation.correlateDemandVariations(1.0); // 1.0 reflects no scaling

			simulation.solutionNumber = rank + interval;
            //simulation.solutionNumber = 1;
            cout << "Calculating rank " << rank << " RDM number " << i << endl;
			calculation_time = simulation.calculation(c_xreal, c_obj, c_constr);
            cout << "Rank " << rank << " RDM number " << i << " took " << calculation_time << " seconds." << endl;
			for (int x = 0; x< c_num_dec; x++)
			{
				out1<<simulation.parameterInput[simulation.solutionNumber][x]<<",";
			}
			for (int x = 0; x < c_num_obj; x++)
			{
				out1 << c_obj[x] << ",";
			}
			out1 << endl;
		}
		MPI_Finalize();
		out1.close();
	}

	zap(c_obj);
	zap(c_xreal);
	zap(c_constr);

	return 0;
}