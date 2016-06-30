#ifndef Simulation_h
#define Simulation_h

#include "WaterUtility.h"
#include "TimeSeriesData.h"
#include "ReservoirStorage.h"
#include "Dates.h"

class Simulation
{
public:
	~Simulation();
	void importDataFiles();
	void writeDataLists();
	void initializeFormulation(int c_num_obj, int c_num_constr);
	void setNumDecisions(int c_num_dec);

	void setNumRealizations(int c_num_rel);
	void setNumYears(int c_terminateYear);
	
	void preconditionData(double unit_demand_multiplier, double future_demand_multiplier, bool firstTime);
	void realizationLoop();
	double calculation (double *c_xreal, double *c_obj, double *c_constr);
	void calculateWaterPrices();
	void calculateWaterSurcharges();
	
	void generateStreamflows();
	void correlateDemandVariations(double demand_variation_multiplier);
	
	void createRiskOfFailure(int real, int synthY, double durhAnnDemand, double owasaAnnDemand, double ralAnnDemand, double carAnnDemand,
							 int discreteintervals);
	int numIntervals;
	
	void createInfrastructureRisk(int real, int synthY, double durhAnnDemand, double owasaAnnDemand, double ralAnnDemand, double carAnnDemand);
	data_t parameterInput;
	void createInfrastructure(int realization);
	void triggerInfrastructure(int realization);
	void updateFallsQuality();
	void setStartYear(int SSY);
	void chooseStreamflows();
	void fixRDMFactors(int rdm_i);
	
	WaterUtility durham, owasa, cary, raleigh;
	TimeSeriesData durhamInflows, owasaInflows, fallsInflows, wheelerInflows, crabtreeInflows, claytonInflows, jordanInflows, lillingtonInflows, littleRiverRaleighInflows;
	TimeSeriesData durhamOwasaEvap, fallsEvap, wheelerEvap;
	
	int formulation;
	int borgToggle;
	bool printDetailedOutput;
	int numRealizationsTOREAD;
	
	data_t RDMInput;
	static const int num_rdm_factors = 30;
	
	string directoryName;
	string historicFlowPath;
	string syntheticFlowPath;
	string evaporationPath;
	string oldstochPath;
	string demanddataPath;
	
	bool use_RDM_ext;
	bool indepReleaseAlloc;
	
	double rdm_factors[30];
	int rdmNumber;
	int nDeeplyUncertainSets;

	int solutionNumber;
	int bondLength;
	int numRecords;
	double bondRate;
	int rank;
	
	ofstream createROFout;
	double availableJLallocation;
	int allowReleases;
	
	double LMreleaseCap;
	double buybackratePerMG;
	double ReleaseContractPrice;
	double FallsSupplyAllocationFraction;
	double FLSPfrac;

	double falls_lake_supply_capacity;
	double falls_lake_wq_capacity;
	double jordan_lake_supply_capacity;
	double jordan_lake_wq_capacity;
	double cary_treatment_capacity;
	double durham_cary_capacity;
	double durham_owasa_capacity;
	double raleigh_cary_capacity;
	
	int numRealizations;
	
private:
	
	//class definitions
	Dates simDates;
	Dates riskOfFailureDates;
	ReservoirStorage systemStorage;
	ReservoirStorage riskOfFailureStorageInf;
	ReservoirStorage riskOfFailureStorageROF;
	ReservoirStorage riskOfFailureStorageIP;

	//for calc water price
	double sewerFractions[12];
	
	//objectives/decisions
	int num_dec, num_obj, num_constr;
	double *xreal;
	
	//data classes for file reading
	data_t littleRiverInflow;
	data_t michieInflow;
	data_t owasaInflow;
	data_t evaporation;
	data_t fallsLakeInflow, lakeWBInflow;
	data_t fallsLakeEvaporation, lakeWheelerEvaporation;
	data_t claytonInflow, crabtreeInflow;
	data_t jordanLakeInflow, lillingtonGaugeInflow, littleRiverRaleighInflow;
	
	data_t littleRiverInflowSYN;
	data_t michieInflowSYN;
	data_t owasaInflowSYN;
	data_t evaporationSYN;
	data_t fallsLakeInflowSYN, lakeWBInflowSYN;
	data_t fallsLakeEvaporationSYN, lakeWheelerEvaporationSYN;
	data_t claytonInflowSYN, crabtreeInflowSYN;
	data_t jordanLakeInflowSYN, lillingtonGaugeInflowSYN, littleRiverRaleighInflowSYN;
	
	data_t streamflowIndex;
	
	data_t caryFutureD, raleighFutureD, durhamFutureD, owasaFutureD;
	data_t caryFutureB, raleighFutureB, durhamFutureB, owasaFutureB;
	data_t durhamROF, owasaROF, raleighROF;
	data_t returnRatio;
	
	//durham variables
	double durhamFlowWeekBaseline;
	int durhamLevel, durhamLevelIns;
	
	//owasa variables
	double owasaFlowWeekBaseline;
	int owasaLevel, owasaLevelIns;

	//cary variables
	double caryAllocCapacity;
	double caryFlowWeekBaseline;
	int caryLevel, caryLevelIns;
	
	//raleigh variables
	double raleighFlowWeekBaseline, raleighFlowWeekBaseline2;
	int raleighLevel, raleighLevelIns;
	
	double durhamSpill, OWASASpill, raleighSpill, insuranceFallsInflow, insuranceJordanInflow;
	
	//other variables
	int year, month, week, numdays;	
	int inflowC, inflowR;
	int billingMonths;		
	double elasticity_Of_Demand[5];
	int terminateYear;
	double transferCosts;
	int numFutureYears;
	int startSimulationYear;
	int volumeIncrements, costRiskLevel;
	double *actualStreamflows;
	
	
	double caryUpgrades[4];
	double caryWTPcosts[3];
	double caryBuild[3];
	double ralIntakeInsuranceTrigger;
	double durhamReclaimedInsuranceTrigger;
	double wwWTPInsuranceTrigger;
	double littleRiverRalInsuranceTrigger;
	double fallsLakeReallocation;
	
	double fallsFailurePoint, maxFallsFailure;
	double *totalFallsFailure;
	int thisYearFalls;
	
	ofstream out1;
	ofstream out3;
	ofstream InfraBuilt;
	
};
#endif
