#include "global.h"
#include "ReservoirStorage.h"

using namespace std;

//ReservoirStorage::ReservoirStorage()
//{
//}

///// Chapel Hill Options

void ReservoirStorage::buildULexp()
{
	ULCapacity = 2999.0;
        // university lake expansion capacity
}
void ReservoirStorage::buildCCexp()
{
	CCRCapacity = 5909.0;
        // cane creek expansion capacity
}
void ReservoirStorage::buildSQlow()
{
	StQCapacity = 1700.0;
        // stone quarry expansion capacity, small expansion
}
void ReservoirStorage::buildSQhigh()
{
	StQCapacity = 2400.0;
        // stone quarry, large expansion
}

///// Durham Options

void ReservoirStorage::buildTeerQuarry()
{
	teerQuarryIntakeCapacity = 15.0;
	teerQuarryOutflowCapacity = 5.2;
}
void ReservoirStorage::buildReclaimedLow()
{
	durhamReclaimedCapacity = 2.2;
}
void ReservoirStorage::buildReclaimedHigh()
{
	durhamReclaimedCapacity = 11.3;
}
void ReservoirStorage::buildMichieLow()
{
	durhamCapacity = durhamCapacity + 2500.0;
	LMRaleighCapacity = 0.0;
}
void ReservoirStorage::buildMichieHigh(double checkval)
{
	if (checkval == 0)
		// skipped small expansion, direct to non-coop large exp 
	{
		durhamCapacity = durhamCapacity + 7700.0;
		LMRaleighCapacity = 0.0;
	}
	else
		// cooperative expansion for small expansion, but not large exp 
		// or non-cooperative expansion for small and large exp 
	{
		durhamCapacity = durhamCapacity + 5200.0;
		LMRaleighCapacity = LMRaleighCapacity;
	}
}

///// Raleigh Options

void ReservoirStorage::buildLittleRiverRal()
{
	littleRiverRaleighCapacity = 3700.0;
}
void ReservoirStorage::buildRalQuarry()
{
	raleighQuarryCapacity = 4000.0;
	raleighQuarryIntakeCapacity = 50.0;
	raleighQuarryOutflowCapacity = 18.7;
}
void ReservoirStorage::buildNeuseIntake()
{
	raleighIntakeCapacity = 16.0;
}
void ReservoirStorage::reallocateFallsLake(double fallsLakeReallocation)
{
	fallsLakeSupplyCapacity += fallsLakeReallocation;
	fallsLakeQualityCapacity -= fallsLakeReallocation;
	fallsLakeSupplyStorage += fallsLakeReallocation;
	fallsLakeQualityStorage -= fallsLakeReallocation;
	if(fallsLakeQualityStorage < 0)
	{
		fallsLakeSupplyStorage += fallsLakeQualityStorage;
		fallsLakeQualityStorage = 0;
	}
}

///// Regional/Cary Options 

void ReservoirStorage::buildWWWTPlow(double oWTPf, double dWTPf, double rWTPf)
{
	westernTreatmentCapacity = 33.0;
	owasaWWTPcapacity = westernTreatmentCapacity*oWTPf;
	durhamWWTPcapacity = westernTreatmentCapacity*dWTPf;
	raleighWWTPcapacity = westernTreatmentCapacity*rWTPf;
	if(raleighWWTPcapacity > RaleighDurhamCapacity)
	{
		raleighWWTPcapacity = RaleighDurhamCapacity;
	}
}
void ReservoirStorage::buildWWWTPhigh(double oWTPf, double dWTPf, double rWTPf)
{
	westernTreatmentCapacity = 54.0;
	owasaWWTPcapacity = westernTreatmentCapacity*oWTPf;
	durhamWWTPcapacity = westernTreatmentCapacity*dWTPf;
	raleighWWTPcapacity = westernTreatmentCapacity*rWTPf;
	if(raleighWWTPcapacity > RaleighDurhamCapacity)
	{
		raleighWWTPcapacity = RaleighDurhamCapacity;
	}
}
void ReservoirStorage::buildMichieSharedLow(double ratio)
{
	durhamCapacity = durhamCapacity + 2500.0 * ratio;
		// michie small expansion is 2500 MG
	LMRaleighCapacity = 2500.0 * (1-ratio);
		// raleigh capacity given in expanded LM 
}
void ReservoirStorage::buildMichieSharedHigh(double ratio, double checkval)
{
	if (checkval == 0)
		// no small exp, direct to cooperative large exp 
	{
		durhamCapacity = durhamCapacity + 7700.0 * ratio;
			// michie large expansion is 7700 MG 
		LMRaleighCapacity = 7700.0 * (1-ratio);
	}
	else if (checkval < 1)
		// small cooperative exp >>>> large coop exp 
	{
		durhamCapacity = durhamCapacity + 5200.0 * ratio;
			// michie large expansion is 7700 MG 
		LMRaleighCapacity = 5200.0 * (1-ratio);
	}
	else
		// small non-coop expansion >>>>> large coop exp 
	{
		durhamCapacity = durhamCapacity + 5200.0 * ratio;
			// michie large expansion is 7700 MG 
		LMRaleighCapacity = 5200.0 * (1-ratio);
	}
}

void ReservoirStorage::initializeReservoirStorage(double durhamCap, double CCRCap, double StQCap, double ULCap, double lakeWBCap, double fallsLakeSupplyCap, double fallsLakeQualityCap, double jordanSupplyCap, double jordanQualityCap, double CaryTreatmentCap, double DurhamCaryCap, double DurhamOWASACap, double RaleighCaryCap, double RaleighDurhamCap, double raleighJordanAlloc, double durhamJordanAlloc, double owasaJordanAlloc, double caryJordanAlloc, double teerQCap, double tQIc, double tQOc, double littleRiverRaleighCap, double westTrtCap, double durhamRecCap, double rQc, double rQIc, double rQOc, double rIc, double cQc, double cQIc, double cQOc, double owasaWWWTPFrac, double durhamWWWTPFrac, double raleighWWWTPFrac, int sig, double DurhamReleaseCap, double DurhamReleaseMin, double RaleighLMCap)
{
	//////// Set reservoir capacities and initial storage volumes
	
	////Durham Storage infrastructure
	durhamCapacity=durhamCap;//Durham capacity
	durhamStorage=durhamCap;//Initial storage set to capacity
	durhamReclaimedCapacity = durhamRecCap;///Durham reclaimed water capacity
	teerQuarryCapacity = teerQCap;
	teerQuarryStorage = teerQCap;
	teerQuarryIntakeCapacity = tQIc;
	teerQuarryOutflowCapacity = tQOc;

	////OWASA Storage infrastructure
	CCRCapacity = CCRCap;//OWASA Cane Creek reservoir capacity
	CCRStorage = CCRCap;//Initial storage set to capacity
	ULCapacity = ULCap;//OWASA University Lake reservoir capacity
	ULStorage = ULCap;//Initial storage set to capacity
	StQCapacity = StQCap;//OWASA Stone Quarry reservoir capacity
	StQStorage = StQCap;//Initial storage set to capacity
	OWASACapacity = CCRCap+StQCap+ULCap;//OWASA total capacity
	OWASAStorage = OWASACapacity;//Initial storage set to capacity

	////Raleigh Storage infrastructure
	lakeWBCapacity = lakeWBCap;//Raleigh Lakes Wheeler and Benson capacity
	lakeWBStorage = lakeWBCap;//Initial storage set to capacity
	fallsLakeSupplyCapacity = fallsLakeSupplyCap;//Capacity of Falls Lake Water Supply allocation
	fallsLakeQualityCapacity = fallsLakeQualityCap;//Capacity of Falls Lake Water Quality allocation
	fallsLakeSupplyStorage = fallsLakeSupplyCap;//Initial storage set to capacity
	fallsLakeQualityStorage = fallsLakeQualityCap;//Initial storage set to capacity
	littleRiverRaleighCapacity = littleRiverRaleighCap;//Little River Reservoir capacity
	littleRiverRaleighStorage = littleRiverRaleighCap;//Initial storage set to capacity
	raleighQuarryCapacity = rQc;
	raleighQuarryStorage = 0.0;//quarry starts empty
	raleighQuarryIntakeCapacity = rQIc;//maximum quarry filling rate
	raleighQuarryOutflowCapacity = rQOc;//maximum withdrawls from quarry
	raleighIntakeCapacity = rIc;//maximum withdrawl from Neuse intake

	LMRaleighCapacity = RaleighLMCap;
	raleighMichieStorage = 0.0;
		// no initial allocation of Lake Michie for Raleigh 

	////Cary Storage Infrastructure
	caryQuarryCapacity = cQc;
	caryQuarryStorage = 0.0;//quarry starts empty
	caryQuarryIntakeCapacity = cQIc;//maximum quarry filling rate
	caryQuarryOutflowCapacity = cQOc;//maximum withdrawls from quarry

	////Interconnections
	DurhamCaryCapacity = DurhamCaryCap;//Capacity of the Durham Cary interconnection
	DurhamOWASACapacityToD = DurhamOWASACap;//Capacity of the Durham OWASA interconnection
	DurhamOWASACapacityToO = DurhamOWASACap;
		// initially the same
	RaleighCaryCapacity = RaleighCaryCap;//Capacity of the Raleigh Cary interconnection
	RaleighDurhamCapacity = RaleighDurhamCap;//Capacity of the Raleigh Durham interconnection
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////// RAW RELEASE "INTERCONNECTION" CONTRAINTS ////////////////////////////////////////////
	
	// DurhamRaleighMaximumDailyRelease = DurhamReleaseCap;
		// maximum water releasable, in MGD
	// DurhamRaleighMinimumDailyRelease = DurhamReleaseMin;
		// minimum water releasable, in MGD
	
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////Jordan Lake treatment Plants
	CaryTreatmentCapacity = CaryTreatmentCap;//Cary WTP capacity
	westernTreatmentCapacity =westTrtCap;////Western Wake WTP capacity
	owasaWWTPcapacity = westernTreatmentCapacity*owasaWWWTPFrac;
	durhamWWTPcapacity = westernTreatmentCapacity*durhamWWWTPFrac;
	raleighWWTPcapacity = westernTreatmentCapacity*raleighWWWTPFrac;
	if(raleighWWTPcapacity>RaleighDurhamCapacity)
	{
		raleighWWTPcapacity = RaleighDurhamCapacity;
	}

	////Jordan Lake accounting
	jordanLakeSupplyCapacity = jordanSupplyCap;
	jordanLakeQualityCapacity = jordanQualityCap;
	jordanLakeSupplyStorage = jordanSupplyCap;
	jordanLakeQualityStorage = jordanQualityCap;

		///Raleigh
	raleighAllocation = raleighJordanAlloc;//Raleigh allocation to jordan Lake
	raleighJordanCapacity = raleighJordanAlloc*jordanLakeSupplyCapacity;
	raleighJordanStorage = raleighJordanCapacity;
		///Cary
	caryAllocation = caryJordanAlloc;//Cary allocation to jordan lake
	caryJordanCapacity = caryJordanAlloc*jordanLakeSupplyCapacity;
	caryJordanStorage = caryJordanCapacity;
		///Durham
	durhamAllocation = durhamJordanAlloc;//Durham allocation to jordan lake
	durhamJordanCapacity = durhamJordanAlloc*jordanLakeSupplyCapacity;
	durhamJordanStorage = durhamJordanCapacity;
		///OWASA
	owasaAllocation = owasaJordanAlloc;// owasa allocation to jordan lake
	owasaJordanCapacity = owasaJordanAlloc*jordanLakeSupplyCapacity;
	owasaJordanStorage = owasaJordanCapacity;
		///Inflow fractions (how to divide the inflows to respective allocations)
	jordanQualityFraction = jordanLakeQualityCapacity/(jordanLakeSupplyCapacity+jordanLakeQualityCapacity);
	jordanRaleighFraction = raleighAllocation*(jordanLakeSupplyCapacity/(jordanLakeSupplyCapacity + jordanLakeQualityCapacity));
	jordanDurhamFraction = durhamAllocation*(jordanLakeSupplyCapacity/(jordanLakeSupplyCapacity + jordanLakeQualityCapacity));
	jordanCaryFraction = caryAllocation*(jordanLakeSupplyCapacity/(jordanLakeSupplyCapacity + jordanLakeQualityCapacity));
	jordanOWASAFraction = owasaAllocation*(jordanLakeSupplyCapacity/(jordanLakeSupplyCapacity + jordanLakeQualityCapacity));

	durhamRequest = 0;//Durham intial transfer request
	owasaRequest = 0;//OWASA initial transfer request
	raleighRequest = 0;//Raleigh initial transfer request
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////// Initialized Release (or release buyback) Request //////////////////////////
	
	RreleaseRequest = 0;
		// Raleigh request for releases from Durham supply
	DbuybackQuantity = 0;
		// Durham request buy back releases owed to Raleigh
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	thingCounter = sig;

	return;
}
void ReservoirStorage::initializeReservoirStorageROF(double durhamCap, double CCRCap, double StQCap, double ULCap, double lakeWBCap, double fallsLakeSupplyCap, double fallsLakeQualityCap, double jordanSupplyCap, double jordanQualityCap, double CaryTreatmentCap, double DurhamCaryCap, double DurhamOWASACap, double RaleighCaryCap, double RaleighDurhamCap, double raleighJordanAlloc, double durhamJordanAlloc, double owasaJordanAlloc, double caryJordanAlloc, double teerQCap, double tQIc, double tQOc,  double littleRiverRaleighCap, double westTrtCap, double durhamRecCap, double rQc, double rQIc, double rQOc, double rIc, double cQc, double cQIc, double cQOc, double owasaWWWTPFrac, double durhamWWWTPFrac, double raleighWWWTPFrac, int sig, double DurhamReleaseCap, double DurhamReleaseMin, double RaleighLMCap)
{
	//Set reservoir capacities and initial storage volumes
	//Durham Storage infrastructure
	durhamCapacity=durhamCap;//Durham capacity
	durhamReclaimedCapacity = durhamRecCap;///Durham reclaimed water capacity
	teerQuarryCapacity = teerQCap;
	teerQuarryIntakeCapacity = tQIc;
	teerQuarryOutflowCapacity = tQOc;

	////OWASA Storage infrastructure
	CCRCapacity = CCRCap;//OWASA Cane Creek reservoir capacity
	ULCapacity = ULCap;//OWASA University Lake reservoir capacity
	StQCapacity = StQCap;//OWASA Stone Quarry reservoir capacity
	OWASACapacity = CCRCap+StQCap+ULCap;//OWASA total capacity

	////Raleigh Storage infrastructure
	lakeWBCapacity = lakeWBCap;//Raleigh Lakes Wheeler and Benson capacity
	fallsLakeSupplyCapacity = fallsLakeSupplyCap;//Capacity of Falls Lake Water Supply allocation
	fallsLakeQualityCapacity = fallsLakeQualityCap;//Capacity of Falls Lake Water Quality allocation
	littleRiverRaleighCapacity = littleRiverRaleighCap;//Little River Reservoir capacity
	raleighQuarryCapacity = rQc;
	raleighQuarryIntakeCapacity = rQIc;//maximum quarry filling rate
	raleighQuarryOutflowCapacity = rQOc;//maximum withdrawls from quarry
	raleighIntakeCapacity = rIc;//maximum withdrawl from Neuse intake

	LMRaleighCapacity = RaleighLMCap;

	////Cary Storage Infrastructure
	caryQuarryCapacity = cQc;
	caryQuarryIntakeCapacity = cQIc;//maximum quarry filling rate
	caryQuarryOutflowCapacity = cQOc;//maximum withdrawls from quarry

	////Interconnections
	DurhamCaryCapacity = DurhamCaryCap;//Capacity of the Durham Cary interconnection
	DurhamOWASACapacityToD = DurhamOWASACap;//Capacity of the Durham OWASA interconnection TO DURHAM
	DurhamOWASACapacityToO = DurhamOWASACap;
		// initially the same
	RaleighCaryCapacity = RaleighCaryCap;//Capacity of the Raleigh Cary interconnection
	RaleighDurhamCapacity = RaleighDurhamCap;//Capacity of the Raleigh Durham interconnection
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////// RAW RELEASE "INTERCONNECTION" CONTRAINTS ////////////////////////////////////////////
	
	// DurhamRaleighMaximumDailyRelease = DurhamReleaseCap;
		// maximum water releasable, in MGD
	// DurhamRaleighMinimumDailyRelease = DurhamReleaseMin;
		// minimum water releasable, in MGD
	
	////////////////////////////////////////////////////////////////////////////////////////////////////

	////Jordan Lake treatment Plants
	CaryTreatmentCapacity = CaryTreatmentCap;//Cary WTP capacity
	westernTreatmentCapacity =westTrtCap;
	owasaWWTPcapacity = westernTreatmentCapacity*owasaWWWTPFrac;
	durhamWWTPcapacity = westernTreatmentCapacity*durhamWWWTPFrac;
	raleighWWTPcapacity = westernTreatmentCapacity*raleighWWWTPFrac;
	if(raleighWWTPcapacity>RaleighDurhamCapacity)
	{
		raleighWWTPcapacity = RaleighDurhamCapacity;
	}


	////Jordan Lake accounting
	jordanLakeSupplyCapacity = jordanSupplyCap;
	jordanLakeQualityCapacity = jordanQualityCap;

		///Raleigh
	raleighAllocation = raleighJordanAlloc;//Raleigh allocation to jordan Lake
	raleighJordanCapacity = raleighJordanAlloc*jordanLakeSupplyCapacity;
		///Cary
	caryAllocation = caryJordanAlloc;//Cary allocation to jordan lake
	caryJordanCapacity = caryJordanAlloc*jordanLakeSupplyCapacity;
		///Durham
	durhamAllocation = durhamJordanAlloc;//Durham allocation to jordan lake
	durhamJordanCapacity = durhamJordanAlloc*jordanLakeSupplyCapacity;
		///OWASA
	owasaAllocation = owasaJordanAlloc;// owasa allocation to jordan lake
	owasaJordanCapacity = owasaJordanAlloc*jordanLakeSupplyCapacity;
		///Inflow fractions (how to divide the inflows to respective allocations)
	jordanQualityFraction = jordanLakeQualityCapacity/(jordanLakeQualityCapacity+jordanLakeSupplyCapacity);
	jordanRaleighFraction = raleighAllocation*(jordanLakeSupplyCapacity/(jordanLakeSupplyCapacity + jordanLakeQualityCapacity));
	jordanDurhamFraction = durhamAllocation*(jordanLakeSupplyCapacity/(jordanLakeSupplyCapacity + jordanLakeQualityCapacity));
	jordanCaryFraction = caryAllocation*(jordanLakeSupplyCapacity/(jordanLakeSupplyCapacity + jordanLakeQualityCapacity));
	jordanOWASAFraction = owasaAllocation*(jordanLakeSupplyCapacity/(jordanLakeSupplyCapacity + jordanLakeQualityCapacity));

	durhamRequest = 0;//Durham intial transfer request
	owasaRequest = 0;//OWASA initial transfer request
	raleighRequest = 0;//Raleigh initial transfer request
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////// Initialized Release (or release buyback) Request //////////////////////////
	
	RreleaseRequest = 0;
		// Raleigh request for releases from Durham supply
	DbuybackQuantity = 0;
		// Durham request buy back releases owed to Raleigh
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	thingCounter = sig;

	return;
}

