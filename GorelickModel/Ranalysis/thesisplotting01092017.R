## output plotting for thesis figures
## Jan 2017
## ------------------------------------------------------------------------------------------------------------
## ------------- set paths and load ---------------------------------------------------------------------------

rm(list = ls())
setwd("C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\Coding\\GorelickModelOutput")
  # two subfolders within this, depending on the type of release contract

figurepath = paste("Figures_", Sys.Date(), "\\", sep = "")
dir.create(figurepath)

numranks = 4
numforms = 3
RDMnum   = 0

source("C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\Coding\\multiplot.R")
library(ggplot2)
library(reshape2)

# --------------------------------------------- figure 0 ------------------------------------------------------
# regional map ------------------------------------------------------------------------------------------------
# (build in arcmap and adjust with paint) ---------------------------------------------------------------------

# --------------------------------------------- figure 1 ------------------------------------------------------
# explanation figure to show how releases are triggered (one plot for option, one for spot) -------------------

RANK = 0
  # choose a single simulation
REALIZATION = 0
  # choose a single realization/timeseries

datafolder  = "spot\\"
  # or "option\\"
FORMULATION = 0
ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)

  # Goal: 3-4 year timeseries window of storage levels for Durham and Raleigh (two timeseries plots) with and without releases
  #       and should maybe include bars indicating the size and weekly instances of releases (think: hyetograph)

Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                              "Rstor", "Dstor", 
                                                              "RaleighTargetStorageFraction", "DurhamTargetStorageFraction",
                                                              "TTR", "TTD", "RRR")]
FORMULATION = 1
ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
Sub1 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                             "Rstor", "Dstor", 
                                                             "RaleighTargetStorageFraction", "DurhamTargetStorageFraction",
                                                             "TTR", "TTD", "RRR")]

Sub0 = Sub0[which(Sub0$Year < 25),]
Sub0 = Sub0[which(Sub0$Year > 20),]
Sub1 = Sub1[which(Sub1$Year < 25),]
Sub1 = Sub1[which(Sub1$Year > 20),]

Sub0 = Sub0[with(Sub0, order(Year, Week)),]
Sub1 = Sub1[with(Sub1, order(Year, Week)),]

Sub0$TimeseriesWeek = c(1:nrow(Sub0))
Sub1$TimeseriesWeek = c(1:nrow(Sub1))

maxrel = max(Sub1$RRR)

Sub1$RRR = Sub1$RRR/max(Sub1$RRR)*0.30
Sub0$TTD = Sub0$TTD/max(Sub0$TTD)*0.30
Sub0$TTR = Sub0$TTR/max(Sub0$TTR)*0.30
  # scale for plotting

