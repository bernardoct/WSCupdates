#include "global.h"
#include "Simulation.h"
#include <time.h>
#include <string>
#include <unistd.h>

using namespace std;

void Simulation::setNumRealizations(int c_num_rel)
{
	numRealizations = c_num_rel;
	return;
}

void Simulation::setNumYears(int c_terminateYear)
{
	terminateYear = c_terminateYear;
	return;
}
void Simulation::setStartYear(int c_startSimulationYear)
{
	startSimulationYear = c_startSimulationYear;
	return;
}

void Simulation::setNumDecisions(int c_num_dec)
{
	num_dec = c_num_dec;
	return;
}
void Simulation::initializeFormulation(int c_num_obj, int c_num_constr)
{
	num_obj = c_num_obj;
	num_constr = c_num_constr;
	return;
}

void Simulation::writeDataLists()
{
	billingMonths = 12;
		//used to create average water revenues from Durham billing data
	transferCosts = 3000.0/1000000.0;
		//Price of water transfers ($MM/MG)
	volumeIncrements = numIntervals;
		// THIS CAN BE CHANGED??
	numFutureYears = 51;
	general_1d_allocate(xreal, num_dec);
		//Decision variables
	general_1d_allocate(actualStreamflows,numRealizations);
	general_1d_allocate(totalFallsFailure, terminateYear, 0.0);
	
	durham.name = "Durham";
	raleigh.name = "Raleigh";
	cary.name = "Cary";
	owasa.name = "OWASA";


	// Define Utility Info
	// (months, years, types, tiers, stages, numFutureYears, failurePoint, numAnnualDecisionPeriods)
	
	durham.configure(billingMonths, 3, 13, 10, 5, numFutureYears, 0.2, 2, terminateYear, volumeIncrements, numRealizations, formulation, 5, numContractRiskYears);
	cary.configure(billingMonths, 1, 9, 9, 5, numFutureYears, 0.2, 2, terminateYear, volumeIncrements, numRealizations, formulation, 0, numContractRiskYears);
	owasa.configure(billingMonths, 1, 8, 8, 4, numFutureYears, 0.2, 1, terminateYear, volumeIncrements, numRealizations, formulation, 4, numContractRiskYears);
	raleigh.configure(billingMonths, 1, 24, 24, 5, numFutureYears, 0.2, 1, terminateYear, volumeIncrements, numRealizations, formulation, 4, numContractRiskYears);

	// Utility Water Prices
	// OWASA: 9 customer classes: 0-4, Residential volumetric tiers 1-5;
	//							  5, Irrigation only use;
	//							  6, Seasonally charged customers
	//							  7, Bulk water rate
	//							  8, Sewer charge

	// Irrigation Season (May - October)
	general_1d_allocate(owasa.waterPrice_Irr, 9, 0.0);
	owasa.waterPrice_Irr[0] = 2580;  owasa.waterPrice_Irr[1] = 6260;
	owasa.waterPrice_Irr[2] = 7680;	 owasa.waterPrice_Irr[3] = 10730;
	owasa.waterPrice_Irr[4] = 19400; owasa.waterPrice_Irr[5] = 8340;
	owasa.waterPrice_Irr[6] = 7750;	 owasa.waterPrice_Irr[7] = 5090;
	owasa.waterPrice_Irr[8] = 6350;

	// Non-irrigation Season (November - April)
	general_1d_allocate(owasa.waterPrice_NonIrr, 9, 0.0);
	owasa.waterPrice_NonIrr[0] = 2580;	owasa.waterPrice_NonIrr[1] = 6260;
	owasa.waterPrice_NonIrr[2] = 7680;	owasa.waterPrice_NonIrr[3] = 10730;
	owasa.waterPrice_NonIrr[4] = 19400;	owasa.waterPrice_NonIrr[5] = 8340;
	owasa.waterPrice_NonIrr[6] = 4080;	owasa.waterPrice_NonIrr[7] = 5090;
	owasa.waterPrice_NonIrr[8] = 6350;

	// Drought surcharge multipliers (currently in place)
	// Regular Use
	general_2d_allocate(owasa.droughtSurcharges, 9, 4, 0.0);

	owasa.droughtSurcharges[0][0] = 1;	owasa.droughtSurcharges[1][0] = 1;
	owasa.droughtSurcharges[2][0] = 1;	owasa.droughtSurcharges[3][0] = 1;
	owasa.droughtSurcharges[4][0] = 1;	owasa.droughtSurcharges[5][0] = 1;
	owasa.droughtSurcharges[6][0] = 1;	owasa.droughtSurcharges[7][0] = 1;
	owasa.droughtSurcharges[8][0] = 1;

	// Stage 1 Restrictions
	owasa.droughtSurcharges[0][1] = 1;    owasa.droughtSurcharges[1][1] = 1;
	owasa.droughtSurcharges[2][1] = 1.25; owasa.droughtSurcharges[3][1] = 1.5;
	owasa.droughtSurcharges[4][1] = 2;    owasa.droughtSurcharges[5][1] = 1.15;
	owasa.droughtSurcharges[6][1] = 1.15; owasa.droughtSurcharges[7][1] = 1.15;
	owasa.droughtSurcharges[8][1] = 1;

	// Stage 2 Restrictions
	owasa.droughtSurcharges[0][2] = 1;    owasa.droughtSurcharges[1][2] = 1.25;
	owasa.droughtSurcharges[2][2] = 1.5;  owasa.droughtSurcharges[3][2] = 2;
	owasa.droughtSurcharges[4][2] = 3;    owasa.droughtSurcharges[5][2] = 1.25;
	owasa.droughtSurcharges[6][2] = 1.25; owasa.droughtSurcharges[7][2] = 1.25;
	owasa.droughtSurcharges[8][2] = 1;

	// Stage 3 Restrictions
	owasa.droughtSurcharges[0][3] = 1;    owasa.droughtSurcharges[1][3] = 1.5;
	owasa.droughtSurcharges[2][3] = 2;    owasa.droughtSurcharges[3][3] = 3;
	owasa.droughtSurcharges[4][3] = 4;    owasa.droughtSurcharges[5][3] = 1.5;
	owasa.droughtSurcharges[6][3] = 1.5;  owasa.droughtSurcharges[7][3] = 1.5;
	owasa.droughtSurcharges[8][3] = 1;


	// Durham: 11 customer classes 		0-4, Volumetric tiers 1-5, residential consumers
	//								    5-9, Volumetric tiers 6-10, commercial/industrial consumers
	//								    10,	Volumetric sewer charges

	general_1d_allocate(durham.waterPrice, 11, 0.0);
	durham.waterPrice[0] = 2299.3;	durham.waterPrice[1] = 3462.3;	durham.waterPrice[2] = 3796.5;
	durham.waterPrice[3] = 4959.5;	durham.waterPrice[4] = 7432.6;
	durham.waterPrice[5] = 2299.3;	durham.waterPrice[6] = 3462.3;	durham.waterPrice[7] = 3796.5;
	durham.waterPrice[8] = 4959.5;	durham.waterPrice[9] = 7432.6; durham.waterPrice[10] = 4384.7;

	// Cary: 9 customer classes 	0-3, Volumetric tiers 1-4, residential consumers
	//								4, Volumetric tier, multi-family consumers
	//								5, Volumetric tier, commercial/industrial consumers
	//								6-7, Irrigation volumetric tiers 1-2, residential consumers
	//								8, Irrigation volumetric tier, commercial/industrial consumers
	//								9,	Volumetric sewer charges

	general_1d_allocate(cary.waterPrice, 10, 0.0);
	cary.waterPrice[0] = 3600;	cary.waterPrice[1] = 4080;	cary.waterPrice[2] = 5790;
	cary.waterPrice[3] = 11290;	cary.waterPrice[4] = 4080; cary.waterPrice[5] = 4080;
	cary.waterPrice[6] = 5790;	cary.waterPrice[7] = 11290;	cary.waterPrice[8] = 6380;
	cary.waterPrice[9] = 8470;

	// Raleigh: 25 customer classes 	0-2, volumetric tiers 1-5, inside city limits (Raleigh and Garner)
	//									3, non-residential rates, inside city limits (Raleigh and Garner)
	//									4, irrigation only rates, inside city limits (Raleigh and Garner)
	//									5, Rolesville, volumetric charge inside city limits
	//									6, Knightsdale, volumetric charge inside city limits
	//									7, Wake Forest, volumetric charge inside city limits
	//									8, Wendell, volumetric charge inside city limits
	//									9, Wendell irrigation, volumetric charge inside city limits
	//									10, Zebulon, volumetric charge inside city limits
	//									11, Zebulon reused water, volumetric charge inside city limits
	//									12-14, volumetric tiers 1-5, outside city limits (Raleigh and Garner)
	//									15, non-residential rates, outside city limits (Raleigh and Garner)
	//									16, irrigation only rates, outside city limits (Raleigh and Garner)
	//									17, Rolesville, volumetric charge outside city limits
	//									18, Knightsdale, volumetric charge outside city limits
	//									19, Wake Forest, volumetric charge outside city limits
	//									20, Wendell, volumetric charge outside city limits
	//									21, Wendell irrigation, volumetric charge outside city limits
	//									22, Zebulon, volumetric charge outside city limits
	//									23, Zebulon reused water, volumetric charge outside city limits
	//									24, Volumetric sewer charge

	general_1d_allocate(raleigh.waterPrice, 25, 0.0);
	raleigh.waterPrice[0] = 1705;	raleigh.waterPrice[1] = 2842;	raleigh.waterPrice[2] = 3792;
	raleigh.waterPrice[3] = 2206;	raleigh.waterPrice[4] = 3792;	raleigh.waterPrice[5] = 1900;
	raleigh.waterPrice[6] = 2902;	raleigh.waterPrice[7] = 3366;	raleigh.waterPrice[8] = 3546;
	raleigh.waterPrice[9] = 3546;	raleigh.waterPrice[10] = 3762;	raleigh.waterPrice[11] = 1676;
	raleigh.waterPrice[12] = 3411;	raleigh.waterPrice[13] = 5685;	raleigh.waterPrice[14] = 7585;
	raleigh.waterPrice[15] = 4413;	raleigh.waterPrice[16] = 7585;	raleigh.waterPrice[17] = 3628;
	raleigh.waterPrice[18] = 5804;	raleigh.waterPrice[19] = 8303;	raleigh.waterPrice[20] = 7091;
	raleigh.waterPrice[21] = 7091;	raleigh.waterPrice[22] = 7517;	raleigh.waterPrice[23] = 3441;
	raleigh.waterPrice[24] = 2177;

	// Fraction of total water use billed as sewer charges
	sewerFractions[0] = .89;	sewerFractions[1] = .91;
	sewerFractions[2] = .84;	sewerFractions[3] = .88;
	sewerFractions[4] = .82;	sewerFractions[5] = .75;
	sewerFractions[6] = .83;	sewerFractions[7] = .84;
	sewerFractions[8] = .87;	sewerFractions[9] = .89;
	sewerFractions[10] = .87;	sewerFractions[11] = .89;


	// Consumer reductions factor (0/1 is irrigation/non, and 0-4 is the restriction stage
	durham.individualReductions[0][0] = 1;
	durham.individualReductions[0][1] = .85;
	durham.individualReductions[0][2] = .7;
	durham.individualReductions[0][3] = .6;
	durham.individualReductions[0][4] = .45;
	durham.individualReductions[1][0] = 1;
	durham.individualReductions[1][1] = .93;
	durham.individualReductions[1][2] = .88;
	durham.individualReductions[1][3] = .83;
	durham.individualReductions[1][4] = .7;

	owasa.individualReductions[0][0] = 1;
	owasa.individualReductions[0][1] = .9;
	owasa.individualReductions[0][2] = .85;
	owasa.individualReductions[0][3] = .8;

	raleigh.individualReductions[0][0] = 1;
	raleigh.individualReductions[0][1] = .952;
	raleigh.individualReductions[0][2] = .91;
	raleigh.individualReductions[0][3] = .724;
	raleigh.individualReductions[0][4] = .634;

	//Cary reductions by stage
	cary.individualReductions[0][0] = 1;
	cary.individualReductions[0][1] = .87;
	cary.individualReductions[0][2] = .68;
	cary.individualReductions[0][3] = .62;
	cary.individualReductions[0][4] = .54;
	cary.individualReductions[1][0] = 1;
	cary.individualReductions[1][1] = .94;
	cary.individualReductions[1][2] = .9;
	cary.individualReductions[1][3] = .83;
	cary.individualReductions[1][4] = .71;


	// Elasticity values for different customer classes
	// Type 0: Residential, low volume use
	// Type 1: Residential, mid volume use
	// Type 2: Residential, high volume use
	// Type 3: Consumer/Industrial, low volume use
	// Type 4: Consumer/Industrial, high volume use
	elasticity_Of_Demand[0] = -.11; elasticity_Of_Demand[1] = -.58;
	elasticity_Of_Demand[2] = -1.16; elasticity_Of_Demand[3] = -.43;
	elasticity_Of_Demand[4] = -.45;

	// Hardcoded elasticity index values for each tier
	durham.elasticityIndex[0] = 0; durham.elasticityIndex[1] = 1;
	durham.elasticityIndex[2] = 1; durham.elasticityIndex[3] = 2;
	durham.elasticityIndex[4] = 2; durham.elasticityIndex[5] = 3;
	durham.elasticityIndex[6] = 3; durham.elasticityIndex[7] = 3;
	durham.elasticityIndex[8] = 4; durham.elasticityIndex[9] = 4;

    owasa.elasticityIndex[0] = 0; owasa.elasticityIndex[1] = 1;
	owasa.elasticityIndex[2] = 1; owasa.elasticityIndex[3] = 2;
	owasa.elasticityIndex[4] = 2; owasa.elasticityIndex[5] = 3;
	owasa.elasticityIndex[6] = 3; owasa.elasticityIndex[7] = 4;

	cary.elasticityIndex[0] = 0; cary.elasticityIndex[1] = 1;
	cary.elasticityIndex[2] = 2; cary.elasticityIndex[3] = 2;
	cary.elasticityIndex[4] = 1; cary.elasticityIndex[5] = 3;
	cary.elasticityIndex[6] = 3; cary.elasticityIndex[7] = 4;
	cary.elasticityIndex[8] = 3;

	raleigh.elasticityIndex[0] = 0; raleigh.elasticityIndex[1] = 1;
	raleigh.elasticityIndex[2] = 2; raleigh.elasticityIndex[3] = 3;
	raleigh.elasticityIndex[4] = 3; raleigh.elasticityIndex[5] = 1;
	raleigh.elasticityIndex[6] = 2; raleigh.elasticityIndex[7] = 1;
	raleigh.elasticityIndex[8] = 1; raleigh.elasticityIndex[9] = 1;
	raleigh.elasticityIndex[10] = 1; raleigh.elasticityIndex[11] = 1;
	raleigh.elasticityIndex[12] = 0; raleigh.elasticityIndex[13] = 1;
	raleigh.elasticityIndex[14] = 2; raleigh.elasticityIndex[15] = 3;
	raleigh.elasticityIndex[16] = 3; raleigh.elasticityIndex[17] = 1;
	raleigh.elasticityIndex[18] = 2; raleigh.elasticityIndex[19] = 1;
	raleigh.elasticityIndex[20] = 1; raleigh.elasticityIndex[21] = 1;
	raleigh.elasticityIndex[22] = 1; raleigh.elasticityIndex[23] = 1;

	return;
}

void Simulation::importDataFiles()
{
	//openFile(out1, "reservoirOutput1PSU.csv");
	//openFile(out3, "reservoirOutput.csv");
	//systemStorage.openResFiles();
	// Read in all input files
	// string directoryName = "./inputfiles/";

	// Weekly inflow data
	// 81 Years: 1930 - 2010
	// 82 Years: 1926 - 2007
	// 83 Years: 1928 - 2010
	// 83 Years: 1929 - 2011 (little river raleigh)
	readFile(michieInflow, directoryName + historicFlowPath + "updatedMichieInflow.csv", 81, 52);
	readFile(littleRiverInflow, directoryName + historicFlowPath + "updatedLittleRiverInflow.csv", 81, 52);
	readFile(owasaInflow, directoryName + historicFlowPath + "updatedOWASAInflow.csv", 81, 52);
	readFile(fallsLakeInflow, directoryName + historicFlowPath + "updatedFallsLakeInflow.csv", 81, 52);
	readFile(lakeWBInflow, directoryName + historicFlowPath + "updatedLakeWBInflow.csv", 81, 52);
	readFile(claytonInflow, directoryName + historicFlowPath + "claytonGageInflow.csv", 81, 52);
	readFile(crabtreeInflow, directoryName + historicFlowPath + "crabtreeCreekInflow.csv", 81, 52);
	readFile(jordanLakeInflow, directoryName + historicFlowPath + "updatedJordanLakeInflow.csv", 81, 52);
	readFile(lillingtonGaugeInflow, directoryName + historicFlowPath + "updatedLillingtonInflow.csv", 81, 52);
	readFile(littleRiverRaleighInflow, directoryName + historicFlowPath + "updatedLittleRiverRaleighInflow.csv", 81, 52);

	readFile(streamflowIndex, directoryName + historicFlowPath + "streamflowSample.csv", 100, 1);


	// Evaporation data (MG/acre)
	// 82 years (1926 - 2007)
	// 83 years (1928 - 2010)
	readFile(evaporation, directoryName + evaporationPath + "updatedEvap.csv", 81, 52); // for Durham and OWASA
	readFile(fallsLakeEvaporation, directoryName + evaporationPath + "fallsLakeEvap.csv", 81, 52);
	readFile(lakeWheelerEvaporation, directoryName + evaporationPath + "lakeWheelerBensonEvap.csv", 81, 52);

	// U files from Cholesky Factorization - combine with standardized inflows
	// To make stochastic inflows for weeks 27-52 (1) and weeks 1-26 (2).
	// Each file is an upper triangular matrix, 52x52
	//readFile(durhamInflows.stoch1, directoryName + oldstochPath + "updatedDurhamStoch1.csv", 52, 52);
	//readFile(durhamInflows.stoch2, directoryName + oldstochPath + "updatedDurhamStoch2.csv", 52, 52);
	//readFile(owasaInflows.stoch1, directoryName + oldstochPath + "updatedOwasaStoch1.csv",  52, 52);
	//readFile(owasaInflows.stoch2, directoryName + oldstochPath + "updatedOwasaStoch2.csv",  52, 52);
	//readFile(fallsInflows.stoch1, directoryName + oldstochPath + "updatedFallsStoch1.csv", 52, 52);
	//readFile(fallsInflows.stoch2, directoryName + oldstochPath + "updatedFallsStoch2.csv", 52, 52);
	//readFile(wheelerInflows.stoch1, directoryName + oldstochPath + "updatedWheelerStoch1.csv", 52, 52);
	//readFile(wheelerInflows.stoch2, directoryName + oldstochPath + "updatedWheelerStoch2.csv", 52, 52);
	//readFile(crabtreeInflows.stoch1, directoryName + oldstochPath + "updatedCrabtreeStoch1.csv", 52, 52);
	//readFile(crabtreeInflows.stoch2, directoryName + oldstochPath + "updatedCrabtreeStoch2.csv", 52, 52);
	//readFile(claytonInflows.stoch1, directoryName + oldstochPath + "updatedClaytonStoch1.csv", 52, 52);
	//readFile(claytonInflows.stoch2, directoryName + oldstochPath + "updatedClaytonStoch2.csv", 52, 52);
	//readFile(jordanInflows.stoch1, directoryName + oldstochPath + "updatedJordanStoch1.csv", 52, 52);
	//readFile(jordanInflows.stoch2, directoryName + oldstochPath + "updatedJordanStoch2.csv", 52, 52);
	//readFile(lillingtonInflows.stoch1, directoryName + oldstochPath + "updatedLillingtonStoch1.csv", 52, 52);
	//readFile(lillingtonInflows.stoch2, directoryName + oldstochPath + "updatedLillingtonStoch2.csv", 52, 52);
	//readFile(littleRiverRaleighInflows.stoch1, directoryName + oldstochPath + "updatedLittleRiverRaleighStoch1.csv", 52, 52);
	//readFile(littleRiverRaleighInflows.stoch2, directoryName + oldstochPath + "updatedLittleRiverRaleighStoch2.csv", 52, 52);

	// U file from Cholesky Factorization - combine with standardized evaporation
	// to make stochastic inflows for weeks 1 - 26 (2) and weeks 27 - 52 (1)
	// Upper triangular matrix, 52x52
	readFile(durhamOwasaEvap.stoch1, directoryName + evaporationPath + "updatedEvapStoch1.csv", 52, 52);
	readFile(durhamOwasaEvap.stoch2, directoryName + evaporationPath + "updatedEvapStoch2.csv", 52, 52);
	readFile(fallsEvap.stoch1, directoryName + evaporationPath + "updatedfallsEvapStoch1.csv", 52, 52);
	readFile(fallsEvap.stoch2, directoryName + evaporationPath + "updatedfallsEvapStoch2.csv", 52, 52);
	readFile(wheelerEvap.stoch1, directoryName + evaporationPath + "updatedwheelerEvapStoch1.csv", 52, 52);
	readFile(wheelerEvap.stoch2, directoryName + evaporationPath + "updatedwheelerEvapStoch2.csv", 52, 52);

	// 18 years (1990-2007) of weekly demand data (18 x 52)
	// Raleigh does not have enough data, so use Cary's instead
	readFile(durham.UnitDemand, directoryName + demanddataPath + "updatedDurhamUnitDemand.csv", 52, 18);
	readFile(owasa.UnitDemand, directoryName + demanddataPath + "updatedOWASAUnitDemand.csv", 52, 18);
	readFile(cary.UnitDemand, directoryName + demanddataPath + "updatedCaryUnitDemand.csv", 52, 18);
	readFile(raleigh.UnitDemand, directoryName + demanddataPath + "updatedRaleighUnitDemand.csv", 52, 1);

	// Usage data (format varies depending on the utility)
	readFile(owasa.useFractions, directoryName + demanddataPath + "updatedOWASARateFractions.csv", 12, 9);
	// Fraction of total use divided between 8 tiers (plus sewer charge as fractions of total water use
	// in the 9th column owasaRateFractions[months][rate tier] - 12x9

	readFile(owasa.useFractions_Restrictions, directoryName + demanddataPath + "updatedOWASARestRateFractions.csv", 12, 9);
	// Fraction of total use divided between 8 tiers (plus sewer charge as fractions of total water use
	// in the 9th column - when restrictions are occuring

	readFile(durham.usePatterns, directoryName + demanddataPath + "updatedDurhamUsePatterns.csv", 468, 6);
	// 3 years (2008-2010) of monthly durham tiered use data
	// 13 total types: indoor, outdoor * (res., comm., indus., instit., multi fam., irr.) & hydrant
	// 12 monthly values for each list type, repeated for three years

	readFile(raleigh.useFractions, directoryName + demanddataPath + "raleighRateFractions.csv", 12, 24);
	// Fraction of total use divided between 24 tiers (plus sewer charge as fractions of total water use
	// in the 25th column - when restrictions are occuring

	readFile(cary.usePatterns, directoryName + demanddataPath + "caryUseFractions.csv", 12, 10);
	//Fraction of total use divided between 9 tiers (plus sewer charge as a fraction of total water use in column 10)

	// 51 years (2010 - 2060) of average daily water use projections
	readFile(caryFutureD, directoryName + demanddataPath + "caryFutureDemand.csv", 1, 51);
	readFile(raleighFutureD, directoryName + demanddataPath + "raleighFutureDemand.csv", 1, 51);
	readFile(durhamFutureD, directoryName + demanddataPath + "durhamFutureDemand.csv", 1, 51);
	readFile(owasaFutureD, directoryName + demanddataPath + "owasaFutureDemand.csv", 1, 51);

	// 51 years (2010-2060) of projections of the water use breakdown into 5
	// use types (residential, commercial, industrial, irrigation, and institutional)
	readFile(caryFutureB, directoryName + demanddataPath + "caryFutureBreakdown.csv", 5, 51);
	readFile(raleighFutureB, directoryName + demanddataPath + "raleighFutureBreakdown.csv", 5, 51);
	readFile(durhamFutureB, directoryName + demanddataPath + "durhamFutureBreakdown.csv", 5, 51);
	readFile(owasaFutureB, directoryName + demanddataPath + "owasaFutureBreakdown.csv", 5, 51);

	// Percent of water use returned by Durham (row 0) and Raleigh (row 1) as wastewater
	readFile(returnRatio, directoryName + demanddataPath + "returnRatio.csv", 2, 52);


}

