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
# ADD A THIRD STORAGE LEVEL FOR CASE WITH BUYBACKS VERSUS SPOT CASE
# HAVE BARS FOR RELEASES ON RALEIGH PLOT, BARS FOR BUYBACKS ON DURHAM PLOT

RANK = 0
  # choose a single simulation
REALIZATION = 0
  # choose a single realization/timeseries
FORMULATION = 0

datafolder  = "figure1data\\zero\\"
ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)

  # Goal: 3-4 year timeseries window of storage levels for Durham and Raleigh (two timeseries plots) with and without releases
  #       and should maybe include bars indicating the size and weekly instances of releases (think: hyetograph)

Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                              "Rstor", "Dstor", 
                                                              "RaleighTargetStorageFraction", "DurhamTargetStorageFraction",
                                                              "TTR", "TTD", "RRR")]
FORMULATION = 1
datafolder  = "figure1data\\spot\\"
ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
Sub1 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                             "Rstor", "Dstor", 
                                                             "RaleighTargetStorageFraction", "DurhamTargetStorageFraction",
                                                             "TTR", "TTD", "RRR")]

datafolder  = "figure1data\\option\\"
RANK = 35
ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
Sub2 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                             "Rstor", "Dstor", 
                                                             "RaleighTargetStorageFraction", "DurhamTargetStorageFraction",
                                                             "TTR", "TTD", "RRR", "BBD")]
  # this is the option read

Sub0 = Sub0[which(Sub0$Year < 41),]
Sub0 = Sub0[which(Sub0$Year > 36),]
Sub1 = Sub1[which(Sub1$Year < 41),]
Sub1 = Sub1[which(Sub1$Year > 36),]
Sub2 = Sub2[which(Sub2$Year < 41),]
Sub2 = Sub2[which(Sub2$Year > 36),]

Sub0 = Sub0[with(Sub0, order(Year, Week)),]
Sub1 = Sub1[with(Sub1, order(Year, Week)),]
Sub2 = Sub2[with(Sub2, order(Year, Week)),]

Sub0$TimeseriesWeek = c(1:nrow(Sub0))
Sub1$TimeseriesWeek = c(1:nrow(Sub1))
Sub2$TimeseriesWeek = c(1:nrow(Sub2))

maxrel = max(Sub1$RRR)
maxbb  = max(Sub2$BBD)

Sub2$BBD = Sub2$BBD/max(Sub2$BBD)*0.30
Sub1$RRR = Sub1$RRR/max(Sub1$RRR)*0.30
Sub0$TTD = Sub0$TTD/max(Sub0$TTD)*0.30
Sub0$TTR = Sub0$TTR/max(Sub0$TTR)*0.30
  # scale for plotting

# assume there is a Sub2 that has formulation 1 data for buyback scenario

