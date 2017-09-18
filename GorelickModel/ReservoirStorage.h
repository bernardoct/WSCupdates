#ifndef ReservoirStorage_h
#define ReservoirStorage_h

class ReservoirStorage
{
public:
	//ReservoirStorage();
	double fallsArea;
	
	void initializeReservoirStorage(double durhamCap, double CCRCap, double StQCap, double ULCap, double lakeWBCap, double fallsLakeSupplyCap, double fallsLakeQualityCap, double jordanLakeSupplyCap, double jordanLakeQualityCap, double CaryTreatmentCap, double DurhamCaryCap, double DurhamOWASACap, double RaleighCaryCap, double RaleighDurhamCap, double raleighAllocation, double durhamAllocation, double owasaAllocation, double caryAllocation, double teerQCap, double tQIc, double tQOc, double littleRiverRaleighCap, double westTrtCap, double durhamRecCap, double rQc, double rQIc, double rQOc, double rIc, double cQs, double cQIs, double cQOs, double oWWWTP, double dWWWTP, double rWWWTP, int s, double DurhamReleaseCap, double DurhamReleaseMin, double RaleighLMCap);
	
	void initializeReservoirStorageROF(double durhamCap, double CCRCap, double StQCap, double ULCap, double lakeWBCap, double fallsLakeSupplyCap, double fallsLakeQualityCap, double jordanLakeSupplyCap, double jordanLakeQualityCap, double CaryTreatmentCap, double DurhamCaryCap, double DurhamOWASACap, double RaleighCaryCap, double RaleighDurhamCap, double raleighAllocation, double durhamAllocation, double owasaAllocation, double caryAllocation, double teerQCap, double tQIc, double tQOc, double littleRiverRaleighCap, double westTrtCap, double durhamRecCap, double rQc, double rQIc, double rQOc, double rIc, double cQs, double cQIs, double cQOs, double oWWWTP, double dWWWTP, double rWWWTP, int s, double DurhamReleaseCap, double DurhamReleaseMin, double RaleighLMCap);
	
	void updateReservoirStorageROF(double durhamS, double teerS, double CCRS, double ULS, double STQS, double owasaS, double lakeWBS, double flSS, double flQS, double jlSS, double jlQS, double caryJordanS, double raleighJordanS, double durhamJordanS, double owasaJordanS, double littleRiverRalS, double raleighQS, double RLMstor);
	void updateReservoirStorageROF();
	
	void setDemands(double durham, double owasa, double cary, double raleigh, int numberOfDaysInWeek);
	
	void setSpillover(int week);
	void calcTransfers(double transferDurham, double durhamR, double transferOWASA, double owasaR, double transferRaleigh, double raleighR, double owasaWD, 
					   ofstream &streamFile, bool printOutput, int formul);		
	
	void setInflow(double durham, double ccr, double ul, double stq, double falls, double wb, double clayton, double crabtree, double jordan, double lillington, double ralRet, double durRet, double durRet2, double owasaRet, double evaporationf, double evaporationwb, double evaporation, double lRR);
	
	void calcRawReleases(double DreleaseWeekCap, double DreleaseMin, double RcriticalStorageLevel, double DcriticalStorageLevel, 
						 double RstorageTarget, double DstorageTarget, double FallsSupplyFraction, double DbuybackLevel,
						 int realization, ofstream &streamFile, int year, int week, int numRealizationsTOOUTPUT, int RANK, bool printOutput,
						 bool ACTIVE);
	void calcSpotReleases(int realization, ofstream &streamFile, bool ACTIVE, int numreal, bool writefiles, 
						  double ReleaseCap, int rank, int year, int week, double Rtrigger, double Dtrigger,
						  double FLSPsize, double Rcutoff, double Dcutoff);
	
	double getRaleighReleases();
	double getDurhamBuybackRequest();
	
	void getReturnRatios(double raleighR, double durhamR);
	void updateStorage(int whatWeek, int formul);
	double updateOWASAStorage();
	double updateDurhamStorage();
	double updateRaleighStorage(int whatWeek, int formul);
	void updateJordanLakeStorage(double ow, double dur, double ral);
	double getDurhamStorage();
	double getOWASAStorage();
	double getRaleighStorage();
	double getRaleighStorageVol();
	double getCaryStorage();
	double getCaryStorageVol();
	double getDurhamTransfers();
	double getOWASATransfers();
	double getRaleighTransfers();
	double getDurhamSpillage();
	double getOWASASpillage();
	double getDurhamReclaimedCap();
	double getDurhamTreatment();
	double getOWASATreatment();
	double getRaleighTreatment();
	double getRaleighIntake();
	double getFallsQuality();
	double getRaleighLMStorage();
	double getRaleighLMCapacity();
	double getRaleighCapacity();
	double getDurhamCapacity();
	