void ReservoirStorage::updateReservoirStorageROF(double durhamS, double teerS, double CCRS, double ULS, double STQS, double owasaS, double lakeWBS, double flSS, double flQS, double jlSS, double jlQS, double caryJordanS, double raleighJordanS, double durhamJordanS, double owasaJordanS, double littleRiverRalS, double raleighQS, double RLMstor)
{
	//Same as above, allowing for variable initial reservoir storage
	// THESE ARE ALL CURRENT LEVELS
	durhamStorage=durhamS;
	teerQuarryStorage = teerS;
	CCRStorage = CCRS;
	ULStorage = ULS;
	StQStorage = STQS;
	OWASAStorage = owasaS;
	lakeWBStorage = lakeWBS;
	fallsLakeSupplyStorage = flSS;
	fallsLakeQualityStorage = flQS;
	jordanLakeSupplyStorage = jlSS;
	jordanLakeQualityStorage = jlQS;
	durhamRequest = 0;//Durham intial transfer request
	owasaRequest = 0;//OWASA initial transfer request
	raleighRequest = 0;//Raleigh initial transfer request
	caryJordanStorage = caryJordanS;
	raleighJordanStorage = raleighJordanS;
	durhamJordanStorage = durhamJordanS;
	owasaJordanStorage = owasaJordanS;
	littleRiverRaleighStorage = littleRiverRalS;
	raleighQuarryStorage = raleighQS;
	caryQuarryStorage = 0;
	
	raleighMichieStorage = RLMstor;
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////// Initialized Release (or release buyback) Request //////////////////////////
	
	RreleaseRequest = 0;
		// Raleigh request for releases from Durham supply
	DbuybackQuantity = 0;
		// Durham request buy back releases owed to Raleigh
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	return;
}
void ReservoirStorage::updateReservoirStorageROF()
{
	//Same as above, allowing for variable initial reservoir storage
	// THESE ARE ALL AS IF RESERVOIRS ARE FULL
	durhamStorage=durhamCapacity;
	teerQuarryStorage = teerQuarryCapacity;
	CCRStorage = CCRCapacity;
	ULStorage = ULCapacity;
	StQStorage = StQCapacity;
	OWASAStorage = OWASACapacity;
	lakeWBStorage = lakeWBCapacity;
	fallsLakeSupplyStorage = fallsLakeSupplyCapacity;
	fallsLakeQualityStorage = fallsLakeQualityCapacity;
	jordanLakeSupplyStorage = jordanLakeSupplyCapacity;
	jordanLakeQualityStorage = jordanLakeQualityCapacity;
	durhamRequest = 0;//Durham intial transfer request
	owasaRequest = 0;//OWASA initial transfer request
	raleighRequest = 0;//Raleigh initial transfer request
	caryJordanStorage = caryJordanCapacity;
	raleighJordanStorage = raleighJordanCapacity;
	durhamJordanStorage = durhamJordanCapacity;
	owasaJordanStorage = owasaJordanCapacity;
	littleRiverRaleighStorage = littleRiverRaleighCapacity;
	raleighQuarryStorage = raleighQuarryCapacity;
	caryQuarryStorage = 0;
	
	raleighMichieStorage = LMRaleighCapacity;
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	///////////////// Initialized Release (or release buyback) Request //////////////////////////
	
	RreleaseRequest = 0;
		// Raleigh request for releases from Durham supply
	DbuybackQuantity = 0;
		// Durham request buy back releases owed to Raleigh
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	return;
}
void ReservoirStorage::openResFiles()
{
	openFile(out2, "reservoirOutput2.csv");
	return;
}

void ReservoirStorage::setDemands(double durham, double owasa, double raleigh, double cary, int numberOfDaysInTheWeek)
{
	durhamUse = durham - durhamReclaimedCapacity;
	OWASAUse = owasa;
	CaryUse = cary;
	raleighUse = raleigh;
	numdays = numberOfDaysInTheWeek;
	return;
}

void ReservoirStorage::getReturnRatios(double raleighR, double durhamR)
{
	raleighReturn = raleighR;
	durhamReturn = durhamR;
	return;
}

void ReservoirStorage::setInflow(double durham, double ccr, double ul, double stq, double falls, double wb, double clayton, double crabtree, double jordan, double lillington,
									double ralRet, double durRet, double durRet2, double owasaRet, double evaporationf, double evaporationwb, double evaporation, double ltlRvrRal)
{
	durhamInflow = durham;//Inflows to Durham Reservoirs
	CCRInflow = ccr;//Inflows to Cane Creek Reservoir
	ULInflow = ul;//Infow to University Lake
	StQInflow = stq;//Inflow to Stone Quarry
	fallsInflow = falls;//Inflow to Falls Lake
	wbInflow = wb;//Inflow to Wheeler Benson
	claytonInflow = clayton;//Inflow to clayton gauge
	crabtreeInflow = crabtree;//Inflow to Crabtree Creek
	jordanInflow = jordan;//Inflow to Jordan Lake
	lillingtonInflow = lillington;//Inflows to lillington Gauge
	raleighReturn = ralRet;//Raleigh WW Returns downstream of Falls Lake
	raleighReturnOut = ralRet;
	durhamReturn = durRet;//Durham WW Returns to the Neuse Basin
	durhamReturnOut = durRet;
	durhamReturn2 = durRet2;//Durham WW Returns to the Cape Fear Basin
	owasaReturn = owasaRet;//OWASA WW Returns to Cape Fear Basin
	evapF = evaporationf;//Evaporation from Falls Lake
	evapWB = evaporationwb;//Evaporation from Wheeler and Benson
	evap = evaporation;//Evaporation from Durham and OWASA
	littleRiverRaleighInflow = ltlRvrRal;//Inflows to Raleigh's little river reservoir
	return;
}