void Simulation::preconditionData(double unit_demand_multiplier, double future_demand_multiplier, bool firstTime)
{
	numFutureYears = 51;

	durham.shouldAllocate = firstTime;
	owasa.shouldAllocate = firstTime;
	raleigh.shouldAllocate = firstTime;
	cary.shouldAllocate = firstTime;

	//this section allows for use of historic record of variable length
	for (int x = 0; x<numFutureYears; x++)
	{
		// Scaling a linear projection -- multiply the slope and subtract off changes to the y-intercept
		cary.futureDemand[x] = caryFutureD[0][x]*future_demand_multiplier - caryFutureD[0][0]*(future_demand_multiplier-1);
		raleigh.futureDemand[x] = raleighFutureD[0][x]*future_demand_multiplier - raleighFutureD[0][0]*(future_demand_multiplier-1);
		durham.futureDemand[x] = durhamFutureD[0][x]*future_demand_multiplier - durhamFutureD[0][0]*(future_demand_multiplier-1);
		owasa.futureDemand[x] = owasaFutureD[0][x]*future_demand_multiplier - owasaFutureD[0][0]*(future_demand_multiplier-1);

		for (int y = 0; y<5; y++)
		{
			cary.futureDemandBreakdown[y][x] = caryFutureB[y][x];
			raleigh.futureDemandBreakdown[y][x] = raleighFutureB[y][x];
			durham.futureDemandBreakdown[y][x] = durhamFutureB[y][x];
			owasa.futureDemandBreakdown[y][x] = owasaFutureB[y][x];
		}
	}

	// Weekly means and standard deviations are used to 'whiten' the historical data,
	// providing data sets where values are converted to 'standard deviations from the weekly mean'
	// for each week of the historical record
	/// Evaporation ///
	///////////////////
	int evapC = 52; // weeks per year in evaporation files
	int evapR = 81; // historical years of evaporation files

	if(firstTime)
	{
		durhamOwasaEvap.allocate(evapR, evapC, terminateYear*52, numRealizations);
		fallsEvap.allocate(evapR, evapC, terminateYear*52, numRealizations);
		wheelerEvap.allocate(evapR, evapC, terminateYear*52, numRealizations);

		//this section allows for use of historic record of variable length

		for (int row = 0; row<evapR; ++row)
		{
			for (int column = 0; column<evapC; ++column)
			{
				durhamOwasaEvap.rawData[row][column] = evaporation[row][column];// evaporation data from this set starts 3 years ealier than the other two
				fallsEvap.rawData[row][column] = fallsLakeEvaporation[row][column];
				wheelerEvap.rawData[row][column] = lakeWheelerEvaporation[row][column];
			}
		}

		durhamOwasaEvap.calculateNormalizations(evapR, evapC);
		fallsEvap.calculateNormalizations(evapR, evapC);
		wheelerEvap.calculateNormalizations(evapR, evapC);
	}

	/// Demand ///
	int UDc = 52;//weeks per year of historical demand data
	int UDr = 18;//historical years of demand data

	if(firstTime)
	{
		durham.UD.allocate(UDr, UDc);
		cary.UD.allocate(UDr, UDc);
		owasa.UD.allocate(UDr, UDc);
		raleigh.UD.allocate(UDr, UDc);
	}

	//this section allows for use of historic record of variable length

	for (int row = 0; row < UDr; row++)
	{
		for (int column = 0; column < UDc; column++)
		{
			cary.UD.rawData[row][column] = (cary.UnitDemand[column][row] - 1)*unit_demand_multiplier + 1;
			owasa.UD.rawData[row][column] = (owasa.UnitDemand[column][row] - 1)*unit_demand_multiplier + 1;
			durham.UD.rawData[row][column] = (durham.UnitDemand[column][row] - 1)*unit_demand_multiplier + 1;
		}
	}

	//performs mean, standard deviation, and 'whitening' calculations on historical demand datasets
	cary.UD.calculateNormalizations(UDr, UDc);
	durham.UD.calculateNormalizations(UDr, UDc);
	owasa.UD.calculateNormalizations(UDr, UDc);

	//only one year of Raleigh demand data exists, Cary standard deviations are used in calcs.
	for (int column = 0; column < UDc; column++)
	{
		raleigh.UD.averages[column] = (raleigh.UnitDemand[column][0] - 1)*unit_demand_multiplier + 1;
		raleigh.UD.standardDeviations[column] = cary.UD.standardDeviations[column];
	}

	/// Inflow ///
	inflowC = 52;//number of weeks in the inflow record
	inflowR = 81;//length of historical streamflow record
	int inflowR2 = numRealizations;
	if(firstTime)
	{
		durhamInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		owasaInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		fallsInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		wheelerInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		crabtreeInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		claytonInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		jordanInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		lillingtonInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		littleRiverRaleighInflows.allocate(inflowR, inflowC, terminateYear*52, numRealizations);
		//Fill out raw data, allowing for use of historic record of variable length
		for (int row = 0; row < inflowR; row++)
		{
			for (int col = 0; col < inflowC; col++)
			{
				durhamInflows.rawData[row][col]		= log(michieInflow[row][col]+littleRiverInflow[row][col]);
				owasaInflows.rawData[row][col]		= log(owasaInflow[row][col]);
				fallsInflows.rawData[row][col]		= log(fallsLakeInflow[row][col]);
				wheelerInflows.rawData[row][col]	= log(lakeWBInflow[row][col]);
				claytonInflows.rawData[row][col]	= log(claytonInflow[row][col]);
				crabtreeInflows.rawData[row][col]	= log(crabtreeInflow[row][col]);
				jordanInflows.rawData[row][col]		= log(jordanLakeInflow[row][col]);
				lillingtonInflows.rawData[row][col]	= log(lillingtonGaugeInflow[row][col]);
				littleRiverRaleighInflows.rawData[row][col] = log(littleRiverRaleighInflow[row][col]);

			}
		}

		int streamflowStartYear = inflowR - UDr;
		durhamInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
		owasaInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
		fallsInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
		wheelerInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
		claytonInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
		crabtreeInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
		jordanInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
		lillingtonInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
		littleRiverRaleighInflows.calculateNormalizations(inflowR, inflowC, streamflowStartYear);
	}

	// Creating joint probability density functions between the whitened demand and inflow data for each utility
	// PDF size is 16x16, with an extra numerical count as the last column
	int rows_PDF = 16;
	int cols_PDF = 17;
	double size1 = 16.0;
	double size2 = 16.0;
	int irrC = 23;
	int nonC1 = 16;
	int nonC2 = 13;

	durham.writeInflowDemandPDF(inflowR, UDr, rows_PDF, cols_PDF, size1, size2, irrC, nonC1, nonC2, &durhamInflows);
	cary.writeInflowDemandPDF(inflowR, UDr, rows_PDF, cols_PDF, size1, size2, irrC, nonC1, nonC2, &jordanInflows);
	owasa.writeInflowDemandPDF(inflowR, UDr, rows_PDF, cols_PDF, size1, size2, irrC, nonC1, nonC2, &owasaInflows);

	// Use Durham's inflow-demand PDF for Raleigh
	if(firstTime)
	{
		general_2d_allocate(raleigh.PDFIrr, rows_PDF, cols_PDF, 0);
		general_2d_allocate(raleigh.PDFNon, rows_PDF, cols_PDF, 0);
		general_2d_allocate(raleigh.CDFIrr, rows_PDF, cols_PDF, 0);
		general_2d_allocate(raleigh.CDFNon, rows_PDF, cols_PDF, 0);
	}

	for(int i = 0; i < rows_PDF; i++)
	{
		for(int j = 0; j < cols_PDF; j++)
		{
			raleigh.PDFIrr[i][j] = durham.PDFIrr[i][j];
			raleigh.CDFIrr[i][j] = durham.CDFIrr[i][j];
			raleigh.PDFNon[i][j] = durham.PDFNon[i][j];
			raleigh.CDFNon[i][j] = durham.CDFNon[i][j];
		}
	}

	// Set the weekly demand baselines before the main loop (saves time)
	int leapYearCounter = 0, daysInWeek = 7;
	for(int year = 0; year < terminateYear; year++)
	{
		for(int week = 0; week < 52; week++)
		{
			if(week == 51)
			{
				if (leapYearCounter == 2)
					daysInWeek = 9;
				else
					daysInWeek = 8;
			}
			else
				daysInWeek = 7;

			if (week + 1 > 21 && week + 1< 47) // Release requirements for Durham and OWASA change at the same week
			{
				durham.demandBaseline[year][week] =
					durham.UD.averages[week]*daysInWeek*durham.futureDemand[year]
					+ durhamOwasaEvap.averages[week]*1069.0+9.05;

				owasa.demandBaseline[year][week] =
					owasa.UD.averages[week]*daysInWeek*owasa.futureDemand[year]
					+ durhamOwasaEvap.averages[week]*722.0+daysInWeek*1.797;
			}
			else
			{
				durham.demandBaseline[year][week] =
					durham.UD.averages[week]*daysInWeek*durham.futureDemand[year]
					+ durhamOwasaEvap.averages[week]*1069.0+daysInWeek*3.877;
				owasa.demandBaseline[year][week] =
					owasa.UD.averages[week]*daysInWeek*owasa.futureDemand[year]
					+ durhamOwasaEvap.averages[week]*722.0+daysInWeek*1.797;
			}

			if (week + 1 < 13 || week + 1 > 43) // Release requirement schedule for Raleigh is independent of Durham and OWASA (incomplete calculations
			{
				raleigh.demandBaseline[year][week] = raleigh.UD.averages[week]*daysInWeek*(raleigh.futureDemand[year] - .25);
			}
			else
			{
				raleigh.demandBaseline[year][week] = raleigh.UD.averages[week]*daysInWeek*(raleigh.futureDemand[year]-.25);
			}

			cary.demandBaseline[year][week] = cary.UD.averages[week]*daysInWeek*cary.futureDemand[year];
		}

		leapYearCounter += 1;

		if (leapYearCounter == 4)
			leapYearCounter=0;
	}

	return;
}
void Simulation::chooseStreamflows()
{
	//openFile(out1,"raleighDemand.csv");
	zeroes(actualStreamflows, numRealizations);
	int x;
	int counter;
	for (int row = 0; row<numRealizations;row++)
	{
		//counter = 0;
		//while (counter == 0)
		//{
			//x = rand() % numRecords;
			//counter = 1;
			//for(int y = 0; y<row;y++)
			//{
				//if(actualStreamflows[y]==x)
				//{
					//counter = 0;
				//}
			//}

		//}
		//x = streamflowIndex[row][0] - 1;
		x = row;
		for (int col = 0; col < terminateYear*52; col++)
		{
			durhamInflows.synthetic[row][col] 	= michieInflowSYN[x][col]+littleRiverInflowSYN[x][col];
				// if I want to use all realizations, not just the worst 100,
				// change x to row in all of these indicies
			owasaInflows.synthetic[row][col] 	= owasaInflowSYN[x][col];
			fallsInflows.synthetic[row][col]	= fallsLakeInflowSYN[x][col];
			wheelerInflows.synthetic[row][col]	= lakeWBInflowSYN[x][col];
			claytonInflows.synthetic[row][col]	= claytonInflowSYN[x][col];
			crabtreeInflows.synthetic[row][col]	= crabtreeInflowSYN[x][col];
			jordanInflows.synthetic[row][col]	= jordanLakeInflowSYN[x][col];
			lillingtonInflows.synthetic[row][col] = lillingtonGaugeInflowSYN[x][col];
			littleRiverRaleighInflows.synthetic[row][col] = littleRiverRaleighInflowSYN[x][col];

		}
	}
}
void Simulation::correlateDemandVariations(double demand_variation_multiplier)
{
	owasa.generateDemandVariation(52, &owasaInflows, demand_variation_multiplier);
	cary.generateDemandVariation(52, &jordanInflows, demand_variation_multiplier);
	raleigh.generateDemandVariation(52, &durhamInflows, demand_variation_multiplier);
	durham.generateDemandVariation(52, &durhamInflows, demand_variation_multiplier);

}
void Simulation::calculateWaterPrices()
{
	durham.calcWaterPrice(elasticity_Of_Demand);
	cary.calcWaterPrice(elasticity_Of_Demand);
	raleigh.calcWaterPrice(elasticity_Of_Demand);
	owasa.calcWaterPrice(elasticity_Of_Demand);
}
void Simulation::calculateWaterSurcharges()
	// this doesnt do anything 
{
	durham.calcSurchargePrice(elasticity_Of_Demand, 0);
	owasa.calcSurchargePrice(elasticity_Of_Demand, 0);
	cary.calcSurchargePrice(elasticity_Of_Demand, 0);
	raleigh.calcSurchargePrice(elasticity_Of_Demand, 0);
}

void Simulation::generateStreamflows()
{
	int **randomInflowNumber;
	general_2d_allocate(randomInflowNumber, numRealizations*(terminateYear+1), 52, 0);

	for (int x = 0; x<(numRealizations*(terminateYear+1)); x++)
	{
		for (int y = 0; y<52; y++)
		{
			randomInflowNumber[x][y] = rand() % inflowR;
		}
	}

	durhamOwasaEvap.generateSynthetic(randomInflowNumber, terminateYear, numRealizations, false);
	fallsEvap.generateSynthetic(randomInflowNumber, terminateYear, numRealizations, false);
	wheelerEvap.generateSynthetic(randomInflowNumber, terminateYear, numRealizations, false);

	zap(randomInflowNumber, numRealizations*(terminateYear+1));

	return;
}