plotR = ggplot() +
  geom_line(data = Sub0, aes(x = TimeseriesWeek, y = Rstor, colour = "Storage (Without Releases)")) +
  geom_line(data = Sub1, aes(x = TimeseriesWeek, y = Rstor, colour = "Storage (Spot Releases)")) +
  geom_line(data = Sub2, aes(x = TimeseriesWeek, y = Rstor, colour = "Storage (Option Releases)")) +
  geom_line(data = Sub0, aes(x = TimeseriesWeek, y = RaleighTargetStorageFraction, colour = "ROF Guide Curve")) +
  geom_bar(data  = Sub1, aes(x = TimeseriesWeek, y = RRR, fill = RRR*maxrel/0.3), stat = "identity") +
  scale_colour_discrete(guide = FALSE) +
  scale_fill_continuous(name = "Releases\n(MGW)") +
  theme(legend.position = c(0.82, 0.23), 
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  geom_text(data = data.frame(), aes(12.5, 0.85), label = "Raleigh", size = 8) +
  xlab("") + ylab("Storage Fraction") + ylim(c(0,1)) +
  guides(fill = guide_colorbar(barwidth = 8, barheight = 0.6, 
                               breaks = c(0, 1000, 2000, 3000), limits = c(0,3000), 
                               label.position = "bottom", direction = "horizontal"))

plotD = ggplot() +
  geom_line(data = Sub0, aes(x = TimeseriesWeek, y = Dstor, colour = "Storage (Without Releases)")) +
  geom_line(data = Sub1, aes(x = TimeseriesWeek, y = Dstor, colour = "Storage (Spot Releases)")) +
#  geom_line(data = Sub2, aes(x = TimeseriesWeek, y = Dstor, colour = "Storage (Option Releases)")) +
  geom_line(data = Sub0, aes(x = TimeseriesWeek, y = DurhamTargetStorageFraction, colour = "ROF Guide Curve")) +
#  geom_bar(data  = Sub2, aes(x = TimeseriesWeek, y = BBD, fill = BBD*maxbb/0.3), stat = "identity") +
  theme(legend.position = c(0.5,0.21), 
        legend.title = element_blank(), 
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  xlab("Time (Weeks)") + ylab("Storage Fraction") +
  guides(colour = guide_legend(ncol = 3)) + ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(12.5, 0.85), label = "Durham", size = 8) +
#  scale_fill_continuous(name = "Buybacks\n(MGW)") +
#  guides(fill = guide_colorbar(barwidth = 8, barheight = 0.6, 
#                               breaks = c(0, 1000, 2000, 3000), limits = c(0,3000), 
#                               label.position = "bottom", direction = "horizontal"))

png(paste(figurepath, "Figure1", ".png", sep = ""), units = "in", width = 12, height = 7, res = 300)
multiplot(plotR, plotD, cols = 1)
dev.off()

# --------------------------------------------- figure 2 ------------------------------------------------------
# single simulation distribution of releases/transfer/restriction use -----------------------------------------
# (looks like histograms on top of each other, show the shift between formulations) ---------------------------

RANK = 0
for (FORMULATION in 0:2)
{
  datafolder  = "spot\\"
    # or "option\\"
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  RSTdata  = read.csv(paste(datafolder, "RestrictionData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization), FUN = sum, na.rm = T)
  Rdata = aggregate(x = RSTdata,  by = list(RSTdata$Realization),  FUN = sum, na.rm = T)
    # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  Tdata = Tdata[,c("Group.1", "TTR", "TTD", "RRR", "BBD")]
  Rdata = Rdata[,c("Group.1", "restR", "restD")]
    # just keep necessary data
  
  colnames(Tdata) = c("Realization", 
                      paste("TTR", FORMULATION, sep = ""),
                      paste("TTD", FORMULATION, sep = ""),
                      paste("RRR", FORMULATION, sep = ""),
                      paste("BBD", FORMULATION, sep = ""))
  colnames(Rdata) = c("Realization", 
                      paste("restR", FORMULATION, sep = ""),
                      paste("restD", FORMULATION, sep = ""))
  plotdata = merge(Tdata, Rdata, by = "Realization")
    # merge and rename
  
  if (FORMULATION == 0)
  {
    alldata = plotdata
    # add to master set
  }
  else
  {
    alldata = merge(alldata, plotdata, by = "Realization")
  }
  
  rm(ALLTdata, RSTdata, Tdata, Rdata, plotdata)

}

# Goal: show the transition of transfer, restriction distributions with introduction of releases
#       and infrastructure.  Set up to print subsets of this plot also for future presentations.

trans = 0.45

TTRdensplot = ggplot(alldata) + 
  geom_density(aes(TTR0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(TTR1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(TTR2, fill = "2", colour = "2"), alpha = trans) +
  scale_fill_discrete(name = "Formulation") +
  guides(colour = F, fill = guide_legend(ncol = 3)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  xlab("Total Realization Transfers (MG)") + ylab("Probability") +
  geom_text(data = data.frame(), aes(50, 0.085), label = "Raleigh", size = 7)

TTDdensplot = ggplot(alldata) + 
  geom_density(aes(TTD0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(TTD1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(TTD2, fill = "2", colour = "2"), alpha = trans) +
#  scale_fill_discrete(name = "Formulation") +
  guides(colour = F, fill = F) + ylim(c(0, 0.0016)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  xlab("Total Realization Transfers (MG)") + ylab("Probability") +
  geom_text(data = data.frame(), aes(900, 0.00153), label = "Durham", size = 7)

RestRdensplot = ggplot(alldata) + 
  geom_density(aes(restR0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(restR1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(restR2, fill = "2", colour = "2"), alpha = trans) +
  #  scale_fill_discrete(name = "Formulation") +
  guides(colour = F, fill = F) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  xlab("Weeks under restriction") + ylab("Probability")
#  geom_text(data = data.frame(), aes(25, 0.05), label = "Raleigh", size = 7)

RestDdensplot = ggplot(alldata) + 
  geom_density(aes(restD0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(restD1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(restD2, fill = "2", colour = "2"), alpha = trans) +
  #  scale_fill_discrete(name = "Formulation") +
  guides(colour = F, fill = F) + ylim(c(0, 0.06)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  xlab("Weeks under restriction") + ylab("Probability")
#  geom_text(data = data.frame(), aes(25, 0.055), label = "Durham", size = 7)

png(paste(figurepath, "Figure2", ".png", sep = ""), units = "in", width = 12, height = 12, res = 400)
multiplot(TTDdensplot, RestDdensplot, TTRdensplot, RestRdensplot, cols = 2)
dev.off()

# --------------------------------------------- figure 3 ------------------------------------------------------
# pareto fronts (3D plot with reliability, transfers, releases, colors change with formulation) ---------------
# (show additional reduction to solutions that meet key constraints?) -----------------------------------------

RDMnum     = 0
datafolder = "figure3data\\"
allobjdata = c()
for (RANK in 0:49)
{
  for (FORMULATION in 1:1)
  {
    ObjData = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
    ObjData = ObjData[,1:105]
    allobjdata = rbind(allobjdata, c(RANK, FORMULATION, ObjData))
  }
}
aodata = as.data.frame(allobjdata)
colnames(aodata) = c("Run", "Formulation", 1:105)

objectivenames = c("Failures", "PeakDebt", "TotalLosses", "Releases", "Restrictions", "Transfers")
aodata = aodata[,c(1:2, 83:107)]
colnames(aodata)[3:27] = c(paste(objectivenames, "D", sep = ""), 
                           paste(objectivenames, "O", sep = ""),
                           paste(objectivenames, "R", sep = ""),
                           paste(objectivenames, "C", sep = ""),
                           "FallsFailures")

rm(allobjdata, ObjData, objectivenames)

# Goal: make 3D pareto fronts that vary in color with formulation
#       may be best done by melting the data

aodata$ReliabilityR = (1 - as.numeric(as.character(aodata$FailuresR))) * 100

library(plotly)

pareto3dplot = plot_ly(aodata, x = ~ReliabilityR, y = ~TransfersR, z = ~RestrictionsR, 
                       color = ~Formulation, colors = c("red", "green", "blue")) %>%
  add_markers() %>% layout(scene = list(xaxis = list(title = 'Reliability (%)'),
                                      yaxis = list(title = 'Peak transfers (MGY)'),
                                      zaxis = list(title = 'Peak restrictions (per year)')))


# --------------------------------------------- figure 4 ------------------------------------------------------
# call out individual extreme pareto solutions and compare timeseries to show parameter differences -----------
# (transfers and releases under different ROF triggers, release/transfer caps, release allocation rule, etc) --
# (also show impact of different infrastructure build dates on timing of transfers and releases)

# First plot: effectiveness of releases under different ACE guidelines
# Take a single realization across several simulation runs for the same formulation (1) and repeat a plot
# like that in figure 1 but with different storage levels for each level of water supply release percentage

RDMnum = 0
FORMULATION = 1
REALIZATION = 0
datafolder = "figure4data\\a\\"
startrank = 5
for (RANK in startrank:(startrank+4))
{
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  ObjData  = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
  
  AllocFrac = ObjData[,63]
  Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                               "Rstor", "Dstor", 
                                                               "RaleighTargetStorageFraction", "DurhamTargetStorageFraction")]
  Sub0 = Sub0[which(Sub0$Year < 23),]
  Sub0 = Sub0[which(Sub0$Year > 20),]
  Sub0 = Sub0[with(Sub0, order(Year, Week)),]
  Sub0$TimeseriesWeek = c(1:nrow(Sub0))
    # collect and subset storage level data
  
  if (RANK == startrank)
  {
    Sub0 = Sub0[,c("TimeseriesWeek","Rstor", "Dstor", 
                   "RaleighTargetStorageFraction", "DurhamTargetStorageFraction")]
  }
  else
  {
    Sub0 = Sub0[,c("TimeseriesWeek","Rstor", "Dstor")]
  }
  
  AllocFrac = rep(AllocFrac, nrow(Sub0))
  
  alld = data.frame(ReleaseAllocationFraction = AllocFrac, Sub0)
  allmelt = melt(alld, id = c("TimeseriesWeek", "ReleaseAllocationFraction"))
  
  if (RANK == startrank)
  {
    Data = allmelt
  }
  else
  {
    Data = rbind(Data, allmelt)
  }
}

Data$variable2 = factor(Data$variable, labels = c("Raleigh Storage", "Durham Storage",
                                                  "Raleigh ROF Guide Curve", "Durham ROF Guide Curve"))

Storages    = Data[which(Data$variable2 != "Raleigh ROF Guide Curve" & Data$variable2 != "Durham ROF Guide Curve"),]
GuideCurves = Data[which(Data$variable2 == "Raleigh ROF Guide Curve" | Data$variable2 == "Durham ROF Guide Curve"),]

RGC = GuideCurves[which(GuideCurves$variable2 == "Raleigh ROF Guide Curve"),]
DGC = GuideCurves[which(GuideCurves$variable2 != "Raleigh ROF Guide Curve"),]

RStorages = Storages[which(Storages$variable == "Rstor"),]
DStorages = Storages[which(Storages$variable == "Dstor"),]

RStorages$colorcol = RStorages$ReleaseAllocationFraction
RGC$colorcol = as.character(RGC$variable2)

Rdata = rbind(RGC, RStorages)

fig4allocfraclevelsR = ggplot() + 
  geom_line(data = Rdata, aes(x = TimeseriesWeek, y = value, colour = colorcol)) + 
  xlab("") + ylab("Storage Fraction") + theme(legend.position = "none") +
  geom_text(data = data.frame(), aes(96, 0.125), label = "Raleigh", size = 7) + ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(1, 0.9), label = "(a)", size = 12)

DStorages$colorcol = DStorages$ReleaseAllocationFraction
DGC$colorcol = as.character(DGC$variable2)

Ddata = rbind(DGC, DStorages)

rm(ALLTdata, alld, Data, allmelt, DGC, RGC, ObjData, DStorages, RStorages, Sub0, 
   AllocFrac, GuideCurves, Storages)

fig4allocfraclevelsD = ggplot() + 
  geom_line(data = Ddata, aes(x = TimeseriesWeek, y = value, colour = colorcol)) + 
  xlab("Time (Weeks)") + ylab("Storage Fraction") +
  scale_colour_discrete(name = "% of releases allocated for water supply",
                        breaks = c("0.423", "0.5", "0.7", "0.9", "1", "Durham ROF Guide Curve"),
                        labels = c("42.3", "50", "70", "90", "100", "2% ROF Level")) +
  theme(legend.position = c(0.23, 0.15), 
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  geom_text(data = data.frame(), aes(96, 0.125), label = "Durham", size = 7) + ylim(c(0,1)) +
  guides(colour = guide_legend(title.position = "top", direction = "horizontal", label.position = "bottom", 
                               ncol = 6,
                               keywidth = 0.52, keyheight = 0.1, default.unit = "inch"))

png(paste(figurepath, "Figure4a", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
multiplot(fig4allocfraclevelsR, fig4allocfraclevelsD, cols = 1)
dev.off()

# REPEAT CODE BUT CHANGE SLIGHTLY TO DO THE SAME FOR DIFFERENT RELEASE SIZES --------------------------------------------

RDMnum = 0
FORMULATION = 1
REALIZATION = 0
datafolder = "figure4data\\b\\"
startrank = 0
for (RANK in startrank:(startrank+4))
{
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  ObjData  = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
  
  RelCap = ObjData[,60]
  Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                               "Rstor", "Dstor", 
                                                               "RaleighTargetStorageFraction", "DurhamTargetStorageFraction")]
  Sub0 = Sub0[which(Sub0$Year < 23),]
  Sub0 = Sub0[which(Sub0$Year > 20),]
  Sub0 = Sub0[with(Sub0, order(Year, Week)),]
  Sub0$TimeseriesWeek = c(1:nrow(Sub0))
  # collect and subset storage level data
  
  if (RANK == startrank)
  {
    Sub0 = Sub0[,c("TimeseriesWeek","Rstor", "Dstor", 
                   "RaleighTargetStorageFraction", "DurhamTargetStorageFraction")]
  }
  else
  {
    Sub0 = Sub0[,c("TimeseriesWeek","Rstor", "Dstor")]
  }
  
  RelCap = rep(RelCap, nrow(Sub0))
  
  alld = data.frame(ReleaseCap = RelCap, Sub0)
  allmelt = melt(alld, id = c("TimeseriesWeek", "ReleaseCap"))
  
  if (RANK == startrank)
  {
    Data = allmelt
  }
  else
  {
    Data = rbind(Data, allmelt)
  }
}

Data$variable2 = factor(Data$variable, labels = c("Raleigh Storage", "Durham Storage",
                                                  "Raleigh ROF Guide Curve", "Durham ROF Guide Curve"))

Storages    = Data[which(Data$variable2 != "Raleigh ROF Guide Curve" & Data$variable2 != "Durham ROF Guide Curve"),]
GuideCurves = Data[which(Data$variable2 == "Raleigh ROF Guide Curve" | Data$variable2 == "Durham ROF Guide Curve"),]

RGC = GuideCurves[which(GuideCurves$variable2 == "Raleigh ROF Guide Curve"),]
DGC = GuideCurves[which(GuideCurves$variable2 != "Raleigh ROF Guide Curve"),]

RStorages = Storages[which(Storages$variable == "Rstor"),]
DStorages = Storages[which(Storages$variable == "Dstor"),]

RStorages$colorcol = RStorages$ReleaseCap
RGC$colorcol = as.character(RGC$variable2)

Rdata = rbind(RGC, RStorages)

fig4relcaplevelsR = ggplot() + 
  geom_line(data = Rdata, aes(x = TimeseriesWeek, y = value, colour = colorcol)) + 
  xlab("") + ylab("Storage Fraction") + theme(legend.position = "none") +
  geom_text(data = data.frame(), aes(96, 0.125), label = "Raleigh", size = 7) + ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(1, 0.9), label = "(b)", size = 12)

DStorages$colorcol = DStorages$ReleaseCap
DGC$colorcol = as.character(DGC$variable2)

Ddata = rbind(DGC, DStorages)

fig4relcaplevelsD = ggplot() + 
  geom_line(data = Ddata, aes(x = TimeseriesWeek, y = value, colour = colorcol)) + 
  xlab("Time (Weeks)") + ylab("Storage Fraction") +
  scale_colour_discrete(name = "Release cap (MGW)",
                        breaks = c("3000", "2000", "1500", "1000", "500", "Durham ROF Guide Curve"),
                        labels = c("3,000", "2,000", "1,500", "1,000", "500", "2% ROF Level")) +
  theme(legend.position = c(0.23, 0.15), 
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  geom_text(data = data.frame(), aes(96, 0.125), label = "Durham", size = 7) + ylim(c(0,1)) +
  guides(colour = guide_legend(title.position = "top", direction = "horizontal", label.position = "bottom", 
                               ncol = 6,
                               keywidth = 0.52, keyheight = 0.1, default.unit = "inch"))

png(paste(figurepath, "Figure4b", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
multiplot(fig4relcaplevelsR, fig4relcaplevelsD, cols = 1)
dev.off()

# show changes due to changing length of LM permitting period on each formulation ----------------------------
# using density plots ----------------------------------------------------------------------------------------
# 3 permitting lengths, 3 formulations, just releases? 

RANK = 0
RDMnum = 0
REALIZATION = 0
for (FORMULATION in 0:2)
{
  datafolder = "figure4data\\c\\period1\\"
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)

  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization), FUN = sum, na.rm = T)
    # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  Tdata = Tdata[,c("Group.1", "TTR", "TTD", "RRR", "BBD")]
    # just keep necessary data
  
  colnames(Tdata) = c("Realization", 
                      paste("TTR", FORMULATION, sep = ""),
                      paste("TTD", FORMULATION, sep = ""),
                      paste("RRR", FORMULATION, sep = ""),
                      paste("BBD", FORMULATION, sep = ""))

  plotdata = Tdata
    # merge and rename
  
  if (FORMULATION == 0)
  {
    alldata = plotdata
      # add to master set
  }
  else
  {
    alldata = merge(alldata, plotdata, by = "Realization")
  }
  
  rm(ALLTdata, Tdata, plotdata)
  
}

trans = 0.45

fig4c1RRRdensplot = ggplot(alldata) + 
#  geom_density(aes(RRR0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(RRR1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(RRR2, fill = "2", colour = "2"), alpha = trans) +
  scale_fill_discrete(name = "Formulation") + xlim(c(75000,200000)) + ylim(c(0,0.00003)) +
  guides(colour = F, fill = F) +
  xlab("") + ylab("Probability") +
  geom_text(data = data.frame(), aes((200000+75000)/2, 0.000028), label = "Build Year 0", size = 7) +
  geom_text(data = data.frame(), aes(1, 0.9), label = "(c)", size = 12)

# REPEAT THIS LOOP AND PLOTTING FOR EACH PERMITTING PERIOD

RANK = 0
RDMnum = 0
REALIZATION = 0
for (FORMULATION in 0:2)
{
  datafolder = "figure4data\\c\\period2\\"
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization), FUN = sum, na.rm = T)
  # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  Tdata = Tdata[,c("Group.1", "TTR", "TTD", "RRR", "BBD")]
  # just keep necessary data
  
  colnames(Tdata) = c("Realization", 
                      paste("TTR", FORMULATION, sep = ""),
                      paste("TTD", FORMULATION, sep = ""),
                      paste("RRR", FORMULATION, sep = ""),
                      paste("BBD", FORMULATION, sep = ""))
  
  plotdata = Tdata
  # merge and rename
  
  if (FORMULATION == 0)
  {
    alldata = plotdata
    # add to master set
  }
  else
  {
    alldata = merge(alldata, plotdata, by = "Realization")
  }
  
  rm(ALLTdata, Tdata, plotdata)
  
}

fig4c2RRRdensplot = ggplot(alldata) + 
  #  geom_density(aes(RRR0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(RRR1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(RRR2, fill = "2", colour = "2"), alpha = trans) +
  scale_fill_discrete(name = "Formulation") + xlim(c(75000,200000)) + ylim(c(0,0.00003)) +
  guides(colour = F, fill = F) +
  xlab("Total Realization Releases (MG)") + ylab("") +
  geom_text(data = data.frame(), aes((200000+75000)/2, 0.000028), label = "Build Year 15", size = 7)

RANK = 0
RDMnum = 0
REALIZATION = 0
for (FORMULATION in 0:2)
{
  datafolder = "figure4data\\c\\period3\\"
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization), FUN = sum, na.rm = T)
  # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  Tdata = Tdata[,c("Group.1", "TTR", "TTD", "RRR", "BBD")]
  # just keep necessary data
  
  colnames(Tdata) = c("Realization", 
                      paste("TTR", FORMULATION, sep = ""),
                      paste("TTD", FORMULATION, sep = ""),
                      paste("RRR", FORMULATION, sep = ""),
                      paste("BBD", FORMULATION, sep = ""))
  
  plotdata = Tdata
  # merge and rename
  
  if (FORMULATION == 0)
  {
    alldata = plotdata
    # add to master set
  }
  else
  {
    alldata = merge(alldata, plotdata, by = "Realization")
  }
  
  rm(ALLTdata, Tdata, plotdata)
  
}

fig4c3RRRdensplot = ggplot(alldata) + 
  #  geom_density(aes(RRR0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(RRR1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(RRR2, fill = "2", colour = "2"), alpha = trans) +
  scale_fill_discrete(name = "Formulation") + xlim(c(75000,200000)) + ylim(c(0,0.00003)) +
  guides(colour = F, fill = guide_legend(ncol = 2)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black")) + 
  xlab("") + ylab("") +
  geom_text(data = data.frame(), aes((200000+75000)/2, 0.000028), label = "Build Year 30", size = 7)

png(paste(figurepath, "Figure4c", ".png", sep = ""), units = "in", width = 15, height = 5, res = 400)
multiplot(fig4c1RRRdensplot, fig4c2RRRdensplot, fig4c3RRRdensplot, cols = 3)
dev.off()

# --------------------------------------------- figure 5 ------------------------------------------------------
# parallel axis plots of all important objectives (regional reliability, raleigh releases, raleigh transfers,
# durham transfers, debt and risk and regional restrictions) --------------------------------------------------

# Here, I will use the interactive tool at https://reed.cee.cornell.edu/parallel-axis-categories/parallel/
# which requires data in a specific format.  So, the following code accepts objective values from results and 
# reorganizes the data into a format that the plotting page will accept.

RDMnum     = 0
datafolder = "figure3data\\"
allobjdata = c()
for (RANK in 0:35)
{
  for (FORMULATION in 0:2)
  {
    ObjData = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
    ObjData = as.numeric(as.character(ObjData[,1:105]))
    
    if (ObjData[76] > 0)
    {
      TypeToggle = "Option"
    }
    else
    {
      TypeToggle = "Spot"
    }
    
    allobjdata = rbind(allobjdata, c(TypeToggle, FORMULATION, ObjData))
  }
}

for (RANK in 36:74)
{
  for (FORMULATION in 1:2)
  {
    ObjData = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
    ObjData = as.numeric(as.character(ObjData[,1:105]))
    
    if (ObjData[76] > 0)
    {
      TypeToggle = "Option"
    }
    else
    {
      TypeToggle = "Spot"
    }
    
    allobjdata = rbind(allobjdata, c(TypeToggle, FORMULATION, ObjData))
  }
}
  # unequal number of runs per formulation, use second loop to make this work

aodata = as.data.frame(allobjdata)
aodata = aodata[,c(1:2, 83:107)]

objectivenames = c("Failures", "PeakDebt", "TotalLosses", "Releases", "Restrictions", "Transfers")
colnames(aodata)[3:27] = c(paste(objectivenames, "D", sep = ""), 
                           paste(objectivenames, "O", sep = ""),
                           paste(objectivenames, "R", sep = ""),
                           paste(objectivenames, "C", sep = ""),
                           "FallsFailures")

# The objective results must be placed into buckets (aka by formulation or by type of release mechanism)
# under the column heading "group".  Furthermore, only a handful of objectives will be necessary:
#   1.  Worst regional reliability value
#   2.  Durham and Raleigh treated transfers
#   3.  Raleigh releases
#   4.  Regionally worst restriction use
#   5.  Regionally worst peak debt
#   6.  Regionally worst financial risk 

FailuresMax    = apply(aodata[,c(3, 9, 15, 21)],  MARGIN = 1, FUN = max)
RestrictionMax = apply(aodata[,c(7, 13, 19, 25)], MARGIN = 1, FUN = max)
PeakDebtMax    = apply(aodata[,c(4, 10, 16, 22)], MARGIN = 1, FUN = max)
TotalLossesMax = apply(aodata[,c(5, 11, 17, 23)], MARGIN = 1, FUN = max)
RaleighRelease = aodata$ReleasesR
DTransfer      = aodata$TransfersD
RTransfer      = aodata$TransfersR

# SET UP TWO DIFFERENT DATASETS. FIRST WILL DIFFERENTIATE BY FORMULATION, SECOND WILL
# DIFFERENTIATE BY TYPE OF RELEASE SCHEME, ONLY FORMULATION 1

ParallelPlotData1 = data.frame(group = aodata[,2],
                               Reliability  = (1-as.numeric(FailuresMax))*100,
                               Restrictions = RestrictionMax,
                               PeakDebt     = PeakDebtMax,
                               TotalLosses  = TotalLossesMax,
                               Releases     = RaleighRelease,
                               RaleighTransfers = RTransfer,
                               DurhamTransfers  = DTransfer)

aodata = aodata[which(aodata[,2] == "1"),]

FailuresMax    = apply(aodata[,c(3, 9, 15, 21)],  MARGIN = 1, FUN = max)
RestrictionMax = apply(aodata[,c(7, 13, 19, 25)], MARGIN = 1, FUN = max)
PeakDebtMax    = apply(aodata[,c(4, 10, 16, 22)], MARGIN = 1, FUN = max)
TotalLossesMax = apply(aodata[,c(5, 11, 17, 23)], MARGIN = 1, FUN = max)
RaleighRelease = aodata$ReleasesR
DTransfer      = aodata$TransfersD
RTransfer      = aodata$TransfersR

ParallelPlotData2 = data.frame(group = aodata[,1],
                               Reliability  = (1-as.numeric(FailuresMax))*100,
                               Restrictions = RestrictionMax,
                               PeakDebt     = PeakDebtMax,
                               TotalLosses  = TotalLossesMax,
                               Releases     = RaleighRelease,
                               RaleighTransfers = RTransfer,
                               DurhamTransfers  = DTransfer)

objnames = c("Reliability", 
             "Restriction Use",
             "Peak Debt",
             "Total Losses",
             "Releases",
             "Raleigh Transfers",
             "Durham Transfers")

colnames(ParallelPlotData1) = c("group", objnames)
colnames(ParallelPlotData2) = c("group", objnames)

write.csv(ParallelPlotData1, paste(figurepath, "ParallelPlotDatabyFormulation.csv", sep = ""), row.names = F)
write.csv(ParallelPlotData2, paste(figurepath, "ParallelPlotDatabyContractType.csv", sep = ""), row.names = F)



























  
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