void ReservoirStorage::setSpillover(int week)
{
	if (week <21 || week>47)//Spillage rules for Durham Reservoir at Little River
	{
		durhamSpillage = 3.877*numdays;
		raleighLittleRiverSpillage = 3.877*numdays;
	}
	else
	{
		durhamSpillage = 9.05;
		raleighLittleRiverSpillage = 9.05;
	}

	if (week>7&&week<16)
	{
		enoSpillage = numdays*30/1.54;
	}
	else
	{
		enoSpillage = numdays*10.0/1.54;
	}

	if(lakeWBStorage <0.3*lakeWBCapacity)
	{
		lakeWBSpillage = numdays/1.547;
	}
	else if(lakeWBStorage < 0.6*lakeWBCapacity)
	{
		lakeWBSpillage = 2.0*numdays/1.547;
	}
	else
	{
		lakeWBSpillage = 3.0*numdays/1.547;
	}
	/////Determining Jordan Lake environmental releases - from Jordan Lake Water Quality Pool
	/////Jordan Lake environmental flows must hit minimum reservoir releases and minimum downstream flow targets
	if (jordanLakeQualityStorage/jordanLakeQualityCapacity>.4)
	{
		jordanLakeMinRelease = 25.8527*numdays;
		////releases at reservoir must be at least 25.8 MGD

		/////downstream flow requirements are based on current storage in Jordan Lake Quality Pool
		if (jordanLakeQualityStorage/jordanLakeQualityCapacity>.8)
		{
			if(jordanLakeMinRelease+lillingtonInflow<387.7901*numdays)
			{
				jordanLakeMinRelease = 387.7901*numdays-lillingtonInflow;
				//////flows at downstream gauge must be at least 387.8 MGD
			}
		}
		else if(jordanLakeQualityStorage/jordanLakeQualityCapacity>.6)
		{
			if(jordanLakeMinRelease+lillingtonInflow<290.8426*numdays)
			{
				jordanLakeMinRelease = 290.8426*numdays-lillingtonInflow;
				//////flows at downstream gauge must be at least 387.8 MGD
			}
		}
		else
		{
			if(jordanLakeMinRelease+lillingtonInflow<193.8951*numdays)
			{
				jordanLakeMinRelease = 193.8951*numdays-lillingtonInflow;
				//////flows at downstream gauge must be at least 387.8 MGD
			}
		}
	}
	////during extremely low storage conditions, downstream flow requirements are abandoned and
	////minimum releases are decreased
	else if (jordanLakeQualityStorage/jordanLakeQualityCapacity>.2)
	{
		jordanLakeMinRelease = 129.2634*numdays;
	}
	else
	{
		jordanLakeMinRelease = 64.6317*numdays;
	}


	return;
}
double ReservoirStorage::updateOWASAStorage()
{

	////OWASA transfers are added to Cane Creek Reservoir Storage
	CCRStorage+=owasaRequest;
	double OWASADemand1 = 0.0;////OWASA demands from Western Wake WTP
	double OWASADemand2 = 0.0;////OWASA demands from Jones Ferry WTP
	if(owasaJordanStorage + OWASAStorage > 0.0)
	{
		OWASADemand1 = OWASAUse*(owasaJordanStorage/(owasaJordanStorage+OWASAStorage));
		OWASADemand2 = OWASAUse*(OWASAStorage/(owasaJordanStorage+OWASAStorage));
		if(OWASADemand2 < 3.0*numdays)
		{
			OWASADemand1 += OWASADemand2 - 3.0*numdays;
			OWASADemand2 = 3.0*numdays;
		}
		
		double Olimiter;
			// find the limiting factor for water movement from the joint WTP on Jordan Lake
			// to OWASA (has to flow through Durham, and OWASA has to share the pipe capacity
			// relative to Raleigh and Durham allocations)
		if (owasaWWTPcapacity < WJLWTPfracO/WJLWTPfracTOT*WJLWTPinterconnectCapacity)
			Olimiter = owasaWWTPcapacity;
		else
			Olimiter = WJLWTPfracO/WJLWTPfracTOT*WJLWTPinterconnectCapacity;
		
		if (Olimiter > DurhamOWASACapacityToO)
			Olimiter = DurhamOWASACapacityToO;
		
		if (WJLWTPfracO <= 0.0)
			Olimiter = 0.0;
	
		if(OWASADemand1 > Olimiter*numdays)
		{
			OWASADemand2 += OWASADemand1 - Olimiter*numdays;
			OWASADemand1 = Olimiter*numdays;
		}
	}
	else
	{
		OWASADemand1 = 0.0;
		OWASADemand2 = OWASAUse;
	}

	//Spillage calculated from Cane Creek Reservoir - based on inflow to CCR
	if (CCRInflow > 1.797*7)
	{
		OWASASpillage = 1.797*7;
		MinCCROut = 1.797*7;
	}
	else if (CCRInflow < .1422*7)
	{
		OWASASpillage = .1422*7;
		MinCCROut = .1422*7;
	}
	else
	{
		OWASASpillage = CCRInflow;
		MinCCROut = CCRInflow;
	}

	//No environmental flow requirements for Stone Quarry, spillage is based on capacity constraints
	if (StQStorage +StQInflow - 10*evap>StQCapacity)
	////Stone quarry has a surface area of 10 acres
	{
		StQSpillage = StQStorage + StQInflow -10*evap - StQCapacity;
		StQStorage = StQCapacity;
	}
	else
	{
		StQStorage = StQStorage + StQInflow - 10*evap;
		StQSpillage = 0;
	}
	owasaExcess = 0.0;
	//OWASA has three reservoirs, and they use water from their different reservoirs depending on storage levels
	if ((ULStorage + ULInflow + StQSpillage - 212*evap - OWASADemand2) > .75*ULCapacity)
	/////University Lake Inflows include Stone Quarry Spillage
	/////Surface area of University Lake is 212 acres
	{
		////All use comes from University Lake
		ULStorage = ULStorage + ULInflow + StQSpillage - 212*evap - OWASADemand2;
		ULDemands = OWASADemand2;
		if (ULStorage > ULCapacity)
		{
			OWASASpillage += ULStorage - ULCapacity;
			ULSpillage = ULStorage-ULCapacity;
			owasaExcess += ULStorage - ULCapacity;
			ULStorage = ULCapacity;
		}
		else
		{
			ULSpillage = 0;
		}
		////No use from Cane Creek Reservoir (Cane Creek Reservoir surface area is 500 acres)
		CCRStorage = CCRStorage +CCRInflow - 500*evap - OWASASpillage;
		CCRDemands = 0;
		if (CCRStorage > CCRCapacity)
		{
			OWASASpillage += CCRStorage - CCRCapacity;
			CCRSpillage = CCRStorage-CCRCapacity+MinCCROut;
			owasaExcess += CCRStorage - CCRCapacity;
			CCRStorage = CCRCapacity;
		}
		else
		{
			CCRSpillage = MinCCROut;
		}
	}
	else if ((ULStorage + ULInflow + StQSpillage - 212*evap)< .25*ULCapacity)
	{
		if (OWASADemand2<70)
		{
			////No water use comes from University Lake
			ULStorage = ULStorage + ULInflow + StQSpillage - 212*evap;
			ULDemands = 0;
			if (ULStorage > ULCapacity)
			{
				OWASASpillage += ULStorage - ULCapacity;
				ULSpillage = ULStorage-ULCapacity;
				owasaExcess += ULStorage - ULCapacity;
				ULStorage = ULCapacity;
			}
			else
			{
				ULSpillage = 0;
			}

			////All water use come from Cane Creek Reservoir
			CCRStorage = CCRStorage +CCRInflow - 500*evap - OWASASpillage - OWASADemand2;
			CCRDemands = OWASADemand2;
			if (CCRStorage > CCRCapacity)
			{
				OWASASpillage += CCRStorage - CCRCapacity;
				CCRSpillage = CCRStorage-CCRCapacity+MinCCROut;
				owasaExcess += CCRStorage - CCRCapacity;
				CCRStorage = CCRCapacity;
			}
			else
			{
				CCRSpillage = MinCCROut;
			}
		}
		else
		{
			////First 70 MG comes from Cane Creek Reservoir, remainder comes from University Lake
			ULStorage = ULStorage + ULInflow + StQSpillage - 212*evap - (OWASADemand2 - 70);
			ULDemands = OWASADemand2 - 70;
			if (ULStorage > ULCapacity)
			{
				OWASASpillage += ULStorage - ULCapacity;
				ULSpillage = ULStorage-ULCapacity;
				owasaExcess += ULStorage - ULCapacity;
				ULStorage = ULCapacity;
			}
			else
			{
				ULSpillage = 0;
			}

			CCRStorage = CCRStorage +CCRInflow - 500*evap - OWASASpillage - 70;
			CCRDemands = 70;
			if (CCRStorage > CCRCapacity)
			{
				OWASASpillage += CCRStorage - CCRCapacity;
				CCRSpillage = CCRStorage-CCRCapacity+MinCCROut;
				owasaExcess += CCRStorage - CCRCapacity;
				CCRStorage = CCRCapacity;
			}
			else
			{
				CCRSpillage = MinCCROut;
			}
		}
	}
	else if ((ULStorage + ULInflow + StQSpillage - evap*212)> .5*OWASADemand2)
	{
		////half of the water use comes from University Lake
		ULStorage = ULStorage + ULInflow + StQSpillage - 212*evap - .5*OWASADemand2;
		ULDemands = .5*OWASADemand2;
		if (ULStorage > ULCapacity)
		{
			OWASASpillage += ULStorage - ULCapacity;
			ULSpillage = ULStorage-ULCapacity;
			owasaExcess += ULStorage - ULCapacity;
			ULStorage = ULCapacity;
		}
		else
		{
			ULSpillage = 0;
		}

		////half of the water use comes from cane Creek Reservoir
		CCRStorage = CCRStorage +CCRInflow - 500*evap - OWASASpillage - .5*OWASADemand2;
		CCRDemands = .5*OWASADemand2;
		if (CCRStorage > CCRCapacity)
		{
			OWASASpillage += CCRStorage - CCRCapacity;
			CCRSpillage = CCRStorage-CCRCapacity+MinCCROut;
			owasaExcess += CCRStorage - CCRCapacity;
			CCRStorage = CCRCapacity;
		}
		else
		{
			CCRSpillage = MinCCROut;
		}
	}
	else if ((CCRStorage + CCRInflow -evap*500 - OWASASpillage)>OWASADemand2)
	{
		ULStorage = ULStorage + ULInflow + StQSpillage - 212*evap;
		ULDemands = 0;
		if (ULStorage > ULCapacity)
		{
			OWASASpillage += ULStorage - ULCapacity;
			ULSpillage = ULStorage-ULCapacity;
			owasaExcess += ULStorage - ULCapacity;
			ULStorage = ULCapacity;
		}
		else
		{
			ULSpillage = 0;
		}
		////all water use comes from Cane Creek Reservoir
		CCRStorage = CCRStorage +CCRInflow - 500*evap - OWASASpillage - OWASADemand2;
		CCRDemands = OWASADemand2;
		if (CCRStorage > CCRCapacity)
		{
			OWASASpillage += CCRStorage - CCRCapacity;
			CCRSpillage = CCRStorage-CCRCapacity+MinCCROut;
			owasaExcess += CCRStorage - CCRCapacity;
			CCRStorage = CCRCapacity;
		}
		else
		{
			CCRSpillage = MinCCROut;
		}
	}
	else
	{
		////water needs not met, Stone Quarry emptied into Cane Creek
		ULStorage = ULStorage + ULInflow + StQSpillage - evap*212;
		ULSpillage = ULStorage;
		ULDemands = 0;
		CCRStorage = CCRStorage + CCRInflow - 500*evap - OWASASpillage;
		CCRStorage = CCRStorage + StQStorage + ULStorage;
		CCRSpillage = OWASASpillage;
		ULStorage = 0;
		StQStorage = 0;
		CCRStorage = CCRStorage - OWASADemand2;
		CCRDemands = OWASADemand2;
	}
	////non-negativity
	if (CCRStorage<0)
	{
		CCRStorage = 0;
	}
	if (ULStorage<0)
	{
		ULStorage = 0;
	}
	if (StQStorage<0)
	{
		StQStorage = 0;
	}
	//////aggregating OWASA Storage
	OWASAStorage = CCRStorage + ULStorage + StQStorage;
	return OWASADemand1;
}
double ReservoirStorage::updateDurhamStorage()
{
	double durhamDemand1 = 0.0;
	double durhamDemand2 = 0.0;
	double durhamDemand3 = 0.0;
	if(teerQuarryStorage == 0.0)
	{
		teerQuarryStorage = .001;
	}
	if(durhamJordanStorage == 0.0)
	{
		durhamJordanStorage = .001;
	}
	if(durhamStorage == 0.0)
	{
		durhamStorage = .001;
	}
	durhamDemand1 = durhamUse*(teerQuarryStorage/(teerQuarryStorage+durhamJordanStorage+durhamStorage));

	durhamDemand2 = durhamUse*(durhamJordanStorage/(teerQuarryStorage+durhamJordanStorage+durhamStorage));

	durhamDemand3 = durhamUse*(durhamStorage/(teerQuarryStorage+durhamJordanStorage+durhamStorage));
	if(durhamDemand1>teerQuarryOutflowCapacity*numdays)
	{
		durhamDemand1 = teerQuarryOutflowCapacity*numdays;

		durhamDemand2 = (durhamUse-durhamDemand1)*(durhamJordanStorage/(durhamJordanStorage+durhamStorage));

		durhamDemand3 = (durhamUse-durhamDemand1)*(durhamStorage/(durhamJordanStorage+durhamStorage));
	}
	
	double Dlimiter;
	if (durhamWWTPcapacity < WJLWTPfracD/WJLWTPfracTOT*WJLWTPinterconnectCapacity)
		Dlimiter = durhamWWTPcapacity;
	else
		Dlimiter = WJLWTPfracD/WJLWTPfracTOT*WJLWTPinterconnectCapacity;
	
	if (Dlimiter > WJLWTPfracD/WJLWTPfracRDonly*DurhamOWASACapacityToD)
		Dlimiter = WJLWTPfracD/WJLWTPfracRDonly*DurhamOWASACapacityToD;
	
	if (WJLWTPfracD <= 0.0)
		Dlimiter = 0.0;
	
	if(durhamDemand2 > Dlimiter*numdays)
	{
		durhamDemand2 = Dlimiter*numdays;

		durhamDemand1 = (durhamUse-durhamDemand2)*(teerQuarryStorage/(teerQuarryStorage+durhamStorage));
		if(durhamDemand1>teerQuarryOutflowCapacity*numdays)
		{
			durhamDemand1 = teerQuarryOutflowCapacity*numdays;
			durhamDemand3 = durhamUse - Dlimiter*numdays - teerQuarryOutflowCapacity*numdays;
		}
		else
		{
			durhamDemand3 = (durhamUse-durhamDemand2)*(durhamStorage/(teerQuarryStorage+durhamStorage));
		}
	}

	double teerDiversionInt = 0.0;
	/////////Eno River Diversions////////////////////////////
	if((fallsInflow - enoSpillage)>numdays*teerQuarryIntakeCapacity)
	{
		teerDiversionInt = numdays*teerQuarryIntakeCapacity;
	}
	else if((fallsInflow - enoSpillage)>0)
	{
		teerDiversionInt = fallsInflow - enoSpillage;
	}
	else
	{
		teerDiversionInt = 0.0;
	}
	if((teerDiversionInt + teerQuarryStorage - durhamDemand1)>teerQuarryCapacity)
	{
		teerDiversion = teerQuarryCapacity - teerQuarryStorage + durhamDemand1;
	}
	else
	{
		teerDiversion = teerDiversionInt;
	}
	///////////////////////////////////////////////////////////


	/////Teer Quarry Water Balance
	teerQuarryStorage = teerQuarryStorage + teerDiversion - evap*(20 + 30*(teerQuarryStorage/(teerQuarryCapacity+.001))) - durhamDemand1;
	/////boundry conditions
	if(teerQuarryStorage>teerQuarryCapacity)
	{
		teerDiversion += teerQuarryCapacity - teerQuarryStorage;
		teerQuarryStorage = teerQuarryCapacity;
	}
	if(teerQuarryStorage<0.0)
	{
		durhamDemand3 -= teerQuarryStorage;
		teerQuarryStorage = 0.0;
	}

	////Little River Reservoir/Lake Michie Water Balance
	durhamStorage = durhamStorage + durhamInflow*(1 - LMRaleighCapacity/(LMRaleighCapacity + durhamCapacity)) - durhamDemand3 - (evap*1069.0 + durhamSpillage)*(1 - LMRaleighCapacity/(LMRaleighCapacity + durhamCapacity));
	raleighMichieStorage = raleighMichieStorage + durhamInflow*(LMRaleighCapacity/(LMRaleighCapacity + durhamCapacity)) - (evap*1069.0 + durhamSpillage)*(LMRaleighCapacity/(LMRaleighCapacity + durhamCapacity));
		// inflows, evaporation, and spillage split relatively between durham and raleigh storages if LM is allocated for raleigh
	
	TeerDemands = durhamDemand1;
	DurhamDemands = durhamDemand3;	
	//Boundary conditions
	
	durhamExcess = 0.0;
	
	if (durhamStorage > durhamCapacity)
	{
		durhamSpillage += durhamStorage - durhamCapacity;
		durhamExcess += durhamStorage - durhamCapacity;
		durhamStorage = durhamCapacity;
		//water greater than combined reservoir capacity is added to the releases for environmental flows
	}

	if (durhamStorage < 0)
	{
		durhamStorage = 0;
	}
		//non negativity constraint
		
	if (raleighMichieStorage > LMRaleighCapacity)
	{
		durhamSpillage += raleighMichieStorage - LMRaleighCapacity;
		durhamExcess += raleighMichieStorage - LMRaleighCapacity;
		raleighMichieStorage = LMRaleighCapacity;
	}	
	
	if (raleighMichieStorage < 0)
	{
		raleighMichieStorage = 0.0;
	}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return durhamDemand2;
}
double ReservoirStorage::updateRaleighStorage(int week, int formul)
{
	
	if((fallsLakeSupplyStorage+fallsLakeQualityStorage)<(fallsLakeSupplyCapacity+fallsLakeQualityCapacity))
	{
		intakeDemand = raleighIntakeCapacity*numdays;

	}
	else
	{
		intakeDemand = 0.0;
	}
	
	double raleighDemand1 = 0.0;
	double raleighDemand2 = 0.0;
	double raleighDemand3 = 0.0;
	double raleighDemand4 = 0.0;
	double raleighDemand5 = 0.0;
	double raleighDemand6 = 0.0;
		// raleighDemand6 added for LakeMichieExp
		
	if(fallsLakeSupplyStorage == 0.0)
	{
		fallsLakeSupplyStorage = 0.001;
	}
	if(lakeWBStorage == 0.0)
	{
		lakeWBStorage = 0.001;
	}
	if(littleRiverRaleighStorage == 0.0)
	{
		littleRiverRaleighStorage = 0.001;
	}
	// if(raleighJordanStorage == 0.0)
	// {
		// raleighJordanStorage = 0.001;
	// }
	if(raleighQuarryStorage == 0.0)
	{
		raleighQuarryStorage = 0.001;
	}
	if(raleighMichieStorage == 0.0)
	{
		raleighMichieStorage = 0.001;
	}
		// ensure not dividing by zero 
	
	double firmDemand = intakeDemand;
	
	double totalStorageRaleigh = fallsLakeSupplyStorage + lakeWBStorage + littleRiverRaleighStorage + raleighJordanStorage + raleighQuarryStorage;
	if (formul > 1)
	{
		totalStorageRaleigh = fallsLakeSupplyStorage + lakeWBStorage + littleRiverRaleighStorage + raleighJordanStorage + raleighQuarryStorage + raleighMichieStorage;
	}
	
	//////Raleigh demands from different sources are dependent on the storage available in that source.  However, Jordan Lake and the quarry
	//////have constraints on how much can be taken out, and the WTP at Falls Lake has a minimum constraint.  It is assumed the WTP at LRR and Lake Wheeler/Benson
	//////are big enough to meet demands that occur
	//////Note: a few more sources/constraints and this might be better solved with a LP
	
	raleighDemand4 = (raleighUse - firmDemand)*(raleighJordanStorage/totalStorageRaleigh);
		// find ratio of storage to be satisfied by this source
	
	if (raleighDemand4 < 0)
		raleighDemand4 = 0;
	
	double Rlimiter;
	if (raleighWWTPcapacity < WJLWTPfracR/WJLWTPfracTOT*WJLWTPinterconnectCapacity)
		Rlimiter = raleighWWTPcapacity;
	else
		Rlimiter = WJLWTPfracR/WJLWTPfracTOT*WJLWTPinterconnectCapacity;
	
	if (Rlimiter > WJLWTPfracR/WJLWTPfracRDonly*DurhamOWASACapacityToD)
		Rlimiter = WJLWTPfracR/WJLWTPfracRDonly*DurhamOWASACapacityToD;
	
	if (Rlimiter > RaleighDurhamCapacity)
		Rlimiter = RaleighDurhamCapacity;
	
	if (WJLWTPfracR <= 0.0)
		Rlimiter = 0.0;
		
	if(raleighDemand4 > Rlimiter*numdays)
		// if the available treatment capacity isn't enough...
	{
		if (raleighDemand4 < 0)
			raleighDemand4 = 0;
		
		raleighDemand4 = Rlimiter*numdays;
		firmDemand += raleighDemand4;
		totalStorageRaleigh -= raleighJordanStorage;
			// adjust everything accordingly

		raleighDemand5 = (raleighUse - firmDemand)*(raleighQuarryStorage/totalStorageRaleigh);
		if(raleighDemand5 > raleighQuarryOutflowCapacity*numdays)
		{
			raleighDemand5 = raleighQuarryOutflowCapacity*numdays;
			firmDemand += raleighDemand5;
			totalStorageRaleigh -= raleighQuarryStorage;
		}
			// shift remaining burden to the next source 
	}
	else
		// if the first source can handle all its demand, check the other first 
	{
		raleighDemand5 = (raleighUse - firmDemand)*(raleighQuarryStorage/totalStorageRaleigh);
		if(raleighDemand5 > raleighQuarryOutflowCapacity*numdays)
		{
			raleighDemand5 = raleighQuarryOutflowCapacity*numdays;
			firmDemand += raleighDemand5;
			totalStorageRaleigh -= raleighQuarryStorage;

			raleighDemand4 = (raleighUse - firmDemand)*(raleighJordanStorage/totalStorageRaleigh);
			if (raleighDemand4 < 0)
				raleighDemand4 = 0;
		
			if(raleighDemand4 > (Rlimiter*numdays - .01))
			{
				raleighDemand4 = Rlimiter*numdays;
				firmDemand += raleighDemand4;
				totalStorageRaleigh -= raleighJordanStorage;
			}
		}
	}

	raleighDemand1 = (raleighUse - firmDemand)*(fallsLakeSupplyStorage/totalStorageRaleigh);
	if(raleighDemand1< 36.0*numdays)
	{
		raleighDemand1 = 36.0*numdays;
		firmDemand = intakeDemand + raleighDemand1;
		
		if (formul > 1)
		{
			totalStorageRaleigh = lakeWBStorage + littleRiverRaleighStorage + raleighJordanStorage + raleighQuarryStorage + raleighMichieStorage;
		}
		else
		{
			totalStorageRaleigh = lakeWBStorage + littleRiverRaleighStorage + raleighJordanStorage + raleighQuarryStorage;
		}
		
		raleighDemand4 = (raleighUse - firmDemand)*(raleighJordanStorage/totalStorageRaleigh);
		
		if (raleighDemand4 < 0)
			raleighDemand4 = 0;
		
		if(raleighDemand4 > Rlimiter*numdays)
		{
			raleighDemand4 = Rlimiter*numdays;
			firmDemand += raleighDemand4;
			totalStorageRaleigh -= raleighJordanStorage;

			raleighDemand5 = (raleighUse - firmDemand)*(raleighQuarryStorage/totalStorageRaleigh);
			if(raleighDemand5 > raleighQuarryOutflowCapacity*numdays)
			{
				raleighDemand5 = raleighQuarryOutflowCapacity*numdays;
				firmDemand += raleighDemand5;
				totalStorageRaleigh -= raleighQuarryStorage;
			}
		}
		else
		{
			raleighDemand5 = (raleighUse - firmDemand)*(raleighQuarryStorage/totalStorageRaleigh);
			if(raleighDemand5 > raleighQuarryOutflowCapacity*numdays)
			{
				raleighDemand5 = raleighQuarryOutflowCapacity*numdays;
				firmDemand += raleighDemand5;
				totalStorageRaleigh -= raleighQuarryStorage;

				raleighDemand4 = (raleighUse - firmDemand)*(raleighJordanStorage/totalStorageRaleigh);
				
				if (raleighDemand4 < 0)
					raleighDemand4 = 0;
		
				if(raleighDemand4 > (Rlimiter*numdays - .01))
				{
					raleighDemand4 = Rlimiter*numdays;
					firmDemand += raleighDemand4;
					totalStorageRaleigh -= raleighJordanStorage;
				}
			}
		}
	}

	raleighDemand2 = (raleighUse - firmDemand)*(lakeWBStorage/totalStorageRaleigh);
	raleighDemand3 = (raleighUse - firmDemand)*(littleRiverRaleighStorage/totalStorageRaleigh);
	
	if (formul > 1)
	{
		raleighDemand6 = (raleighUse - firmDemand)*(raleighMichieStorage/totalStorageRaleigh);
	}
	else
	{
		raleighDemand6 = 0.0;
	}
		// proportional demands allocated for raleigh's LM storage
		// if it exists, later it will be accounted for 

	if(week<13||week>43)
	{
		fallsSpillage = numdays*60.0/1.547;///minimum spillage from reservoir
		if(fallsSpillage + raleighReturn + claytonInflow + crabtreeInflow - intakeDemand <130.0*numdays)
		{
			fallsSpillage = 130.0*numdays + intakeDemand - raleighReturn - claytonInflow - crabtreeInflow;////minimum flow target at downstream gauge
		}
	}
	else
	{
		fallsSpillage = 100.0*numdays/1.547;///minimum spillage from reservoir
		if(fallsSpillage + raleighReturn + claytonInflow + crabtreeInflow - intakeDemand <180.0*numdays)
		{
			fallsSpillage = 180.0*numdays + intakeDemand - raleighReturn - claytonInflow - crabtreeInflow;
		}
	}

	claytonGage = fallsSpillage + raleighReturn + claytonInflow + crabtreeInflow - intakeDemand;
	if(week<13||week>43)
	{
		if((claytonGage - 130.0*numdays) > numdays*raleighQuarryIntakeCapacity)
		{
			raleighQuarryDiversion = numdays*teerQuarryIntakeCapacity;
		}
		else if((claytonGage - 130.0*numdays) > 0 )
		{
			raleighQuarryDiversion = claytonGage - 130.0*numdays;
		}
		else
		{
			raleighQuarryDiversion = 0.0;
		}
	}
	else
	{
		if((claytonGage - 180.0*numdays) > numdays*raleighQuarryIntakeCapacity)
		{
			raleighQuarryDiversion = numdays*teerQuarryIntakeCapacity;
		}
		else if((claytonGage - 180.0*numdays) > 0 )
		{
			raleighQuarryDiversion = claytonGage - 180.0*numdays;
		}
		else
		{
			raleighQuarryDiversion = 0.0;
		}
	}


	if((raleighQuarryDiversion + raleighQuarryStorage - raleighDemand5)>raleighQuarryCapacity)
	{
		raleighQuarryDiversion = raleighQuarryCapacity - raleighQuarryStorage + raleighDemand5;
	}

	///////////Falls Lake Water Balance////////////////////
	//area calculation for evaporation
	
	if ((fallsLakeSupplyStorage + fallsLakeQualityStorage + 5734) < 29000)
	{
		fallsArea = .32*(fallsLakeSupplyStorage+fallsLakeQualityStorage+5734);
	}
	else
	{
		fallsArea = .28*(fallsLakeSupplyStorage+fallsLakeQualityStorage+5734);
	}
	////Inflows are divided between the water storage supply and water quality supply proportionatly (14.7BG supply storage, 20 BG quality storage)
	////Durham reservoir releases and wastewater returns are added to Falls Lake inflows
	fallsSupplyInflow = (fallsInflow + durhamSpillage - fallsArea*evapF + durhamReturn - teerDiversion)*(fallsLakeSupplyCapacity/(fallsLakeQualityCapacity + fallsLakeSupplyCapacity));
	fallsQualityInflow = (fallsInflow + durhamSpillage - fallsArea*evapF + durhamReturn - teerDiversion)*(fallsLakeQualityCapacity/(fallsLakeQualityCapacity + fallsLakeSupplyCapacity));

	////Environmental Releases come from the water quality storage portion of Falls Lake
	fallsLakeQualityStorage += fallsQualityInflow - fallsSpillage;

	//boundry conditions on the water quality storage in Falls Lake
	if (fallsLakeQualityStorage > fallsLakeQualityCapacity)
	{
		////If excess water quality storage exists, it is allocated to the water supply storage portion of Falls Lake

		fallsSupplyInflow += fallsLakeQualityStorage - fallsLakeQualityCapacity;
		fallsQualityInflow += fallsLakeQualityCapacity - fallsLakeQualityStorage;
		fallsLakeQualityStorage = fallsLakeQualityCapacity;
	}
	if (fallsLakeQualityStorage<0)
	{
		fallsLakeQualityStorage = 0;
	}

	////Municipal water demand comes from the water suply storage portion of Falls Lake
	fallsLakeSupplyStorage += fallsSupplyInflow - raleighDemand1;
	FallsLakeDemands = raleighDemand1;
	/////boundry conditions on water supply storage in Falls Lake (don't need to keep track of excess spillage from Falls Lake)
	if (fallsLakeSupplyStorage>fallsLakeSupplyCapacity)
	{
		fallsSpillage+=fallsLakeSupplyStorage - fallsLakeSupplyCapacity; // added by dgold
		fallsLakeSupplyStorage = fallsLakeSupplyCapacity;
	}
	if (fallsLakeSupplyStorage<0)
	{
		fallsLakeSupplyStorage = 0;
	}


	/////Water balance on other three raleigh storage sources AND LM
	lakeWBStorage += wbInflow - raleighDemand2 - lakeWBSpillage - (lakeWBStorage*0.3675*evapWB);
	littleRiverRaleighStorage +=littleRiverRaleighInflow - raleighDemand3 - raleighLittleRiverSpillage + evapF*(320.0+826.0*(littleRiverRaleighStorage/(.001+littleRiverRaleighCapacity)));
	raleighQuarryStorage += raleighQuarryDiversion - evap*(20 + 30*(raleighQuarryStorage/(raleighQuarryCapacity+.001))) - raleighDemand5;
	
	WBDemands = raleighDemand2;
	LRRDemands = raleighDemand3;
	RichlandDemands = raleighDemand5;

	if (formul > 1)
	{
		raleighMichieStorage -= raleighDemand6;
			// all environmental factors and releases are dealt with when this storage is 
			// determined in the durham storage recalculation function 
	}
	
	raleighExcess = 0.0;
	if(lakeWBStorage>lakeWBCapacity)
	{
		lakeWBSpillage+=lakeWBStorage - lakeWBCapacity;
		raleighExcess += lakeWBStorage - lakeWBCapacity;
		lakeWBStorage = lakeWBCapacity;
	}
	if(littleRiverRaleighStorage>littleRiverRaleighCapacity)
	{
		raleighLittleRiverSpillage +=littleRiverRaleighStorage - littleRiverRaleighCapacity;
		if(littleRiverRaleighCapacity > 100.0)
		{
			raleighExcess += littleRiverRaleighStorage - littleRiverRaleighCapacity;
		}
		littleRiverRaleighStorage = littleRiverRaleighCapacity;
	}
	if (fallsLakeSupplyStorage>fallsLakeSupplyCapacity)
	{
		fallsSpillage+=fallsLakeSupplyStorage - fallsLakeSupplyCapacity;
		raleighExcess += fallsLakeSupplyStorage - fallsLakeSupplyCapacity;
		fallsLakeSupplyStorage = fallsLakeSupplyCapacity;
	}
	if(raleighQuarryStorage>raleighQuarryCapacity)
	{
		raleighQuarryStorage = raleighQuarryCapacity;
	}
	if(lakeWBStorage<0)
	{
		lakeWBStorage = 0;
	}
	if (fallsLakeSupplyStorage<0)
	{
		fallsLakeSupplyStorage = 0;
	}
	if(raleighQuarryStorage<0)
	{
		raleighQuarryStorage = 0;
	}
	if(littleRiverRaleighStorage<0)
	{
		littleRiverRaleighStorage = 0;
	}
	if(raleighMichieStorage < 0)
	{
		raleighMichieStorage = 0.0;
	}
	return raleighDemand4;
}
void ReservoirStorage::updateJordanLakeStorage(double owasaJordanDemand, double durhamJordanDemand, double raleighJordanDemand)
{
///////Jordan Lake//////////////////////////////////////////////////////////////////////////////////

	//////apportioning inflows to each Jordan Lake pool (water quality and individual utility supply storage)
	/////surface area of Jordan Lake is 13940 acres
	jordanLakeQualityStorage += (jordanInflow+durhamReturn2+owasaReturn+OWASASpillage-evap*13940)*jordanQualityFraction-jordanLakeMinRelease;
	raleighJordanStorage += (jordanInflow+durhamReturn2+owasaReturn+OWASASpillage-evap*13940)*jordanRaleighFraction - raleighRequest - raleighJordanDemand;
	durhamJordanStorage += (jordanInflow+durhamReturn2+owasaReturn+OWASASpillage-evap*13940)*jordanDurhamFraction - durhamRequest - durhamJordanDemand;
	owasaJordanStorage += (jordanInflow+durhamReturn2+owasaReturn+OWASASpillage-evap*13940)*jordanOWASAFraction - owasaRequest - owasaJordanDemand;
	caryJordanStorage += (jordanInflow+durhamReturn2+owasaReturn+OWASASpillage-evap*13940)*jordanCaryFraction - CaryUse;
	/////if water quality storage exceeds quality pool capacity, excess water is apportioned to the individual supply storage pools
	JLAllDemands = CaryUse;
	jordanSpillage = 0;
	jordanQualityExcess = 0;
	if (jordanLakeQualityStorage>jordanLakeQualityCapacity)
	{

		raleighJordanStorage+=raleighAllocation*(jordanLakeQualityStorage-jordanLakeQualityCapacity);
		durhamJordanStorage+=durhamAllocation*(jordanLakeQualityStorage-jordanLakeQualityCapacity);
		owasaJordanStorage+=owasaAllocation*(jordanLakeQualityStorage-jordanLakeQualityCapacity);
		caryJordanStorage+=caryAllocation*(jordanLakeQualityStorage-jordanLakeQualityCapacity);
		
		jordanQualityExcess += (jordanLakeQualityStorage -jordanLakeQualityCapacity);
		jordanQualityExcess -= raleighAllocation*(jordanLakeQualityStorage-jordanLakeQualityCapacity);
		jordanQualityExcess -= durhamAllocation*(jordanLakeQualityStorage-jordanLakeQualityCapacity);
		jordanQualityExcess -= owasaAllocation*(jordanLakeQualityStorage-jordanLakeQualityCapacity);
		jordanQualityExcess -= caryAllocation*(jordanLakeQualityStorage-jordanLakeQualityCapacity);
		jordanSpillage += jordanQualityExcess;

		jordanLakeQualityStorage=jordanLakeQualityCapacity;
	}
	if (jordanLakeQualityStorage<0)
	{
		jordanLakeQualityStorage = 0;
	}

	////if an individual utility's supply storage is below zero, they cannot withdraw water for transfers
	if(raleighJordanStorage<0)
	{
		JLAllDemands += raleighRequest + raleighJordanDemand +raleighJordanStorage; // demand actually taken from JL
		fallsLakeSupplyStorage+=raleighJordanStorage;
		fallsJordanWithdraw = raleighJordanStorage;
		raleighRequest+=raleighJordanStorage;

		if (fallsLakeSupplyStorage<0)
		{
			fallsLakeSupplyStorage = 0;
		}
		if (raleighRequest<0)
		{
			raleighRequest = 0;
		}
		raleighJordanStorage = 0;
	}
	else
	{
		JLAllDemands += raleighRequest+raleighJordanDemand;
	}
	////no need to calculate Jordan lake spillage  (D Gold adding calculation 8/2017)
	if(raleighJordanStorage>raleighJordanCapacity)
	{
		jordanSpillage += raleighJordanStorage-raleighJordanCapacity;
		raleighJordanStorage = raleighJordanCapacity;
	}
	if(durhamJordanStorage<0)
	{
		JLAllDemands += durhamRequest + durhamJordanDemand + durhamJordanStorage; // demand actually taken from JL
		durhamStorage+=durhamJordanStorage;
		durhamRequest+=durhamJordanStorage;
		
		if (durhamRequest<0)
		{
			durhamRequest = 0;
		}
		if (durhamStorage<0)
		{
			durhamStorage = 0;
		}
		durhamJordanStorage = 0;
	}
	else
	{
		JLAllDemands += durhamRequest + durhamJordanDemand;
	}

	if(durhamJordanStorage>durhamJordanCapacity)
	{
		jordanSpillage += durhamJordanStorage - durhamJordanCapacity;
		durhamJordanStorage = durhamJordanCapacity;
	}
	if(owasaJordanStorage<0)
	{
		JLAllDemands += owasaRequest + owasaJordanDemand + owasaJordanStorage; // demand actually taken from JL
		CCRStorage+=owasaJordanStorage;
		OWASAStorage+=owasaJordanStorage;
		owasaRequest+=owasaJordanStorage;
		if (owasaRequest<0)
		{
			owasaRequest = 0;
		}
		if (CCRStorage<0)
		{
			CCRStorage = 0;
		}
		if (OWASAStorage <0)
		{
			OWASAStorage = 0;
		}
		owasaJordanStorage = 0;
	}
	else
	{
		JLAllDemands += owasaRequest + owasaJordanDemand;
	}

	if(owasaJordanStorage>owasaJordanCapacity)
	{
		jordanSpillage += owasaJordanStorage - owasaJordanCapacity;
		owasaJordanStorage = owasaJordanCapacity;
	}
	if(caryJordanStorage<0)
	{
		caryJordanStorage = 0;
	}
	if(caryJordanStorage>caryJordanCapacity)
	{
		caryExcess = caryJordanStorage - caryJordanCapacity;
		jordanSpillage += caryExcess;
		caryJordanStorage = caryJordanCapacity;
	}

	jordanTotalStorageVol = caryJordanStorage+owasaJordanStorage+raleighJordanStorage+durhamJordanStorage+jordanLakeQualityStorage;
	jordanSpillage += jordanLakeMinRelease; 

}
void ReservoirStorage::updateStorage(int week, int formul)
{
	double oJD = 0.0;
	double dJD = 0.0;
	double rJD = 0.0;
	setSpillover(week);
	oJD = updateOWASAStorage();
	dJD = updateDurhamStorage();
	rJD = updateRaleighStorage(week, formul);
	
	RJLWestWTPdem = rJD;
	DJLWestWTPdem = dJD;
	OJLWestWTPdem = oJD;
		// set the transfer demands from the jointly constructed plant 
		
	updateJordanLakeStorage(oJD, dJD, rJD);

	return;
}

