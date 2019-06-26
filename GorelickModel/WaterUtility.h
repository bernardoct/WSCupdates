#ifndef WaterUtility_h
#define WaterUtility_h

#include "global.h"
#include "TimeSeriesData.h"
#include "InsuranceFund.h"

class WaterUtility
{
	public:
	
	WaterUtility();
	~WaterUtility();
	
	string name;
	
	data_t UnitDemand;
	data_t FutureDemand;
	double *inflows1DIrr , *inflows1DNon, *demand1DIrr, *demand1DNon;
	double **inflows2DIrr, **inflows2DNon, **demand2DIrr, **demand2DNon;
	double **actualPDF;

	TimeSeriesData UD; // unit demand

	// Demand PDF (fixed number of discrete buckets)
	int **PDFIrr, **PDFNon, **CDFIrr, **CDFNon;

	int terminateYear, numRealizations;
	double **demandBaseline, **demandVariation;
	double averageUse;
	double futureDemand[51];
	double futureDemandBreakdown[5][51];

	double weeklyDemand;
	bool shouldAllocate;


	//~~~~~~~~~~~~~~~~ Public functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int getPDFIndex(double inflow);
	void writeInflowDemandPDF(int InflowR, int UDr, int rows_PDF, int cols_PDF, double size1, double size2, int irrC, int nonC1, int nonC2, TimeSeriesData* Inflows);
	void generateDemandVariation(int numWeeks, TimeSeriesData *Inflows, double demand_variation_multiplier);
	void calculateDemand(int realizations, int week, int numdays, int year);
};

#endif
