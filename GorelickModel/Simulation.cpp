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

void Simulation::writeDataLists()
{
	numFutureYears = 51;
		//Decision variables
	general_1d_allocate(actualStreamflows,numRealizations);
	general_1d_allocate(totalFallsFailure, terminateYear, 0.0);
	
	durham.name = "Durham";
	raleigh.name = "Raleigh";
	cary.name = "Cary";
	owasa.name = "OWASA";

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
	readFile(michieInflow, historicFlowPath + "updatedMichieInflow.csv", 81, 52);
	readFile(littleRiverInflow, historicFlowPath + "updatedLittleRiverInflow.csv", 81, 52);
	readFile(owasaInflow, historicFlowPath + "updatedOWASAInflow.csv", 81, 52);
	readFile(fallsLakeInflow, historicFlowPath + "updatedFallsLakeInflow.csv", 81, 52);
	readFile(lakeWBInflow, historicFlowPath + "updatedLakeWBInflow.csv", 81, 52);
	readFile(claytonInflow, historicFlowPath + "claytonGageInflow.csv", 81, 52);
	readFile(crabtreeInflow, historicFlowPath + "crabtreeCreekInflow.csv", 81, 52);
	readFile(jordanLakeInflow, historicFlowPath + "updatedJordanLakeInflow.csv", 81, 52);
	readFile(lillingtonGaugeInflow, historicFlowPath + "updatedLillingtonInflow.csv", 81, 52);
	readFile(littleRiverRaleighInflow, historicFlowPath + "updatedLittleRiverRaleighInflow.csv", 81, 52);

	readFile(streamflowIndex, historicFlowPath + "streamflowSample.csv", 100, 1);

	// 18 years (1990-2007) of weekly demand data (18 x 52)
	// Raleigh does not have enough data, so use Cary's instead
	readFile(durham.UnitDemand, demandDataPath + "updatedDurhamUnitDemand.csv", 52, 18);
	readFile(owasa.UnitDemand, demandDataPath + "updatedOWASAUnitDemand.csv", 52, 18);
	readFile(cary.UnitDemand, demandDataPath + "updatedCaryUnitDemand.csv", 52, 18);
	readFile(raleigh.UnitDemand, demandDataPath + "updatedRaleighUnitDemand.csv", 52, 1);

	// 51 years (2010 - 2060) of average daily water use projections
	readFile(caryFutureD, demandDataPath + "caryFutureDemand.csv", 1, 51);
	readFile(raleighFutureD, demandDataPath + "raleighFutureDemand.csv", 1, 51);
	readFile(durhamFutureD, demandDataPath + "durhamFutureDemand.csv", 1, 51);
	readFile(owasaFutureD, demandDataPath + "owasaFutureDemand.csv", 1, 51);

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

	}

	// Weekly means and standard deviations are used to 'whiten' the historical data,
	// providing data sets where values are converted to 'standard deviations from the weekly mean'
	// for each week of the historical record
	/// Evaporation ///
	///////////////////
	int evapC = 52; // weeks per year in evaporation files
	int evapR = 81; // historical years of evaporation files


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
	int inflowC = 52;//number of weeks in the inflow record
	int inflowR = 81;//length of historical streamflow record
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
	raleigh.writeInflowDemandPDF(inflowR, UDr, rows_PDF, cols_PDF, size1, size2, irrC, nonC1, nonC2, &owasaInflows);

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

	return;
}

void Simulation::correlateDemandVariations(double demand_variation_multiplier)
{
	cary.generateDemandVariation(52, &jordanInflows, demand_variation_multiplier);
	raleigh.generateDemandVariation(52, &durhamInflows, demand_variation_multiplier);
	durham.generateDemandVariation(52, &durhamInflows, demand_variation_multiplier);

}