// MORDM EXTENSION - READ THE RIGHT SYNTHETIC STREAMFLOWS FILE BASED ON RDM FACTORS
void Simulation::fixRDMFactors(int rdm_i)
{
	rdmNumber = rdm_i;
	// cout << "Setting up solution " << solutionNumber << " RDM " << rdmNumber << endl;

	for (int r = 0; r < num_rdm_factors; r++)
	{
		rdm_factors[r] = RDMInput[rdm_i][r];
		// cout << "RDM factors " << rdm_i << " " << r << " " << rdm_factors[r] << endl;
	}

	// usleep(rdm_i * 10000);
	// cout << "Setting up SOW info." << endl;
	// cout << "Reading Hydrologies" << endl;
	
	if (use_RDM_ext)
	{
		//MORDM EXTENSION -  int combination = RDMInput[i] + ...
		
		string hydrology_folder = "./synthetic-inflows/inflow-synthetic/";
			// set where the synthetic inflow files are
		
		string hydrology_number;	
		stringstream hn;
		hn << (int)rdm_factors[0];
		hydrology_number = hn.str();
			// reads the first element of the row of RDM factors used 
			// if david is just using one row, this isnt needed (hence the if statement)
		
		readFile(michieInflowSYN, hydrology_folder + "updatedMichieInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(littleRiverInflowSYN, hydrology_folder + "updatedLittleRiverInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(owasaInflowSYN, hydrology_folder + "updatedOWASAInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(fallsLakeInflowSYN, hydrology_folder + "updatedFallsLakeInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(lakeWBInflowSYN, hydrology_folder + "updatedLakeWBInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(claytonInflowSYN, hydrology_folder + "claytonGageInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(crabtreeInflowSYN, hydrology_folder + "crabtreeCreekInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(jordanLakeInflowSYN, hydrology_folder + "updatedJordanLakeInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(lillingtonGaugeInflowSYN, hydrology_folder + "updatedLillingtonInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
		readFile(littleRiverRaleighInflowSYN, hydrology_folder + "updatedLittleRiverRaleighInflowSYN" + hydrology_number + ".csv", numRecords, 70*52);
	}
	else
	{
		if (runHistoric)
		{
			readFile(michieInflowSYN, directoryName + fakesynthFlowPath + "updatedMichieInflowSYN" + ".csv", numRecords, 70*52);
			readFile(littleRiverInflowSYN, directoryName + fakesynthFlowPath + "updatedLittleRiverInflowSYN" + ".csv", numRecords, 70*52);
			readFile(owasaInflowSYN, directoryName + fakesynthFlowPath + "updatedOWASAInflowSYN" + ".csv", numRecords, 70*52);
			readFile(fallsLakeInflowSYN, directoryName + fakesynthFlowPath + "updatedFallsLakeInflowSYN" + ".csv", numRecords, 70*52);
			readFile(lakeWBInflowSYN, directoryName + fakesynthFlowPath + "updatedLakeWBInflowSYN" + ".csv", numRecords, 70*52);
			readFile(claytonInflowSYN, directoryName + fakesynthFlowPath + "claytonGageInflowSYN" + ".csv", numRecords, 70*52);
			readFile(crabtreeInflowSYN, directoryName + fakesynthFlowPath + "crabtreeCreekInflowSYN" + ".csv", numRecords, 70*52);
			readFile(jordanLakeInflowSYN, directoryName + fakesynthFlowPath + "updatedJordanLakeInflowSYN" + ".csv", numRecords, 70*52);
			readFile(lillingtonGaugeInflowSYN, directoryName + fakesynthFlowPath + "updatedLillingtonInflowSYN" + ".csv", numRecords, 70*52);
			readFile(littleRiverRaleighInflowSYN, directoryName + fakesynthFlowPath + "updatedLittleRiverRaleighInflowSYN" + ".csv", numRecords, 70*52);
		}
		else
		{
			readFile(michieInflowSYN, directoryName + syntheticFlowPath + "updatedMichieInflowSYN" + ".csv", numRecords, 70*52);
			readFile(littleRiverInflowSYN, directoryName + syntheticFlowPath + "updatedLittleRiverInflowSYN" + ".csv", numRecords, 70*52);
			readFile(owasaInflowSYN, directoryName + syntheticFlowPath + "updatedOWASAInflowSYN" + ".csv", numRecords, 70*52);
			readFile(fallsLakeInflowSYN, directoryName + syntheticFlowPath + "updatedFallsLakeInflowSYN" + ".csv", numRecords, 70*52);
			readFile(lakeWBInflowSYN, directoryName + syntheticFlowPath + "updatedLakeWBInflowSYN" + ".csv", numRecords, 70*52);
			readFile(claytonInflowSYN, directoryName + syntheticFlowPath + "claytonGageInflowSYN" + ".csv", numRecords, 70*52);
			readFile(crabtreeInflowSYN, directoryName + syntheticFlowPath + "crabtreeCreekInflowSYN" + ".csv", numRecords, 70*52);
			readFile(jordanLakeInflowSYN, directoryName + syntheticFlowPath + "updatedJordanLakeInflowSYN" + ".csv", numRecords, 70*52);
			readFile(lillingtonGaugeInflowSYN, directoryName + syntheticFlowPath + "updatedLillingtonInflowSYN" + ".csv", numRecords, 70*52);
			readFile(littleRiverRaleighInflowSYN, directoryName + syntheticFlowPath + "updatedLittleRiverRaleighInflowSYN" + ".csv", numRecords, 70*52);
		}
	}
	
	for (int row = 0; row<numRealizations;row++)
	{
		int weekCounter = 0;
		for (int col = 0; col < terminateYear*52; col++)
		{
			durhamInflows.simulatedData[row][col] 	= (log(michieInflowSYN[row][col]+littleRiverInflowSYN[row][col]) - durhamInflows.averages[weekCounter])/durhamInflows.standardDeviations[weekCounter];
			owasaInflows.simulatedData[row][col] 	= (log(owasaInflowSYN[row][col]) - owasaInflows.averages[weekCounter])/owasaInflows.standardDeviations[weekCounter];
			fallsInflows.simulatedData[row][col]	= (log(fallsLakeInflowSYN[row][col]) - fallsInflows.averages[weekCounter])/fallsInflows.standardDeviations[weekCounter];
			wheelerInflows.simulatedData[row][col]	= (log(lakeWBInflowSYN[row][col])-wheelerInflows.averages[weekCounter])/wheelerInflows.standardDeviations[weekCounter];
			claytonInflows.simulatedData[row][col]	= (log(claytonInflowSYN[row][col])-claytonInflows.averages[weekCounter])/claytonInflows.standardDeviations[weekCounter];
			crabtreeInflows.simulatedData[row][col]	= (log(crabtreeInflowSYN[row][col])-crabtreeInflows.averages[weekCounter])/crabtreeInflows.standardDeviations[weekCounter];
			jordanInflows.simulatedData[row][col]	= (log(jordanLakeInflowSYN[row][col])-jordanInflows.averages[weekCounter])/jordanInflows.standardDeviations[weekCounter];
			lillingtonInflows.simulatedData[row][col] = (log(lillingtonGaugeInflowSYN[row][col])-lillingtonInflows.averages[weekCounter])/lillingtonInflows.standardDeviations[weekCounter];
			littleRiverRaleighInflows.simulatedData[row][col] = (log(littleRiverRaleighInflowSYN[row][col])-littleRiverRaleighInflows.averages[weekCounter])/littleRiverRaleighInflows.standardDeviations[weekCounter];
			weekCounter = weekCounter + 1;
			if(weekCounter == 52)
			{
				weekCounter = 0;
			}
		}
	}


	// cout << "Future demand multiplier" << endl;
	//this section allows for use of historic record of variable length
	for (int x = 0; x<numFutureYears; x++)
	{
		// MORDM EXTENSION - REPLACE future_demand_multiplier WITH THE RIGHT MULTIPLIERS FROM RDMInput
		// Scaling a linear projection -- multiply the slope and subtract off changes to the y-intercept
		cary.futureDemand[x] = caryFutureD[0][x]*rdm_factors[1] - caryFutureD[0][0]*(rdm_factors[1]-1);
		raleigh.futureDemand[x] = raleighFutureD[0][x]*rdm_factors[2] - raleighFutureD[0][0]*(rdm_factors[2]-1);
		durham.futureDemand[x] = durhamFutureD[0][x]*rdm_factors[3] - durhamFutureD[0][0]*(rdm_factors[3]-1);
		owasa.futureDemand[x] = owasaFutureD[0][x]*rdm_factors[4] - owasaFutureD[0][0]*(rdm_factors[4]-1);
			// default RDM factors here should be 1
	}


	// cout << "Applying demand restriction multiplier" << endl;
	// Consumer reductions factor (0/1 is irrigation/non, and 0-4 is the restriction stage
	// MORDM EXTENSION - ADD CONSERVATION POTENTIAL FACTORS HERE
	
	durham.individualReductions[0][0] = 1;
	durham.individualReductions[0][1] = .85 * rdm_factors[5];
	durham.individualReductions[0][2] = .7 * rdm_factors[5];
	durham.individualReductions[0][3] = .6 * rdm_factors[5];
	durham.individualReductions[0][4] = .45 * rdm_factors[5];
	// ASK IF THESE NON-IRRIGATION REDUCTIONS ARE BEING USED
	durham.individualReductions[1][0] = 1;
	durham.individualReductions[1][1] = .93 * rdm_factors[5];
	durham.individualReductions[1][2] = .88 * rdm_factors[5];
	durham.individualReductions[1][3] = .83 * rdm_factors[5];
	durham.individualReductions[1][4] = .7 * rdm_factors[5];

	owasa.individualReductions[0][0] = 1;
	owasa.individualReductions[0][1] = .9 * rdm_factors[6];
	owasa.individualReductions[0][2] = .85 * rdm_factors[6];
	owasa.individualReductions[0][3] = .8 * rdm_factors[6];

	raleigh.individualReductions[0][0] = 1;
	raleigh.individualReductions[0][1] = .952 * rdm_factors[7];
	raleigh.individualReductions[0][2] = .91 * rdm_factors[7];
	raleigh.individualReductions[0][3] = .724 * rdm_factors[7];
	raleigh.individualReductions[0][4] = .634 * rdm_factors[7];

	//Cary reductions by stage
	cary.individualReductions[0][0] = 1;
	cary.individualReductions[0][1] = .87 * rdm_factors[8];
	cary.individualReductions[0][2] = .68 * rdm_factors[8];
	cary.individualReductions[0][3] = .62 * rdm_factors[8];
	cary.individualReductions[0][4] = .54 * rdm_factors[8];
	cary.individualReductions[1][0] = 1;
	cary.individualReductions[1][1] = .94 * rdm_factors[8];
	cary.individualReductions[1][2] = .9 * rdm_factors[8];
	cary.individualReductions[1][3] = .83 * rdm_factors[8];
	cary.individualReductions[1][4] = .71 * rdm_factors[8];
	
		// set all these RDM factors to 1 if default
	
	// cout << "Capacities multiplier" << endl;
	//MORDM EXTENSION - add allocation, and capacities multipliers	
	falls_lake_supply_capacity = 14700.0*rdm_factors[9];
	falls_lake_wq_capacity = (14700.0 - falls_lake_supply_capacity) + 20000.0;
	jordan_lake_supply_capacity = 14924.0*rdm_factors[10];
	jordan_lake_wq_capacity = (14924.0 - jordan_lake_supply_capacity) + 30825.0;
	cary_treatment_capacity = 40.0*rdm_factors[11];
	durham_cary_capacity = 10.0*rdm_factors[12];
	durham_owasa_capacity = 7.0*rdm_factors[12];
	raleigh_cary_capacity = 10.8*rdm_factors[12];
		// set all RDM factors to 1 for default 
	
	// FALLS LAKE CONSERVATION POOL ALLOCATIONS
	// store the fraction of falls lake conservation pool used for WQ and for supply
	
	FLSPfrac = falls_lake_supply_capacity/(falls_lake_supply_capacity + falls_lake_wq_capacity);
		// fraction of FL conservation pool used for raleigh water supply 

	// cout << "Financial info" << endl;
	//MORDM EXTENSION - add bond rate and length
	bondRate = rdm_factors[13];
		// default is 0.04
	bondLength = rdm_factors[14];
		// default is 20 years 

	// cout << "Setting permiting times" << endl;
	//MORDM EXTENSION - permiting periods
	owasa.infMatrix[0][2] = rdm_factors[15];//17;
		// permitting period, cant be built until 17th year
	owasa.infMatrix[1][2] = rdm_factors[16];
	
	owasa.infMatrix[2][2] = rdm_factors[17];
	owasa.infMatrix[3][2] = rdm_factors[18];
	durham.infMatrix[0][2] = rdm_factors[19];
	durham.infMatrix[1][2] = rdm_factors[20];
	durham.infMatrix[2][2] = rdm_factors[21];
	durham.infMatrix[3][2] = rdm_factors[22];
	durham.infMatrix[4][2] = rdm_factors[23];
	raleigh.infMatrix[0][2] = rdm_factors[24];
	raleigh.infMatrix[1][2] = rdm_factors[25];
	raleigh.infMatrix[2][2] = rdm_factors[26];
	raleigh.infMatrix[3][2] = rdm_factors[27];

	// for these last 6, the 12s and 27s must be the same.
	owasa.infMatrix[4][2] = rdm_factors[28]; // 12
	owasa.infMatrix[5][2] = rdm_factors[29]; // 27 
	durham.infMatrix[5][2] = rdm_factors[28];
	durham.infMatrix[6][2] = rdm_factors[29];
	raleigh.infMatrix[4][2] = rdm_factors[28];
	raleigh.infMatrix[5][2] = rdm_factors[29];
}

double Simulation::calculation(double *c_xreal, double *c_obj, double *c_constr)
{

	time_t start = time(0);
	// Decision variable
	if (borgToggle ==3)
        // doesnt happen if using the MOEA
        // MAKE SURE IM USING 3 WHEN LOOKING AT not the MOEA
	{
		cout << "Setting up deccision vars sol. " << solutionNumber << endl;
		for (int i = 0; i < num_dec; i++)
		{
			cout << "Dec var " << solutionNumber << " ";
			cout << i << " " << xreal[i] << endl;
			xreal[i] = parameterInput[solutionNumber][i];

                // solutionNumber is defined in triangleSimulation.cpp
		}
		//xreal[0] = 0.02;
		//xreal[1] = 0.02;
		//xreal[2] = 0.02;
		//xreal[3] = 0.02;
		//xreal[4] = 0.02;
		//xreal[5] = 0.02;
		//xreal[6] = 0.02;
		//xreal[7] = 0.1;
		//xreal[8] = 0.1;
		//xreal[9] = 0.1;
		//xreal[10] = 0.5;
		//xreal[11] = 0.00;
		//xreal[12] = 0.00;
		//xreal[13] = 0.00;
		//xreal[14] = 0.00;
		//xreal[15] = 0.1;
		//xreal[16] = 0.1;
		//xreal[17] = 0.1;
		//xreal[18] = 0.1;
		//xreal[19] = .0;
		//xreal[20] = .0;
		//xreal[21] = .0;
		//xreal[22] = .0;
		//xreal[23] = .02;
		//xreal[24] = .02;
		//xreal[25] = .02;
		//xreal[26] = .02;
		//xreal[27] = .01;
		//xreal[28] = .02;
		//xreal[29] = .03;
		//xreal[30] = .04;
		//xreal[31] = .05;
		//xreal[32] = .06;
		//xreal[33] = .07;
		//xreal[34] = .08;
		//xreal[35] = .09;
		//xreal[36] = .1;
		//xreal[37] = .11;
		//xreal[38] = .12;
		//xreal[39] = .13;
		//xreal[40] = .14;
		//xreal[41] = .15;
		//xreal[42] = .16;
		//xreal[43] = .17;
		//xreal[44] = .18;
		//xreal[45] = .19;
		//xreal[46] = 25.0;
		//xreal[47] = 30.0;
		//xreal[48] = 35.0;
		//xreal[49] = .25;
		//xreal[50] = .25;
		//xreal[51] = .25;
		//xreal[52] = 2000.0;
	}
	else
	{
		for (int i = 0; i < num_dec; i++)
		{
			xreal[i] = c_xreal[i];
		}
	}

	// cout << "calculating water prices and surcharges sol " << solutionNumber << endl;
	calculateWaterPrices();
	calculateWaterSurcharges();
		// unnecessary now
		
	cout << "Choosing streamflows sol " << solutionNumber << endl;
	chooseStreamflows();
		// right now, picks 100 worst years
		
	// cout << "Setting up triggers, restrictions, and other data." << endl;
	durham.insurancePremium = 1.2;
	cary.insurancePremium = 1.2;
	raleigh.insurancePremium = 1.2;
	owasa.insurancePremium = 1.2;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 5; j++) {
			if(durham.individualReductions[i][j] > 1) durham.individualReductions[i][j] = 1;
			if(cary.individualReductions[i][j] > 1) cary.individualReductions[i][j] = 1;

			if(i == 0 && raleigh.individualReductions[i][j] > 1) raleigh.individualReductions[i][j] = 1;
			if(i == 0 && j < 4 && owasa.individualReductions[i][j] > 1) owasa.individualReductions[i][j] = 1;
		}
	}
		// nothing?

	//Water use restrictions:  Durham and Raleigh have 4 stages, OWASA has three
	// 			Two decision variables per utility:  storage levels (Durham and Raleigh) or risk of failure (OWASA) which causes stage 1 restrictions
	//												 1 trigger for May-October (irrigation season), 1 trigger from November-April (non-irrigation season)
	//			Other stages of restrictions are triggered based on their relative value compared to stage 1 restrictions as currently practiced (stage 2 begins when storage
	//			is drawn down 20% farther than stage 1, etc).
	//			Triggers to turn off restricitons are different than the triggers to turn them on, and they also are based on their relative value compared to the triggers activating
	// 			restrictions
	durham.restrictionsOnTemplate[0][0] = xreal[0];//Durham stage 1 restriction trigger, irrigation season
	durham.restrictionsOnTemplate[0][1] = xreal[0] - .2;//Durham stage 2 restriction trigger, irrigation season
	durham.restrictionsOnTemplate[0][2] = xreal[0] - .35;//Durham stage 3 restriction trigger, irrigation season
	durham.restrictionsOnTemplate[0][3] = xreal[0] - .45;//Durham stage 4 restriction trigger, irrigation season
	durham.restrictionsOnTemplate[0][4] = -1;
	durham.restrictionsOnTemplate[1][0] = xreal[1];//Durham stage 1 restriction trigger, non-irrigation season
	durham.restrictionsOnTemplate[1][1] = xreal[1] - .05;//Durham stage 2 restriction trigger, non-irrigation season
	durham.restrictionsOnTemplate[1][2] = xreal[1] - .1;//Durham stage 3 restriction trigger, non-irrigation season
	durham.restrictionsOnTemplate[1][3] = xreal[1] - .2;//Durham stage 1 restriction trigger, non-irrigation season
	durham.restrictionsOnTemplate[1][4] = -1;
	owasa.restrictionsOnTemplate[0][0] = xreal[2];//OWASA stage 1 restriction trigger (ROF), irrigation season
	owasa.restrictionsOnTemplate[0][1] = xreal[2] - .2;//OWASA stage 2 restriction trigger (ROF), irrigation season
	owasa.restrictionsOnTemplate[0][2] = xreal[2] - .35;//OWASA stage 3 restriction trigger (ROF), irrigation season
	owasa.restrictionsOnTemplate[0][3] = -1;
	owasa.restrictionsOnTemplate[1][0] = xreal[3];//OWASA stage 1 restriction trigger (ROF), non-irrigation season
	owasa.restrictionsOnTemplate[1][1] = xreal[3] - .05;//OWASA stage 2 restriction trigger (ROF), non-irrigation season
	owasa.restrictionsOnTemplate[1][2] = xreal[3] - .1;//OWASA stage 3 restriction trigger (ROF), non-irrigation season
	owasa.restrictionsOnTemplate[1][3] = -1;
	raleigh.restrictionsOnTemplate[0][0] = xreal[3];//Raleigh stage 1 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOnTemplate[0][1] = xreal[3] - .2;//Raleigh stage 2 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOnTemplate[0][2] = xreal[3] - .35;//Raleigh stage 3 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOnTemplate[0][3] = xreal[3] - .45;//Raleigh stage 4 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOnTemplate[0][4] = -1;
	raleigh.restrictionsOnTemplate[1][0] = xreal[4];//Raleigh stage 1 restriction trigger (ROF), non-irrigation season
	raleigh.restrictionsOnTemplate[1][1] = xreal[4] - .05;//Raleigh stage 1 restriction trigger (ROF), non-irrigation season
	raleigh.restrictionsOnTemplate[1][2] = xreal[4] - .1;//Raleigh stage 1 restriction trigger (ROF), non-irrigation season
	raleigh.restrictionsOnTemplate[1][3] = xreal[4] - 0.2;//Raleigh stage 1 restriction trigger (ROF), non-irrigation season
	raleigh.restrictionsOnTemplate[1][4] = -1;
	cary.restrictionsOnTemplate[0][0] = xreal[5];//Cary stage 1 restriction trigger, irrigation season
	cary.restrictionsOnTemplate[0][1] = xreal[5] - .2;//Cary stage 2 restriction trigger, irrigation season
	cary.restrictionsOnTemplate[0][2] = xreal[5] - .35;//Cary stage 3 restriction trigger, irrigation season
	cary.restrictionsOnTemplate[0][3] = xreal[5] - .45;//Cary stage 4 restriction trigger, irrigation season
	cary.restrictionsOnTemplate[0][4] = -1;
	cary.restrictionsOnTemplate[1][0] = xreal[6];//Cary stage 1 restriction trigger, non-irrigation season
	cary.restrictionsOnTemplate[1][1] = xreal[6] - .05;//Cary stage 2 restriction trigger, non-irrigation season
	cary.restrictionsOnTemplate[1][2] = xreal[6] - .1;//Cary stage 3 restriction trigger, non-irrigation season
	cary.restrictionsOnTemplate[1][3] = xreal[6] - .2;//Cary stage 1 restriction trigger, non-irrigation season
	cary.restrictionsOnTemplate[1][4] = -1;
        // NONE OF THESE USED ANYMORE

	durham.restrictionsOnTemplateROF[0] = xreal[0];//Durham stage 1 restriction trigger, irrigation season
	durham.restrictionsOnTemplateROF[1] = xreal[0] +.15;//Durham stage 2 restriction trigger, irrigation season
	durham.restrictionsOnTemplateROF[2] = xreal[0] +.35;//Durham stage 3 restriction trigger, irrigation season
	durham.restrictionsOnTemplateROF[3] = xreal[0] +.6;//Durham stage 4 restriction trigger, irrigation season
	durham.restrictionsOnTemplateROF[4] = 1.1;
	owasa.restrictionsOnTemplateROF[0] = xreal[1];//OWASA stage 1 restriction trigger (ROF), irrigation season
	owasa.restrictionsOnTemplateROF[1] = xreal[1] +.15;//OWASA stage 2 restriction trigger (ROF), irrigation season
	owasa.restrictionsOnTemplateROF[2] = xreal[1] +.35;//OWASA stage 3 restriction trigger (ROF), irrigation season
	owasa.restrictionsOnTemplateROF[3] = 1.1;
	raleigh.restrictionsOnTemplateROF[0] = xreal[2];//Raleigh stage 1 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOnTemplateROF[1] = xreal[2] +.15;//Raleigh stage 2 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOnTemplateROF[2] = xreal[2] +.35;//Raleigh stage 3 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOnTemplateROF[3] = xreal[2] +.6;//Raleigh stage 4 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOnTemplateROF[4] = 1.1;
	cary.restrictionsOnTemplateROF[0] = xreal[3];//Cary stage 1 restriction trigger, irrigation season
	cary.restrictionsOnTemplateROF[1] = xreal[3] +.15;//Cary stage 2 restriction trigger, irrigation season
	cary.restrictionsOnTemplateROF[2] = xreal[3] +.35;//Cary stage 3 restriction trigger, irrigation season
	cary.restrictionsOnTemplateROF[3] = xreal[3] +.6;//Cary stage 4 restriction trigger, irrigation season
	cary.restrictionsOnTemplateROF[4] = 1.1;
        // CHECK OUT CURRENT RESTRICTION PLANNING
        // TO SEE ABOUT THESE ASSUMPTIONS


	for (int x = 0; x < 2;x++)//Make sure that triggers do not take values outside of [0, 1] (relationships are additive)
	{
		for(int y = 0; y < 4; y++)
		{
			if(durham.restrictionsOnTemplate[x][y] < 0)
				durham.restrictionsOnTemplate[x][y] = 0;
			if(cary.restrictionsOnTemplate[x][y] < 0)
				cary.restrictionsOnTemplate[x][y] = 0;
			if(raleigh.restrictionsOnTemplate[x][y] < 0)
				raleigh.restrictionsOnTemplate[x][y] = 0;
		}
		for(int y = 0; y < 3; y++)
		{
			if(owasa.restrictionsOnTemplate[x][y] < 0)
				owasa.restrictionsOnTemplate[x][y] = 0;
		}
	}
	for(int y = 0; y < 4; y++)
	{
		if(durham.restrictionsOnTemplateROF[y] > 1)
			durham.restrictionsOnTemplateROF[y] = 1;
		if(cary.restrictionsOnTemplateROF[y] > 1)
			cary.restrictionsOnTemplateROF[y] = 1;
		if(raleigh.restrictionsOnTemplateROF[y] > 1)
			raleigh.restrictionsOnTemplateROF[y] = 1;
	}
	for(int y = 0; y < 3; y++)
	{
		if(owasa.restrictionsOnTemplateROF[y] > 1)
			owasa.restrictionsOnTemplateROF[y] = 1;
	}
	durham.restrictionsOffTemplate[0][1] = .95;//Durham stage 1 restriction trigger off, irrigation season
	durham.restrictionsOffTemplate[0][2] = durham.restrictionsOnTemplate[0][2]+.05;//Durham stage 2 restriction trigger off, irrigation season
	durham.restrictionsOffTemplate[0][3] = durham.restrictionsOnTemplate[0][3]+.05;//Durham stage 3 restriction trigger off, irrigation season
	durham.restrictionsOffTemplate[0][4] = durham.restrictionsOnTemplate[0][4]+.1;//Durham stage 4 restriction trigger off, irrigation season
	durham.restrictionsOffTemplate[0][0] = 1.1;
	durham.restrictionsOffTemplate[1][1] = .95;//Durham stage 1 restriction trigger off, non-irrigation season
	durham.restrictionsOffTemplate[1][2] = durham.restrictionsOnTemplate[1][2]+.2;//Durham stage 2 restriction trigger off, non-irrigation season
	durham.restrictionsOffTemplate[1][3] = durham.restrictionsOnTemplate[1][3]+.15;//Durham stage 3 restriction trigger off, non-irrigation season
	durham.restrictionsOffTemplate[1][4] = durham.restrictionsOnTemplate[1][4]+.1;//Durham stage 4 restriction trigger off, non-irrigation season
	durham.restrictionsOffTemplate[1][0] = 1.1;
	owasa.restrictionsOffTemplate[0][1] = .95;//OWASA stage 1 restriction trigger off, irrigation season
	owasa.restrictionsOffTemplate[0][2] = owasa.restrictionsOnTemplate[0][2]+.05;//OWASA stage 2 restriction trigger off, irrigation season
	owasa.restrictionsOffTemplate[0][3] = owasa.restrictionsOnTemplate[0][3]+.05;//OWASA stage 3 restriction trigger off, irrigation season
	owasa.restrictionsOffTemplate[0][0] = 1.1;
	owasa.restrictionsOffTemplate[1][1] = .95;//OWASA stage 1 restriction trigger off, non-irrigation season
	owasa.restrictionsOffTemplate[1][2] = owasa.restrictionsOnTemplate[1][2]+.05;//OWASA stage 2 restriction trigger off, non-irrigation season
	owasa.restrictionsOffTemplate[1][3] = owasa.restrictionsOnTemplate[1][3]+.05;//OWASA stage 3 restriction trigger off, non-irrigation season
	owasa.restrictionsOffTemplate[1][0] = 1.1;
	raleigh.restrictionsOffTemplate[0][1] = .95;//Raleigh stage 1 restriction trigger off, irrigation season
	raleigh.restrictionsOffTemplate[0][2] = raleigh.restrictionsOnTemplate[0][1]+.05;//Raleigh stage 2 restriction trigger off, irrigation season
	raleigh.restrictionsOffTemplate[0][3] = raleigh.restrictionsOnTemplate[0][2]+.05;
	raleigh.restrictionsOffTemplate[0][4] = raleigh.restrictionsOnTemplate[0][3]+.1;
	raleigh.restrictionsOffTemplate[0][0] = 1.1;
	raleigh.restrictionsOffTemplate[1][1] = 95;//Raleigh stage 1 restriction trigger off, non-irrigation season
	raleigh.restrictionsOffTemplate[1][2] = raleigh.restrictionsOnTemplate[1][1]+.2;//Raleigh stage 2 restriction trigger off, non-irrigation season
	raleigh.restrictionsOffTemplate[1][3] = raleigh.restrictionsOnTemplate[1][2]+.15;//Raleigh stage 3 restriction trigger off, non-irrigation season
	raleigh.restrictionsOffTemplate[1][4] = raleigh.restrictionsOnTemplate[1][3]+.1;//Raleigh stage 4 restriction trigger off, non-irrigation season
	raleigh.restrictionsOffTemplate[1][0] = 1.1;
	cary.restrictionsOffTemplate[0][1] = .95;//Cary stage 1 restriction trigger off, irrigation season
	cary.restrictionsOffTemplate[0][2] = cary.restrictionsOnTemplate[0][2]+.05;//Cary stage 2 restriction trigger off, irrigation season
	cary.restrictionsOffTemplate[0][3] = cary.restrictionsOnTemplate[0][3]+.05;//Cary stage 3 restriction trigger off, irrigation season
	cary.restrictionsOffTemplate[0][4] = cary.restrictionsOnTemplate[0][4]+.1;//Cary stage 4 restriction trigger off, irrigation season
	cary.restrictionsOffTemplate[0][0] = 1.1;
	cary.restrictionsOffTemplate[1][1] = .95;//Cary stage 1 restriction trigger off, non-irrigation season
	cary.restrictionsOffTemplate[1][2] = cary.restrictionsOnTemplate[1][2]+.2;//Cary stage 2 restriction trigger off, non-irrigation season
	cary.restrictionsOffTemplate[1][3] = cary.restrictionsOnTemplate[1][3]+.15;//Cary stage 3 restriction trigger off, non-irrigation season
	cary.restrictionsOffTemplate[1][4] = cary.restrictionsOnTemplate[1][4]+.1;//Cary stage 4 restriction trigger off, non-irrigation season
	cary.restrictionsOffTemplate[1][0] = 1.1;
        // Not used

	durham.restrictionsOffTemplateROF[1] = .02;//Durham stage 1 restriction trigger, irrigation season
	durham.restrictionsOffTemplateROF[2] = durham.restrictionsOnTemplateROF[0];//Durham stage 2 restriction trigger, irrigation season
	durham.restrictionsOffTemplateROF[3] = durham.restrictionsOnTemplateROF[1];//Durham stage 3 restriction trigger, irrigation season
	durham.restrictionsOffTemplateROF[4] = durham.restrictionsOnTemplateROF[2];//Durham stage 4 restriction trigger, irrigation season
	durham.restrictionsOffTemplateROF[0] = -1;
	owasa.restrictionsOffTemplateROF[1] = .02;//OWASA stage 1 restriction trigger (ROF), irrigation season
	owasa.restrictionsOffTemplateROF[2] = owasa.restrictionsOnTemplateROF[0];//OWASA stage 2 restriction trigger (ROF), irrigation season
	owasa.restrictionsOffTemplateROF[3] = owasa.restrictionsOnTemplateROF[1];//OWASA stage 3 restriction trigger (ROF), irrigation season
	owasa.restrictionsOffTemplateROF[0] = -1;
	raleigh.restrictionsOffTemplateROF[1] = .02;//Raleigh stage 1 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOffTemplateROF[2] = raleigh.restrictionsOnTemplateROF[0];//Raleigh stage 2 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOffTemplateROF[3] = raleigh.restrictionsOnTemplateROF[1];//Raleigh stage 3 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOffTemplateROF[4] = raleigh.restrictionsOffTemplateROF[2];//Raleigh stage 4 restriction trigger (ROF), irrigation season
	raleigh.restrictionsOffTemplateROF[0] = -1;
	cary.restrictionsOffTemplateROF[1] = .02;//Cary stage 1 restriction trigger, irrigation season
	cary.restrictionsOffTemplateROF[2] = cary.restrictionsOnTemplateROF[0];//Cary stage 2 restriction trigger, irrigation season
	cary.restrictionsOffTemplateROF[3] = cary.restrictionsOnTemplateROF[1];//Cary stage 3 restriction trigger, irrigation season
	cary.restrictionsOffTemplateROF[4] = cary.restrictionsOnTemplateROF[2];//Cary stage 4 restriction trigger, irrigation season
	cary.restrictionsOffTemplateROF[0] = -1;


	for (int x = 0; x < 2;x++)//Make sure that triggers do not take out-of-range values (relationships are additive)
	{
		for(int y = 1; y < 5; y++)
		{
			if(durham.restrictionsOffTemplate[x][y] > 1)
				durham.restrictionsOffTemplate[x][y] = 1;
			if(cary.restrictionsOffTemplate[x][y] > 1)
				cary.restrictionsOffTemplate[x][y] = 1;
			if(raleigh.restrictionsOffTemplate[x][y] > 1)
				raleigh.restrictionsOffTemplate[x][y] = 1;
			if(y < 4 && owasa.restrictionsOffTemplate[x][y] > 1)
				owasa.restrictionsOffTemplate[x][y] = 1;
		}
	}
	for(int y = 1; y < 5; y++)
	{
		if(durham.restrictionsOffTemplateROF[y] < 0)
			durham.restrictionsOffTemplateROF[y] = 0;
		if(cary.restrictionsOffTemplateROF[y] < 0)
			cary.restrictionsOffTemplateROF[y] = 0;
		if(raleigh.restrictionsOffTemplateROF[y] < 0)
			raleigh.restrictionsOffTemplateROF[y] = 0;
		if(y < 4 && owasa.restrictionsOffTemplateROF[y] < 0)
			owasa.restrictionsOffTemplateROF[y] = 0;
	}

	//Water transfer triggers - Risk of failure, one value per utility
	if (formulation == 0)//No transfers, triggers are set to 200% ROF
	{
		durham.TTriggerN = 2;
		durham.TTriggerI = 2;
		owasa.TTriggerN = 2;
		owasa.TTriggerI = 2;
		raleigh.TTriggerI = 2;
		raleigh.TTriggerN = 2;
		
		raleigh.RRtrigger = 2;
		durham.RRtrigger  = 2;
			// raw release triggers
		
		durham.jordanLakeAlloc = 0;
		raleigh.jordanLakeAlloc = 0;
		owasa.jordanLakeAlloc = 0;
		cary.jordanLakeAlloc = xreal[10];
	}
	else // TRANSFERS ALLOWED, TRANSFER TRIGGERS BASED ON OPTIMIZED VARIABLES
		 // for now, i will use the treated water triggers for the raw releases also	
		 // in the future, once i am confident with the optimization, should develop separate triggers
	{
		durham.TTriggerN = xreal[4];
		durham.TTriggerI = xreal[4];
			// TTtriggerI variables arent used 
			
		owasa.TTriggerN = xreal[5];
		owasa.TTriggerI = xreal[5];
		raleigh.TTriggerI = xreal[6];
		raleigh.TTriggerN = xreal[6];
		
		durham.RRtrigger  = xreal[58];
		raleigh.RRtrigger = xreal[57];
			// raw release triggers
			// for now, same as treated transfer triggers
			// should be their own variables later

		if((xreal[7]+xreal[8]+xreal[9]+xreal[10]) > availableJLallocation)
			// JL allocations for all 4 utilities
			// May 2016: based on allocations to Chatham County
			// these must be limited to 69% of JL available
			
			// SHOULD ALL OF THESE IF STATEMENTS BE REDONE AND UN-NESTED???
			
		{
			raleigh.jordanLakeAlloc = availableJLallocation - xreal[7] - xreal[9] - xreal[10];
				// based on Durham, OWASA, Cary allocations.
				// if JL is overallocated, Raleigh allocation is cut

			if(raleigh.jordanLakeAlloc < 0.0)
			{
				raleigh.jordanLakeAlloc = 0.0;
				owasa.jordanLakeAlloc = availableJLallocation - xreal[9] - xreal[10];
					// OWASA allocation is the next limited factor
					// based on Durham and Cary allocations
					// if by reducing Raleigh's allocation the total
					// allocation is still above the available quantity to allocate.
					
				if(owasa.jordanLakeAlloc < 0.05)
				{
					owasa.jordanLakeAlloc = 0.05;
						// OWASA allocation has to be at least 5% though
						
					durham.jordanLakeAlloc = availableJLallocation - xreal[10] - 0.05;
						// if Raleigh allocation goes to 0 and OWASA's is minimized,
						// durham's allocation is next to be curtailed
						
					if(durham.jordanLakeAlloc < 0.10)
					{
						durham.jordanLakeAlloc = 0.10;
							// but, Durham has to have at least a 10% stake in JL,
							// so if the allocations can't be set properly,
							// and Raleigh, OWASA, and Durham allocations are
							// minimized, the final allocations are
							// Cary = availableJLallocation - durham - raleigh - owasa
							
						cary.jordanLakeAlloc = availableJLallocation - durham.jordanLakeAlloc - owasa.jordanLakeAlloc - raleigh.jordanLakeAlloc;
						//cary.jordanLakeAlloc = 0.85;
							// in this scenario, D allocation is 10%, OWASA is 5%, R is 0%
					}
					else
					{
						cary.jordanLakeAlloc = xreal[10];
							// if, after adjusting D, R and OWASA allocations,
							// everything is kosher, then Cary allocation 
							// doesnt need to be adjusted
					}
				}
				else
				{
					durham.jordanLakeAlloc = xreal[9];
					cary.jordanLakeAlloc = xreal[10];
						// if everything is under the limit
						// after R and OWASA are adjusted,
						// D and Cary allocations are as inputted.
				}
			}
			else
			{
				owasa.jordanLakeAlloc = xreal[7];
				durham.jordanLakeAlloc = xreal[9];
				cary.jordanLakeAlloc = xreal[10];
			}
		}
		else
		{
			owasa.jordanLakeAlloc = xreal[7];
			raleigh.jordanLakeAlloc = xreal[8];
			durham.jordanLakeAlloc = xreal[9];
			cary.jordanLakeAlloc = xreal[10];
				// should xreal inputs for owasa and durham be switched?
				// May 2016: durham, originally xreal[7] is changed to xreal[9]
				// to match the other parts of the loop, and 
				// OWASA is changed from xreal[9] to xreal[7] for the same reason
		}
	}

	//Mitigation variables
	//Annual payment - size of the annual contribution to a contingency fund, as a percentage of expected total annual volumetric revenue
	durham.annualPayment = xreal[11];
	owasa.annualPayment = xreal[12];
	raleigh.annualPayment = xreal[13];
	cary.annualPayment = xreal[14];

	// what ROF should trigger insurance, and how much is paid in triggered weeks (% of AVR)
	durham.insuranceUse = xreal[15];
	owasa.insuranceUse = xreal[16];
	raleigh.insuranceUse = xreal[17];
	cary.insuranceUse = xreal[18];

	durham.insurancePayment = xreal[19];
	owasa.insurancePayment = xreal[20];
	raleigh.insurancePayment = xreal[21];
	cary.insurancePayment = xreal[22];


	//Infrastructure triggers - what forward-risk-of-failure threshold is used to trigger new infrastructure?

	durham.infTrigger = xreal[23];
	owasa.infTrigger = xreal[24];
	raleigh.infTrigger = xreal[25];
	cary.infTrigger = xreal[26];

	////Infrastructure rankings - what order should new infrastructure be built once it is triggered?
	///OWASA infrastructure
	owasa.infMatrix[0][0] = xreal[27];////University Lake expansion
		// ranking values for when to buildC
		// between 0 and 1 (higher = build first)
		// once built, this number is changed to 1
		
		
	owasa.infMatrix[0][4] = 107.0;
		// total cost, $MM
		
	owasa.infMatrix[0][5] = 3 + rand() % 3;
		// construction time
		
	// index = 1 toggle to begin construction
	// index = 3 means counter of years of construction
		
	owasa.infMatrix[1][0] = xreal[28];////Cane Creek expansion
		// MAY 2016: changed so that this will never be built
		// (on consult of OWASA, model is saying that after 100 years
		//  it is ok to build, which means it will never happen)
		// was originally 17 permitting years 

	// MORDM EXTENTION - REPLACE x in infMatrix[n][2] BY THE PERMITING VALUES. 15 TOTAL BECAUSE THE ONES FOR SHARED INFRACTRUCTURE (LINES 1262, 1271, 1280 AND SAME PLUS 3) MUST BE THE SAME 
	owasa.infMatrix[1][4] = 127.0;
	owasa.infMatrix[1][5] = 3 + rand() % 3;
	owasa.infMatrix[2][0] = xreal[29];////Quarry reservoir expansion (shallow)
	owasa.infMatrix[2][4] = 1.4;
	owasa.infMatrix[2][5] = 3 + rand() % 3;
	owasa.infMatrix[3][0] = xreal[30];////Quarry reservoir expansion (deep)
	owasa.infMatrix[3][4] = 1.4;
	owasa.infMatrix[3][5] = 3 + rand() % 3;
	///Durham infrastructure
	durham.infMatrix[0][0] = xreal[31];////Teer quarry expansion
	durham.infMatrix[0][4] = 22.6;
	durham.infMatrix[0][5] = 3 + rand() % 3;
	durham.infMatrix[1][0] = xreal[32];////Reclaimed water (low)
	durham.infMatrix[1][4] = 27.5;
	durham.infMatrix[1][5] = 3 + rand() % 3;
	durham.infMatrix[2][0] = xreal[33];////Reclaimed water (high)
	durham.infMatrix[2][4] = 104.4;
	durham.infMatrix[2][5] = 3 + rand() % 3;
	durham.infMatrix[3][0] = xreal[34];////Lake Michie expansion (low)
	durham.infMatrix[3][4] = 158.3;
	durham.infMatrix[3][5] = 3 + rand() % 3;
	durham.infMatrix[4][0] = xreal[35];////Lake Michie expansion (high)
	durham.infMatrix[4][4] = 203.3;
	durham.infMatrix[4][5] = 3 + rand() % 3;
	///Raleigh infrastructure
	raleigh.infMatrix[0][0] = xreal[37];////Little River Reservoir
	raleigh.infMatrix[0][4] = 263.0;
	raleigh.infMatrix[0][5] = 3 + rand() % 3;
	raleigh.infMatrix[1][0] = xreal[37];////Richland Creek Quarry
	raleigh.infMatrix[1][4] = 265.0;
	raleigh.infMatrix[1][5] = 3 + rand() % 3;
	raleigh.infMatrix[2][0] = xreal[38];////Neuse River Intake
	raleigh.infMatrix[2][4] = 225.5;
	raleigh.infMatrix[2][5] = 3 + rand() % 3;
	raleigh.infMatrix[3][0] = xreal[39];////Reallocate Falls Lake


		// May 2016: changed to 100 years, so it can't be done as a 
		// experiment to see what happens if durham expands while raleigh
		// doesnt.
		// originally 12 years permitting time
		// put back down at 12 years for some new results 
	raleigh.infMatrix[3][4] = 68.2;
	raleigh.infMatrix[3][5] = 3 + rand() % 3;
	if(formulation == 2)
	{
	/////All utilities have the potential to trigger the Western Wake Treatment plant
			// BY WESTERN WAKE, WE ARE REFERRING TO
			// NEW WTP ON THE WESTERN SIDE OF JORDAN LAKE????
		
		///OWASA infrastructure
		owasa.infMatrix[4][0] = xreal[40];////Western Wake Treatment plant
		owasa.infMatrix[4][4] = 243.3*xreal[49];
		owasa.infMatrix[4][5] = 3 + rand() % 3;
		owasa.infMatrix[5][0] = xreal[41];////Western Wake Treatment plant
		owasa.infMatrix[5][4] = 316.8*xreal[49];
		owasa.infMatrix[5][5] = 3 + rand() % 3;
		///Durham infrastructure
		durham.infMatrix[5][0] = xreal[42];/////Western Wake Treatment plant
		durham.infMatrix[5][4] = 243.3*xreal[50];
		durham.infMatrix[5][5] = 3 + rand() % 3;
		durham.infMatrix[6][0] = xreal[43];////Western Wake Treatment plant
		durham.infMatrix[6][4] = 316.8*xreal[50];
		durham.infMatrix[6][5] = 3 + rand() % 3;
		///Raleigh infrastructure
		raleigh.infMatrix[4][0] = xreal[44];//Western Wake Treatment plant
		raleigh.infMatrix[4][4] = 243.3*xreal[51];
		raleigh.infMatrix[4][5] = 3 + rand() % 3;
		raleigh.infMatrix[5][0] = xreal[45];////Western Wake Treatment plant
		raleigh.infMatrix[5][4] = 316.8*xreal[51];
		raleigh.infMatrix[5][5] = 3 + rand() % 3;
	}
	caryUpgrades[0] = xreal[46];
	caryUpgrades[1] = xreal[47];
	caryUpgrades[2] = xreal[48];
	caryUpgrades[3] = 999.0;
	if(xreal[49]+xreal[50]+xreal[51]>1.0)
        // double check when you apply this constraint
	{
		double WWWTPtotal = xreal[49]+xreal[50]+xreal[51];
		owasa.westernWakeTreatmentFrac = xreal[49]/WWWTPtotal;
		durham.westernWakeTreatmentFrac = xreal[50]/WWWTPtotal;
		raleigh.westernWakeTreatmentFrac = xreal[51]/WWWTPtotal;
	}
	else
	{
		owasa.westernWakeTreatmentFrac = xreal[49];
		durham.westernWakeTreatmentFrac = xreal[50];
		raleigh.westernWakeTreatmentFrac = xreal[51];
	}
	fallsLakeReallocation = xreal[52];
	//fallsLakeReallocation = 10000;

	caryWTPcosts[0] = 69.1;
	caryWTPcosts[1] = 21.4;
	caryWTPcosts[2] = 23.9;

	durham.infBuffer = xreal[53];
	owasa.infBuffer = xreal[54];
	raleigh.infBuffer = xreal[55];
	cary.infBuffer = xreal[56];
		// adjusts daily demands by a factor
		// to approximate future demands
		
	// NEW PARAMETERS --------------------------------------------------------------------------
	// May 2016 --------------------------------------------------------------------------------
	
	LMreleaseCap = xreal[59];
		// maximum weekly release volume from Durham to Falls Lake (MG per week)
		// set to 0 when no releases should occur
	buybackratePerMG = xreal[60];
		// average cost of a MG of water in buybacks (should be $3,500 per MG, in millions of dollars, so $0.0035 MM)
		// can set to 0 when releases arent happening, but wont get used anyway 
	//ReleaseContractPrice = xreal[61];
		// total cost in $MM of a release contract
		// set to $0 when no releases are occuring
		// replaced by annualpayment 
	FallsSupplyAllocationFraction = xreal[62];
		// fraction of Durham releases that are added to
		// the Falls Lake water supply pool 
		// this is overridden if the allocation of releases
		// is to be equal to the allocation of Falls Lake 
	BuybackROFZone = xreal[63];
		// this is the difference in ROF trigger levels between
		// when releases are not granted (the floor) and
		// when buybacks are used by Durham (the ceiling)
	contractlength = (int)(xreal[64]+1);
		// the length of a releases contract
		// converting to an integer will round down the number 
		// make sure this is greater than 1 by adding 1 to it
	raleigh.RRcontractTrigger = xreal[65];
	durham.RRcontractTrigger  = xreal[66];
		// ROF triggers used for release contract negotiation 
	annualpayment = xreal[61];
		// annual payment by Raleigh to Durham for release contracts 
		// in $MM 

		
	if (durham.RRtrigger - BuybackROFZone < 0)
	{
		BuybackROFZone = 1 - (1-durham.RRtrigger);
			// if the ROF range where buybacks occur extends to ROFs less than 0
			// change the range to bottom out at 0 
	}

	//Drought surcharges - 2 variables for each utility, one for residential customers and one for commercial/industrial/irrigation customers
	if (formulation<6)
	{
		for(int x = 0; x<5; x++)
		{
			for (int y = 0; y<10;y++)
			{
				durham.fractionPricing[x][y] = 1;
			}
			for (int y = 0; y<24;y++)
			{
				raleigh.fractionPricing[x][y] = 1;
			}
			for (int y = 0; y<9;y++)
			{
				cary.fractionPricing[x][y] = 1;
			}
		}
		for(int x = 0;x<4;x++)
		{
			for (int y = 0; y<8; y++)
			{
				owasa.fractionPricing[x][y] = 1;
			}
		}
	}
		// nothing 

	//Takes water use distributions calculated in calc_Water_Price functions and applies drought surcharge to calculate
	//the average revenue recieved from the sale of 1 MG of water, the consumer surplus lost from water use reductions/cost increases,
	//and overall reductions from restrictions and price increases
	//(Only do it once at the beginning. If formulation is 5, redo it every simulation).
	if(formulation == 6)
	{
		durham.calcSurchargePrice(elasticity_Of_Demand, 1);
		owasa.calcSurchargePrice(elasticity_Of_Demand, 1);
		cary.calcSurchargePrice(elasticity_Of_Demand, 1);
		raleigh.calcSurchargePrice(elasticity_Of_Demand, 1);
	}
		// nothing 


	// Initialize variables back to 0 to start the next simulation
	durham.clearVariablesForSimulation();
	owasa.clearVariablesForSimulation();
	cary.clearVariablesForSimulation();
	raleigh.clearVariablesForSimulation();
        // reset the variables, be careful when variables are reset for the next run
        // nested simulation cycles: 52 years of data, 1000 years of realizations, 1000000 optimization cycle
        // ABOUT TO ENTER REALIZATION LOOP, CLEAR OLD OBJECTIVE VALUES AND ANYTHING TO CALCULATE THEM

	durham.setCapacity(6349.0);
	owasa.setCapacity(3558.0);
	raleigh.setCapacity(falls_lake_supply_capacity + 2789.66);
	cary.setCapacity(cary.jordanLakeAlloc*14924.0);
		// should I switch this away from hardcoded numbers?
		// what are these referencing?

	//Actual reservoir simulation loop

	// to this point, only assigned decision variables to their model variables,
	// now we are actually running the model.

	cout << "Beginning realization loop sol " << solutionNumber << endl;
	realizationLoop();
        // reservoir simulation done here

	//Writing objective variables
	if(borgToggle == 0)//Using MOEA, with overall objectives determined by the largest individual utility value for the given objective
	{
		// The first three objectives (reliability, restriction frequency, and total cost) are used in all formulations
		c_obj[0] = maxValue(durham.maxFailures, owasa.maxFailures, raleigh.maxFailures, cary.maxFailures, maxFallsFailure);
		c_obj[1] = durham.expectedNPC + owasa.expectedNPC + raleigh.expectedNPC + cary.expectedNPC;
		c_obj[2] = maxValue(durham.peakDebt, owasa.peakDebt, raleigh.peakDebt, cary.peakDebt);
		c_obj[3] = maxValue(durham.totalLosses, owasa.totalLosses, raleigh.totalLosses, cary.totalLosses);
		c_obj[4] = maxValue(durham.maxRestrictions, owasa.maxRestrictions, raleigh.maxRestrictions, cary.maxRestrictions);
		c_obj[5] = xreal[7] + xreal[8] + xreal[9] + xreal[10];
	}

	else if(borgToggle == 3) //Mode to allow for manual input of parameters and individual objective function evaluation
	{
		// RefSet Reruns - want the 6 objectives from O0

		// The first three objectives (reliability, restriction frequency, and total cost) are used in all formulations
		/*c_obj[0] = maxValue(durham.maxFailures, owasa.maxFailures, raleigh.maxFailures, cary.maxFailures);
		c_obj[1] = maxValue(durham.restrictionFreq, owasa.restrictionFreq, raleigh.restrictionFreq, cary.restrictionFreq);
		c_obj[2] = maxValue(durham.totalCosts, owasa.totalCosts, raleigh.totalCosts, cary.totalCosts);
		c_obj[3] = durham.jordanLakeAlloc + owasa.jordanLakeAlloc + raleigh.jordanLakeAlloc + cary.jordanLakeAlloc;
		c_obj[4] = maxValue(durham.totalLosses, owasa.totalLosses, raleigh.totalLosses, cary.totalLosses);
		c_obj[5] = maxValue(durham.consumerFractions, owasa.consumerFractions, raleigh.consumerFractions, cary.consumerFractions);*/

		c_obj[0] = durham.maxFailures;
		c_obj[1] = durham.expectedNPC;
		c_obj[2] = durham.peakDebt;
		c_obj[3] = durham.totalLosses;
		c_obj[4] = durham.maxRestrictions;
		c_obj[5] = durham.peakInsurance;
		c_obj[6] = xreal[7];

		c_obj[7] = owasa.maxFailures;
		c_obj[8] = owasa.expectedNPC;
		c_obj[9] = owasa.peakDebt;
		c_obj[10] = owasa.totalLosses;
		c_obj[11] = owasa.maxRestrictions;
		c_obj[12] = owasa.peakInsurance;
		c_obj[13] = xreal[8];

		c_obj[14] = raleigh.maxFailures;
		c_obj[15] = raleigh.expectedNPC;
		c_obj[16] = raleigh.peakDebt;
		c_obj[17] = raleigh.totalLosses;
		c_obj[18] = raleigh.maxRestrictions;
		c_obj[19] = raleigh.peakInsurance;
		c_obj[20] = xreal[9];

		c_obj[21] = cary.maxFailures;
		c_obj[22] = cary.expectedNPC;
		c_obj[23] = cary.peakDebt;
		c_obj[24] = cary.totalLosses;
		c_obj[25] = cary.maxRestrictions;
		c_obj[26] = cary.peakInsurance;
		c_obj[27] = xreal[10];

		c_obj[28] = maxFallsFailure;

	}

	// If optimizing, also calculate the constraints
	// JDH 11/12: Turning off constraints for now
	/*
	if(borgToggle == 0 || borgToggle == 1)
	{
		if(num_constr >= 3)
		{
			c_constr[0] = -1*c_obj[0]/.02 + 1;
			c_constr[1] = -1*c_obj[1]/.50 + 1;
			c_constr[2] = -1*c_obj[2]/.25 + 1;
		}
		if(num_constr >= 4)
		{
			c_constr[3] = -1*c_obj[3] + 1;
		}
		if(num_constr >= 5)
		{
			if (c_obj[2]>0)
				c_constr[4] = -1*c_obj[4]/c_obj[2] + 2;
			else
				c_constr[4] = 0;
		}
		if(num_constr >= 6)
		{
			c_constr[5] = 0;
		}

		for (int x = 0; x < num_constr;x++)
		{
			if (c_constr[x] > 0)
				c_constr[x] = 0;
		}
	}*/

	return difftime( time(0), start);
}

void Simulation::createRiskOfFailure(int realization, int synthYear, double durhamDemandValue, double owasaDemandValue, double raleighDemandValue, double caryDemandValue,
									 int discreteintervals)
{
	// Risk of failure measures the risk, given a certain week of the year and storage volume, of reservoirs running out of water using historical
	// streamflows.  This function goes through every combination of storage (discritized at 5% intervals) and week-of-year and runs through 52 weeks of
	// streamflow levels 78 seperate times, beginning at the same week of the year in each of the first 78 years of historical streamflow record.  At each
	// distinct storage/week point, a percentage is calculating, showing the probability that reservoir levels will drop below 20% of total storage capacity
	// at least once over the next 52 weeks.  Demand is updated for every year with demand growth estimates, such that a distinct risk-of-failure is calculated
	// in each future year.  52(weeks per year) X 20 (storage discritizations) X 51 (max number of future years) distinct risk values are calculated.
	
	double durhamROFDemand = 0.0;
	double owasaROFDemand = 0.0;
	double raleighROFDemand = 0.0;
	double caryROFDemand = 0.0;
	double durhamROFInflow = 0.0;
	double raleighROFInflow = 0.0;
	double owasaROFInflow = 0.0;
	double wbROFInflow = 0.0;
	double claytonROFInflow = 0.0;
	double crabtreeROFInflow = 0.0;
	double jordanROFInflow = 0.0;
	double lillingtonROFInflow = 0.0;
	double littleRiverRaleighROFInflow = 0.0;
	double ROFevap = 0.0;
	double fallsROFevap = 0.0;
	double wbROFevap = 0.0;
	
	double oROFs = 0.0;
	double dROFs = 0.0;
	double rROFs = 0.0;
	double cROFs = 0.0;
	
	double oIPs = 0.0;
	double dIPs = 0.0;
	double rIPs = 0.0;
	double cIPs = 0.0;
	
	double oIPex = 0.0;
	double dIPex = 0.0;
	double rIPex = 0.0;
	double cIPex = 0.0;

	int oROFstage = 0.0;
	int dROFstage = 0.0;
	int rROFstage = 0.0;
	int cROFstage = 0.0;
	int counter = 0;
	int syntheticIndex = 0;
	
	int thisTimeO = 0.0;
	int thisTimeR = 0.0;
	int thisTimeD = 0.0;
	int thisTimeC = 0.0;
	
	int thisTimeOIP[discreteintervals];
	int thisTimeDIP[discreteintervals];
	int thisTimeCIP[discreteintervals];
	int thisTimeRIP[discreteintervals];
		// changed this and any other occurrence 
		// of the hardcoded tiers = 20 to
		// discreteintervals
	
	int histYear = 80;
	int yearROF;
	int monthROF;
	int weekROF;
	int numdaysROF;
	int insurancePriceCounter;

	owasa.riskOfFailure = 0.0;
	durham.riskOfFailure = 0.0;
	raleigh.riskOfFailure = 0.0;
	cary.riskOfFailure = 0.0;
	
	for(int x = 0; x < discreteintervals; x++)
	{
		owasa.storageRisk[x] = 0.0;
		durham.storageRisk[x] = 0.0;
		cary.storageRisk[x] = 0.0;
		raleigh.storageRisk[x] = 0.0;
		
		durham.ReleaseStorageRisk[x] = 0.0;
		raleigh.ReleaseStorageRisk[x] = 0.0;
			// added these two lines
	}

	double durhamS = systemStorage.getDurhamStorageVol();
	double teerS =  systemStorage.getTeerStorageVol();
	double CCRS =  systemStorage.getCCRStorageVol();
	double ULS =  systemStorage.getULStorageVol();
	double STQS =  systemStorage.getSTQStorageVol();
	double owasaS =  systemStorage.getOWASAStorageVol();
	double lakeWBS = systemStorage.getLakeWBStorageVol();
	double flSS =  systemStorage.getFallsSupplyStorageVol();
	double flQS = systemStorage.getFallsQualityStorageVol();
	double jlSS = systemStorage.getJordanSupplyStorageVol();
	double jlQS = systemStorage.getJordanQualityStorageVol();
	double caryJordanS = systemStorage.getCaryJordanStorageVol();
	double raleighJordanS = systemStorage.getRaleighJordanStorageVol();
	double durhamJordanS = systemStorage.getDurhamJordanStorageVol();
	double owasaJordanS = systemStorage.getOWASAJordanStorageVol();
	double littleRiverRalS = systemStorage.getLittleRiverRalStorageVol();
	double raleighQS = systemStorage.getRaleighQuarryStorageVol();
	
	int numRiskYears = 50;
	int startingHistoricalYear = 80 - (numRiskYears - synthYear + 1);
	int startingSynthYear = synthYear - numRiskYears;
	
	if(startingSynthYear < 0)
	{
		startingSynthYear = 0;
	}
	
	for (int histRealizations = startingHistoricalYear; histRealizations<histYear; histRealizations++)
		// determines the year of the historical streamflow record to be used in calculations
        // runs 52 weeks from current week, to find where levels drop below 20% over this year
        // this is run for 50 years of the immediate past, based on the current year
	{

		counter = 0;
		
		thisTimeO = 0;
		thisTimeD = 0;
		thisTimeR = 0;
		thisTimeC = 0;
		
		oIPex = 0.0;
		dIPex = 0.0;
		cIPex = 0.0;
		rIPex = 0.0;

		for(int x = 0; x < discreteintervals; x++)
		{
			thisTimeOIP[x] = 0;
			thisTimeDIP[x] = 0;
			thisTimeRIP[x] = 0;
			thisTimeCIP[x] = 0;
		}
		
		riskOfFailureDates.initializeDates(startSimulationYear,1,week,7,0);
			//each simulation is run independently (52 week intervals, then the slate is cleaned)
		riskOfFailureStorageROF.updateReservoirStorageROF(durhamS, teerS, CCRS, ULS, STQS, owasaS, lakeWBS, flSS, flQS, jlSS, jlQS, caryJordanS,  raleighJordanS,  durhamJordanS, owasaJordanS, littleRiverRalS, raleighQS);
            // sets initial ROF storage to current storage levels
		riskOfFailureStorageIP.updateReservoirStorageROF();
            // sets initial IP storage to full

		while (counter < 52)
		{
			yearROF    = riskOfFailureDates.getYear();
			monthROF   = riskOfFailureDates.getMonth();
			weekROF    = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();
				// Retrieving date information

			durhamROFDemand  = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand   = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand    = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands
                // expected demand set for a given week
				// While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)
			
			riskOfFailureStorageROF.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);
				// passes demands to storage calcs
                // demands set every week

			riskOfFailureStorageIP.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);
				// passes demands to storage calcs

				
			durhamROFInflow = michieInflow[histRealizations+yearROF-startSimulationYear][weekROF-1]+littleRiverInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Durham Inflows
			raleighROFInflow = fallsLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Raleigh
			owasaROFInflow = owasaInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//OWASA
			wbROFInflow = lakeWBInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Wheeler&Benson
			claytonROFInflow = claytonInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Clayton gauge
			crabtreeROFInflow = crabtreeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				// Crabtree creek
			jordanROFInflow = jordanLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			lillingtonROFInflow = lillingtonGaugeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			littleRiverRaleighROFInflow = littleRiverRaleighInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
                // inflow taken from historical record


			ROFevap      = evaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Durham and OWASA evap
			fallsROFevap = fallsLakeEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				// Falls Lake evap
			wbROFevap    = lakeWheelerEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Wheeler Benson evap
                // same for evap

			riskOfFailureStorageROF.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);

			riskOfFailureStorageIP.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);

			riskOfFailureStorageROF.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageIP.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity

			riskOfFailureStorageROF.updateStorage(weekROF-1);//storage calcs
			riskOfFailureStorageIP.updateStorage(weekROF-1);//storage calcs
                // advance res operation a week

			oROFs = riskOfFailureStorageROF.getOWASAStorage();//retrieve overall storage, OWASA
			dROFs = riskOfFailureStorageROF.getDurhamStorage();//retrieve overall storage, Durham
			rROFs = riskOfFailureStorageROF.getRaleighStorage();//retrieve overall storage, Raleigh
			cROFs = riskOfFailureStorageROF.getCaryStorage();
                // find new storage fraction


			if (oROFs<.2)
			{
				thisTimeO = 1;//Mark OWASA failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (dROFs<.2)
			{
				thisTimeD = 1;//Mark Durham failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (rROFs<.2)
			{
				thisTimeR = 1;//Mark Raleigh failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (cROFs<.2)
			{
				thisTimeC = 1;
			}
                // this was for ROF, insurance is below
                // they are different because ROF is used to calculate insurance payments?

			riskOfFailureDates.increase();//increase week by one
			counter++;
			
			oIPs = riskOfFailureStorageIP.getOWASAStorage();//retrieve overall storage, OWASA
			dIPs = riskOfFailureStorageIP.getDurhamStorage();//retrieve overall storage, Durham
			rIPs = riskOfFailureStorageIP.getRaleighStorage();//retrieve overall storage, Raleigh
			cIPs = riskOfFailureStorageIP.getCaryStorage();
                // current week's storage fraction, from 0 to 1, 1 being the reservoir is full

			oIPex += riskOfFailureStorageIP.getExcessO();//retrieve overall storage, OWASA
			dIPex += riskOfFailureStorageIP.getExcessD();//retrieve overall storage, Durham
			rIPex += riskOfFailureStorageIP.getExcessR();//retrieve overall storage, Raleigh
			cIPex += riskOfFailureStorageIP.getExcessC();
                // if the reservoir was full, what was spilled over as excess

			if(oIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeOIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(oIPs < (0.2 + double(x)/double(discreteintervals) - oIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeOIP[y] = 1;
						}
						break;
					}
				}
			}

			// repeat for all utilities....

			if(dIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeDIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(dIPs < (0.2 + double(x)/double(discreteintervals) - dIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeDIP[y] = 1;
						}
						break;
					}
				}
			}

			if(rIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeRIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(rIPs<(0.2 + double(x)/double(discreteintervals) - rIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeRIP[y] = 1;
						}
						break;
					}
				}
			}
		

			if(cIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeCIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(cIPs < (0.2 + double(x)/double(discreteintervals) - cIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeCIP[y] = 1;
						}
						break;
					}
				}
			}
		}
		
		if (thisTimeO==1)
			//Fill out the risk matricies (every historical year that causes at least one week of failure results in an increase in risk of 1.28% (1/78)
		{
			owasa.riskOfFailure += 1.0;
		}
		if (thisTimeD==1)
		{
			durham.riskOfFailure += 1.0;
		}
		if (thisTimeR==1)
		{
			raleigh.riskOfFailure += 1.0;
		}
		if (thisTimeC==1)
		{
			cary.riskOfFailure += 1.0;
		}
		
		for (int x = 0; x < discreteintervals; x++)
		{
			if(thisTimeOIP[x] == 1)
			{
				owasa.storageRisk[x] += 1.0;
			}
			if(thisTimeDIP[x] == 1)
			{
				durham.storageRisk[x] += 1.0;
				durham.ReleaseStorageRisk[x] += 1.0;
			}
			if(thisTimeRIP[x] == 1)
			{
				raleigh.storageRisk[x] += 1.0;
				raleigh.ReleaseStorageRisk[x] += 1.0;
			}
			if(thisTimeCIP[x] == 1)
			{
				cary.storageRisk[x] += 1.0;
			}
		}
	}
	
	for (int synthRealizations = startingSynthYear; synthRealizations<synthYear; synthRealizations++)
		// determines the year of the historical streamflow record to be used in calculations
        // same loop as above but with the synthetic record
	{
		counter = 0;
		
		thisTimeO = 0;
		thisTimeD = 0;
		thisTimeR = 0;
		thisTimeC = 0;
		
		oIPex = 0.0;
		dIPex = 0.0;
		cIPex = 0.0;
		rIPex = 0.0;
				
		for(int x = 0; x < discreteintervals; x++)
		{
			thisTimeOIP[x] = 0;
			thisTimeDIP[x] = 0;
			thisTimeRIP[x] = 0;
			thisTimeCIP[x] = 0;
		}
		
		riskOfFailureDates.initializeDates(synthRealizations,1,week,7,0);//each simulation is run independently (52 week intervals, then the slate is cleaned)

		riskOfFailureStorageROF.updateReservoirStorageROF(durhamS, teerS, CCRS, ULS, STQS, owasaS, lakeWBS, flSS, flQS, jlSS, jlQS, caryJordanS,  raleighJordanS,  durhamJordanS, owasaJordanS, littleRiverRalS, raleighQS);
		riskOfFailureStorageIP.updateReservoirStorageROF();

		while (counter < 52)
		{
			//Retrieving date information
			yearROF = riskOfFailureDates.getYear();
			monthROF = riskOfFailureDates.getMonth();
			weekROF = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();

			//Demand calcs
			//While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)
			durhamROFDemand = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands

			riskOfFailureStorageROF.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);//passes demands to storage calcs
			riskOfFailureStorageIP.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);//passes demands to storage calcs

			//Inflow Calcs
			syntheticIndex = (yearROF)*52+weekROF-1;
			durhamROFInflow = durhamInflows.synthetic[realization][syntheticIndex];//Durham Inflows
			raleighROFInflow = fallsInflows.synthetic[realization][syntheticIndex];//Raleigh
			owasaROFInflow = owasaInflows.synthetic[realization][syntheticIndex];//OWASA
			wbROFInflow = wheelerInflows.synthetic[realization][syntheticIndex];//Wheeler&Benson
			claytonROFInflow = claytonInflows.synthetic[realization][syntheticIndex];//Clayton gauge
			crabtreeROFInflow = crabtreeInflows.synthetic[realization][syntheticIndex];// Crabtree creek
			jordanROFInflow = jordanInflows.synthetic[realization][syntheticIndex];
			lillingtonROFInflow = lillingtonInflows.synthetic[realization][syntheticIndex];
			littleRiverRaleighROFInflow = littleRiverRaleighInflows.synthetic[realization][syntheticIndex];
			//Evap Calcs

			ROFevap = durhamOwasaEvap.synthetic[realization][syntheticIndex];//Durham and OWASA evap
			fallsROFevap = fallsEvap.synthetic[realization][syntheticIndex];// Falls Lake evap
			wbROFevap= wheelerEvap.synthetic[realization][syntheticIndex];//Wheeler Benson evap

			riskOfFailureStorageROF.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);

			riskOfFailureStorageIP.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);

			riskOfFailureStorageROF.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageIP.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity

			riskOfFailureStorageROF.updateStorage(weekROF-1);//storage calcs
			riskOfFailureStorageIP.updateStorage(weekROF-1);//storage calcs

			oROFs = riskOfFailureStorageROF.getOWASAStorage();//retrieve overall storage, OWASA
			dROFs = riskOfFailureStorageROF.getDurhamStorage();//retrieve overall storage, Durham
			rROFs = riskOfFailureStorageROF.getRaleighStorage();//retrieve overall storage, Raleigh
			cROFs = riskOfFailureStorageROF.getCaryStorage();

			if (oROFs<.2)
			{
				thisTimeO = 1;//Mark OWASA failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (dROFs<.2)
			{
				thisTimeD = 1;//Mark Durham failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (rROFs<.2)
			{
				thisTimeR = 1;//Mark Raleigh failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (cROFs<.2)
			{
				thisTimeC = 1;
			}

			riskOfFailureDates.increase();//increase week by one
			counter++;

			oIPs = riskOfFailureStorageIP.getOWASAStorage();//retrieve overall storage, OWASA
			dIPs = riskOfFailureStorageIP.getDurhamStorage();//retrieve overall storage, Durham
			rIPs = riskOfFailureStorageIP.getRaleighStorage();//retrieve overall storage, Raleigh
			cIPs = riskOfFailureStorageIP.getCaryStorage();

			oIPex += riskOfFailureStorageIP.getExcessO();//retrieve overall storage, OWASA
			dIPex += riskOfFailureStorageIP.getExcessD();//retrieve overall storage, Durham
			rIPex += riskOfFailureStorageIP.getExcessR();//retrieve overall storage, Raleigh
			cIPex += riskOfFailureStorageIP.getExcessC();

			if(oIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeOIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(oIPs < (0.2 + double(x)/double(discreteintervals) - oIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeOIP[y] = 1;
						}
						break;
					}
				}
			}

			// repeat for all utilities....

			if(dIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeDIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(dIPs < (0.2 + double(x)/double(discreteintervals) - dIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeDIP[y] = 1;
						}
						break;
					}
				}
			}

			if(rIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeRIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(rIPs<(0.2 + double(x)/double(discreteintervals) - rIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeRIP[y] = 1;
						}
						break;
					}
				}
			}
		

			if(cIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeCIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(cIPs < (0.2 + double(x)/double(discreteintervals) - cIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeCIP[y] = 1;
						}
						break;
					}
				}
			}
		}
		
		if (thisTimeO==1)
			//Fill out the risk matricies (every historical year that causes at least one week of failure results in an increase in risk of 1.28% (1/78)
		{
			owasa.riskOfFailure += 1.0;
		}
		if (thisTimeD==1)
		{
			durham.riskOfFailure += 1.0;
		}
		if (thisTimeR==1)
		{
			raleigh.riskOfFailure += 1.0;
		}
		if (thisTimeC==1)
		{
			cary.riskOfFailure += 1.0;
		}
		
		for (int x = 0; x < discreteintervals; x++)
		{
			if(thisTimeOIP[x] == 1)
			{
				owasa.storageRisk[x] += 1.0;
			}
			if(thisTimeDIP[x] == 1)
			{
				durham.storageRisk[x] += 1.0;
				durham.ReleaseStorageRisk[x] += 1.0;
			}
			if(thisTimeRIP[x] == 1)
			{
				raleigh.storageRisk[x] += 1.0;
				raleigh.ReleaseStorageRisk[x] += 1.0;
			}
			if(thisTimeCIP[x] == 1)
			{
				cary.storageRisk[x] += 1.0;
			}
		}
	}
	
	owasa.riskOfFailure   = owasa.riskOfFailure/(double(numRiskYears));
	durham.riskOfFailure  = durham.riskOfFailure/(double(numRiskYears));
	raleigh.riskOfFailure = raleigh.riskOfFailure/(double(numRiskYears));
	cary.riskOfFailure    = cary.riskOfFailure/(double(numRiskYears));
        // divide by 50 to get a percentage of years in failure

	for(int x = 0; x < discreteintervals; x++)
	{
		owasa.storageRisk[x] = owasa.storageRisk[x]/(double(numRiskYears));
		if(owasa.storageRisk[x] > owasa.insuranceUse)
            // what storage level incurs an insurance payout?
            // known insurance level, what storage level will trigger this?
		{
			owasa.riskVolume[week-1] = (double(discreteintervals) - double(x))/double(discreteintervals);

			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		durham.storageRisk[x] = durham.storageRisk[x]/(double(numRiskYears));
		if(durham.storageRisk[x] > durham.insuranceUse)
		{
			durham.riskVolume[week-1] = (double(discreteintervals) - double(x))/double(discreteintervals);
			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		raleigh.storageRisk[x] = raleigh.storageRisk[x]/(double(numRiskYears));
		if(raleigh.storageRisk[x] > raleigh.insuranceUse)
		{
			raleigh.riskVolume[week-1] = (double(discreteintervals) - double(x))/double(discreteintervals);
			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		cary.storageRisk[x] = cary.storageRisk[x]/(double(numRiskYears));
		if(cary.storageRisk[x] > cary.insuranceUse)
		{
			cary.riskVolume[week-1] = (double(discreteintervals) - double(x))/double(discreteintervals);
			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		durham.ReleaseStorageRisk[x] = durham.ReleaseStorageRisk[x]/(double(numRiskYears));
		
		if (durham.ReleaseStorageRisk[x] > durham.RRtrigger - BuybackROFZone)
			// if the risk of failure becomes greater than the ROF trigger level 
			// for buybacks (equal to the ROF trigger for release denial - ROF buffer amount)
		{
			durham.BuybackRiskVolume[week-1] = (double(discreteintervals) - double(x-1))/double(discreteintervals);
				// reservoir level ceiling for buybacks. above this level, buybacks won't be made on water.  between this
				// and the cutoff reservoir level (releaseriskvolume), that supply will be bought back 
			
			if (durham.BuybackRiskVolume[week-1] > 1.0)
			{
				durham.BuybackRiskVolume[week-1] = 1.0;
			}
		}
		
		if (durham.ReleaseStorageRisk[x] > durham.RRtrigger)
			// if the risk of failure becomes greater than the ROF trigger for releases
			// at the specific x, meaning current reservoir level
		{
			durham.ReleaseRiskVolume[week-1] = (double(discreteintervals) - double(x-1))/double(discreteintervals);
				// the desired reservoir level that releases need to achieve is this
			if (durham.ReleaseRiskVolume[week-1] > 1.0)
			{
				durham.ReleaseRiskVolume[week-1] = 1.0;
			}
			
			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		raleigh.ReleaseStorageRisk[x] = raleigh.ReleaseStorageRisk[x]/(double(numRiskYears));
		
		if (raleigh.ReleaseStorageRisk[x] > raleigh.RRtrigger)
		{
			raleigh.ReleaseRiskVolume[week-1] = (double(discreteintervals) - double(x-1))/double(discreteintervals);
			
			if (raleigh.ReleaseRiskVolume[week-1] > 1.0)
			{
				raleigh.ReleaseRiskVolume[week-1] = 1.0;
			}
			
			break;
		}
	}

	return;
}

void Simulation::createInfrastructure(int realization)
{
	int owasaConstruction;
	int durhamConstruction;
	int raleighConstruction;
	for(int x = 0; x<owasa.infrastructureCount; x++)
	{
		owasaConstruction = owasa.buildInfrastructure(x);
			// for infrastructure option x, check buildInfrastructure option
			// to see if this option has been built or not, if it has, enter the if statement below
		if(owasaConstruction == 1)
		{
			InfraBuilt << solutionNumber << "," << rdmNumber << "," << realization << "," << year << ",";
			
			switch(x)
			{
				case 0:
					systemStorage.buildULexp();
					riskOfFailureStorageROF.buildULexp();
					riskOfFailureStorageIP.buildULexp();
					owasa.addCapacity(2550.0);
					
					InfraBuilt << "OWASA" << "," << "University Lake Expansion" << endl;
					
					break;
				case 1:
					systemStorage.buildCCexp();
					riskOfFailureStorageROF.buildCCexp();
					riskOfFailureStorageIP.buildCCexp();
					owasa.addCapacity(3000.0);
					
					InfraBuilt << "OWASA" << "," << "CCR Expansion" << endl;
					
					break;
				case 2:
					systemStorage.buildSQlow();
					riskOfFailureStorageROF.buildSQlow();
					riskOfFailureStorageIP.buildSQlow();
					owasa.addCapacity(1500.0);
					
					InfraBuilt << "OWASA" << "," << "Stone Quarry Expansion (Small)" << endl;
					
					break;
				case 3:
					systemStorage.buildSQhigh();
					riskOfFailureStorageROF.buildSQhigh();
					riskOfFailureStorageIP.buildSQhigh();
					owasa.addCapacity(2200.0);
					
					InfraBuilt << "OWASA" << "," << "Stone Quarry Expansion (Large)" << endl;
					
					break;
				case 4:
					systemStorage.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageROF.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageIP.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					wwWTPInsuranceTrigger = 1.0;
					
					InfraBuilt << "OWASA" << "," << "Western JL WTP (Small)" << endl;
					
					break;
				case 5:
					systemStorage.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageROF.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageIP.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					wwWTPInsuranceTrigger = 1.0;
					
					InfraBuilt << "OWASA" << "," << "Western JL WTP (Large)" << endl;
					
					break;
				default:
					InfraBuilt << "OWASA" << "," << "NO REMAINING PROJECTS" << endl;
			}
		}
	}

	// repeat for other utilities...

	int constructionToggle2 = 0;
	for(int x = 0; x<durham.infrastructureCount; x++)
	{
		durhamConstruction = durham.buildInfrastructure(x);

		if(durhamConstruction == 1)
		{
			InfraBuilt << solutionNumber << "," << rdmNumber << "," << realization << "," << year << ",";
			
			constructionToggle2 = x;
			switch(x)
			{
				case 0:
					systemStorage.buildTeerQuarry();
					riskOfFailureStorageROF.buildTeerQuarry();
					riskOfFailureStorageIP.buildTeerQuarry();
					
					InfraBuilt << "Durham" << "," << "Teer Quarry" << endl;
					
					break;
				case 1:
					systemStorage.buildReclaimedLow();
					riskOfFailureStorageROF.buildReclaimedLow();
					riskOfFailureStorageIP.buildReclaimedLow();
					durhamReclaimedInsuranceTrigger = 1.0;
					
					InfraBuilt << "Durham" << "," << "Reclaimed Water (Small)" << endl;
					
					break;
				case 2:
					systemStorage.buildReclaimedHigh();
					riskOfFailureStorageROF.buildReclaimedHigh();
					riskOfFailureStorageIP.buildReclaimedHigh();
					durhamReclaimedInsuranceTrigger = 1.0;
					
					InfraBuilt << "Durham" << "," << "Reclaimed Water (Large)" << endl;
					
					break;
				case 3:
					systemStorage.buildMichieLow();
					riskOfFailureStorageROF.buildMichieLow();
					riskOfFailureStorageIP.buildMichieLow();
					durham.setCapacity(8849.0);
					
					InfraBuilt << "Durham" << "," << "Lake Michie Expansion (Small)" << endl;
					
					break;
				case 4:
					systemStorage.buildMichieHigh();
					riskOfFailureStorageROF.buildMichieHigh();
					riskOfFailureStorageIP.buildMichieHigh();
					durham.setCapacity(14049.0);
					
					InfraBuilt << "Durham" << "," << "Lake Michie Expansion (Large)" << endl;
					
					break;
				case 5:
					systemStorage.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageROF.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageIP.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					wwWTPInsuranceTrigger = 1.0;
					
					InfraBuilt << "Durham" << "," << "Western JL WTP (Small)" << endl;
					
					break;
				case 6:
					systemStorage.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageROF.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageIP.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					wwWTPInsuranceTrigger = 1.0;
					
					InfraBuilt << "Durham" << "," << "Western JL WTP (Large)" << endl;
					
					break;
				default:
					InfraBuilt << "Durham" << "," << "NO REMAINING PROJECTS" << endl;
			}
		}
	}
	int constructionToggle = 0;
	for(int x = 0; x< raleigh.infrastructureCount; x++)
	{
		raleighConstruction = raleigh.buildInfrastructure(x);
		if(raleighConstruction == 1)
		{
			InfraBuilt << solutionNumber << "," << rdmNumber << "," << realization << "," << year << ",";
			
			constructionToggle = x;
			switch(x)
			{
				case 0:
					systemStorage.buildLittleRiverRal();
					riskOfFailureStorageROF.buildLittleRiverRal();
					riskOfFailureStorageIP.buildLittleRiverRal();
					raleigh.addCapacity(3700.0);
					littleRiverRalInsuranceTrigger = 1.0;
					
					InfraBuilt << "Raleigh" << "," << "LRR Raleigh" << endl;
					
					break;
				case 1:
					systemStorage.buildRalQuarry();
					riskOfFailureStorageROF.buildRalQuarry();
					riskOfFailureStorageIP.buildRalQuarry();
					
					InfraBuilt << "Raleigh" << "," << "Raleigh Quarry" << endl;
					
					break;
				case 2:
					systemStorage.buildNeuseIntake();
					riskOfFailureStorageROF.buildNeuseIntake();
					riskOfFailureStorageIP.buildNeuseIntake();
					ralIntakeInsuranceTrigger = 1.0;
					
					InfraBuilt << "Raleigh" << "," << "Neuse River Intake" << endl;
					
					break;
				case 3:
					systemStorage.reallocateFallsLake(fallsLakeReallocation);
					riskOfFailureStorageROF.reallocateFallsLake(fallsLakeReallocation);
					riskOfFailureStorageIP.reallocateFallsLake(fallsLakeReallocation);
					raleigh.addCapacity(fallsLakeReallocation);
					raleigh.addInsStorage(fallsLakeReallocation);
					
					InfraBuilt << "Raleigh" << "," << "Reallocate Falls Lake" << endl;
					
					break;
				case 4:
					systemStorage.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageROF.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageIP.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					wwWTPInsuranceTrigger = 1.0;
					
					InfraBuilt << "Raleigh" << "," << "Western JL WTP (Small)" << endl;
					
					break;
				case 5:
					systemStorage.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageROF.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					riskOfFailureStorageIP.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
					wwWTPInsuranceTrigger = 1.0;
					
					InfraBuilt << "Raleigh" << "," << "Western JL WTP (Large)" << endl;
					
					break;
				default:
					InfraBuilt << "Raleigh" << "," << "NO REMAINING PROJECTS" << endl;
			}
		}
	}

	// not done for cary bc they are just expanding WTP

	return;
}

void Simulation::createInfrastructureRisk(int realization, int synthYear, double durhamDemandValue, double owasaDemandValue, double raleighDemandValue, double caryDemandValue)
{
	// Risk of failure measures the risk, given a certain week of the year and storage volume, of reservoirs running out of water using historical
	// streamflows.  This function goes through every combination of storage (discritized at 5% intervals) and week-of-year and runs through 52 weeks of
	// streamflow levels 78 seperate times, beginning at the same week of the year in each of the first 78 years of historical streamflow record.  At each
	// distinct storage/week point, a percentage is calculating, showing the probability that reservoir levels will drop below 20% of total storage capacity
	// at least once over the next 52 weeks.  Demand is updated for every year with demand growth estimates, such that a distinct risk-of-failure is calculated
	// in each future year.  52(weeks per year) X 20 (storage discritizations) X 51 (max number of future years) distinct risk values are calculated.
	double durhamROFDemand = 0.0;
	double owasaROFDemand = 0.0;
	double raleighROFDemand = 0.0;
	double caryROFDemand = 0.0;
	double durhamROFInflow = 0.0;
	double raleighROFInflow = 0.0;
	double owasaROFInflow = 0.0;
	double wbROFInflow = 0.0;
	double claytonROFInflow = 0.0;
	double crabtreeROFInflow = 0.0;
	double jordanROFInflow = 0.0;
	double lillingtonROFInflow = 0.0;
	double littleRiverRaleighROFInflow = 0.0;
	double ROFevap = 0.0;
	double fallsROFevap = 0.0;
	double wbROFevap = 0.0;
	double oROFs = 0.0;
	double dROFs = 0.0;
	double rROFs = 0.0;
	double cROFs = 0.0;
	double oIPs = 0.0;
	double dIPs = 0.0;
	double rIPs = 0.0;
	double cIPs = 0.0;

	int counter = 0;
	int syntheticIndex = 0;
	double thisTimeO = 0.0;
	double thisTimeR = 0.0;
	double thisTimeD = 0.0;
	double thisTimeC = 0.0;
	int histYear = 80;
	durham.infRisk = 0.0;
	raleigh.infRisk = 0.0;
	cary.infRisk = 0.0;
	owasa.infRisk = 0.0;
	durham.probReach = 0.0;
	raleigh.probReach = 0.0;
	cary.probReach = 0.0;
	owasa.probReach = 0.0;
	int yearROF;
	int monthROF;
	int weekROF;
	int numdaysROF;

	double durhamS = systemStorage.getDurhamStorageVol();
	double teerS =  systemStorage.getTeerStorageVol();
	double CCRS =  systemStorage.getCCRStorageVol();
	double ULS =  systemStorage.getULStorageVol();
	double STQS =  systemStorage.getSTQStorageVol();
	double owasaS =  systemStorage.getOWASAStorageVol();
	double lakeWBS = systemStorage.getLakeWBStorageVol();
	double flSS =  systemStorage.getFallsSupplyStorageVol();
	double flQS = systemStorage.getFallsQualityStorageVol();
	double jlSS = systemStorage.getJordanSupplyStorageVol();
	double jlQS = systemStorage.getJordanQualityStorageVol();
	double caryJordanS = systemStorage.getCaryJordanStorageVol();
	double raleighJordanS = systemStorage.getRaleighJordanStorageVol();
	double durhamJordanS = systemStorage.getDurhamJordanStorageVol();
	double owasaJordanS = systemStorage.getOWASAJordanStorageVol();
	double littleRiverRalS = systemStorage.getLittleRiverRalStorageVol();
	double raleighQS = systemStorage.getRaleighQuarryStorageVol();


	int numRiskYears = 50;
	int startingHistoricalYear = 81 - (numRiskYears - synthYear);
	int startingSynthYear = synthYear - numRiskYears;
	if(startingSynthYear < 0)
	{
		startingSynthYear = 0;
	}
	for (int histRealizations = startingHistoricalYear; histRealizations<histYear; histRealizations++)// determines the year of the historical streamflow record to be used in calculations
	{
		counter = 0;
		thisTimeO=0;
		thisTimeD = 0;
		thisTimeR = 0;
		thisTimeC = 0;
		riskOfFailureDates.initializeDates(startSimulationYear,1,1,7,0);//each simulation is run independently (52 week intervals, then the slate is cleaned)
		riskOfFailureStorageIP.updateReservoirStorageROF(durhamS, teerS, CCRS, ULS, STQS, owasaS, lakeWBS, flSS, flQS, jlSS, jlQS, caryJordanS,  raleighJordanS,  durhamJordanS, owasaJordanS, littleRiverRalS, raleighQS);
		riskOfFailureStorageInf.updateReservoirStorageROF();

		while (counter<78)
		{
			//Retrieving date information
			yearROF = riskOfFailureDates.getYear();
			monthROF = riskOfFailureDates.getMonth();
			weekROF = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();

			//Demand calcs
			//While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)
			durhamROFDemand = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands

			riskOfFailureStorageInf.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);//passes demands to storage calcs

			//Inflow Calcs
			durhamROFInflow = michieInflow[histRealizations+yearROF-startSimulationYear][weekROF-1]+littleRiverInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//Durham Inflows
			raleighROFInflow = fallsLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//Raleigh
			owasaROFInflow = owasaInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//OWASA
			wbROFInflow = lakeWBInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//Wheeler&Benson
			claytonROFInflow = claytonInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//Clayton gauge
			crabtreeROFInflow = crabtreeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];// Crabtree creek
			jordanROFInflow = jordanLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			lillingtonROFInflow = lillingtonGaugeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			littleRiverRaleighROFInflow = littleRiverRaleighInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			//Evap Calcs
			ROFevap = evaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];//Durham and OWASA evap
			fallsROFevap = fallsLakeEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];// Falls Lake evap
			wbROFevap= lakeWheelerEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];//Wheeler Benson evap

			riskOfFailureStorageInf.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);


			riskOfFailureStorageInf.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageInf.updateStorage(weekROF-1);//storage calcs

			oROFs = riskOfFailureStorageInf.getOWASAStorage();//retrieve overall storage, OWASA
			dROFs = riskOfFailureStorageInf.getDurhamStorage();//retrieve overall storage, Durham
			rROFs = riskOfFailureStorageInf.getRaleighStorage();//retrieve overall storage, Raleigh
			cROFs = riskOfFailureStorageInf.getCaryStorage();
			if (oROFs<.2)
			{
				thisTimeO = 1;//Mark OWASA failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (dROFs<.2)
			{
				thisTimeD = 1;//Mark Durham failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (rROFs<.2)
			{
				thisTimeR = 1;//Mark Raleigh failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (cROFs<.2)
			{
				thisTimeC = 1;
			}
			if(counter < 52)
			{
				riskOfFailureStorageIP.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, 	numdaysROF);//passes demands to storage calcs

				riskOfFailureStorageIP.setInflow(durhamROFInflow, //passes inflows to storage calcs
					31.4*owasaROFInflow,
					28.7*owasaROFInflow,
					1.2*owasaROFInflow,
					raleighROFInflow,
					wbROFInflow,
					claytonROFInflow,
					crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
					raleighROFDemand*returnRatio[1][weekROF-1],
					durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
					fallsROFevap,
					wbROFevap,
					ROFevap, littleRiverRaleighROFInflow);
				riskOfFailureStorageIP.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
				riskOfFailureStorageIP.updateStorage(weekROF-1);

				oIPs = riskOfFailureStorageIP.getOWASAStorage();//retrieve overall storage, OWASA
				if(oIPs < owasa.riskVolume[weekROF-1])
				{
					owasa.probReach += 1.0;
				}
				dIPs = riskOfFailureStorageIP.getDurhamStorage();//retrieve overall storage, Durham
				if(dIPs < durham.riskVolume[weekROF -1])
				{
					durham.probReach += 1.0;
				}
				rIPs = riskOfFailureStorageIP.getRaleighStorage();//retrieve overall storage, Raleigh
				if(rIPs < raleigh.riskVolume[weekROF-1])
				{
					raleigh.probReach += 1.0;
				}
				cIPs = riskOfFailureStorageIP.getCaryStorage();
				if(cIPs < cary.riskVolume[weekROF-1])
				{
					cary.probReach += 1.0;
				}
			}

			riskOfFailureDates.increase();//increase week by one
			counter++;

		}
		if (thisTimeO==1)//Fill out the risk matricies (every historical year that causes at least one week of failure results in an increase in risk of 1.28% (1/78)
		{
			owasa.infRisk += 1.0;
		}
		if (thisTimeD==1)
		{
			durham.infRisk += 1.0;
		}
		if (thisTimeR==1)
		{
			raleigh.infRisk += 1.0;
		}
		if (thisTimeC==1)
		{
			cary.infRisk += 1.0;
		}
	}
	for (int synthRealizations = startingSynthYear; synthRealizations<synthYear; synthRealizations++)// determines the year of the historical streamflow record to be used in calculations
	{
		counter = 0;
		thisTimeO=0;
		thisTimeD = 0;
		thisTimeR = 0;
		thisTimeC = 0;
		riskOfFailureDates.initializeDates(synthRealizations,1,1,7,0);//each simulation is run independently (52 week intervals, then the slate is cleaned)
		riskOfFailureStorageInf.updateReservoirStorageROF();
		riskOfFailureStorageIP.updateReservoirStorageROF(durhamS, teerS, CCRS, ULS, STQS, owasaS, lakeWBS, flSS, flQS, jlSS, jlQS, caryJordanS,  raleighJordanS,  durhamJordanS, owasaJordanS, littleRiverRalS, raleighQS);

		while (counter<78)
		{
			//Retrieving date information
			yearROF = riskOfFailureDates.getYear();
			monthROF = riskOfFailureDates.getMonth();
			weekROF = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();

			//Demand calcs
			//While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)
			durhamROFDemand = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands

			riskOfFailureStorageInf.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);//passes demands to storage calcs

			//Inflow Calcs
			syntheticIndex = (yearROF)*52+weekROF-1;

			durhamROFInflow = durhamInflows.synthetic[realization][syntheticIndex];//Durham Inflows
			raleighROFInflow = fallsInflows.synthetic[realization][syntheticIndex];//Raleigh
			owasaROFInflow = owasaInflows.synthetic[realization][syntheticIndex];//OWASA
			wbROFInflow = wheelerInflows.synthetic[realization][syntheticIndex];//Wheeler&Benson
			claytonROFInflow = claytonInflows.synthetic[realization][syntheticIndex];//Clayton gauge
			crabtreeROFInflow = crabtreeInflows.synthetic[realization][syntheticIndex];// Crabtree creek
			jordanROFInflow = jordanInflows.synthetic[realization][syntheticIndex];
			lillingtonROFInflow = lillingtonInflows.synthetic[realization][syntheticIndex];
			littleRiverRaleighROFInflow = littleRiverRaleighInflows.synthetic[realization][syntheticIndex];
			//Evap Calcs
			ROFevap = durhamOwasaEvap.synthetic[realization][syntheticIndex];//Durham and OWASA evap
			fallsROFevap = fallsEvap.synthetic[realization][syntheticIndex];// Falls Lake evap
			wbROFevap= wheelerEvap.synthetic[realization][syntheticIndex];//Wheeler Benson evap

			riskOfFailureStorageInf.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);
			riskOfFailureStorageInf.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageInf.updateStorage(weekROF-1);//storage calcs

			oROFs = riskOfFailureStorageInf.getOWASAStorage();//retrieve overall storage, OWASA
			dROFs = riskOfFailureStorageInf.getDurhamStorage();//retrieve overall storage, Durham
			rROFs = riskOfFailureStorageInf.getRaleighStorage();//retrieve overall storage, Raleigh
			cROFs = riskOfFailureStorageInf.getCaryStorage();
			if (oROFs<.2)
			{
				thisTimeO = 1;//Mark OWASA failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (dROFs<.2)
			{
				thisTimeD = 1;//Mark Durham failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (rROFs<.2)
			{
				thisTimeR = 1;//Mark Raleigh failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (cROFs<.2)
			{
				thisTimeC = 1;
			}
			if(counter < 52)
			{
				riskOfFailureStorageIP.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, 	numdaysROF);//passes demands to storage calcs

				riskOfFailureStorageIP.setInflow(durhamROFInflow, //passes inflows to storage calcs
					31.4*owasaROFInflow,
					28.7*owasaROFInflow,
					1.2*owasaROFInflow,
					raleighROFInflow,
					wbROFInflow,
					claytonROFInflow,
					crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
					raleighROFDemand*returnRatio[1][weekROF-1],
					durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
					fallsROFevap,
					wbROFevap,
					ROFevap, littleRiverRaleighROFInflow);
				riskOfFailureStorageIP.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
				riskOfFailureStorageIP.updateStorage(weekROF-1);

				oIPs = riskOfFailureStorageIP.getOWASAStorage();//retrieve overall storage, OWASA
				if(oIPs < owasa.riskVolume[weekROF-1])
				{
					owasa.probReach += 1.0;
				}
				dIPs = riskOfFailureStorageIP.getDurhamStorage();//retrieve overall storage, Durham
				if(dIPs < durham.riskVolume[weekROF -1])
				{
					durham.probReach += 1.0;
				}
				rIPs = riskOfFailureStorageIP.getRaleighStorage();//retrieve overall storage, Raleigh
				if(rIPs < raleigh.riskVolume[weekROF-1])
				{
					raleigh.probReach += 1.0;
				}
				cIPs = riskOfFailureStorageIP.getCaryStorage();
				if(cIPs < cary.riskVolume[weekROF-1])
				{
					cary.probReach += 1.0;
				}
			}
			riskOfFailureDates.increase();//increase week by one
			counter++;
		}
		if (thisTimeO==1)//Fill out the risk matricies (every historical year that causes at least one week of failure results in an increase in risk of 1.28% (1/78)
		{
			owasa.infRisk += 1.0;
		}
		if (thisTimeD==1)
		{
			durham.infRisk += 1.0;
		}
		if (thisTimeR==1)
		{
			raleigh.infRisk += 1.0;
		}
		if (thisTimeC==1)
		{
			cary.infRisk += 1.0;
		}

	}
	owasa.infRisk = owasa.infRisk/(double(numRiskYears));
	owasa.probReach = owasa.probReach/(double(numRiskYears));
	durham.infRisk = durham.infRisk/(double(numRiskYears));
	durham.probReach = durham.probReach/(double(numRiskYears));
	raleigh.infRisk = raleigh.infRisk/(double(numRiskYears));
	raleigh.probReach = raleigh.probReach/(double(numRiskYears));
	cary.infRisk = cary.infRisk/(double(numRiskYears));
	cary.probReach = cary.probReach/(double(numRiskYears));
        // results in the infra ROF and expected number of weeks per year in failure

	return;
}
void Simulation::triggerInfrastructure(int realization)
{
	int owasaIndex;
	int durhamIndex;
	int raleighIndex;
	if(owasa.infRisk>owasa.infTrigger)
	{
		owasaIndex = owasa.startNewInfrastructure(year);
		switch(owasaIndex)
		{

			case 0:
				owasa.addDebt(year, realization, owasa.infMatrix[0][4], bondLength, bondRate);
				owasa.infMatrix[0][3] = 1.0;
				riskOfFailureStorageInf.buildULexp();
				break;
			case 1:
				owasa.addDebt(year, realization, owasa.infMatrix[1][4], bondLength, bondRate);
				owasa.infMatrix[1][3] = 1.0;
				riskOfFailureStorageInf.buildCCexp();
				break;
			case 2:
				owasa.addDebt(year, realization, owasa.infMatrix[2][4], bondLength, bondRate);
				owasa.infMatrix[2][3] = 1.0;
				riskOfFailureStorageInf.buildSQlow();
				break;
			case 3:
				if(owasa.infMatrix[2][1]<1.0)
				{
					owasa.addDebt(year, realization, owasa.infMatrix[3][4], bondLength, bondRate);
				}
				else
				{
					owasa.addDebt(year, realization, owasa.infMatrix[3][4]-owasa.infMatrix[2][4], bondLength, bondRate);
				}
				owasa.infMatrix[3][3] = 1.0;
				owasa.infMatrix[2][1] = 2.0;
				riskOfFailureStorageInf.buildSQhigh();

				break;
			case 4:
				owasa.addDebt(year, realization, owasa.infMatrix[4][4], bondLength, bondRate);
				durham.addDebt(year, realization, durham.infMatrix[5][4], bondLength, bondRate);
				raleigh.addDebt(year, realization, raleigh.infMatrix[4][4], bondLength, bondRate);
				owasa.infMatrix[4][3] = 1.0;
				durham.infMatrix[5][1] = 2.0;
				raleigh.infMatrix[4][1] = 2.0;
				riskOfFailureStorageInf.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
				break;
			case 5:
				if(owasa.infMatrix[4][1]<1.0)
				{
					owasa.addDebt(year, realization, owasa.infMatrix[5][4], bondLength, bondRate);
					durham.addDebt(year, realization, durham.infMatrix[6][4], bondLength, bondRate);
					raleigh.addDebt(year, realization, raleigh.infMatrix[5][4], bondLength, bondRate);
				}
				else
				{
					owasa.addDebt(year, realization, owasa.infMatrix[5][4] - owasa.infMatrix[4][4], bondLength, bondRate);
					durham.addDebt(year, realization, durham.infMatrix[6][4] - durham.infMatrix[5][4], bondLength, bondRate);
					raleigh.addDebt(year, realization, raleigh.infMatrix[5][4] - raleigh.infMatrix[4][4], bondLength, bondRate);
				}
				owasa.infMatrix[5][3] = 1.0;
				durham.infMatrix[5][1] = 2.0;
				raleigh.infMatrix[4][1] = 2.0;
				owasa.infMatrix[4][1]  = 2.0;
				durham.infMatrix[6][1] = 2.0;
				raleigh.infMatrix[5][1] = 2.0;
				riskOfFailureStorageInf.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
				break;
		}

	}
	if(durham.infRisk>durham.infTrigger)
	{
		durhamIndex = durham.startNewInfrastructure(year);
		switch(durhamIndex)
		{
			case 0:
				durham.addDebt(year, realization, durham.infMatrix[0][4], bondLength, bondRate);
				durham.infMatrix[0][3] = 1.0;
				riskOfFailureStorageInf.buildTeerQuarry();
				owasa.addDebt(year, realization, durham.infMatrix[3][4], bondLength, bondRate);
				break;
			case 1:
				durham.addDebt(year, realization, durham.infMatrix[1][4], bondLength, bondRate);
				durham.infMatrix[1][3] = 1.0;
				riskOfFailureStorageInf.buildReclaimedLow();
				break;
			case 2:
				if(durham.infMatrix[1][1]<1.0)
				{
					durham.addDebt(year, realization, durham.infMatrix[2][4], bondLength, bondRate);
				}
				else
				{
					durham.addDebt(year, realization, durham.infMatrix[2][4] - durham.infMatrix[1][4], bondLength, bondRate);
				}
				durham.infMatrix[2][3] = 1.0;
				durham.infMatrix[1][1] = 2.0;
				riskOfFailureStorageInf.buildReclaimedHigh();
				break;
			case 3:
				durham.addDebt(year, realization, durham.infMatrix[3][4], bondLength, bondRate);
				durham.infMatrix[3][3] = 1.0;
				riskOfFailureStorageInf.buildMichieLow();
			case 4:
				if(durham.infMatrix[3][1]<1.0)
				{
					durham.addDebt(year, realization, durham.infMatrix[4][4], bondLength, bondRate);
				}
				else
				{
					durham.addDebt(year, realization, durham.infMatrix[4][4] - durham.infMatrix[3][4], bondLength, bondRate);
				}
				durham.infMatrix[4][3] = 1.0;
				durham.infMatrix[3][1] = 2.0;
				riskOfFailureStorageInf.buildMichieHigh();
				break;
			case 5:
				owasa.addDebt(year, realization, owasa.infMatrix[4][4], bondLength, bondRate);
				durham.addDebt(year, realization, durham.infMatrix[5][4], bondLength, bondRate);
				raleigh.addDebt(year, realization, raleigh.infMatrix[4][4], bondLength, bondRate);
				durham.infMatrix[5][3] = 1.0;
				owasa.infMatrix[4][1] = 2.0;
				raleigh.infMatrix[4][1] = 2.0;
				riskOfFailureStorageInf.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
				break;
			case 6:
				if(owasa.infMatrix[4][1]<1.0)
				{
					owasa.addDebt(year, realization, owasa.infMatrix[5][4], bondLength, bondRate);
					durham.addDebt(year, realization, durham.infMatrix[6][4], bondLength, bondRate);
					raleigh.addDebt(year, realization, raleigh.infMatrix[5][4], bondLength, bondRate);
				}
				else
				{
					owasa.addDebt(year, realization, owasa.infMatrix[5][4] - owasa.infMatrix[4][4], bondLength, bondRate);
					durham.addDebt(year, realization, durham.infMatrix[6][4] - durham.infMatrix[5][4], bondLength, bondRate);
					raleigh.addDebt(year, realization, raleigh.infMatrix[5][4] - raleigh.infMatrix[4][4], bondLength, bondRate);
				}
				durham.infMatrix[6][3] = 1.0;
				durham.infMatrix[5][1] = 2.0;
				raleigh.infMatrix[4][1] = 2.0;
				owasa.infMatrix[4][1]  = 2.0;
				owasa.infMatrix[5][1] = 2.0;
				raleigh.infMatrix[5][1] = 2.0;
				riskOfFailureStorageInf.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
				break;
		}
	}
	if(raleigh.infRisk>raleigh.infTrigger)
	{

		raleighIndex = raleigh.startNewInfrastructure(year);
		switch(raleighIndex)
		{
			case 0:
				raleigh.addDebt(year, realization, raleigh.infMatrix[0][4], bondLength, bondRate);
				raleigh.infMatrix[0][3] = 1.0;
				riskOfFailureStorageInf.buildLittleRiverRal();
				break;
			case 1:
				raleigh.addDebt(year, realization, raleigh.infMatrix[1][4], bondLength, bondRate);
				raleigh.infMatrix[1][3] = 1.0;
				riskOfFailureStorageInf.buildRalQuarry();
				break;
			case 2:
				raleigh.addDebt(year, realization, raleigh.infMatrix[2][4], bondLength, bondRate);
				raleigh.infMatrix[2][3] = 1.0;
				riskOfFailureStorageInf.buildNeuseIntake();
				break;
			case 3:
				raleigh.addDebt(year, realization, raleigh.infMatrix[3][4], bondLength, bondRate);
				raleigh.infMatrix[3][3] = 1.0;
				riskOfFailureStorageInf.reallocateFallsLake(fallsLakeReallocation);
				break;
			case 4:
				owasa.addDebt(year, realization, owasa.infMatrix[4][4], bondLength, bondRate);
				durham.addDebt(year, realization, durham.infMatrix[5][4], bondLength, bondRate);
				raleigh.addDebt(year, realization, raleigh.infMatrix[4][4], bondLength, bondRate);
				raleigh.infMatrix[4][3] = 1.0;
				owasa.infMatrix[4][1] = 2.0;
				durham.infMatrix[5][1] = 2.0;
				riskOfFailureStorageInf.buildWWWTPlow(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
				break;
			case 5:
				if(owasa.infMatrix[4][1]<1.0)
				{
					owasa.addDebt(year, realization, owasa.infMatrix[5][4], bondLength, bondRate);
					durham.addDebt(year, realization, durham.infMatrix[6][4], bondLength, bondRate);
					raleigh.addDebt(year, realization, raleigh.infMatrix[5][4], bondLength, bondRate);
				}
				else
				{
					owasa.addDebt(year, realization, owasa.infMatrix[5][4] - owasa.infMatrix[4][4], bondLength, bondRate);
					durham.addDebt(year, realization, durham.infMatrix[6][4] - durham.infMatrix[5][4], bondLength, bondRate);
					raleigh.addDebt(year, realization, raleigh.infMatrix[5][4] - raleigh.infMatrix[4][4], bondLength, bondRate);
				}
				raleigh.infMatrix[5][3] = 1.0;
				durham.infMatrix[5][1] = 2.0;
				raleigh.infMatrix[4][1] = 2.0;
				owasa.infMatrix[4][1]  = 2.0;
				owasa.infMatrix[5][1] = 2.0;
				raleigh.infMatrix[5][1] = 2.0;
				riskOfFailureStorageInf.buildWWWTPhigh(owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac);
				break;
		}
	}
return;
}

void Simulation::realizationLoop()
{
	double durhamActualInflow, owasaActualInflow, fallsActualInflow, wbActualInflow, claytonActualInflow;
	double crabtreeActualInflow, jordanActualInflow, lillingtonActualInflow,actualEvap, actualFallsEvap, actualWBEvap;
	double littleRiverRaleighActualInflow;
	int season = 1, syntheticIndex = 0; // week 1 is non-irrigation season (season = 1)
	double raleighBaselineMultiplier = 40434.0*.32*(falls_lake_supply_capacity/(falls_lake_supply_capacity + falls_lake_wq_capacity));
	double durham_res_supply_capacity = 6349.0;
	double cane_creek_supply_capacity = 2909.0;
	double stone_quarry_supply_capacity = 200.0;
	double university_lake_supply_capacity = 449.0;
	double lake_wheeler_benson_supply_capacity = 2789.66;
	double raleigh_durham_capacity = 10.0;
	double teer_quarry_supply_capacity = 1315.0;
	double teer_quarry_intake_capacity = 0.0;
	double teer_quarry_outflow_capacity = 0.0;
	double little_river_raleigh_supply_capacity = 0.0;
	double western_wake_treatment_capacity = 0.0;
	double durham_reclaimation_capacity = 0.0;
	double raleigh_quarry_capacity = 4000.0;
	double raleigh_quarry_intake_capacity = 0.0;
	double raleigh_quarry_outflow_capacity = 0.0;
	double raleigh_intake_capacity = 0.0;
	double cary_quarry_capacity = 0.0;
	double cary_quarry_intake_capacity = 0.0;
	double cary_quarry_outflow_capacity = 0.0;
	
	/////////////////////////////////////////////////////////////////////////
	///////// PUT RELEASE CONSTRAINTS HERE (DEPENDS ON CONTRACT) ////////////
	
	//double LMreleaseCap = 2000.0;
		// assume that the max release allowed is 20% of total Durham storage 
		// but that this is an additional, firm ceiling constraint
		// now included above, read in the parameter input file
	double LMreleaseMin = 0.0;
		// no minimum contractual release
	//double FallsSupplyAllocationFraction = 0.423;
	double FLSPreleaseFrac;
		// fraction of FL conservation pool dedicated to
		// water supply storage (the rest is the WQ pool)
		// now in parameter input file
	double NearFailureLimit = 0.2;
		// if FL storage is below this, any releases 
		// will go 100% into the water supply pool 
	
	/////////////////////////////////////////////////////////////////////////
	///////// CONTRACT SPECIFICATIONS FOR RELEASES HERE /////////////////////
	
	// int allowReleases = 1;
		// logical to decide if releases are allowed or not
		// specified in trianglesimulation 
		
	// numIntervals = 20;
		// the number of discrete intervals of reservoir volume
		// that will be tested in the createReservoirTargets function
		// THIS IS SPECIFIED IN TRIANGLESIMULATIONSCRIPT
	
	//double ReleaseContractLength = 100.0*52;
		// number of weeks the contract is good
		// replaced by contractlength variable 
	//double ReleaseContractPrice = 30000000.0/1000000.0;
		// total cost of the releases contract in millions, to be paid on an annual basis?
		// now included in parameter input file
	
	double RcriticalStorageLevel = 1.00;
	double DcriticalStorageLevel = 0.25;
		// currently arbitrarily set, later should be optimized
		// previous to March 15, 2016, releases were based on 
		// R and D reservoir levels alone, now they are based on
		// the same ROF values as treated transfers

	//double buybackratePerMG = 3500.0/1000000.0;
		// currently equal to cost per quantity of treated transfer
		// MAY 2016: RAISED TO 3500
		// now included in parameter input file
	
	// MAY 2016 CHANGES - INCREASED TO 16 RANKS -----------------------------------
	
	rank = solutionNumber;
		// solutionNumber is assigned as rank in triangleSimulation.cpp
	
	// ALL OF THIS COMMENTED CHUNK IS NOW DEALT WITH IN THE PARAMETER INPUT FILE 
	/* durham.RRtrigger  = 0.02;
	durham.TTriggerN  = 0.02;
		// keep this firm at a very low level to preserve durham risk
	
	if (rank < 8)
	{
		raleigh.RRtrigger = 0.02 + rank * 0.02;
			// vary this at 2% increasing by rank
	}
	else
	{
		raleigh.RRtrigger = 0.02 + (rank-8) * 0.02;
			// vary this at 2% increasing by rank
	}
		// for now, I am running 16 ranks and I want 
		// 2 matching sets of 8 runs
		// because half will be done without releases and half with them.
		
	raleigh.TTriggerN = 0.02;
		// hold this low across all ranks */
	
	// ----------------------------------------------------------------------------

	/////////////////////////////////////////////////////////////////////////
	///////////////// CREATE OUTPUT NAMES AND LOCATIONS /////////////////////
	
	if (printDetailedOutput)
	{
		std::string filenameA = "output/JLTreatedTransfers";
		std::string filenameC = "output/RRfuncOutput";
		std::string filenameD = "output/weeklyRiskParams";
		std::string filenameG = "output/InfraBuilt"; 
		
		std::string filenameEND = ".csv";
			
		std::string completeFilenameA;
		std::string completeFilenameC;
		std::string completeFilenameD;
		std::string completeFilenameG;
			
		std::stringstream sstmA;
		std::stringstream sstmC;
		std::stringstream sstmD;
		std::stringstream sstmG;
			
		sstmA << filenameA << rank << filenameEND;
		sstmC << filenameC << rank << filenameEND;
		sstmD << filenameD << rank << filenameEND;
		sstmG << filenameG << solutionNumber << "_" << rdmNumber << filenameEND;
		
		completeFilenameA = sstmA.str();
		completeFilenameC = sstmC.str();
		completeFilenameD = sstmD.str();
		completeFilenameG = sstmG.str();
		
		openFile(out100, completeFilenameA);
		openFile(outNew, completeFilenameC);
		openFile(outRiskParams, completeFilenameD);
		openFile(InfraBuilt, completeFilenameG);
			// all these are defined in the header file 
		
		InfraBuilt << "Solution" << "," << "RDMnum" << "," << "Realization" << "," << "Year" << "," << "Utility" << "," << "Project" << endl;
			// column headers for infrastructure builds 
		
		out100 << "Rank" << "," << "Realization" << "," << "Year" << "," << "Week" << ",";
		out100 << "RaleighDirectTransferVolume" << "," << "DurhamDirectTransferVolume" << ",";
		out100 << "RaleighIndirectTransferVolume" << "," << "DurhamIndirectTransferVolume" << ",";
		out100 << "OWASATransferVolume" << "," << "OWASAExtraVolume" << ",";
		out100 << "RaleighShorttermROF" << "," << "DurhamShorttermROF" << "," << "OWASAshorttermROF" << endl;
			// csv column headers of treated transfer output data
			
		outNew << "Rank" << "," << "Realization" << "," << "Year" << "," << "Week" << ",";
		outNew << "RaleighStorageRatio" << "," << "RaleighActualStorage" << "," << "RsupplyCapacity" << "," << "DurhamStorageRatio" << "," << "DurhamSpillage" << ",";
		outNew << "ReleaseRequest" << "," << "BuybackQuantity" << "," << "BuybackStorageLevel" << "," << "FLsupplyStorage" << "," << "DurhamActualStorage" << ",";
		outNew << "RaleighTargetStorageFraction" << "," << "DurhamTargetStorageFraction" << "," << "DurhamTargetBuybackLevel" << ",";
		outNew << "ReleaseMaxLimit" << "," << "AdjustedRequestLogical" << "," << "MinimumEnvReleaseToFL" << ",";
		outNew << "FLqualityStorage" << "," << "FLqualityCapacity" << "," << "FLsupplyCapacity" << "," << "DsupplyCapacity" << "," << "ReleaseToFLSupplyFraction" << endl;
			// csv column headers for raw release output data and storage levels
			
		outRiskParams << "Rank" << "," << "Realization" << ",";
		outRiskParams << "RaleighRRtrigger" << "," << "RaleighTTtrigger" << "," << "RaleighINFtrigger" << ",";
		outRiskParams << "DurhamRRtrigger"  << "," << "DurhamTTtrigger"  << "," << "DurhamINFtrigger"  << ",";
		outRiskParams << "RaleighJLalloc" << "," << "DurhamJLalloc" << "," << "JLallocationCap" << endl;
			// csv column headers for csv containing ROF values
	}
	
	////////////// OTHER PARAMS /////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
		
	int weekcounter;
	int contractriskyearcounter;
	int LOOPCHECKER;
	int contractcount;
	
	fallsFailurePoint = 0.2;
	zeroes(totalFallsFailure, terminateYear);
	maxFallsFailure = 0.0;
        // setting initial infrastructure values, storage and capacities
        // must all be equal to 0 at beginning of each realization, before 60yrs go by

	//systemStorage.openResFiles();
	//openFile(out1,"raleighDemand.csv");
	//openFile(out3,"riskOfFailureFile.csv");
	
	///////////// RUN THE LOOP //////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	
	for (int realization = 0; realization < numRealizations; realization++)
	{
		///////////////////// RESET COUNTERS /////////////////////////////////////////////////////////////////////////////
		
		weekcounter = 0;
		yearcounter = 0;
		contractriskyearcounter = 0;
		allowReleaseContract = true;
		contractcount = 1;
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		// Initialize demand and reservoir storage objects (year, month, week, daysPerWeek, leapYearCounter)
		simDates.initializeDates(startSimulationYear,1,1,7,0);

		systemStorage.initializeReservoirStorage(durham_res_supply_capacity,
													cane_creek_supply_capacity, stone_quarry_supply_capacity,university_lake_supply_capacity, lake_wheeler_benson_supply_capacity, falls_lake_supply_capacity, falls_lake_wq_capacity,
													jordan_lake_supply_capacity, jordan_lake_wq_capacity, cary_treatment_capacity, durham_cary_capacity, durham_owasa_capacity, raleigh_cary_capacity, raleigh_durham_capacity, 
													raleigh.jordanLakeAlloc, durham.jordanLakeAlloc, owasa.jordanLakeAlloc, cary.jordanLakeAlloc, teer_quarry_supply_capacity, teer_quarry_intake_capacity, teer_quarry_outflow_capacity, 
													little_river_raleigh_supply_capacity, western_wake_treatment_capacity, durham_reclaimation_capacity, raleigh_quarry_capacity, raleigh_quarry_intake_capacity, 
													raleigh_quarry_outflow_capacity, raleigh_intake_capacity, cary_quarry_capacity, cary_quarry_intake_capacity, cary_quarry_outflow_capacity, 
													owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac,1,
													LMreleaseCap, LMreleaseMin); 
														// infrastructure included in the model
														// MICHIE RELEASE MIN AND CAP INCLUDED HERE (BY DAVID)
            // actual reservoir storage

		riskOfFailureStorageInf.initializeReservoirStorageROF(durham_res_supply_capacity,
																cane_creek_supply_capacity,	stone_quarry_supply_capacity,				
																university_lake_supply_capacity, lake_wheeler_benson_supply_capacity,falls_lake_supply_capacity, 
																falls_lake_wq_capacity, jordan_lake_supply_capacity, jordan_lake_wq_capacity, cary_treatment_capacity, 
																durham_cary_capacity, durham_owasa_capacity, raleigh_cary_capacity, raleigh_durham_capacity,raleigh.jordanLakeAlloc, 
																durham.jordanLakeAlloc, owasa.jordanLakeAlloc, cary.jordanLakeAlloc, teer_quarry_supply_capacity, 
																teer_quarry_intake_capacity, teer_quarry_outflow_capacity, little_river_raleigh_supply_capacity, 
																western_wake_treatment_capacity, durham_reclaimation_capacity, raleigh_quarry_capacity, 
																raleigh_quarry_intake_capacity, raleigh_quarry_outflow_capacity, raleigh_intake_capacity, 
																cary_quarry_capacity, cary_quarry_intake_capacity, cary_quarry_outflow_capacity, 
																owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac,0,
																LMreleaseCap, LMreleaseMin);
            // used once per year to calculate ROF

		riskOfFailureStorageROF.initializeReservoirStorageROF(durham_res_supply_capacity,
																cane_creek_supply_capacity,	stone_quarry_supply_capacity,					
																university_lake_supply_capacity, lake_wheeler_benson_supply_capacity,			
																falls_lake_supply_capacity, falls_lake_wq_capacity,					
																jordan_lake_supply_capacity, jordan_lake_wq_capacity,					
																cary_treatment_capacity, durham_cary_capacity, durham_owasa_capacity, raleigh_cary_capacity, 
																raleigh_durham_capacity, raleigh.jordanLakeAlloc, durham.jordanLakeAlloc, owasa.jordanLakeAlloc, 
																cary.jordanLakeAlloc, teer_quarry_supply_capacity, teer_quarry_intake_capacity, teer_quarry_outflow_capacity, 
																little_river_raleigh_supply_capacity, western_wake_treatment_capacity, durham_reclaimation_capacity, 
																raleigh_quarry_capacity, raleigh_quarry_intake_capacity, raleigh_quarry_outflow_capacity, 
																raleigh_intake_capacity, cary_quarry_capacity, cary_quarry_intake_capacity, cary_quarry_outflow_capacity, 
																owasa.westernWakeTreatmentFrac, durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac,0,
																LMreleaseCap, LMreleaseMin);
            // calculated every week using current storage

		riskOfFailureStorageIP.initializeReservoirStorageROF(durham_res_supply_capacity,
																cane_creek_supply_capacity,	stone_quarry_supply_capacity,					
																university_lake_supply_capacity, lake_wheeler_benson_supply_capacity,			
																falls_lake_supply_capacity, falls_lake_wq_capacity,					
																jordan_lake_supply_capacity, jordan_lake_wq_capacity,					
																cary_treatment_capacity, durham_cary_capacity, durham_owasa_capacity, raleigh_cary_capacity, 
																raleigh_durham_capacity, raleigh.jordanLakeAlloc, durham.jordanLakeAlloc, owasa.jordanLakeAlloc, 
																cary.jordanLakeAlloc, teer_quarry_supply_capacity, teer_quarry_intake_capacity, teer_quarry_outflow_capacity, 
																little_river_raleigh_supply_capacity, western_wake_treatment_capacity, durham_reclaimation_capacity, 
																raleigh_quarry_capacity, raleigh_quarry_intake_capacity, raleigh_quarry_outflow_capacity, raleigh_intake_capacity, 
																cary_quarry_capacity, cary_quarry_intake_capacity, cary_quarry_outflow_capacity, owasa.westernWakeTreatmentFrac, 
																durham.westernWakeTreatmentFrac, raleigh.westernWakeTreatmentFrac,0,
																LMreleaseCap, LMreleaseMin);
            // based on weekly changes this calculates insurance payouts

		year = simDates.getYear();//passes the dates from the simDates class to the main simulation
		month = simDates.getMonth();
		week = simDates.getWeek();
		numdays = simDates.getDays();
		
		if (yearcounter == 0)
		{
			int firstyear = year;
			adjustedannualpayment = annualpayment;
			adjustedbuybackpayment = buybackratePerMG;
		}

		durham.clearVariablesForRealization(year);
		owasa.clearVariablesForRealization(year);
		raleigh.clearVariablesForRealization(year);
		cary.clearVariablesForRealization(year);
            // another level of resetting

		durham.priceInsurance(year, realization);
		owasa.priceInsurance(year, realization);
		raleigh.priceInsurance(year, realization);
		cary.priceInsurance(year, realization);

		int caryWTPcounter = 0;
		double durhamReclaimedCap = durham_reclaimation_capacity;
		double durhamTreatmentCap = western_wake_treatment_capacity*durham.westernWakeTreatmentFrac;
		double owasaTreatmentCap = western_wake_treatment_capacity*owasa.westernWakeTreatmentFrac;
		double raleighTreatmentCap = western_wake_treatment_capacity*raleigh.westernWakeTreatmentFrac;
		double raleighIntakeCap = raleigh_intake_capacity;
		zeroes(caryBuild,3);
		thisYearFalls = 0;
		
		while (year-1<(terminateYear))
            // in a single realization, from year to year
		{
			// July 2016: new weekly order of operations for non-structural options
			//	0.	Dates are updated, etc.
			//	1. 	ROF calculated for insurance and releases 
			//	2.	Releases calculated, storage and spillage re-set 
			//	3.	ROF for transfers and restrictions calculated 
			//	4.	Restrictions imposed, demand recalculated
			//	5.	Treated transfers calculated, storage etc. updated
			
			LOOPCHECKER = 0;
			
			syntheticIndex = (year-1)*52+week-1;

			// update on/off triggers for each restriction stage
			if (week > 16 && week < 39) 
				// Irrigation season
				season = 0;
			else
				season = 1;


			createRiskOfFailure_InsuranceReleases(realization, year, durham.averageUse, owasa.averageUse, raleigh.averageUse, cary.averageUse,
												  numIntervals);
                // gives the ROF of this given week
				// July 2016: this function just does ROF for insurance and releases 

			if (allowReleases == 1 && allowReleaseContract)
				// allow transfers, apply raw releases 
				// in a given week, transfers calculated before transfers
				// however, ROF is not updated.  this should work out as long as
				// release quantities are not capped at a low level
				// (think: first week of a month of triggered ROF will see large
				// transfers and releases, but the releases in first week will lead to
				// no transfers in following weeks)
			{	
				/////////////////////////////////////////////////////////////////////////////////////////////////////////
				/////////////////////////// CALCULATE RELEASES HERE /////////////////////////////////////////////////////
				
				if (systemStorage.getRaleighStorage() < NearFailureLimit)
				{
					FLSPreleaseFrac = 1.0;
						// if Raleigh's total storage is below 30%
						// any request releases can be completely used to
						// augment water supply, rather than be split between
						// water supply and water quality FL pools
				}
				else
				{
					if (indepReleaseAlloc)
					{
						FLSPreleaseFrac = FallsSupplyAllocationFraction;
					}
					else
					{
						FLSPreleaseFrac = systemStorage.getFallsSupplyAllocFrac();
					}
						// the input parameter for how much of each release goes to water supply
						// is overwritten to be equal to the current conservation pool ratio 
				}
				
				systemStorage.calcRawReleases(LMreleaseCap, LMreleaseMin, RcriticalStorageLevel, DcriticalStorageLevel, 
											  raleigh.ReleaseRiskVolume[week-1], durham.ReleaseRiskVolume[week-1], FLSPreleaseFrac, durham.BuybackRiskVolume[week-1],
											  realization, outNew, year, week, numRealizationsTOREAD, rank, printDetailedOutput);
												
				durham.weeklyBuybackVolume = systemStorage.getDurhamBuybackRequest();
				raleigh.weeklyBuybackVolume = systemStorage.getDurhamBuybackRequest();
				raleigh.weeklyReleaseVolume = systemStorage.getRaleighReleases();
				
				if (durham.weeklyBuybackVolume > 0.0)
				{
					durham.payForBuybacks(buybackratePerMG);
					raleigh.acceptBuybackPayment(buybackratePerMG);
				}
				
				if (week == 1)
				{
					raleigh.payForReleases(adjustedannualpayment, contractlength);
					durham.acceptReleasePayment(adjustedannualpayment, contractlength);
						// annual payment
						// because payment type has been changed to annual type,
						// the contract length variable is unnecessary 
				}
				
				
				//Update reservoir storage levels
				systemStorage.setSpillover(week-1);
				systemStorage.updateStorage(week-1);
					// set min releases
			}
			
			createRiskOfFailure_RestrictionsTransfers(realization, year, durham.averageUse, owasa.averageUse, raleigh.averageUse, cary.averageUse);
				// generate the week's ROF for restrictions and transfers
				// which are applied after releases are in a week 
			
			durham.fillRestrictionsArray(season);
			owasa.fillRestrictionsArray(season);
			cary.fillRestrictionsArray(season);
			raleigh.fillRestrictionsArray(season);
			
			// Use the demand PDF to estimate uncertain demand, also finds current level of restrictions (and revenue losses from them)
			// Raleigh uses Durham's data for the inflow-demand PDF
			durham.calculateDemand(realization, week, numdays, year);
			owasa.calculateDemand(realization, week, numdays, year);
			cary.calculateDemand(realization, week, numdays, year);
			raleigh.calculateDemand(realization, week, numdays, year);
                // give unrestricted demand in each week
				
			durham.calculateRestrictions(year, week, numdays, month, realization);
			owasa.calculateRestrictions(year, week, numdays, month, realization);
			cary.calculateRestrictions(year, week, numdays, month, realization);
			raleigh.calculateRestrictions(year, week, numdays, month, realization);
                // is ROF high enough to implement restrictions?

			systemStorage.setDemands(durham.weeklyDemand, owasa.weeklyDemand, raleigh.weeklyDemand, cary.weeklyDemand, numdays);
				//pass weekly demand values to reservoir storage update function

			//Translate inflows and evap from weekly variations to a physical volume
			durhamActualInflow = durhamInflows.synthetic[realization][syntheticIndex];
			owasaActualInflow = owasaInflows.synthetic[realization][syntheticIndex];
			fallsActualInflow = fallsInflows.synthetic[realization][syntheticIndex];
			wbActualInflow = wheelerInflows.synthetic[realization][syntheticIndex];
			claytonActualInflow = claytonInflows.synthetic[realization][syntheticIndex];
			crabtreeActualInflow = crabtreeInflows.synthetic[realization][syntheticIndex];
			jordanActualInflow = jordanInflows.synthetic[realization][syntheticIndex];
			lillingtonActualInflow = lillingtonInflows.synthetic[realization][syntheticIndex];
			littleRiverRaleighActualInflow = littleRiverRaleighInflows.synthetic[realization][syntheticIndex];

			actualEvap = durhamOwasaEvap.synthetic[realization][syntheticIndex];
			actualFallsEvap = fallsEvap.synthetic[realization][syntheticIndex];
			actualWBEvap = wheelerEvap.synthetic[realization][syntheticIndex];
                // real inflow and evap records

			//Pass along inflow values
			systemStorage.setInflow(durhamActualInflow, 31.4*owasaActualInflow,
				28.7*owasaActualInflow, 1.2*owasaActualInflow, fallsActualInflow, wbActualInflow, claytonActualInflow, crabtreeActualInflow, jordanActualInflow, lillingtonActualInflow,
							raleigh.weeklyDemand*returnRatio[1][week-1], durham.weeklyDemand*returnRatio[0][week-1], durham.weeklyDemand*(returnRatio[1][week-1]-returnRatio[0][week-1]),
							owasa.weeklyDemand*returnRatio[1][week-1],actualFallsEvap, actualWBEvap, actualEvap, littleRiverRaleighActualInflow);
			
			if (formulation > 0)
			{
				//Transfer requests are granted based on the limitations of infrastructure
				systemStorage.calcTransfers(durham.TTriggerN,durham.riskOfFailure, owasa.TTriggerN, owasa.riskOfFailure, raleigh.TTriggerN, raleigh.riskOfFailure, owasa.weeklyDemand);
                    // who gets transfers based on want and availability

				durham.weeklyTransferVolume = systemStorage.getDurhamTransfers();
				owasa.weeklyTransferVolume = systemStorage.getOWASATransfers();
				raleigh.weeklyTransferVolume = systemStorage.getRaleighTransfers();
                    // each utility assigned the costs of the transfers they get
					
				if (allowReleases == 1)
				{
					if (raleigh.weeklyTransferVolume > 0)
					{
						raleigh.TransferHistory[contractriskyearcounter]   += raleigh.weeklyTransferVolume;
						raleigh.TransferFrequency[contractriskyearcounter] += 1;
					}
					
					if (durham.weeklyTransferVolume > 0)
					{
						durham.TransferHistory[contractriskyearcounter]   += durham.weeklyTransferVolume;
						durham.TransferFrequency[contractriskyearcounter] += 1;
					}					
				}
					// collect transfer information of most recent 20 years for release contract negotiation 
				
				if ((realization < numRealizationsTOREAD) && printDetailedOutput) 
				{
					out100 << rank << "," << realization << "," << year << "," << week << ",";
					out100 << systemStorage.raleighDirect << "," << systemStorage.durhamDirect << ",";
					out100 << systemStorage.raleighIndirect << "," << systemStorage.durhamIndirect << ",";
					out100 << systemStorage.owasaDirect << "," << systemStorage.extraCap << ",";
					out100 << raleigh.riskOfFailure << "," << durham.riskOfFailure << "," << owasa.riskOfFailure << endl;
				}
				
				durham.payForTransfers(transferCosts);
				owasa.payForTransfers(transferCosts);
				raleigh.payForTransfers(transferCosts);
					
				if((durham.weeklyTransferVolume+owasa.weeklyTransferVolume+raleigh.weeklyTransferVolume)>0.0)
				{
					cary.Fund.add((durham.weeklyTransferVolume + owasa.weeklyTransferVolume + raleigh.weeklyTransferVolume)*transferCosts);
						// Use the mitigation fund to calculate water transfer payments (sent to Cary)
				}
				
				//Update reservoir storage levels
				systemStorage.setSpillover(week-1);
				systemStorage.updateStorage(week-1);
					// set min releases
				
				weekcounter += 1;
			}
			
			//Update reservoir storage levels
			systemStorage.setSpillover(week-1);
			systemStorage.updateStorage(week-1);
                // set min releases

			//Retrieve the weekly transfers
			// Get current weekly demand baseline values for each utility
			durhamFlowWeekBaseline = durham.demandBaseline[year-1][week-1] - durhamReclaimedInsuranceTrigger*durhamReclaimedCap*numdays - wwWTPInsuranceTrigger*durhamTreatmentCap*numdays;
			owasaFlowWeekBaseline = owasa.demandBaseline[year-1][week-1]  - wwWTPInsuranceTrigger*owasaTreatmentCap*numdays;
			raleighFlowWeekBaseline = raleigh.demandBaseline[year-1][week-1] + fallsEvap.averages[week-1]*raleigh.Fund.insuranceStorage*raleighBaselineMultiplier - wwWTPInsuranceTrigger*raleighTreatmentCap*numdays - ralIntakeInsuranceTrigger*raleighIntakeCap*numdays;
			caryFlowWeekBaseline = cary.demandBaseline[year-1][week-1];
                // expect withdrawal from reservoirs, corrected for reclaimed water or treatment at JL

			durhamSpill = systemStorage.getDurhamSpillage();
			OWASASpill = systemStorage.getOWASASpillage();
			insuranceFallsInflow = (fallsActualInflow + durhamSpill + durham.weeklyDemand*returnRatio[0][week-1]-systemStorage.fallsArea*actualFallsEvap)*(falls_lake_supply_capacity/34700.0) + raleigh.weeklyTransferVolume;
			insuranceJordanInflow = (OWASASpill + owasa.weeklyDemand*returnRatio[1][week-1] + durham.weeklyDemand*(returnRatio[1][week-1] - returnRatio[0][week-1]) +
					jordanActualInflow-actualEvap*13900)*cary.jordanLakeAlloc*(45800.0/(94600.0+45800.0));

			//Determine insurance payout (goes directly to the mitigation fund)
			durham.setInsurancePayment(durhamFlowWeekBaseline, durhamActualInflow + durham.weeklyTransferVolume, week);
			owasa.setInsurancePayment(owasaFlowWeekBaseline, owasaActualInflow*61.3 + owasa.weeklyTransferVolume, week);
			raleigh.setInsurancePayment(raleighFlowWeekBaseline, insuranceFallsInflow + littleRiverRalInsuranceTrigger*littleRiverRaleighActualInflow, week);
			cary.setInsurancePayment(caryFlowWeekBaseline, insuranceJordanInflow, week);

			//retrieve updated storage levels
			owasa.storageFraction = systemStorage.getOWASAStorage();
			durham.storageFraction = systemStorage.getDurhamStorage();
			raleigh.storageFraction = systemStorage.getRaleighStorage();
			cary.storageFraction = systemStorage.getCaryStorage();

			//update timestep
			simDates.increase();
			year = simDates.getYear();
			month = simDates.getMonth();
			week = simDates.getWeek();
			numdays = simDates.getDays();
			durham.weeklyUpdate();
			owasa.weeklyUpdate();
			raleigh.weeklyUpdate();
			cary.weeklyUpdate();
			updateFallsQuality();
			
			if (week == 1)
                // if on a new year...
			{
				createInfrastructure(realization);
				createInfrastructureRisk(realization, year-1, durham.averageUse + durham.infBuffer, owasa.averageUse + owasa.infBuffer, raleigh.averageUse + raleigh.infBuffer, cary.averageUse + cary.infBuffer);
					// infrastructure risk is also used for renegotiating release contracts 
					
				raleigh.CurrentRisk[contractriskyearcounter] = raleigh.infRisk;
				durham.CurrentRisk[contractriskyearcounter]  = durham.infRisk;
					// store the most recent 20 years of annual baseline ROF 
				
				contractriskyearcounter += 1;
				if (contractriskyearcounter > 19)
				{
					contractriskyearcounter = 0;
				}					
					// adjust this index every 20 years to overwrite itself with new ROF data 
					
				triggerInfrastructure(realization);
                    // check on infra

				durham.annualUpdate(year-1, realization);
				owasa.annualUpdate(year-1, realization);
				cary.annualUpdate(year-1, realization);
				raleigh.annualUpdate(year-1, realization);
                    // update of financial stuff

				//Upgrade Cary WTP to 56 MGD in 2016
				if (caryUpgrades[caryWTPcounter]<cary.averageUse&&caryBuild[caryWTPcounter]==0)
                    // thresholds based on annual demand
				{
					cary.addDebt(year, realization, caryWTPcosts[caryWTPcounter], bondLength, bondRate);
					caryBuild[caryWTPcounter] += 1;
					caryWTPcounter++;
				}
				for(int x = 0; x<3; x++)
				{
					if(caryBuild[x]>0&&caryBuild[x]<4)
					{
						caryBuild[x] += 1;
						if(caryBuild[x]==4)
						{
							systemStorage.upgradeCaryTreatmentPlant(x);
						}
					}
				}
				if(thisYearFalls == 1)
				{
					totalFallsFailure[year-2] += 1.0/double(numRealizations);
                        // failure for falls lake WQ pool
				}
				thisYearFalls = 0;
				durhamReclaimedCap = systemStorage.getDurhamReclaimedCap();
				durhamTreatmentCap = systemStorage.getDurhamTreatment();
				owasaTreatmentCap = systemStorage.getOWASATreatment();
				raleighTreatmentCap = systemStorage.getRaleighTreatment();
				raleighIntakeCap = systemStorage.getRaleighIntake();
                    // all reset for the week
			
				if (allowReleases == 1)
				{
					/////////////////////////////////////////////////////////////////////////////////////////////////////////
					/////////////////////////// CALCULATE SPINUP RISK ///////////////////////////////////////////////////////
					
					if (yearcounter == 0)
					{
						LOOPCHECKER = 1;
						
						for (yr = 0; yr < numContractRiskYears; yr++)
						{
							createInfrastructureRisk_spinup(realization, ((currentYear - startYear + 1) - (numContractRiskYears - yr)), 
															durham.averageUse + durham.infBuffer, owasa.averageUse + owasa.infBuffer, 
															raleigh.averageUse + raleigh.infBuffer, cary.averageUse + cary.infBuffer);
															
							raleigh.SpinupRisk[yr] = raleigh.infRisk;
							durham.SpinupRisk[yr]  = durham.infRisk;
								// this loop determines 20 years of risk for years before the real simulation starts
								// as a basis for release contract determination 
								// SHOULD ONLY RUN IN THE FIRST WEEK OF THE FIRST YEAR 
						}
						
						yearcounter = 1;
					}

					/////////////////////////////////////////////////////////////////////////////////////////////////////////
					/////////////////////////// RENEGOTIATE RELEASE CONTRACT ////////////////////////////////////////////////
					
					// In re-negotiating a release contract, baseline ROF records, treated transfer history, 
					// and past contract financial performance of the last 20 years 
					// are used a decision variables to determine:
					//	1.	whether or not to have a new release contract
					//	2.	the size annual payments to Durham and spot price of buybacks by Durham 
					//	3.	constraints upon the frequency and magnitude of releases 
					
					if (contractlength < firstyear)
					{
						firstyear += contractlength;
							// this ensures the next if statement modulus works properly
					}
					
					
					if (year % contractlength == firstyear)
						// contract re-negotiated in the first week of the first year and every 10 years afterward
					{
						if (allowReleaseContract && contractcount > 1)
						{
							previousContract = true;
						}
						else
						{
							previousContract = false;
						}
							// check if there was a contract during the last contract period 
						
						if (contractcount == 1)
							// if this is the first contract, just use spinup risk and no transfer info 
							// assumes the contract is of some reasonably long length (> 5 years)
						{
							if (maxValue_array(raleigh.SpinupRisk) > raleigh.RRcontractTrigger && minValue_array(durham.SpinupRisk) < durham.RRcontractTrigger)
								// a contract will be made if Raleigh's risk is great enough and Durham's is low enough
								// based on the most recent 20 years 
							{
								allowReleaseContract = true;
									// first, determine whether to have a contract or not 
									
								double RtriggerDiff = raleigh.RRcontractTrigger - average_array(raleigh.SpinupRisk);
								double DtriggerDiff = durham.RRcontractTrigger - average_array(durham.SpinupRisk);
									// find differences between each city's average past risk and their contract triggers
									// for Raleigh, if RtriggerDiff > 0, on average they want releases, if < 0, the opposite.
									// for Durham, if DtriggerDiff > 0, they average too much risk to want to do releases.
									// the magnitude and direction of these indicators asks as a proxy for how much leverage 
									// each city has during contract negotiations.  There will be 9 scenarios:
									//	1.	DtriggerDiff > 0 and RtriggerDiff > 0
									//	2.	DtriggerDiff > 0 and RtriggerDiff < 0 
									//	3.	DtriggerDiff < 0 and RtriggerDiff > 0 
									// 	4.	DtriggerDiff < 0 and RtriggerDiff > 0 
									// 	5.	DtriggerDiff = 0 and RtriggerDiff > 0 
									// 	6.	DtriggerDiff = 0 and RtriggerDiff < 0 
									// 	7.	DtriggerDiff < 0 and RtriggerDiff = 0 
									// 	8.	DtriggerDiff > 0 and RtriggerDiff = 0
									// 	9.	DtriggerDiff = 0 and RtriggerDiff = 0 
									// The annual contract payment will be set arbitrarily the first time, as no treated transfer
									// record or previous contract record has been established. This contract price, x, will be 
									// adjusted by multipliers, based on the state of negotiation leverage. The multipliers are:
									//	1.	if DtriggerDiff > 0, the average payments by Raleigh are larger (Durham needs to be
									//		persuaded more to be part of the contract), annualpayment = annualpayment * (1 + DtriggerDiff)
									//	2.	if DtriggerDiff < 0, average payments are lower (Durham has excess risk capacity), 
									//		annualpayment = annualpayment * (1 + DtriggerDiff)
									//	3.	if RtriggerDiff > 0, Raleigh needs releases more 
									//		annualpayment = annualpayment * (1 + RtriggerDiff)
									//	4.	if RtriggerDiff < 0, on average Raleigh doesn't need extra releases 
									//		annualpayment = annualpayment * (1 + RtriggerDiff)
									//	5.	in any other case, when the trigger equals the average risk, the annual payment is unchanged
									
								double triggerDiff = RtriggerDiff + DtriggerDiff;
									// because the effects of each are the same sign (positive R or D triggerDiff increases the annual payment)
									// add them and adjust the annual payment
								
								adjustedannualpayment = annualpayment * (1 + triggerDiff);
								
								double RBBstddev = std_dev_vector(raleigh.SpinupRisk, numContractRiskYears);
								double DBBstddev = std_dev_vector(durham.SpinupRisk, numContractRiskYears);
									// a similar system is employed to determine price of buybacks 
									// the variability of risk affects spot prices 
									// for both, high variability = high spot price for release water
									// as a addition to $3,000 per MG, or whatever the base price for
									// transfers is? 
								
								adjustedbuybackpayment = buybackratePerMG + RBBstddev + DBBstddev;
							}
							else
							{
								allowReleaseContract = false;
									// if triggers aren't met, no contract 
							}
						}
					}
				}
			}

		} // End weekly loop
		
		if ((realization < numRealizationsTOREAD) && printDetailedOutput)
		{
			outRiskParams << rank << "," << realization << ",";
			outRiskParams << raleigh.RRtrigger << "," << raleigh.TTriggerN << "," << raleigh.infTrigger << ",";
			outRiskParams << durham.RRtrigger  << "," << durham.TTriggerN  << "," << durham.infTrigger  << ",";
			outRiskParams << raleigh.jordanLakeAlloc << "," << durham.jordanLakeAlloc << "," << availableJLallocation << endl;
				// write outputs for risk and other triggers that don't change over the course of simulation
		}

	} //end realization loop
	
	if (printDetailedOutput)
	{
		out100.close();
		outNew.close();
		outRiskParams.close();
		InfraBuilt.close();
	}

	durham.calculateObjectives();
	owasa.calculateObjectives();
	cary.calculateObjectives();
	raleigh.calculateObjectives();
        // calculate objectives once all simulations are finished

	for(int x = 0; x< terminateYear; x++)
	{
		if(totalFallsFailure[x]>maxFallsFailure)
		{
			maxFallsFailure = totalFallsFailure[x];
		}
	}

	return;
}
void Simulation::updateFallsQuality()
{
	double qualityStorage = 0.0;
	qualityStorage = systemStorage.getFallsQuality();
	if(qualityStorage < fallsFailurePoint)
	{
		thisYearFalls = 1;
	}
	return;
}
Simulation::~Simulation()
{
	zap(xreal);
	zap(actualStreamflows);
	zap(totalFallsFailure);
}

void Simulation::createRiskOfFailure_InsuranceReleases(int realization, int synthYear, double durhamDemandValue, double owasaDemandValue, double raleighDemandValue, double caryDemandValue,
													   int discreteintervals)
{
	double durhamROFDemand = 0.0;
	double owasaROFDemand = 0.0;
	double raleighROFDemand = 0.0;
	double caryROFDemand = 0.0;
	double durhamROFInflow = 0.0;
	double raleighROFInflow = 0.0;
	double owasaROFInflow = 0.0;
	double wbROFInflow = 0.0;
	double claytonROFInflow = 0.0;
	double crabtreeROFInflow = 0.0;
	double jordanROFInflow = 0.0;
	double lillingtonROFInflow = 0.0;
	double littleRiverRaleighROFInflow = 0.0;
	double ROFevap = 0.0;
	double fallsROFevap = 0.0;
	double wbROFevap = 0.0;
	
	double oIPs = 0.0;
	double dIPs = 0.0;
	double rIPs = 0.0;
	double cIPs = 0.0;
	
	double oIPex = 0.0;
	double dIPex = 0.0;
	double rIPex = 0.0;
	double cIPex = 0.0;

	int oROFstage = 0.0;
	int dROFstage = 0.0;
	int rROFstage = 0.0;
	int cROFstage = 0.0;
	int counter = 0;
	int syntheticIndex = 0;
	
	int thisTimeOIP[discreteintervals];
	int thisTimeDIP[discreteintervals];
	int thisTimeCIP[discreteintervals];
	int thisTimeRIP[discreteintervals];
		// changed this and any other occurrence 
		// of the hardcoded tiers = 20 to
		// discreteintervals
	
	int histYear = 80;
	int yearROF;
	int monthROF;
	int weekROF;
	int numdaysROF;
	int insurancePriceCounter;
	
	for(int x = 0; x < discreteintervals; x++)
	{
		owasa.storageRisk[x] = 0.0;
		durham.storageRisk[x] = 0.0;
		cary.storageRisk[x] = 0.0;
		raleigh.storageRisk[x] = 0.0;
		
		durham.ReleaseStorageRisk[x] = 0.0;
		raleigh.ReleaseStorageRisk[x] = 0.0;
			// added these two lines
	}

	double durhamS = systemStorage.getDurhamStorageVol();
	double teerS =  systemStorage.getTeerStorageVol();
	double CCRS =  systemStorage.getCCRStorageVol();
	double ULS =  systemStorage.getULStorageVol();
	double STQS =  systemStorage.getSTQStorageVol();
	double owasaS =  systemStorage.getOWASAStorageVol();
	double lakeWBS = systemStorage.getLakeWBStorageVol();
	double flSS =  systemStorage.getFallsSupplyStorageVol();
	double flQS = systemStorage.getFallsQualityStorageVol();
	double jlSS = systemStorage.getJordanSupplyStorageVol();
	double jlQS = systemStorage.getJordanQualityStorageVol();
	double caryJordanS = systemStorage.getCaryJordanStorageVol();
	double raleighJordanS = systemStorage.getRaleighJordanStorageVol();
	double durhamJordanS = systemStorage.getDurhamJordanStorageVol();
	double owasaJordanS = systemStorage.getOWASAJordanStorageVol();
	double littleRiverRalS = systemStorage.getLittleRiverRalStorageVol();
	double raleighQS = systemStorage.getRaleighQuarryStorageVol();
	
	int numRiskYears = 50;
	int startingHistoricalYear = 80 - (numRiskYears - synthYear + 1);
	int startingSynthYear = synthYear - numRiskYears;
	
	if(startingSynthYear < 0)
	{
		startingSynthYear = 0;
	}
	
	for (int histRealizations = startingHistoricalYear; histRealizations<histYear; histRealizations++)
		// determines the year of the historical streamflow record to be used in calculations
        // runs 52 weeks from current week, to find where levels drop below 20% over this year
        // this is run for 50 years of the immediate past, based on the current year
	{

		counter = 0;
		
		oIPex = 0.0;
		dIPex = 0.0;
		cIPex = 0.0;
		rIPex = 0.0;

		for(int x = 0; x < discreteintervals; x++)
		{
			thisTimeOIP[x] = 0;
			thisTimeDIP[x] = 0;
			thisTimeRIP[x] = 0;
			thisTimeCIP[x] = 0;
		}
		
		riskOfFailureDates.initializeDates(startSimulationYear,1,week,7,0);
			//each simulation is run independently (52 week intervals, then the slate is cleaned)
		riskOfFailureStorageIP.updateReservoirStorageROF();
            // sets initial IP storage to full

		while (counter < 52)
		{
			yearROF    = riskOfFailureDates.getYear();
			monthROF   = riskOfFailureDates.getMonth();
			weekROF    = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();
				// Retrieving date information

			durhamROFDemand  = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand   = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand    = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands
                // expected demand set for a given week
				// While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)

			riskOfFailureStorageIP.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);
				// passes demands to storage calcs

				
			durhamROFInflow = michieInflow[histRealizations+yearROF-startSimulationYear][weekROF-1]+littleRiverInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Durham Inflows
			raleighROFInflow = fallsLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Raleigh
			owasaROFInflow = owasaInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//OWASA
			wbROFInflow = lakeWBInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Wheeler&Benson
			claytonROFInflow = claytonInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Clayton gauge
			crabtreeROFInflow = crabtreeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				// Crabtree creek
			jordanROFInflow = jordanLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			lillingtonROFInflow = lillingtonGaugeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			littleRiverRaleighROFInflow = littleRiverRaleighInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
                // inflow taken from historical record


			ROFevap      = evaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Durham and OWASA evap
			fallsROFevap = fallsLakeEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				// Falls Lake evap
			wbROFevap    = lakeWheelerEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Wheeler Benson evap
                // same for evap

			riskOfFailureStorageIP.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);

			riskOfFailureStorageIP.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageIP.updateStorage(weekROF-1);//storage calcs
                // advance res operation a week

			riskOfFailureDates.increase();//increase week by one
			counter++;
			
			oIPs = riskOfFailureStorageIP.getOWASAStorage();//retrieve overall storage, OWASA
			dIPs = riskOfFailureStorageIP.getDurhamStorage();//retrieve overall storage, Durham
			rIPs = riskOfFailureStorageIP.getRaleighStorage();//retrieve overall storage, Raleigh
			cIPs = riskOfFailureStorageIP.getCaryStorage();
                // current week's storage fraction, from 0 to 1, 1 being the reservoir is full

			oIPex += riskOfFailureStorageIP.getExcessO();//retrieve overall storage, OWASA
			dIPex += riskOfFailureStorageIP.getExcessD();//retrieve overall storage, Durham
			rIPex += riskOfFailureStorageIP.getExcessR();//retrieve overall storage, Raleigh
			cIPex += riskOfFailureStorageIP.getExcessC();
                // if the reservoir was full, what was spilled over as excess

			if(oIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeOIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(oIPs < (0.2 + double(x)/double(discreteintervals) - oIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeOIP[y] = 1;
						}
						break;
					}
				}
			}

			// repeat for all utilities....

			if(dIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeDIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(dIPs < (0.2 + double(x)/double(discreteintervals) - dIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeDIP[y] = 1;
						}
						break;
					}
				}
			}

			if(rIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeRIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(rIPs<(0.2 + double(x)/double(discreteintervals) - rIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeRIP[y] = 1;
						}
						break;
					}
				}
			}
		

			if(cIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeCIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(cIPs < (0.2 + double(x)/double(discreteintervals) - cIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeCIP[y] = 1;
						}
						break;
					}
				}
			}
		}
		
		for (int x = 0; x < discreteintervals; x++)
		{
			if(thisTimeOIP[x] == 1)
			{
				owasa.storageRisk[x] += 1.0;
			}
			if(thisTimeDIP[x] == 1)
			{
				durham.storageRisk[x] += 1.0;
				durham.ReleaseStorageRisk[x] += 1.0;
			}
			if(thisTimeRIP[x] == 1)
			{
				raleigh.storageRisk[x] += 1.0;
				raleigh.ReleaseStorageRisk[x] += 1.0;
			}
			if(thisTimeCIP[x] == 1)
			{
				cary.storageRisk[x] += 1.0;
			}
		}
	}
	
	for (int synthRealizations = startingSynthYear; synthRealizations<synthYear; synthRealizations++)
		// determines the year of the historical streamflow record to be used in calculations
        // same loop as above but with the synthetic record
	{
		counter = 0;
		
		oIPex = 0.0;
		dIPex = 0.0;
		cIPex = 0.0;
		rIPex = 0.0;
				
		for(int x = 0; x < discreteintervals; x++)
		{
			thisTimeOIP[x] = 0;
			thisTimeDIP[x] = 0;
			thisTimeRIP[x] = 0;
			thisTimeCIP[x] = 0;
		}
		
		riskOfFailureDates.initializeDates(synthRealizations,1,week,7,0);//each simulation is run independently (52 week intervals, then the slate is cleaned)
		riskOfFailureStorageIP.updateReservoirStorageROF();

		while (counter < 52)
		{
			//Retrieving date information
			yearROF = riskOfFailureDates.getYear();
			monthROF = riskOfFailureDates.getMonth();
			weekROF = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();

			//Demand calcs
			//While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)
			durhamROFDemand = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands

			riskOfFailureStorageIP.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);//passes demands to storage calcs

			//Inflow Calcs
			syntheticIndex = (yearROF)*52+weekROF-1;
			durhamROFInflow = durhamInflows.synthetic[realization][syntheticIndex];//Durham Inflows
			raleighROFInflow = fallsInflows.synthetic[realization][syntheticIndex];//Raleigh
			owasaROFInflow = owasaInflows.synthetic[realization][syntheticIndex];//OWASA
			wbROFInflow = wheelerInflows.synthetic[realization][syntheticIndex];//Wheeler&Benson
			claytonROFInflow = claytonInflows.synthetic[realization][syntheticIndex];//Clayton gauge
			crabtreeROFInflow = crabtreeInflows.synthetic[realization][syntheticIndex];// Crabtree creek
			jordanROFInflow = jordanInflows.synthetic[realization][syntheticIndex];
			lillingtonROFInflow = lillingtonInflows.synthetic[realization][syntheticIndex];
			littleRiverRaleighROFInflow = littleRiverRaleighInflows.synthetic[realization][syntheticIndex];
			//Evap Calcs

			ROFevap = durhamOwasaEvap.synthetic[realization][syntheticIndex];//Durham and OWASA evap
			fallsROFevap = fallsEvap.synthetic[realization][syntheticIndex];// Falls Lake evap
			wbROFevap= wheelerEvap.synthetic[realization][syntheticIndex];//Wheeler Benson evap

			riskOfFailureStorageIP.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);

			riskOfFailureStorageIP.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageIP.updateStorage(weekROF-1);//storage calcs

			riskOfFailureDates.increase();//increase week by one
			counter++;

			oIPs = riskOfFailureStorageIP.getOWASAStorage();//retrieve overall storage, OWASA
			dIPs = riskOfFailureStorageIP.getDurhamStorage();//retrieve overall storage, Durham
			rIPs = riskOfFailureStorageIP.getRaleighStorage();//retrieve overall storage, Raleigh
			cIPs = riskOfFailureStorageIP.getCaryStorage();

			oIPex += riskOfFailureStorageIP.getExcessO();//retrieve overall storage, OWASA
			dIPex += riskOfFailureStorageIP.getExcessD();//retrieve overall storage, Durham
			rIPex += riskOfFailureStorageIP.getExcessR();//retrieve overall storage, Raleigh
			cIPex += riskOfFailureStorageIP.getExcessC();

			if(oIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeOIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(oIPs < (0.2 + double(x)/double(discreteintervals) - oIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeOIP[y] = 1;
						}
						break;
					}
				}
			}

			// repeat for all utilities....

			if(dIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeDIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(dIPs < (0.2 + double(x)/double(discreteintervals) - dIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeDIP[y] = 1;
						}
						break;
					}
				}
			}

			if(rIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeRIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(rIPs<(0.2 + double(x)/double(discreteintervals) - rIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeRIP[y] = 1;
						}
						break;
					}
				}
			}
		

			if(cIPs < 0.2)
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					thisTimeCIP[x] = 1;
				}
			}
			else
			{
				for(int x = 0; x < discreteintervals; x++)
				{
					if(cIPs < (0.2 + double(x)/double(discreteintervals) - cIPex))
					{
						for(int y = x; y < discreteintervals; y++)
						{
							thisTimeCIP[y] = 1;
						}
						break;
					}
				}
			}
		}
		
		for (int x = 0; x < discreteintervals; x++)
		{
			if(thisTimeOIP[x] == 1)
			{
				owasa.storageRisk[x] += 1.0;
			}
			if(thisTimeDIP[x] == 1)
			{
				durham.storageRisk[x] += 1.0;
				durham.ReleaseStorageRisk[x] += 1.0;
			}
			if(thisTimeRIP[x] == 1)
			{
				raleigh.storageRisk[x] += 1.0;
				raleigh.ReleaseStorageRisk[x] += 1.0;
			}
			if(thisTimeCIP[x] == 1)
			{
				cary.storageRisk[x] += 1.0;
			}
		}
	}

	for(int x = 0; x < discreteintervals; x++)
	{
		owasa.storageRisk[x] = owasa.storageRisk[x]/(double(numRiskYears));
		if(owasa.storageRisk[x] > owasa.insuranceUse)
            // what storage level incurs an insurance payout?
            // known insurance level, what storage level will trigger this?
		{
			owasa.riskVolume[week-1] = (double(discreteintervals) - double(x))/double(discreteintervals);
			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		durham.storageRisk[x] = durham.storageRisk[x]/(double(numRiskYears));
		if(durham.storageRisk[x] > durham.insuranceUse)
		{
			durham.riskVolume[week-1] = (double(discreteintervals) - double(x))/double(discreteintervals);
			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		raleigh.storageRisk[x] = raleigh.storageRisk[x]/(double(numRiskYears));
		if(raleigh.storageRisk[x] > raleigh.insuranceUse)
		{
			raleigh.riskVolume[week-1] = (double(discreteintervals) - double(x))/double(discreteintervals);
			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		cary.storageRisk[x] = cary.storageRisk[x]/(double(numRiskYears));
		if(cary.storageRisk[x] > cary.insuranceUse)
		{
			cary.riskVolume[week-1] = (double(discreteintervals) - double(x))/double(discreteintervals);
			break;
		}
	}
	
	bool notfound = true;
	
	for(int x = 0; x < discreteintervals; x++)
	{
		durham.ReleaseStorageRisk[x] = durham.ReleaseStorageRisk[x]/(double(numRiskYears));
		
		if (durham.ReleaseStorageRisk[x] > durham.RRtrigger - BuybackROFZone && notfound)
			// if the risk of failure becomes greater than the ROF trigger level 
			// for buybacks (equal to the ROF trigger for release denial - ROF buffer amount)
		{
			durham.BuybackRiskVolume[week-1] = (double(discreteintervals) - double(x-1))/double(discreteintervals);
				// reservoir level ceiling for buybacks. above this level, buybacks won't be made on water.  between this
				// and the cutoff reservoir level (releaseriskvolume), that supply will be bought back 
			
			if (durham.BuybackRiskVolume[week-1] > 1.0)
			{
				durham.BuybackRiskVolume[week-1] = 1.0;
			}
			
			notfound = false;
		}
		
		if (durham.ReleaseStorageRisk[x] > durham.RRtrigger)
				// if the risk of failure becomes greater than the ROF trigger for releases
				// at the specific x, meaning current reservoir level
		{
			durham.ReleaseRiskVolume[week-1] = (double(discreteintervals) - double(x-1))/double(discreteintervals);
				// the desired reservoir level that releases need to achieve is this
			if (durham.ReleaseRiskVolume[week-1] > 1.0)
			{
				durham.ReleaseRiskVolume[week-1] = 1.0;
			}
			
			break;
		}
	}
	
	for(int x = 0; x < discreteintervals; x++)
	{
		raleigh.ReleaseStorageRisk[x] = raleigh.ReleaseStorageRisk[x]/(double(numRiskYears));
		
		if (raleigh.ReleaseStorageRisk[x] > raleigh.RRtrigger)
		{
			raleigh.ReleaseRiskVolume[week-1] = (double(discreteintervals) - double(x-1))/double(discreteintervals);
			
			if (raleigh.ReleaseRiskVolume[week-1] > 1.0)
			{
				raleigh.ReleaseRiskVolume[week-1] = 1.0;
			}
			
			break;
		}
	}

	return;
}

void Simulation::createRiskOfFailure_RestrictionsTransfers(int realization, int synthYear, 
														   double durhamDemandValue, double owasaDemandValue, double raleighDemandValue, double caryDemandValue)
{
	double durhamROFDemand = 0.0;
	double owasaROFDemand = 0.0;
	double raleighROFDemand = 0.0;
	double caryROFDemand = 0.0;
	double durhamROFInflow = 0.0;
	double raleighROFInflow = 0.0;
	double owasaROFInflow = 0.0;
	double wbROFInflow = 0.0;
	double claytonROFInflow = 0.0;
	double crabtreeROFInflow = 0.0;
	double jordanROFInflow = 0.0;
	double lillingtonROFInflow = 0.0;
	double littleRiverRaleighROFInflow = 0.0;
	double ROFevap = 0.0;
	double fallsROFevap = 0.0;
	double wbROFevap = 0.0;
	
	double oROFs = 0.0;
	double dROFs = 0.0;
	double rROFs = 0.0;
	double cROFs = 0.0;

	int oROFstage = 0.0;
	int dROFstage = 0.0;
	int rROFstage = 0.0;
	int cROFstage = 0.0;
	int counter = 0;
	int syntheticIndex = 0;
	
	int thisTimeO = 0.0;
	int thisTimeR = 0.0;
	int thisTimeD = 0.0;
	int thisTimeC = 0.0;
	
	int histYear = 80;
	int yearROF;
	int monthROF;
	int weekROF;
	int numdaysROF;
	int insurancePriceCounter;

	owasa.riskOfFailure = 0.0;
	durham.riskOfFailure = 0.0;
	raleigh.riskOfFailure = 0.0;
	cary.riskOfFailure = 0.0;

	double durhamS = systemStorage.getDurhamStorageVol();
	double teerS =  systemStorage.getTeerStorageVol();
	double CCRS =  systemStorage.getCCRStorageVol();
	double ULS =  systemStorage.getULStorageVol();
	double STQS =  systemStorage.getSTQStorageVol();
	double owasaS =  systemStorage.getOWASAStorageVol();
	double lakeWBS = systemStorage.getLakeWBStorageVol();
	double flSS =  systemStorage.getFallsSupplyStorageVol();
	double flQS = systemStorage.getFallsQualityStorageVol();
	double jlSS = systemStorage.getJordanSupplyStorageVol();
	double jlQS = systemStorage.getJordanQualityStorageVol();
	double caryJordanS = systemStorage.getCaryJordanStorageVol();
	double raleighJordanS = systemStorage.getRaleighJordanStorageVol();
	double durhamJordanS = systemStorage.getDurhamJordanStorageVol();
	double owasaJordanS = systemStorage.getOWASAJordanStorageVol();
	double littleRiverRalS = systemStorage.getLittleRiverRalStorageVol();
	double raleighQS = systemStorage.getRaleighQuarryStorageVol();
	
	int numRiskYears = 50;
	int startingHistoricalYear = 80 - (numRiskYears - synthYear + 1);
	int startingSynthYear = synthYear - numRiskYears;
	
	if(startingSynthYear < 0)
	{
		startingSynthYear = 0;
	}
	
	for (int histRealizations = startingHistoricalYear; histRealizations<histYear; histRealizations++)
		// determines the year of the historical streamflow record to be used in calculations
        // runs 52 weeks from current week, to find where levels drop below 20% over this year
        // this is run for 50 years of the immediate past, based on the current year
	{

		counter = 0;
		
		thisTimeO = 0;
		thisTimeD = 0;
		thisTimeR = 0;
		thisTimeC = 0;
		
		riskOfFailureDates.initializeDates(startSimulationYear,1,week,7,0);
			//each simulation is run independently (52 week intervals, then the slate is cleaned)
		riskOfFailureStorageROF.updateReservoirStorageROF(durhamS, teerS, CCRS, ULS, STQS, owasaS, lakeWBS, flSS, flQS, jlSS, jlQS, caryJordanS,  raleighJordanS,  durhamJordanS, owasaJordanS, littleRiverRalS, raleighQS);
            // sets initial ROF storage to current storage levels

		while (counter < 52)
		{
			yearROF    = riskOfFailureDates.getYear();
			monthROF   = riskOfFailureDates.getMonth();
			weekROF    = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();
				// Retrieving date information

			durhamROFDemand  = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand   = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand    = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands
                // expected demand set for a given week
				// While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)
			
			riskOfFailureStorageROF.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);
				// passes demands to storage calcs
                // demands set every week
				
			durhamROFInflow = michieInflow[histRealizations+yearROF-startSimulationYear][weekROF-1]+littleRiverInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Durham Inflows
			raleighROFInflow = fallsLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Raleigh
			owasaROFInflow = owasaInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//OWASA
			wbROFInflow = lakeWBInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Wheeler&Benson
			claytonROFInflow = claytonInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Clayton gauge
			crabtreeROFInflow = crabtreeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
				// Crabtree creek
			jordanROFInflow = jordanLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			lillingtonROFInflow = lillingtonGaugeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			littleRiverRaleighROFInflow = littleRiverRaleighInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
                // inflow taken from historical record

			ROFevap      = evaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Durham and OWASA evap
			fallsROFevap = fallsLakeEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				// Falls Lake evap
			wbROFevap    = lakeWheelerEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];
				//Wheeler Benson evap
                // same for evap

			riskOfFailureStorageROF.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);

			riskOfFailureStorageROF.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageROF.updateStorage(weekROF-1);//storage calcs

			oROFs = riskOfFailureStorageROF.getOWASAStorage();//retrieve overall storage, OWASA
			dROFs = riskOfFailureStorageROF.getDurhamStorage();//retrieve overall storage, Durham
			rROFs = riskOfFailureStorageROF.getRaleighStorage();//retrieve overall storage, Raleigh
			cROFs = riskOfFailureStorageROF.getCaryStorage();
                // find new storage fraction


			if (oROFs<.2)
			{
				thisTimeO = 1;//Mark OWASA failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (dROFs<.2)
			{
				thisTimeD = 1;//Mark Durham failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (rROFs<.2)
			{
				thisTimeR = 1;//Mark Raleigh failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (cROFs<.2)
			{
				thisTimeC = 1;
			}
                // this was for ROF, insurance is below
                // they are different because ROF is used to calculate insurance payments?

			riskOfFailureDates.increase();//increase week by one
			counter++;
		}
		
		if (thisTimeO==1)
			//Fill out the risk matricies (every historical year that causes at least one week of failure results in an increase in risk of 1.28% (1/78)
		{
			owasa.riskOfFailure += 1.0;
		}
		if (thisTimeD==1)
		{
			durham.riskOfFailure += 1.0;
		}
		if (thisTimeR==1)
		{
			raleigh.riskOfFailure += 1.0;
		}
		if (thisTimeC==1)
		{
			cary.riskOfFailure += 1.0;
		}
	}
	
	for (int synthRealizations = startingSynthYear; synthRealizations<synthYear; synthRealizations++)
		// determines the year of the historical streamflow record to be used in calculations
        // same loop as above but with the synthetic record
	{
		counter = 0;
		
		thisTimeO = 0;
		thisTimeD = 0;
		thisTimeR = 0;
		thisTimeC = 0;
		
		riskOfFailureDates.initializeDates(synthRealizations,1,week,7,0);//each simulation is run independently (52 week intervals, then the slate is cleaned)
		riskOfFailureStorageROF.updateReservoirStorageROF(durhamS, teerS, CCRS, ULS, STQS, owasaS, lakeWBS, flSS, flQS, jlSS, jlQS, caryJordanS,  raleighJordanS,  durhamJordanS, owasaJordanS, littleRiverRalS, raleighQS);

		while (counter < 52)
		{
			//Retrieving date information
			yearROF = riskOfFailureDates.getYear();
			monthROF = riskOfFailureDates.getMonth();
			weekROF = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();

			//Demand calcs
			//While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)
			durhamROFDemand = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands

			riskOfFailureStorageROF.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);//passes demands to storage calcs

			//Inflow Calcs
			syntheticIndex = (yearROF)*52+weekROF-1;
			durhamROFInflow = durhamInflows.synthetic[realization][syntheticIndex];//Durham Inflows
			raleighROFInflow = fallsInflows.synthetic[realization][syntheticIndex];//Raleigh
			owasaROFInflow = owasaInflows.synthetic[realization][syntheticIndex];//OWASA
			wbROFInflow = wheelerInflows.synthetic[realization][syntheticIndex];//Wheeler&Benson
			claytonROFInflow = claytonInflows.synthetic[realization][syntheticIndex];//Clayton gauge
			crabtreeROFInflow = crabtreeInflows.synthetic[realization][syntheticIndex];// Crabtree creek
			jordanROFInflow = jordanInflows.synthetic[realization][syntheticIndex];
			lillingtonROFInflow = lillingtonInflows.synthetic[realization][syntheticIndex];
			littleRiverRaleighROFInflow = littleRiverRaleighInflows.synthetic[realization][syntheticIndex];
			//Evap Calcs

			ROFevap = durhamOwasaEvap.synthetic[realization][syntheticIndex];//Durham and OWASA evap
			fallsROFevap = fallsEvap.synthetic[realization][syntheticIndex];// Falls Lake evap
			wbROFevap= wheelerEvap.synthetic[realization][syntheticIndex];//Wheeler Benson evap

			riskOfFailureStorageROF.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);

			riskOfFailureStorageROF.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageROF.updateStorage(weekROF-1);//storage calcs

			oROFs = riskOfFailureStorageROF.getOWASAStorage();//retrieve overall storage, OWASA
			dROFs = riskOfFailureStorageROF.getDurhamStorage();//retrieve overall storage, Durham
			rROFs = riskOfFailureStorageROF.getRaleighStorage();//retrieve overall storage, Raleigh
			cROFs = riskOfFailureStorageROF.getCaryStorage();

			if (oROFs<.2)
			{
				thisTimeO = 1;//Mark OWASA failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (dROFs<.2)
			{
				thisTimeD = 1;//Mark Durham failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (rROFs<.2)
			{
				thisTimeR = 1;//Mark Raleigh failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (cROFs<.2)
			{
				thisTimeC = 1;
			}

			riskOfFailureDates.increase();//increase week by one
			counter++;
		}
		
		if (thisTimeO==1)
			//Fill out the risk matricies (every historical year that causes at least one week of failure results in an increase in risk of 1.28% (1/78)
		{
			owasa.riskOfFailure += 1.0;
		}
		if (thisTimeD==1)
		{
			durham.riskOfFailure += 1.0;
		}
		if (thisTimeR==1)
		{
			raleigh.riskOfFailure += 1.0;
		}
		if (thisTimeC==1)
		{
			cary.riskOfFailure += 1.0;
		}
	}
	
	owasa.riskOfFailure   = owasa.riskOfFailure/(double(numRiskYears));
	durham.riskOfFailure  = durham.riskOfFailure/(double(numRiskYears));
	raleigh.riskOfFailure = raleigh.riskOfFailure/(double(numRiskYears));
	cary.riskOfFailure    = cary.riskOfFailure/(double(numRiskYears));
        // divide by 50 to get a percentage of years in failure

	return;
}

void Simulation::createInfrastructureRisk_spinup(int realization, int synthYear, 
												 double durhamDemandValue, double owasaDemandValue, double raleighDemandValue, double caryDemandValue)
{
	double durhamROFDemand = 0.0;
	double owasaROFDemand = 0.0;
	double raleighROFDemand = 0.0;
	double caryROFDemand = 0.0;
	
	double durhamROFInflow = 0.0;
	double raleighROFInflow = 0.0;
	double owasaROFInflow = 0.0;
	double wbROFInflow = 0.0;
	double claytonROFInflow = 0.0;
	double crabtreeROFInflow = 0.0;
	double jordanROFInflow = 0.0;
	double lillingtonROFInflow = 0.0;
	double littleRiverRaleighROFInflow = 0.0;
	double ROFevap = 0.0;
	double fallsROFevap = 0.0;
	double wbROFevap = 0.0;

	double dROFs = 0.0;
	double rROFs = 0.0;

	int counter = 0;
	int histYear = 80;
	
	double thisTimeR = 0.0;
	double thisTimeD = 0.0;
	
	durham.infRisk = 0.0;
	raleigh.infRisk = 0.0;
	cary.infRisk = 0.0;
	owasa.infRisk = 0.0;
	
	int yearROF;
	int monthROF;
	int weekROF;
	int numdaysROF;

	int numRiskYears = 50;
	int startingHistoricalYear = 81 - (numRiskYears - synthYear);

	for (int histRealizations = startingHistoricalYear; histRealizations<histYear; histRealizations++)// determines the year of the historical streamflow record to be used in calculations
	{
		counter = 0;

		thisTimeD = 0;
		thisTimeR = 0;

		riskOfFailureDates.initializeDates(startSimulationYear,1,1,7,0);//each simulation is run independently (52 week intervals, then the slate is cleaned)
		riskOfFailureStorageInf.updateReservoirStorageROF();

		while (counter<78)
		{
			//Retrieving date information
			yearROF = riskOfFailureDates.getYear();
			monthROF = riskOfFailureDates.getMonth();
			weekROF = riskOfFailureDates.getWeek();
			numdaysROF = riskOfFailureDates.getDays();

			//Demand calcs
			//While seasonal trends and future growth are incorporated, no demand uncertainty is used (weekly averages only)
			durhamROFDemand = durham.UD.averages[weekROF-1]*numdaysROF*durhamDemandValue;//Durham demands
			owasaROFDemand = owasa.UD.averages[weekROF-1]*numdaysROF*owasaDemandValue;//OWASA demands
			raleighROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*raleighDemandValue;//Raleigh Demands
			caryROFDemand = raleigh.UD.averages[weekROF-1]*numdaysROF*caryDemandValue;//Raleigh Demands

			riskOfFailureStorageInf.setDemands(durhamROFDemand, owasaROFDemand, raleighROFDemand, caryROFDemand, numdaysROF);//passes demands to storage calcs

			//Inflow Calcs
			durhamROFInflow = michieInflow[histRealizations+yearROF-startSimulationYear][weekROF-1]+littleRiverInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//Durham Inflows
			raleighROFInflow = fallsLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//Raleigh
			owasaROFInflow = owasaInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//OWASA
			wbROFInflow = lakeWBInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//Wheeler&Benson
			claytonROFInflow = claytonInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];//Clayton gauge
			crabtreeROFInflow = crabtreeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];// Crabtree creek
			jordanROFInflow = jordanLakeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			lillingtonROFInflow = lillingtonGaugeInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			littleRiverRaleighROFInflow = littleRiverRaleighInflow[histRealizations+yearROF-startSimulationYear][weekROF-1];
			
			//Evap Calcs
			ROFevap = evaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];//Durham and OWASA evap
			fallsROFevap = fallsLakeEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];// Falls Lake evap
			wbROFevap= lakeWheelerEvaporation[histRealizations+yearROF-startSimulationYear][weekROF-1];//Wheeler Benson evap

			riskOfFailureStorageInf.setInflow(durhamROFInflow, //passes inflows to storage calcs
				31.4*owasaROFInflow,
				28.7*owasaROFInflow,
				1.2*owasaROFInflow,
				raleighROFInflow,
				wbROFInflow,
				claytonROFInflow,
				crabtreeROFInflow, jordanROFInflow, lillingtonROFInflow,
				raleighROFDemand*returnRatio[1][weekROF-1],
				durhamROFDemand*returnRatio[0][weekROF-1], durhamROFDemand*returnRatio[0][weekROF-1],owasaROFDemand*returnRatio[0][weekROF-1],
				fallsROFevap,
				wbROFevap,
				ROFevap, littleRiverRaleighROFInflow);


			riskOfFailureStorageInf.setSpillover(weekROF-1);//reservoir releases to meet downstream needs and/or reservoir capacity
			riskOfFailureStorageInf.updateStorage(weekROF-1);//storage calcs

			dROFs = riskOfFailureStorageInf.getDurhamStorage();
				//retrieve overall storage, Durham
			rROFs = riskOfFailureStorageInf.getRaleighStorage();
				//retrieve overall storage, Raleigh

			if (dROFs<.2)
			{
				thisTimeD = 1;//Mark Durham failure (not a count, there is no difference between 1 week of failure and 52)
			}
			if (rROFs<.2)
			{
				thisTimeR = 1;//Mark Raleigh failure (not a count, there is no difference between 1 week of failure and 52)
			}

			riskOfFailureDates.increase();//increase week by one
			counter++;
		}
		
		if (thisTimeD==1)
		{
			durham.infRisk += 1.0;
		}
		if (thisTimeR==1)
		{
			raleigh.infRisk += 1.0;
		}
	}

	durham.infRisk  = durham.infRisk/(double(numRiskYears));
	raleigh.infRisk = raleigh.infRisk/(double(numRiskYears));
        // results in the infra ROF and expected number of weeks per year in failure

	return;
}