double ReservoirStorage::getDurhamStorage()
{
	return durhamStorage/durhamCapacity;
}
double ReservoirStorage::getDurhamCapacity()
{
	return durhamCapacity;
}
double ReservoirStorage::getOWASAStorage()
{
	return OWASAStorage/OWASACapacity;
}
double ReservoirStorage::getRaleighStorage()
{
	return (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity);
}
double ReservoirStorage::getRaleighStorageVol()
{
	return (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage);
}
double ReservoirStorage::getRaleighLMStorage()
{
	return raleighMichieStorage;
}
double ReservoirStorage::getRaleighLMCapacity()
{
	return LMRaleighCapacity;
}
double ReservoirStorage::getFallsSupplyStorage()
{
	return (fallsLakeSupplyStorage)/(fallsLakeSupplyCapacity);
}
double ReservoirStorage::getFallsSpillage()
{
	return fallsSpillage/fallsLakeSupplyCapacity;
}
double ReservoirStorage::getCaryStorage()
{
	return (caryJordanStorage)/(caryJordanCapacity);
}
double ReservoirStorage::getCaryStorageVol()
{
	return (caryJordanStorage);
}
double ReservoirStorage::getDurhamTransfers()
{
	return (durhamDirect + durhamIndirect + DJLWestWTPdem);
}
double ReservoirStorage::getOWASATransfers()
{
	return (owasaDirect + OJLWestWTPdem);
}
double ReservoirStorage::getRaleighTransfers()
{
	return (raleighDirect + raleighIndirect + RJLWestWTPdem);
}
double ReservoirStorage::getOWASASpillage()
{
	return OWASASpillage;
}
double ReservoirStorage::getDurhamSpillage()
{
	return durhamSpillage;
}
double ReservoirStorage::getDurhamReclaimedCap()
{
	return durhamReclaimedCapacity;
}
double ReservoirStorage::getDurhamTreatment()
{
	return durhamWWTPcapacity;
}
double ReservoirStorage::getOWASATreatment()
{
	return owasaWWTPcapacity;
}
double ReservoirStorage::getRaleighTreatment()
{
	return raleighWWTPcapacity;
}
double ReservoirStorage::getRaleighIntake()
{
	return raleighIntakeCapacity;
}
double ReservoirStorage::getFallsQuality()
{
	return fallsLakeQualityStorage/fallsLakeQualityCapacity;
}
double ReservoirStorage::getExcessO()
{
	return owasaExcess/OWASACapacity;
}
double ReservoirStorage::getExcessD()
{
	return durhamExcess/(durhamCapacity+LMRaleighCapacity);
		// raleigh's stake in Lake Michie also contributes to Durham outflows
}
double ReservoirStorage::getExcessR()
{
	return raleighExcess/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity);
		// raleigh's stake in LM contributes to durham excess, not raleigh excess 
}
double ReservoirStorage::getExcessC()
{
	return caryExcess/caryJordanCapacity;
}
double ReservoirStorage::getDurhamStorageVol()
{
	return durhamStorage;
}
double ReservoirStorage::getTeerStorageVol()
{
	return teerQuarryStorage;
}
double ReservoirStorage::getCCRStorageVol()
{
	return CCRStorage;
}
double ReservoirStorage::getULStorageVol()
{
	return ULStorage;
}
double ReservoirStorage::getSTQStorageVol()
{
	return StQStorage;
}
double ReservoirStorage::getOWASAStorageVol()
{
	return OWASAStorage;
}
double ReservoirStorage::getLakeWBStorageVol()
{
	return lakeWBStorage;
}
double ReservoirStorage::getFallsSupplyStorageVol()
{
	return fallsLakeSupplyStorage;
}
double ReservoirStorage::getFallsQualityStorageVol()
{
	return fallsLakeQualityStorage;
}
double ReservoirStorage::getFallsSupplyAllocFrac()
{
	return fallsLakeSupplyCapacity/(fallsLakeQualityCapacity + fallsLakeSupplyCapacity);
}
double ReservoirStorage::getJordanSupplyStorageVol()
{
	return jordanLakeSupplyStorage;
}
double ReservoirStorage::getJordanQualityStorageVol()
{
	return jordanLakeQualityStorage;
}
double ReservoirStorage::getCaryJordanStorageVol()
{
	return caryJordanStorage;
}
double ReservoirStorage::getRaleighJordanStorageVol()
{
	return raleighJordanStorage;
}
double ReservoirStorage::getDurhamJordanStorageVol()
{
	return durhamJordanStorage;
}
double ReservoirStorage::getOWASAJordanStorageVol()
{
	return owasaJordanStorage;
}
double ReservoirStorage::getLittleRiverRalStorageVol()
{
	return littleRiverRaleighStorage;
}
double ReservoirStorage::getRaleighQuarryStorageVol()
{
	return raleighQuarryStorage;
}
double ReservoirStorage::getRaleighCapacity()
{
	return (fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity);
}
void ReservoirStorage::upgradeCaryTreatmentPlant(int counter)
{
	if(counter == 0)
	{
		CaryTreatmentCapacity = 64;
	}
	else if(counter == 1)
	{
		CaryTreatmentCapacity = 72;
	}
	else
	{
		CaryTreatmentCapacity = 80;
	}
}
void ReservoirStorage::upgradeCaryTreatmentPlant2()
{
	CaryTreatmentCapacity = 64;
}
void ReservoirStorage::upgradeDurhamCaryConnection()
{
	DurhamCaryCapacity = 23;
}
void ReservoirStorage::upgradeDurhamOWASAConnection()
{
	DurhamOWASACapacityToD = WJLWTPinterconnectCapacity;
		// the larger pipe (that also connects to the WJLWTP)
		// is expanded, assume to the point that it can ferry
		// all of the new WTP capacity 
}
void ReservoirStorage::upgradeDurhamOWASAConnectionTWO()
{
	OWASADurhamInterconnectTWO += 3.3;
		// in 2018 a 3 MGD connection from Durham to OWASA,
		// as well as an upgrade of the existing small interconnection
		// that will add 0.3 MGD capacity are added 
}