	double getFallsSupplyStorage();
	double getFallsSupplyAllocFrac();
	double getFallsSpillage();
	
	double raleighDirect;
	double durhamDirect;
	double raleighIndirect;
	double durhamIndirect;
	double extraCap;
	double owasaDirect;
	double LMRaleighCapacity;
	double raleighMichieStorage;
	
	void buildMichieSharedLow(double ratio);
	void buildMichieSharedHigh(double ratio, double checkval);
	
	void upgradeCaryTreatmentPlant(int counter);
	void upgradeCaryTreatmentPlant2();
	void upgradeDurhamCaryConnection();
	void upgradeDurhamOWASAConnection();
	void upgradeDurhamOWASAConnectionTWO();
	void openResFiles();
	void buildULexp();
	void buildCCexp();
	void buildSQlow();
	void buildSQhigh();
	void buildTeerQuarry();
	void buildReclaimedLow();
	void buildReclaimedHigh();
	void buildMichieLow();
	void buildMichieHigh(double checkval);
	void buildLittleRiverRal();
	void buildRalQuarry();
	void buildNeuseIntake();
	void reallocateFallsLake(double fallsLakeRe);
	void buildWWWTPlow(double oF, double dF, double rF);
	void buildWWWTPhigh(double oF, double dF, double rF);
	double getExcessO();
	double getExcessD();
	double getExcessR();
	double getExcessC();
	double getDurhamStorageVol();
	double getTeerStorageVol();
	double getCCRStorageVol();
	double getULStorageVol();
	double getSTQStorageVol();
	double getOWASAStorageVol();
	double getLakeWBStorageVol();
	double getFallsSupplyStorageVol();
	double getFallsQualityStorageVol();
	double getJordanSupplyStorageVol();
	double getJordanQualityStorageVol();
	double getCaryJordanStorageVol();
	double getRaleighJordanStorageVol();
	double getDurhamJordanStorageVol();
	double getOWASAJordanStorageVol();
	double getLittleRiverRalStorageVol();
	double getRaleighQuarryStorageVol();
	
	int ReqCount;
	int ReqCurtail;
	
	double RJLWestWTPdem;
	double DJLWestWTPdem;
	double OJLWestWTPdem;
		// weekly demands from the western joint wtp on jordan lake 
		// to be counted as transfers 
		
	double WJLWTPinterconnectCapacity;
	double OWASADurhamInterconnectTWO;
		// smallest pipe connection before reaching Durham-OWASA interconnection 
		// and the smaller OWASA interconnection
		
	double WJLWTPfracD;
	double WJLWTPfracO;
	double WJLWTPfracR;
	double WJLWTPfracTOT;
	double WJLWTPfracRDonly;
		// fraction of treatment plant capacity available for each city 

	/////Spillage
	double durhamSpillage;
	double OWASASpillage;
	double CCRSpillage;
	double MinCCROut;
	double ULSpillage;
	double StQSpillage;
	double fallsSpillage;
	double lakeWBSpillage;
	double jordanLakeMinRelease;
	double raleighLittleRiverSpillage;
	double enoSpillage;
	double fallsLakeSupplyExcess;
	double fallsJordanWithdraw;

	///Inflow gains
	double durhamInflow;
	double CCRInflow;
	double ULInflow;
	double StQInflow;
	double fallsInflow;
	double wbInflow;
	double claytonInflow;
	double crabtreeInflow;
	double jordanInflow;
	double lillingtonInflow;
	double littleRiverRaleighInflow;
	double raleighReturn;
	double raleighReturnOut;
	double durhamReturn;
	double durhamReturnOut;
	double durhamReturn2;
	double owasaReturn;
	double evapF;
	double evapWB;
	double evap;
	double fallsSupplyInflow;
	double fallsQualityInflow;
	double jordanLakeQualityInflow;
	double jordanLakeRaleighSupplyInflow;
	double jordanLakeDurhamSupplyInflow;
	double jordanlakeCarySupplyInflow;
	double jordanLakeOWASASupplyInflow;
	double claytonGage;
	double teerDiversion;
	double raleighQuarryDiversion;
	
	//Durham Storage 
	double durhamCapacity;////Little River Reservoir and Lake Michie
	double durhamStorage;
	double durhamReclaimedCapacity;////Reclaimed capacity
	double teerQuarryCapacity;
	double teerQuarryStorage;
	double teerQuarryIntakeCapacity;
	double teerQuarryOutflowCapacity;

