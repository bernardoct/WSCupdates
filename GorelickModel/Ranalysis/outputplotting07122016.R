## output analysis for the new release contracts
## July 2016
## ------------------------------------------------------------------------------------

## ------------- set paths and load ---------------------------------------------------

rm(list = ls())
setwd("C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\Coding")

resultpath = paste("GorelickModelOutput\\ModelResults_", Sys.Date(), "\\", sep = "")
dir.create(resultpath)

figurepath = paste("GorelickModelOutput\\Figures_", Sys.Date(), "\\", sep = "")
dir.create(figurepath)

datafolder = "GorelickModel\\output\\"
numranks   = 1

for (RANK in 0:(numranks-1))
{
  TTdata = read.csv(paste(datafolder, "JLTreatedTransfers", RANK, ".csv", sep = ""), header = T)
  RRdata = read.csv(paste(datafolder, "RRfuncOutput", RANK, ".csv", sep = ""), header = T)
  # these csvs are of weekly data for every realization
  
  ALLRR = merge(TTdata, RRdata,  by = c("Rank", "Realization", "Year", "Week"))
  
  rm(TTdata, RRdata)
  
  RCdata = read.csv(paste(datafolder, "ReleaseContract", RANK, "_", 0, ".csv", sep = ""), header = T)
}
  # no more data writing, just check it out 
  
## -------------- plot the data, find statistics ------------------------------------------
## -------------- just plotting annual stats ----------------------------------------------

for (rlz in 0:10)
{
  
realization = rlz
rank1 = 0
startyear = 0
numyears = 45
titlestring = "Release Contract Variables"
figurep = figurepath
datapath = resultpath

## ----------------------------------------------------------------------------------------

Years = seq(2015, (2015 + numyears), 1)
line = Years
rm(Years)

## --------------------- all the plots compare at least one rank/realization ------------
## --------------------- read in the data here ------------------------------------------

data1     = RCdata
weekdata1 = data1[which(data1$Realization == realization),]
rm(data1)

weekdata1 = weekdata1[with(weekdata1, order(Year, Week)),]
weekdata1$TimeseriesWeek = c(1:nrow(weekdata1))

# ----------------------------------------------------------------------------------------
# ------------------ make 3 year timeseries plots ----------------------------------------

weeks1 = weekdata1[(startyear*1):((startyear+numyears+1)),]

# ------------------ compare releases and transfers --------------------------------------

DataToPlot1 = data.frame(TimeseriesWeek  = weeks1$TimeseriesWeek,
                         RRQ             = weeks1$RinfROF,
                         dfss            = weeks1$RBBstddev,
                         BBQ             = weeks1$DinfROF,
                         ded             = weeks1$DBBstddev,
                         TTRD            = weeks1$RTTmag,
                         df              = weeks1$RTTfreq,
                         TTRI            = weeks1$DTTmag,
                         TTDD            = weeks1$DTTfreq)

DataToPlot = DataToPlot1
DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
DataMelted$variable2 = factor(DataMelted$variable, labels = c("Raleigh ROF",
                                                              "Raleigh ROF Std Dev",
                                                              "Durham ROF",
                                                              "D ROF Std Dev",
                                                              "R TT Magnitude",
                                                              "R TT Frequency",
                                                              "D TT Magn.",
                                                              "D TT Freq."))


allgraph = ggplot(DataMelted) + 
  geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
  facet_grid(variable2 ~ ., scales = "free_y") +
  xlab("") + 
  ylab("") + 
  theme(legend.position = "none") + 
  ggtitle(paste(titlestring, sep = "")) +
  scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]), 
                     labels = line)

ggsave(filename = paste(figurep, realization, "_", rank1, "_", numyears, "years", "_singlecomp_", titlestring, ".png", sep = ""),
         units = "in", width = (numyears/3), height = 15)
  




DataToPlot2 = data.frame(TimeseriesWeek  = weeks1$TimeseriesWeek,
                         RRQ             = weeks1$contractcount,
                         cont            = weeks1$allowReleaseContract,
                         dfss            = weeks1$previousContract,
                         BBQ             = weeks1$contractSplits,
                         ded             = weeks1$RtriggerDiff,
                         TTRD            = weeks1$DtriggerDiff,
                         df              = weeks1$RTTmagnitudeDiff,
                         TTRI            = weeks1$RTTfrequencyDiff,
                         TTDD            = weeks1$DTTmagnitudeDiff,
                         TTDI            = weeks1$DTTfrequencyDiff,
                         oierj           = weeks1$adjustedannualpayment,
                         eoiuh           = weeks1$adjustedbuybackpayment)

DataToPlot = DataToPlot2
DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
DataMelted$variable2 = factor(DataMelted$variable, labels = c("Count",
                                                              "Contract?",
                                                              "Previous Contract?",
                                                              "Prev. Financial Balance",
                                                              "R ROF difference",
                                                              "D ROF diff",
                                                              "R TT Magn. Diff.",
                                                              "R TT Freq. Diff.",
                                                              "D TT Magn. Diff.",
                                                              "D TT Freq. Diff.",
                                                              "Annual Payment",
                                                              "Buyback Rate"))

allgraph = ggplot(DataMelted) + 
  geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
  facet_grid(variable2 ~ ., scales = "free_y") +
  xlab("") + 
  ylab("") + 
  theme(legend.position = "none") + 
  ggtitle(paste(titlestring, sep = "")) +
  scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]), 
                     labels = line)

ggsave(filename = paste(figurep, realization, "_", rank1, "_", numyears, "years", "_singlecompSet2_", titlestring, ".png", sep = ""),
       units = "in", width = (numyears/3), height = 15)

}