/// TREATED TRANSFER FUNCTION /////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ReservoirStorage::calcTransfers(double transferDurham, double durhamRisk, double transferOWASA, double owasaRisk, double transferRaleigh, double raleighRisk, double OWASAD, 
									 ofstream &streamFile, bool printOutput, int formul)
{
	double durhamRequestO;
	double owasaRequestO;
	double raleighRequestO;
		//Determines how much water each utility needs to request to reach an acceptable risk-of-failure

	// BINARY INDICATORS OF WHETHER OR NOT A GIVEN UTILITY WILL REQUEST A TRANSFER
	if (owasaRisk > transferOWASA)
		owasaRequestO = 1.0;
	else
		owasaRequestO = 0.0;

	if (durhamRisk > transferDurham)
		durhamRequestO = 1.0;
	else
		durhamRequestO = 0.0;

	if (raleighRisk > transferRaleigh)
		raleighRequestO = 1.0;
	else
		raleighRequestO = 0.0;


	////Cary wants at least a 5 MGD buffer of unused capacity in its treatment plant
	caryTreatmentBuffer = 5.0*numdays;

	////Peaking factor at the Cary WTP used to move from daily capacity to average weekly capacity
	caryPeakingFactor = 0.85;

	//Total spare capacity to sell transfers at the Cary WTP
	caryExtraCapacity = (CaryTreatmentCapacity*numdays - caryTreatmentBuffer)*caryPeakingFactor - CaryUse;

	double DOCap = OWASADurhamInterconnectTWO * numdays;
		// all transfers from Jordan Lake coming into OWASA from Cary WTP 
		// must come through the smaller interconnection due to the 
		// joint WTP

	//Weekly capacity of the Durham-Cary interconnection
	double DCCap = DurhamCaryCapacity*numdays;

	//Weekly capacity of the Raleigh-Cary interconnection
	double RCCap = RaleighCaryCapacity*numdays;
	double raleighExtra = 0.0;
	double raleighExtraToDurham = 0.0;
	double raleighExtraToOWASA = 0.0;
	double durhamExtra = 0.0;
	
	raleighDirect = 0.0;
	raleighIndirect = 0.0;
	durhamDirect = 0.0;
	durhamIndirect = 0.0;
	extraCap = 0.0;
	owasaDirect = 0.0;
	
	double RDCap = RaleighDurhamCapacity * numdays;
		// weekly capacity of the Raleigh-Durham interconnection 
	
	if (RJLWestWTPdem > 0)
	{
		RDCap -= RJLWestWTPdem;
	}
		// no water can move from Raleigh to Durham 
		// if Raleigh is pulling from its Jordan Lake allocation 
		// but indirect water can wheel on C-D-R route
		// if there is interconnect capacity remaining
		
	if (RDCap < 0.0)
	{
		RDCap = 0.0;
	}
	if (DCCap < 0.0)
	{
		DCCap = 0.0;
	}
		// non-negativity constraints 

	/////OWASA must maintain at least 3 MGD production in its own treatment plant
	/////Transfers to OWASA cannot reduce OWASA domestic production below  3 MGD
	double OWASAminimumUse = 3.0*numdays;
	if (OWASAD - DOCap < OWASAminimumUse)
	{
		DOCap = OWASAD-OWASAminimumUse;
	}
	if (DOCap < 0.0)
	{
		DOCap = 0.0;
	}

	//No transfers if Cary does not have the extra treatment capacity
	if (caryExtraCapacity < 0.0)
	{
		caryExtraCapacity = 0.0;
		durhamRequest = 0.0;
		owasaRequest = 0.0;
		raleighRequest = 0.0;
	}
	else ///////////////check infrastructure constraints
	{
		////////Check Cary WTP (through line 480)
		/////Figure out transfers as if the Cary WTP is the limiting factor
		durhamRequest  = (caryExtraCapacity*durhamRisk*durhamRequestO)/(raleighRisk*raleighRequestO+owasaRisk*owasaRequestO+durhamRisk*durhamRequestO+.00001);
		owasaRequest   = (caryExtraCapacity*owasaRisk*owasaRequestO)/(raleighRisk*raleighRequestO+owasaRisk*owasaRequestO+durhamRisk*durhamRequestO+.00001);
		raleighRequest = (caryExtraCapacity*raleighRisk*raleighRequestO)/(raleighRisk*raleighRequestO+owasaRisk*owasaRequestO+durhamRisk*durhamRequestO+.00001);
			// determine the request size of each utility based on how much each requests relative to each other, and the extra cary water is divided up 
			// limited by the WTP at Cary, not by interconnection capacities 
			
		////Make sure each utility gets at least 25% of cary capacity
		double transferFloor = .25;
		if(durhamRequest < transferFloor*caryExtraCapacity*durhamRequestO)
		{
			durhamRequest = transferFloor*caryExtraCapacity*durhamRequestO;

			owasaRequest = ((caryExtraCapacity-durhamRequest)*owasaRisk*owasaRequestO)/(raleighRisk*raleighRequestO+owasaRisk*owasaRequestO+.00001);

			raleighRequest = ((caryExtraCapacity-durhamRequest)*raleighRisk*raleighRequestO)/(raleighRisk*raleighRequestO+owasaRisk*owasaRequestO+.00001);

			if(owasaRequest < transferFloor*caryExtraCapacity*owasaRequestO)
			{
				owasaRequest = transferFloor*caryExtraCapacity*owasaRequestO;

				raleighRequest = (caryExtraCapacity-durhamRequest-owasaRequest)*raleighRequestO;
			}
			if(raleighRequest < transferFloor*caryExtraCapacity*raleighRequestO)
			{
				raleighRequest = transferFloor*caryExtraCapacity*raleighRequestO;

				owasaRequest = (caryExtraCapacity - durhamRequest - raleighRequest)*owasaRequestO;
			}
		}
		if(owasaRequest < transferFloor*caryExtraCapacity*owasaRequestO)
		{
			owasaRequest = transferFloor*caryExtraCapacity*owasaRequestO;

			durhamRequest = ((caryExtraCapacity-owasaRequest)*durhamRisk*durhamRequestO)/(raleighRisk*raleighRequestO+durhamRisk*durhamRequestO+.00001);

			raleighRequest = ((caryExtraCapacity-owasaRequest)*raleighRisk*raleighRequestO)/(raleighRisk*raleighRequestO+durhamRisk*durhamRequestO+.00001);

			if(durhamRequest < transferFloor*caryExtraCapacity*durhamRequestO)
			{
				durhamRequest = transferFloor*caryExtraCapacity*durhamRequestO;

				raleighRequest = (caryExtraCapacity-durhamRequest-owasaRequest)*raleighRequestO;
			}
			if(raleighRequest < transferFloor*caryExtraCapacity*raleighRequestO)
			{
				raleighRequest = transferFloor*caryExtraCapacity*raleighRequestO;

				durhamRequest = (caryExtraCapacity - owasaRequest - raleighRequest)*durhamRequestO;
			}
		}
		if(raleighRequest < transferFloor*caryExtraCapacity*raleighRequestO)
		{
			raleighRequest = transferFloor*caryExtraCapacity*raleighRequestO;

			durhamRequest = ((caryExtraCapacity-raleighRequest)*durhamRisk*durhamRequestO)/(owasaRisk*owasaRequestO+durhamRisk*durhamRequestO+.00001);

			owasaRequest = ((caryExtraCapacity-raleighRequest)*owasaRisk*owasaRequestO)/(owasaRisk*owasaRequestO+durhamRisk*durhamRequestO+.00001);

			if(durhamRequest < transferFloor*caryExtraCapacity*durhamRequestO)
			{
				durhamRequest = transferFloor*caryExtraCapacity*durhamRequestO;

				owasaRequest = (caryExtraCapacity-durhamRequest-raleighRequest)*owasaRequestO;
			}
			if(owasaRequest < transferFloor*caryExtraCapacity*owasaRequestO)
			{
				owasaRequest = transferFloor*caryExtraCapacity*owasaRequestO;

				durhamRequest = (caryExtraCapacity - owasaRequest - raleighRequest)*durhamRequestO;
			}
		}

		///////////Check for conveyence constraints
		
		// June/July 2016: David adds ability for Raleigh and Durham to wheel water to eachother through shared interconnections
		// 	to do so, this is the new order of operations:
		//		0.	OWASA's request is checked to see if it is too large for the Durham-OWASA interconnection 
		//		1.	Check if total combined request from all 3 utilities is less/greater than the sum of C-D and C-R connections
		//				a.  it is also important to know if whatever water bound for OWASA will fit through the R-D interconnect,
		//				    but this interconnection is currently larger than the D-O interconnect, so it isn't an issue
		//				b.	if this statement is true, can allocate all requests without needing to do any risk weighting
		//				c.	if not, then proceed with a risk weighting and allocation 
		//		2.	All of OWASA's request goes through C-D connect
		//		3.	Raleigh's request is calculated, extra routed through C-D-R 
		//		4.	Durham calculated last. 
		
		// Key for terms:
		//	owasaDirect     = water flowing from JL through Cary-Durham-OWASA (C-D-O) interconnection path. Only way to get water to OWASA.
		//	durhamDirect    = water flowing through C-D interconnection to Durham  
		//	raleighDirect   = water flowing through C-R interconnection to Raleigh
		//  durhamIndirect  = water taking C-R-D pathway to Durham 
		//  raleighIndirect = water taking C-D-R pathway to Raleigh 
		
					
		if (owasaRequest > DOCap)
		{
			owasaRequest = DOCap;
			extraCap     = owasaRequest - DOCap;
			
			durhamRequest  += (extraCap)*(durhamRisk*durhamRequestO)/(raleighRisk*raleighRequestO + durhamRisk*durhamRequestO + 0.00001);
			raleighRequest += (extraCap)*(raleighRisk*raleighRequestO)/(raleighRisk*raleighRequestO + durhamRisk*durhamRequestO + 0.00001);
				// take the remaining capacity once OWASA is constrained and divide among D and R 
		}
			// STEP 0.  If OWASA's 25% of Cary extra capacity is too large for D-O interconnect, the extra capacity
			// is re-allocated to raleigh and durham 
			
		if ((raleighRequest + durhamRequest + owasaRequest) < (RCCap + DCCap))
		{
			// ASSUME ALL OF OWASAs REQUEST GOES THROUGH C-D-O PATH, NOT C-R-D-O PATH
			// THIS WILL PREVENT THE NEED OF WATER TO MOVE BOTH WAYS IN A WEEK THROUGH R-D PATH
			//		this also assumes the D-O is always smaller than the C-D interconnect 
			
			// now, if the C-R and C-D connects have enough capacity for a week's total requests,
			// once water is sent to OWASA, remaining transfers will only need to move one way 
			// through the R-D interconnect 
			
			if (raleighRequest > RCCap)
			{
				raleighDirect   = RCCap;
				raleighIndirect = raleighRequest - raleighDirect;
			}	
			else
			{
				raleighDirect = raleighRequest;
			}
				// set Raleigh's needs next.  if Raleigh needs its entire C-R connection, extra is routed through C-D-R path 
				// if not, all goes direct and none is indirect.
			
			if (durhamRequest > (DCCap - owasaRequest))
			{
				durhamDirect   = DCCap - owasaRequest;
				durhamIndirect = durhamRequest - durhamDirect;
			}
			else
			{
				durhamDirect = durhamRequest;
			}
				// Durham is calculated last.  if the remaining capacity in the C-D interconnect after OWASA request has been added 
				// is not enough, extra water is routed through the C-R-D interconnect path  
		}
		else
		{
			// in this scenario, more water is requested than there is conveyance to move 
			// so, total capacity that can be moved (C-D + C-R interconnect capacities) is divided
			// among the 3 utilities based on their risk levels.  previously, requests were limited
			// by Cary WTP extra capacity, now need to be redone based on interconnect capacities.
			
			durhamRequest  = (RCCap + DCCap)*(durhamRisk*durhamRequestO)/(raleighRisk*raleighRequestO + owasaRisk*owasaRequestO + durhamRisk*durhamRequestO + 0.00001);
			owasaRequest   = (RCCap + DCCap)*(owasaRisk*owasaRequestO)/(raleighRisk*raleighRequestO + owasaRisk*owasaRequestO + durhamRisk*durhamRequestO + 0.00001);
			raleighRequest = (RCCap + DCCap)*(raleighRisk*raleighRequestO)/(raleighRisk*raleighRequestO + owasaRisk*owasaRequestO + durhamRisk*durhamRequestO + 0.00001);
				// this will only happen if requests are large enough to total more than the interconnection capacities and/or 
				// the extra Cary capacity is larger than the interconnection capacities
			
			if (owasaRequest > DOCap)
			{
				owasaRequest = DOCap;
				extraCap     = owasaRequest - DOCap;
				
				durhamRequest  += (extraCap)*(durhamRisk*durhamRequestO)/(raleighRisk*raleighRequestO + durhamRisk*durhamRequestO + 0.00001);
				raleighRequest += (extraCap)*(raleighRisk*raleighRequestO)/(raleighRisk*raleighRequestO + durhamRisk*durhamRequestO + 0.00001);
					// take the remaining capacity once OWASA is constrained and divide among D and R 
			}
				// STEP 0
				
			if (raleighRequest > RCCap)
			{
				raleighDirect   = RCCap;
				raleighIndirect = raleighRequest - raleighDirect;
			}	
			else
			{
				raleighDirect = raleighRequest;
			}
				// set Raleigh's needs next.  if Raleigh needs its entire C-R connection, extra is routed through C-D-R path 
				// if not, all goes direct and none is indirect.
			
			if (durhamRequest > (DCCap - owasaRequest))
			{
				durhamDirect   = DCCap - owasaRequest;
				durhamIndirect = durhamRequest - durhamDirect;
			}
			else
			{
				durhamDirect = durhamRequest;
			}
		}
		
		if (raleighIndirect > RDCap)
		{
			raleighIndirect = RDCap;
		}
		if (durhamIndirect > RDCap)
		{
			durhamIndirect = RDCap;
		}
		if (RJLWestWTPdem > 0)
		{
			durhamIndirect = 0;
		}
			// make sure the D-R interconnect isn't over-used 
			// and that if Raleigh is pulling water from WJLWTP
			// that Durham cannot wheel through Raleigh 
		
	}
	
	owasaDirect = owasaRequest;

	durhamUse  -= (durhamDirect + durhamIndirect);
	OWASAUse   -= owasaDirect;
	raleighUse -= (raleighDirect + raleighIndirect);
	
	durhamRequest  = (durhamDirect + durhamIndirect);
	raleighRequest = (raleighDirect + raleighIndirect);
	
	if (printOutput && (formul < 1))
		// for each week, output all this info to a csv only for the formulation where releases aren't used 
	{
		streamFile << ((fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity)) << ",";
		streamFile << (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage) << "," << (fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity) << ",";
		streamFile << (durhamStorage/durhamCapacity) << "," << durhamSpillage << ",";
		
		streamFile << fallsLakeSupplyStorage << "," << durhamStorage << ",";
		
		streamFile << fallsLakeQualityStorage << "," << fallsLakeQualityCapacity << "," << fallsLakeSupplyCapacity << "," << durhamCapacity << ",";
		//streamFile << RstorageTarget << "," << DstorageTarget << endl;
	}

	return;
}