plotR = ggplot() +
  geom_line(data = Sub0, aes(x = TimeseriesWeek, y = Rstor, colour = "Storage (Without Releases)")) +
  geom_line(data = Sub1, aes(x = TimeseriesWeek, y = Rstor, colour = "Storage (With Releases)")) +
  geom_line(data = Sub0, aes(x = TimeseriesWeek, y = RaleighTargetStorageFraction, colour = "ROF Guide Curve")) +
  geom_bar(data  = Sub1, aes(x = TimeseriesWeek, y = RRR, fill = RRR*maxrel/0.3), stat = "identity") +
  scale_colour_discrete(guide = FALSE) +
  scale_fill_continuous(name = "Releases\n(MGW)") +
  theme(legend.position = c(0.70, 0.21), 
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  geom_text(data = data.frame(), aes(7, 0.75), label = "Raleigh", size = 7) +
  xlab("") + ylab("Storage Fraction") + ylim(c(0,1)) +
  guides(fill = guide_colorbar(barwidth = 8, barheight = 0.6, 
                               breaks = c(0, 1000, 2000, 3000), limits = c(0,3000), 
                               label.position = "bottom", direction = "horizontal"))

plotD = ggplot() +
  geom_line(data = Sub0, aes(x = TimeseriesWeek, y = Dstor, colour = "Storage (Without Releases)")) +
  geom_line(data = Sub1, aes(x = TimeseriesWeek, y = Dstor, colour = "Storage (With Releases)")) +
  geom_line(data = Sub0, aes(x = TimeseriesWeek, y = DurhamTargetStorageFraction, colour = "ROF Guide Curve")) +
  theme(legend.position = c(0.5,0.21), 
        legend.title = element_blank(), 
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  xlab("Time (Weeks)") + ylab("Storage Fraction") +
  guides(colour = guide_legend(ncol = 3)) + ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(7, 0.75), label = "Durham", size = 7)

png(paste(figurepath, "Figure1", ".png", sep = ""), units = "in", width = 12, height = 7, res = 300)
multiplot(plotR, plotD, cols = 1)
dev.off()

# --------------------------------------------- figure 2 ------------------------------------------------------
# single simulation distribution of releases/transfer/restriction use -----------------------------------------
# (looks like histograms on top of each other, show the shift between formulations) ---------------------------

RANK = 0
# choose a single simulation

for (REALIZATION in 0:99)
  # compare a timeseries with and without releases
{
  datafolder  = "spot\\"
  # or "option\\"
  FORMULATION = 0
  
  #  TTdata = read.csv(paste(datafolder, "JLTreatedTransfers", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  #  RRdata = read.csv(paste(datafolder, "RRfuncOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  #  LMdata = read.csv(paste(datafolder, "LMrealloc", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  #  RSKpar = read.csv(paste(datafolder, "weeklyRiskParams", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  # these csvs are of weekly data for every realization
  #  checkdata = read.csv(paste(datafolder, "storagecheck", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  #  RSTdata = read.csv(paste(datafolder, "RestrictionData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  #  INFdata = read.csv(paste(datafolder, "InfraBuilt", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  #  SimOut  = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  # annual or other outputs
  
  # Goal: 3-4 year timeseries window of storage levels for Durham and Raleigh (two timeseries plots) with and without releases
  #       and should maybe include bars indicating the size and weekly instances of releases (think: hyetograph)
}

# --------------------------------------------- figure 3 ------------------------------------------------------
# pareto fronts (3D plot with reliability, transfers, releases, colors change with formulation) ---------------
# (show additional reduction to solutions that meet key constraints?) -----------------------------------------



# --------------------------------------------- figure 4 ------------------------------------------------------
# call out individual extreme pareto solutions and compare timeseries to show parameter differences -----------
# (transfers and releases under different ROF triggers, release/transfer caps, release allocation rule, etc) --
# (also show impact of different infrastructure build dates on timing of transfers and releases)








































  
## -------------- plot the data, find statistics ------------------------------------------
## -------------- just plotting annual stats ----------------------------------------------

for (rlz in 0:4)
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
                         oierj           = weeks1$adjustedspotpayment)

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
                                                              "Spot Payment"))

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

## plot weekly stats of releases, etc. ------------------------------------------

weekdata1 = ALLRR[which(ALLRR$Realization == realization),]

weekdata1 = weekdata1[with(weekdata1, order(Year, Week)),]
weekdata1$TimeseriesWeek = c(1:nrow(weekdata1))

weeks1 = weekdata1[(startyear*52+1):((startyear+numyears)*52),]

DataToPlot3 = data.frame(TimeseriesWeek  = weeks1$TimeseriesWeek,
                         RRQ             = weeks1$ReleaseRequest,
                         BBQ             = weeks1$ReleaseMaxLimit,
                         TTR             = (weeks1$RaleighDirectTransferVolume + weeks1$RaleighIndirectTransferVolume),
                         TTD             = (weeks1$DurhamDirectTransferVolume + weeks1$DurhamIndirectTransferVolume),
                         Frac            = weeks1$ReleaseToFLSupplyFraction,
                         VolD            = weeks1$RsupplyCapacity,
                         VolR            = weeks1$DsupplyCapacity)

DataMelted = melt(DataToPlot3, id = c("TimeseriesWeek"))
  # organize just the data I want to plot
  # in a form better suited to ggplot

DataMelted$variable2 = factor(DataMelted$variable, labels = c(paste("Releases", sep = ""),
                                                              paste("Release Cap", sep = ""),
                                                              paste("Transfers (Raleigh)", sep = ""),
                                                              paste("Transfers (Durham)", sep = ""),
                                                              "FLSP Fraction",
                                                              "Raleigh Capacity",
                                                              "Durham Capacity"))

Years = seq(2015, (2015 + numyears), 5)
line = Years
rm(Years)

allgraph = ggplot(DataMelted) + 
  geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
  facet_grid(variable2 ~ ., scales = "free_y") +
  xlab("") + 
  ylab("") + 
  theme(legend.position = "none") + 
  ggtitle(paste(titlestring, sep = "")) +
  scale_x_continuous(breaks = c(seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)], by = 52*5), nrow(weeks1)), 
                     labels = line)

ggsave(filename = paste(figurep, realization, "_", rank1, "_", numyears, "years", "_singlecompSet3_", titlestring, ".png", sep = ""),
       units = "in", width = (numyears/3), height = 15)

}