// MORDM EXTENSION - READ THE RIGHT SYNTHETIC STREAMFLOWS FILE BASED ON RDM FACTORS
void Simulation::fixRDMFactors()
{

	// usleep(rdm_i * 10000);
	// cout << "Setting up SOW info." << endl;
	// cout << "Reading Hydrologies" << endl;

	use_RDM_ext = true;

	data_t littleRiverInflowSYN;
	data_t durhamInflowsSYN;
	data_t owasaInflowSYN1, owasaInflowSYN2, owasaInflowSYN3;
	data_t evaporationSYN;
	data_t fallsLakeInflowSYN, lakeWBInflowSYN;
	data_t fallsLakeEvaporationSYN, lakeWheelerEvaporationSYN;
	data_t claytonInflowSYN, crabtreeInflowSYN;
	data_t jordanLakeInflowSYN, lillingtonGaugeInflowSYN, littleRiverRaleighInflowSYN;
	
	if (use_RDM_ext)
	{
		//MORDM EXTENSION -  int combination = RDMInput[i] + ...
			// set where the synthetic inflow files are

			// reads the first element of the row of RDM factors used 
			// if david is just using one row, this isnt needed (hence the if statement)

		readFile(durhamInflowsSYN, synthFlowsPath + "durham_inflows.csv", numRecords, 70*52);
		readFile(owasaInflowSYN1, synthFlowsPath + "cane_creek_inflows.csv", numRecords, 70*52);
		readFile(owasaInflowSYN2, synthFlowsPath + "university_lake_inflows.csv", numRecords, 70*52);
		readFile(owasaInflowSYN3, synthFlowsPath + "stone_quarry_inflows.csv", numRecords, 70*52);
		readFile(fallsLakeInflowSYN, synthFlowsPath + "falls_lake_inflows.csv", numRecords, 70*52);
		readFile(lakeWBInflowSYN, synthFlowsPath + "lake_wb_inflows.csv", numRecords, 70*52);
		readFile(claytonInflowSYN, synthFlowsPath + "clayton_inflows.csv", numRecords, 70*52);
		readFile(crabtreeInflowSYN, synthFlowsPath + "crabtree_inflows.csv", numRecords, 70*52);
		readFile(jordanLakeInflowSYN, synthFlowsPath + "jordan_lake_inflows.csv", numRecords, 70*52);
		readFile(lillingtonGaugeInflowSYN, synthFlowsPath + "lillington_inflows.csv", numRecords, 70*52);
		readFile(littleRiverRaleighInflowSYN, synthFlowsPath + "little_river_raleigh_inflows.csv", numRecords, 70*52);
	}

	// cout << "Future demand multiplier" << endl;
	//this section allows for use of historic record of variable length
	for (int x = 0; x<numFutureYears; x++)
	{
//		// MORDM EXTENSION - REPLACE future_demand_multiplier WITH THE RIGHT MULTIPLIERS FROM RDMInput
//		// Scaling a linear projection -- multiply the slope and subtract off changes to the y-intercept
//		cary.futureDemand[x] = caryFutureD[0][x]*rdm_factors[0] - caryFutureD[0][0]*(rdm_factors[0]-1);
//		durham.futureDemand[x] = durhamFutureD[0][x]*rdm_factors[0] - durhamFutureD[0][0]*(rdm_factors[0]-1);
//		raleigh.futureDemand[x] = raleighFutureD[0][x]*rdm_factors[0] - raleighFutureD[0][0]*(rdm_factors[0]-1);
//			// default RDM factors here should be 1

		cary.futureDemand[x] = caryFutureD[0][x];
		durham.futureDemand[x] = durhamFutureD[0][x];
		raleigh.futureDemand[x] = raleighFutureD[0][x];
	}

}

double Simulation::calculation(double *c_xreal, double *c_obj, double *c_constr)
{

	realizationLoop();
        // reservoir simulation done here
}

void Simulation::realizationLoop()
{

	int weekcounter, yearcounter, season;

	ofstream durham_demand, raleigh_demand, cary_demand;
	durham_demand.open(demandOutputPath + "/durham_demand.csv");
	raleigh_demand.open(demandOutputPath + "/raleigh_demand.csv");
	cary_demand.open(demandOutputPath + "/cary_demand.csv");
	
	for (int realization = 0; realization < numRealizations; realization++) {

        // Initialize demand and reservoir storage objects (year, month, week, daysPerWeek, leapYearCounter)
        simDates.initializeDates(startSimulationYear, 1, 1, 7, 0);

        year = simDates.getYear();//passes the dates from the simDates class to the main simulation
        month = simDates.getMonth();
        week = simDates.getWeek();
        numdays = simDates.getDays();

        while (year - 1 < (terminateYear))
            // in a single realization, from year to year
        {

            // Use the demand PDF to estimate uncertain demand, also finds current level of restrictions (and revenue losses from them)
            // Raleigh uses Durham's data for the inflow-demand PDF
            durham.calculateDemand(realization, week, numdays, year);
            cary.calculateDemand(realization, week, numdays, year);
            raleigh.calculateDemand(realization, week, numdays, year);
            //pass weekly demand values to reservoir storage update function

			durham_demand << durham.weeklyDemand << ",";
			raleigh_demand << raleigh.weeklyDemand << ",";
			cary_demand << cary.weeklyDemand << ",";

            //update timestep
            simDates.increase();
            year = simDates.getYear();
            month = simDates.getMonth();
            week = simDates.getWeek();
            numdays = simDates.getDays();
        } // End weekly loop

		durham_demand << endl;
		raleigh_demand << endl;
		cary_demand << endl;
    }
	durham_demand.close();
	raleigh_demand.close();
	cary_demand.close();
}

Simulation::~Simulation() {}

Simulation::Simulation() {}