/// CALCULATING RELASES FUNCTION //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ReservoirStorage::calcRawReleases(double DreleaseWeekCap, double DreleaseMin, double RcriticalStorageLevel, double DcriticalStorageLevel,  
									   double RstorageTarget, double DstorageTarget, double FallsSupplyFraction, double DbuybackLevel,
									   int realization, ofstream &streamFile, int year, int week, int numRealizationsTOOUTPUT, int RANK, bool printOutput,
									   bool ACTIVE)
	// function accepts release max or min constraints, plus critical storage levels,
	// returns the volume of water requested for release, as well as the volume	
	// of water Durham wants to "buy back" to avoid releasing it.
{
	RreleaseRequest  = 0.0;
	DbuybackQuantity = 0.0;
		// initially zero
	DreleaseMax = durhamStorage / 5.0;
		// releases can only be up to 20% of Durham's current supply storage
		// or capped at another arbitrary limit of 2000 MGW (DreleaseWeekCap)
	
	if (ACTIVE)
	{
		if (DreleaseMax > DreleaseWeekCap)
		{
			DreleaseMax = DreleaseWeekCap;
				// if this is set to 0, no releases will occur 
		}
			
		if (week < 21 || week > 47)
			// Spillage rules for Durham Reservoir at Little River
		{
			DminEnvSpill = 3.877 * numdays;
				// determine minimum environmental release from Durham to Falls Lake
		}
		else
		{
			DminEnvSpill = 9.05;
		}

		if (RstorageTarget > (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity))
			// if raleigh is requesting releases or not
			// AFTER MARCH 15, 2016: now based on ROF triggers
			// the storage target is a rating curve determined using the RR ROF trigger in the createROF function
		{
			RreleaseRequest = ((RstorageTarget * (fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity)) - (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)) / FallsSupplyFraction;
				// initial request is an attempt to draw falls lake water supply pool back to the level at which
				// it is no longer at risk of failure for raleigh as a whole, a storage fraction represented by RstorageTarget
				// RreleaseRequest is the difference between current and ideal storage levels
				// May 2016: this quantity is divided by the fraction of each release that will be allocated for water supply
				// (because Falls Lake has water supply and water quality pools within its conservation pool,
				//  releases will not just go toward augmenting the water supply pool)

			if ((fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity) >= RcriticalStorageLevel)	
			{
				RreleaseRequest = 0.0;
					// if raleigh already has storage at a given level, no releases requested
			}
				
			if (RreleaseRequest > DreleaseMax)
			{
				RreleaseRequest = DreleaseMax;
					// this represents the size of the release in MG in the given WEEK
			} 
			
			if (RreleaseRequest < DreleaseMin)
			{
				RreleaseRequest = 0.0;
					// this and the directly above statement cover release constraints
			}
			
			if (fallsLakeSupplyStorage + (RreleaseRequest * FallsSupplyFraction) > RcriticalStorageLevel*fallsLakeSupplyCapacity)
			{
				RreleaseRequest = (RcriticalStorageLevel * fallsLakeSupplyCapacity - fallsLakeSupplyStorage) / FallsSupplyFraction;
					// if Falls Lake is too full to accept all requested water up to critical level
			}
			
			
			if (durhamStorage/durhamCapacity <= DcriticalStorageLevel)
				// if durham's storage is too low, they can deny releases
			{
				RreleaseRequest = 0.0;
			}
			
			if (durhamSpillage > DminEnvSpill)
			{
				RreleaseRequest -= (durhamSpillage - DminEnvSpill);
					// if Durham is spilling water beyond minimum requirements, no need to "release"
			}
			
			if (RreleaseRequest < 0.0)
			{
				RreleaseRequest = 0.0;
					// ensure non-negative value (possibly introduced when accounting for spillage)
			}
			
			if ((durhamStorage - RreleaseRequest)/durhamCapacity < DcriticalStorageLevel)
				// if the resultant storage level is below the critical threshold
			{
				RreleaseRequest -= (DcriticalStorageLevel*durhamCapacity - (durhamStorage - RreleaseRequest));
					// limit the request by the deficit relative to durham's critical threshold
				if (RreleaseRequest < 0.0)
				{
					RreleaseRequest = 0.0;
						// ensure non-negative value (possibly introduced when accounting for spillage)
				}
			}
			
			if ((durhamStorage - RreleaseRequest)/durhamCapacity < DstorageTarget)
				// if the resultant storage level is below the critical ROF floor 
			{
				RreleaseRequest -= (DstorageTarget*durhamCapacity - (durhamStorage - RreleaseRequest));
					// limit the request by the deficit relative to durham's critical threshold
				if (RreleaseRequest < 0.0)
				{
					RreleaseRequest = 0.0;
						// ensure non-negative value (possibly introduced when accounting for spillage)
				}
			}
			
			if (DbuybackLevel > durhamStorage/durhamCapacity)
				// storage target is based on Durham RR trigger - buyback buffer zone 
				// and subsequently calculated rating curve for Durham
			{
				DbuybackStorageLevel = DbuybackLevel;
					// if Durham has ROF levels above the trigger for transfers,
					// it will act to buy back releases at a proportional level
					// to its risk.
			}
			else 
			{
				DbuybackStorageLevel = DcriticalStorageLevel;
					// if not at risk, will not buy back releases
			}
			
			
			if ((durhamStorage - RreleaseRequest)/durhamCapacity < DbuybackStorageLevel)
				// this statement asks if the resultant available storage for durham is lower than desired
			{
				if ((DbuybackStorageLevel*durhamCapacity - (durhamStorage - RreleaseRequest)) > RreleaseRequest)
					// the case where Raleigh requests release, it drops Durham's reservoir too low,
					// but the requested release volume, if bought back, will still not bring Durham's
					// supply level back to the buyback threshold.  in other words, if the deficit created
					// by the possible release, between the level durham wishes to maintain via buybacks
					// and the expected water level, is greater than the volume of the requested releases,
					// using this deficit as a correction factor and assuming it is all bought back
					// will result in a negative release request volume.
				{
					DbuybackQuantity = RreleaseRequest;
					RreleaseRequest  = 0.0;
				}
				else 
				{
					DbuybackQuantity = (DbuybackStorageLevel*durhamCapacity - (durhamStorage - RreleaseRequest));
					RreleaseRequest -= DbuybackQuantity;
						// adjust request to reflect what Durham is willing to give
				}
					
			}
			
		}
		else 
		{
			RreleaseRequest      = 0.0;
			DbuybackQuantity     = 0.0;
			DbuybackStorageLevel = 0.0;
		}
	
		durhamStorage          -= RreleaseRequest;
		fallsLakeSupplyStorage += RreleaseRequest * FallsSupplyFraction;
		
		if (fallsLakeQualityStorage + (RreleaseRequest * (1-FallsSupplyFraction)) > fallsLakeQualityCapacity)
			// because the only objective of releases is to fill the water supply pool, release requests are made
			// without consideration of whether or not the water quality pool will overrun.  if the water supply request 
			// is met, and the water allocated to the WQ pool overruns the WQ pool capacity, the request will be curtailed
			// but this reduction WILL NOT affect the amount of water that goes to water supply (calculated already)
		{
			RreleaseRequest -= ((fallsLakeQualityStorage + (RreleaseRequest * (1-FallsSupplyFraction))) - fallsLakeQualityCapacity);
				// if Falls Lake quality pool is too full to accept all requested water up to critical level
			fallsLakeQualityStorage = fallsLakeQualityCapacity;
			requestadjusted = 1;
		}
		else
		{
			fallsLakeQualityStorage += RreleaseRequest * (1 - FallsSupplyFraction);
			requestadjusted = 0;
		}
			// ADJUST THE EXISTING STORAGE LEVELS
			// DOING SO HERE CHANGES R AND D DECISIONS
			// TO USE JL IN THE CURRENT WEEK
			// May 2016: assume that releases are proportioned into 
			// falls lake water quality and water supply pools
			// at a division equal to their relative allocations 
			// within falls lake conservation pool
	}
	else
	{
		RreleaseRequest      = 0.0;
		DbuybackQuantity     = 0.0;
		DbuybackStorageLevel = 0.0;
	}
	
	if ((realization < numRealizationsTOOUTPUT) && printOutput)
		// for each week, output all this info to a csv
	{
		streamFile << RANK << "," << realization << "," << year << "," << week << ",";
		streamFile << ((fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity)) << ",";
		streamFile << (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage) << "," << (fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity) << ",";
		streamFile << (durhamStorage/durhamCapacity) << "," << durhamSpillage << ",";
		streamFile << RreleaseRequest << "," << DbuybackQuantity << "," << DbuybackStorageLevel << "," << fallsLakeSupplyStorage << "," << durhamStorage << ",";
		streamFile << RstorageTarget << "," << DstorageTarget << "," << DbuybackLevel << ",";
		streamFile << DreleaseMax << "," << requestadjusted << ","<< DminEnvSpill << ",";
		streamFile << fallsLakeQualityStorage << "," << fallsLakeQualityCapacity << "," << fallsLakeSupplyCapacity << "," << durhamCapacity << "," << FallsSupplyFraction << endl;
	}
	
	return;
}

