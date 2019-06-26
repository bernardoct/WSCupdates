#include "global.h"
#include "WaterUtility.h"

using namespace std;

WaterUtility::WaterUtility()
{
    numRealizations = 1000;
    terminateYear = 46;
}

WaterUtility::~WaterUtility()
{}

void WaterUtility::writeInflowDemandPDF(int inflowR, int UDr, int rows_PDF, int cols_PDF, double size1, double size2, int irrC, int nonC1, int nonC2, TimeSeriesData* Inflows)
{
	// Local variables intermediate calculations

	if(shouldAllocate)
	{
		general_1d_allocate(inflows1DIrr, UDr*irrC);
		general_1d_allocate(inflows1DNon, UDr*(nonC1+nonC2));
		general_1d_allocate(demand1DIrr, UDr*irrC);
		general_1d_allocate(demand1DNon, UDr*(nonC1+nonC2));

		general_2d_allocate(inflows2DIrr, inflowR, irrC);
		general_2d_allocate(inflows2DNon, inflowR, nonC1+nonC2);
		general_2d_allocate(demand2DIrr, UDr, irrC);
		general_2d_allocate(demand2DNon, UDr, nonC1+nonC2);
	}

    general_2d_allocate(demandVariation, numRealizations, 52*terminateYear, 0.0);
    general_2d_allocate(actualPDF, 16, 17, 0.0);

	for(int y = 0; y < irrC; y++)
	{
		for (int x = 0; x < inflowR; x++)
		{
			inflows2DIrr[x][y] = Inflows->normalizedDataAdj[x][y+16];
		}
		for(int x = 0; x <UDr; x++)
			demand2DIrr[x][y] = UD.normalizedData[x][y+16];
	}
	for (int y = 0; y < nonC1; y++)
	{
		for (int x = 0; x < inflowR; x++)
			inflows2DNon[x][y] = Inflows->normalizedDataAdj[x][y];

		for (int x = 0; x < UDr; x++)
			demand2DNon[x][y] = UD.normalizedData[x][y];
	}
	for (int y = 0; y < nonC2; y++)
	{
		for (int x = 0; x < inflowR; x++)
			inflows2DNon[x][y+16] = Inflows->normalizedDataAdj[x][y+39];

		for (int x = 0; x < UDr; x++)
			demand2DNon[x][y+16] = UD.normalizedData[x][y+39];
	}
	index_array(inflows1DIrr, UDr, irrC, inflowR - UDr, inflows2DIrr);
	index_array(inflows1DNon, UDr, nonC1+nonC2, inflowR - UDr, inflows2DNon);
	index_array(demand1DIrr, UDr, irrC, 0, demand2DIrr);
	index_array(demand1DNon, UDr, nonC1+nonC2, 0, demand2DNon);

	if(shouldAllocate)
	{
		general_2d_allocate(PDFIrr, rows_PDF, cols_PDF, 0);
		general_2d_allocate(PDFNon, rows_PDF, cols_PDF, 0);
		general_2d_allocate(CDFIrr, rows_PDF, cols_PDF, 0);
		general_2d_allocate(CDFNon, rows_PDF, cols_PDF, 0);
	}

	make_PDF(inflows1DIrr, demand1DIrr, UDr*irrC, PDFIrr, size1, size2);
	make_PDF(inflows1DNon, demand1DNon, UDr*(nonC1+nonC2), PDFNon, size1, size2);

	// Calculate cumulative sums up front (saves time in the main loop)
	int tempSumIrr, tempSumNon;
	for(int i = 0; i < rows_PDF; i++)
	{
		tempSumIrr = 0; tempSumNon = 0;
		for(int j = 0; j < cols_PDF; j++)
		{
			tempSumIrr += PDFIrr[i][j];

			CDFIrr[i][j] = tempSumIrr;
			tempSumNon += PDFNon[i][j];
			CDFNon[i][j] = tempSumNon;
		}
	}
}

void WaterUtility::generateDemandVariation(int numWeeks, TimeSeriesData *Inflows, double demand_variation_multiplier)
{

	int totalNumber, partialNumber, counter, index;
	double demandLevel;
	counter = 0;
	partialNumber = 0;

	for (int realization = 0; realization < numRealizations; realization++)
	{
		for (int years = 0; years<terminateYear;years++)
		{
			for (int week = 0; week < numWeeks; week++)
			{
				index = getPDFIndex(Inflows->simulatedData[realization][week+years*52]); // "inflow" index of the inflow-demand PDF
				if (week > 16 && week < 39)
					totalNumber = PDFIrr[index][16] - 1; // this is the total number of historical streamflows that fall into the current streamflow PDF 'bin'
				else
					totalNumber = PDFNon[index][16] - 1;

				if(totalNumber == -1)
					demandLevel = 4;
				else
				{
					if(totalNumber == 0)
						totalNumber = 1;

					partialNumber = rand() % totalNumber + 1; // generate a random demand for this week
					counter = 0;

					// Find the first demand level greater than or equal to partialNumber
					if (week > 16 && week < 39)
						while (CDFIrr[index][counter] < partialNumber)
							counter++;
					else
						while (CDFNon[index][counter] < partialNumber)
							counter++;

					demandLevel = double(counter);

					if (demandLevel > 15)
						demandLevel = 15;
				}


				demandVariation[realization][week+years*52] = ((demandLevel - 8.0)/2.0 + double(rand() % 501)/1000.0)*demand_variation_multiplier;
				if (week>16 && week<39)
				{
					actualPDF[index][counter]+=1;
					actualPDF[index][16] +=1;
				}

			}
		}

	}
}

void WaterUtility::calculateDemand(int realization, int week, int numdays, int year)
{
	weeklyDemand = numdays*futureDemand[year-1]*(demandVariation[realization][week-1+ (year-1)*52]*UD.standardDeviations[week-1] + UD.averages[week-1]);
}

int WaterUtility::getPDFIndex(double inflow)
{
	double counter = -1.0;
	int counterAsInt = -1;
	int loopTrig = 0;
	while (loopTrig == 0)//Finding the inflow condition for joint demand/inflow PDF, owasa
	{
		counter = counter + 1;
		counterAsInt = counterAsInt + 1;
		if (inflow < ((counter/2) - 3.5))
			loopTrig = 1;
	}

	if (counterAsInt > 15)//only 16 discritizations to joint PDF
		counterAsInt = 15;

	return counterAsInt;
}