	////OWASA Storage infrastructure
	double CCRCapacity;//OWASA Cane Creek reservoir
	double CCRStorage;
	double ULCapacity;//OWASA University Lake reservoir
	double ULStorage;
	double StQCapacity;//OWASA Stone Quarry reservoir
	double StQStorage;
	double OWASACapacity;//OWASA total
	double OWASAStorage;
	////Raleigh Storage infrastructure
	double lakeWBCapacity;//Raleigh Lakes Wheeler and Benson capacity
	double lakeWBStorage;
	double fallsLakeSupplyCapacity;//Capacity of Falls Lake Water Supply allocation
	double fallsLakeQualityCapacity;//Capacity of Falls Lake Water Quality allocation
	double fallsLakeSupplyStorage;
	double fallsLakeQualityStorage;
	double littleRiverRaleighCapacity;//Little River Reservoir capacity
	double littleRiverRaleighStorage;
	double raleighQuarryCapacity;////Ofline quarry at richland creek
	double raleighQuarryStorage;
	double raleighQuarryIntakeCapacity;//maximum quarry filling rate
	double raleighQuarryOutflowCapacity;//maximum withdrawls from quarry
	double raleighIntakeCapacity;//maximum withdrawl from Neuse intake
	double intakeDemand;
	////Cary Storage Infrastructure
	double caryQuarryCapacity;
	double caryQuarryStorage;
	double caryQuarryIntakeCapacity;//maximum quarry filling rate
	double caryQuarryOutflowCapacity;//maximum withdrawls from quarry
	////Interconnections
	double DurhamCaryCapacity;//Capacity of the Durham Cary interconnection
	double DurhamOWASACapacityToD;//Capacity of the Durham OWASA interconnection to durham
	double DurhamOWASACapacityToO;// capacity of the interconnect to owasa 
		// the difference is needed to incorporate the WJLWTP 
	double RaleighCaryCapacity;//Capacity of the Raleigh Cary interconnection
	double RaleighDurhamCapacity;//Capacity of the Raleigh Durham interconnection
	////Jordan Lake treatment Plants
	double CaryTreatmentCapacity;//Cary WTP capacity
	double westernTreatmentCapacity;////Western Wake WTP capacity
	double owasaWWTPcapacity;
	double durhamWWTPcapacity;
	double raleighWWTPcapacity;
	double caryWWTPcapacity;
	
	////Jordan Lake accounting
	double jordanLakeSupplyCapacity;
	double jordanLakeQualityCapacity;
	double jordanLakeSupplyStorage;
	double jordanLakeQualityStorage;
		///Raleigh
	double raleighAllocation;//Raleigh allocation to jordan Lake
	double raleighJordanCapacity;
	double raleighJordanStorage;
		///Cary
	double caryAllocation;//Cary allocation to jordan lake
	double caryJordanCapacity;
	double caryJordanStorage;
		///Durham
	double durhamAllocation;//Durham allocation to jordan lake
	double durhamJordanCapacity;
	double durhamJordanStorage;
		///OWASA
	double owasaAllocation;// owasa allocation to jordan lake
	double owasaJordanCapacity;
	double owasaJordanStorage;
		///Inflow fractions (how to divide the inflows to respective allocations)
	double jordanQualityFraction;
	double jordanRaleighFraction;
	double jordanDurhamFraction;
	double jordanCaryFraction;
	double jordanOWASAFraction;

	double owasaExcess;
	double raleighExcess;
	double durhamExcess;
	double caryExcess;
	
	/////Demands
	double durhamUse;
	double OWASAUse;
	double CaryUse;
	double raleighUse;

	//Reservoir Demands
	double JLAllDemands;
	double JLCaryDemands;
	double JLDurhamDemands;
	double JLRaleighDemands;
	double JLOWASADemands;
	double FallsLakeDemands;
	double WBDemands;
	double LRRDemands;
	double RichlandDemands;
	double CCRDemands;
	double ULDemands;
	double TeerDemands;
	double DurhamDemands;

	double jordanTotalStorageVol;
	double jordanSpillage;
	double jordanQualityExcess;


private:
	
	
	
	double numdays;
	int thingCounter;
	
	//calcTransfers variables
	double percentFull_O;
	double percentFull_D;
	double percentFull_R;
	double durhamRequest;//Durham intial transfer request
	double owasaRequest;//OWASA initial transfer request
	double raleighRequest;//Raleigh initial transfer request	
	double caryTreatmentBuffer;
	double caryPeakingFactor;
	double caryExtraCapacity;
	double transferLimit;
	double totalRequest;
	
	//// Releases
	
	double RreleaseRequest;
	double DbuybackQuantity;
	double DbuybackStorageLevel;
	double DreleaseMax;
	double DminEnvSpill;
	int requestadjusted;
	
	ofstream out2;
};

#endif