void ReservoirStorage::calcSpotReleases(int realization, ofstream &streamFile, bool ACTIVE, int numreal, bool writefiles, 
										double ReleaseCap, int rank, int year, int week, double Rtrigger, double Dtrigger,
										double FLSPsize, double Rcutoff, double Dcutoff)
{
	// This function will assume that Durham is under no obligation to release water (unlike in an option contract)
	// and all releases will be paid for on the spot by Raleigh, based on some price per unit of water agreed upon
	// in contract negotiations between the cities (which will factor in risk aversion). Releases will be determined
	// based on a few bounding contstraints (how much water is available to move, what ROF each city feels in a week). 
	
	int Curtails = 0;
	
	// Because Durham is not obligated to release water, there is no punishment for denying release requests by Raleigh.
	// However, the number of times requests are denied will be stored and used in contract negotiations in the future.
	
	if (ACTIVE)
		// if the release contract is allowed
	{
		RreleaseRequest  = 0.0;
		DbuybackQuantity = 0.0;
		DbuybackStorageLevel = 0.0;
			// initially zero
			// FOR THIS FUNCTION, BUYBACKS WILL ALWAYS BE 0 BECAUSE 
			// THIS IS NOT AN OPTION CONTRACT 
			// (must be set here because it is declared in simulation.cpp so it can't be unset)
			
		DreleaseMax = durhamStorage / 5.0;
			// releases can only be up to 20% of Durham's current supply storage
			// or capped at another arbitrary limit of 2000 MGW (LMreleaseCap)
		
		if (DreleaseMax > ReleaseCap)
		{
			DreleaseMax = ReleaseCap;
				// if this is set to 0, no releases will occur 
		}
			
		if (week < 21 || week > 47)
			// Spillage rules for Durham Reservoir at Little River
		{
			DminEnvSpill = 3.877 * numdays;
				// determine minimum environmental release from Durham to Falls Lake
		}
		else
		{
			DminEnvSpill = 9.05;
		}
		
		if (Rtrigger > (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity))
			// Raleigh requests a release if their current storage is below their target levels, determined by ROF 
		{
			ReqCount += 1;
			
			///////// ROF Constraints //////////
			
			if (Dtrigger >= durhamStorage/durhamCapacity)
			{
				RreleaseRequest = 0.0;
				Curtails = 1;
			}
			else if (Rtrigger <= (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity))
			{
				RreleaseRequest = 0.0;
			}
			else 
				// here, Durham's storage is above their ROF trigger level, Raleigh's is below theirs, and Durham's ROF < Raleigh's ROF 
				// now, must locate the storages for each city which have the same ROF (equal marginal benefit for either city)
			{
				RreleaseRequest = ((Rtrigger * (fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity)) - (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)) / FLSPsize;
					// 1: Determine what Raleigh requires to meet ROF target storage 
				
				if (RreleaseRequest > (durhamStorage - Dtrigger*durhamCapacity))
				{
					RreleaseRequest = (durhamStorage - Dtrigger*durhamCapacity);
					Curtails = 1;
				}
					// 2: Check if the request overwhelms Durham, and adjust if so
			}
			
			//////// General Bounding Constraints ////////
			
			if (durhamSpillage > DminEnvSpill)
			{
				RreleaseRequest -= (durhamSpillage - DminEnvSpill);
					// if Durham is spilling water beyond minimum requirements, no need to "release"
			}
			
			if (RreleaseRequest > DreleaseMax)
			{
				RreleaseRequest = DreleaseMax;
					// this represents the size of the release in MG in the given WEEK
			} 
			
			if (fallsLakeSupplyStorage + (RreleaseRequest * FLSPsize) > Rcutoff*fallsLakeSupplyCapacity)
			{
				RreleaseRequest = (Rcutoff * fallsLakeSupplyCapacity - fallsLakeSupplyStorage) / FLSPsize;
					// if Falls Lake is too full to accept all requested water up to critical level
			}
			
			if ((fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity) >= Rcutoff)	
			{
				RreleaseRequest = 0.0;
					// if raleigh already has storage at a given level, no releases requested
			}
			
			if (durhamStorage/durhamCapacity <= Dcutoff)
				// if durham's storage is too low, they can deny releases
			{
				RreleaseRequest = 0.0;
				Curtails = 1;
			}
			
			if (RreleaseRequest < 0.0)
			{
				RreleaseRequest = 0.0;
					// ensure non-negative value (possibly introduced when accounting for spillage)
			}
			
		}
		
		durhamStorage           -= RreleaseRequest;
		fallsLakeSupplyStorage  += RreleaseRequest * FLSPsize;
		
		if (fallsLakeQualityStorage + (RreleaseRequest * (1-FLSPsize)) > fallsLakeQualityCapacity)
			// because the only objective of releases is to fill the water supply pool, release requests are made
			// without consideration of whether or not the water quality pool will overrun.  if the water supply request 
			// is met, and the water allocated to the WQ pool overruns the WQ pool capacity, the request will be curtailed
			// but this reduction WILL NOT affect the amount of water that goes to water supply (calculated already)
		{
			RreleaseRequest -= ((fallsLakeQualityStorage + (RreleaseRequest * (1-FLSPsize))) - fallsLakeQualityCapacity);
				// if Falls Lake quality pool is too full to accept all requested water up to critical level
			fallsLakeQualityStorage = fallsLakeQualityCapacity;
			requestadjusted = 1;
		}
		else
		{
			fallsLakeQualityStorage += RreleaseRequest * (1 - FLSPsize);
			requestadjusted = 0;
		}
	}
	else 
	{
		RreleaseRequest = 0.0;
		DbuybackQuantity = 0.0;
		DbuybackStorageLevel = 0.0;
	}
	
	if (Curtails > 0)
	{
		ReqCurtail += 1;
			// if any state causes a request curtail, record it as such 
	}
	
	if ((realization < numreal) && writefiles)
		// for each week, output all this info to a csv
	{
		streamFile << rank << "," << realization << "," << year << "," << week << ",";
		streamFile << ((fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage)/(fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity)) << ",";
		streamFile << (fallsLakeSupplyStorage+lakeWBStorage+littleRiverRaleighStorage+raleighMichieStorage) << "," << (fallsLakeSupplyCapacity+lakeWBCapacity+littleRiverRaleighCapacity+LMRaleighCapacity) << ",";
		streamFile << (durhamStorage/durhamCapacity) << "," << durhamSpillage << ",";
		streamFile << RreleaseRequest << "," << fallsLakeSupplyStorage << "," << durhamStorage << ",";
		streamFile << Rtrigger << "," << Dtrigger << ",";
		streamFile << DreleaseMax << "," << requestadjusted << ","<< DminEnvSpill << ",";
		streamFile << fallsLakeQualityStorage << "," << fallsLakeQualityCapacity << "," << fallsLakeSupplyCapacity << "," << durhamCapacity << "," << FLSPsize << endl;
	}	
	
	return;
}

double ReservoirStorage::getRaleighReleases()
{
	return RreleaseRequest;
}

double ReservoirStorage::getDurhamBuybackRequest()
{
	return DbuybackQuantity;
}



