#ifndef Simulation_h
#define Simulation_h

#include "WaterUtility.h"
#include "TimeSeriesData.h"
#include "ReservoirStorage.h"
#include "Dates.h"

class Simulation
{
public:
    Simulation();
	~Simulation();
	void importDataFiles();
	void writeDataLists();

	void setNumRealizations(int c_num_rel);
	void setNumYears(int c_terminateYear);
	
	void preconditionData(double unit_demand_multiplier, double future_demand_multiplier, bool firstTime);
	void realizationLoop();
	double calculation (double *c_xreal, double *c_obj, double *c_constr);
	void correlateDemandVariations(double demand_variation_multiplier);
	void setStartYear(int SSY);
	void fixRDMFactors();
	
	WaterUtility durham, owasa, cary, raleigh;
	TimeSeriesData durhamInflows, owasaInflows, fallsInflows, wheelerInflows, crabtreeInflows, claytonInflows, jordanInflows, lillingtonInflows, littleRiverRaleighInflows;
	TimeSeriesData durhamOwasaEvap;

	int startYear;
	int endYear;
	int currentYear;
	
	string directoryName;
	string historicFlowPath;
	string demandDataPath;
	string demandOutputPath;
	string synthFlowsPath;
	
	bool use_RDM_ext;
	int numRecords;
	
	int numRealizations;

    int terminateYear;
private:
	
	//class definitions
	Dates simDates;
	
	//data classes for file reading
	data_t littleRiverInflow;
	data_t michieInflow;
	data_t owasaInflow;
	data_t fallsLakeInflow, lakeWBInflow;
	data_t claytonInflow, crabtreeInflow;
	data_t jordanLakeInflow, lillingtonGaugeInflow, littleRiverRaleighInflow;

	
	data_t streamflowIndex;
	
	data_t caryFutureD, raleighFutureD, durhamFutureD, owasaFutureD;
	data_t caryFutureB, raleighFutureB, durhamFutureB, owasaFutureB;
	
	//other variables
	int year, month, week, numdays;
    int numFutureYears;
	int startSimulationYear;
	double *actualStreamflows;
	double *totalFallsFailure;
};
#endif
