## output plotting for thesis figures
## Jan 2017
## ------------------------------------------------------------------------------------------------------------
## ------------- set paths and load ---------------------------------------------------------------------------

rm(list = ls())
setwd("C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\Coding\\GorelickModelOutput")
  # two subfolders within this, depending on the type of release contract

figurepath = paste("Figures_", Sys.Date(), "\\", sep = "")
gispath    = "C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\GIS\\"
dir.create(figurepath)

source("C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\Coding\\multiplot.R")
library(ggplot2)
library(reshape2)
library(gridExtra)

RDMnum = 0

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

startyear = 20
endyear   = 23

Years = seq((2015 + startyear - 6 + 1), (2015 + endyear - 6 - 1), 1)

Suby0 = Sub0[which(Sub0$Year < endyear & Sub0$Year > startyear),]
Suby1 = Sub1[which(Sub1$Year < endyear & Sub1$Year > startyear),]

Suby0 = Suby0[with(Suby0, order(Year, Week)),]
Suby1 = Suby1[with(Suby1, order(Year, Week)),]

Suby0$TimeseriesWeek = c(1:nrow(Suby0))
Suby1$TimeseriesWeek = c(1:nrow(Suby1))

maxrel1 = max(Suby1$RRR)

Suby1$RRR = Suby1$RRR/max(Suby1$RRR)*0.30
Suby0$TTD = Suby0$TTD/max(Suby0$TTD)*0.30
Suby0$TTR = Suby0$TTR/max(Suby0$TTR)*0.30
  # scale for plotting

# prep using melt and reshape -----------------------------------

Rstorage = data.frame(TimeseriesWeek = Suby0$TimeseriesWeek,
                   Rstor0 = Suby0$Rstor,
                   Rstor1 = Suby1$Rstor)

Rtarget  = data.frame(TimeseriesWeek = Suby0$TimeseriesWeek,
                      Suby1$RaleighTargetStorageFraction)

RstorageMelt = melt(Rstorage, id = "TimeseriesWeek")
RtargetMelt  = melt(Rtarget,  id = "TimeseriesWeek")

Dstorage = data.frame(TimeseriesWeek = Suby0$TimeseriesWeek,
                      Dstor0 = Suby0$Dstor,
                      Dstor1 = Suby1$Dstor)

Dtarget  = data.frame(TimeseriesWeek = Suby0$TimeseriesWeek,
                      Suby1$DurhamTargetStorageFraction)

DstorageMelt = melt(Dstorage, id = "TimeseriesWeek")
DtargetMelt  = melt(Dtarget,  id = "TimeseriesWeek")

linesize = 1.3

plotR = ggplot() + 
  scale_linetype_identity() +
  geom_line(data = RstorageMelt,
            aes(x = TimeseriesWeek, y = value, colour = variable), 
            size = linesize) +
  geom_line(data = RtargetMelt,
            aes(x = TimeseriesWeek, y = value, linetype = "dashed"), 
            size = linesize) +
  scale_x_continuous(breaks = seq(Suby0$TimeseriesWeek[1],Suby0$TimeseriesWeek[nrow(Suby0)], by = 52), 
                     labels = Years) +
  ylab("Storage Fraction") + ylim(c(0,1)) +
  guides(colour = guide_legend(ncol = 2), keywidth = 0.52, keyheight = 0.1, default.unit = "inch") + 
  
  
  
  scale_fill_gradient(name = "Raw water transfers (MGW)", low = "black", high = "grey70") +
  theme(legend.position = c(0.06, 0.05),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.title = element_text(face = "bold", size = 14),
        legend.justification = c(0,0),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.y = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold")) +
  geom_text(data = data.frame(), aes(15, 0.38), label = "Downstream", size = 8) +
  ylab("Storage Fraction") + ylim(c(0,1)) +
  


plotD = ggplot() +
  scale_linetype_identity() +
  geom_line(data = Suby1, aes(x = TimeseriesWeek, y = Dstor,
#                              colour = "red",
                              linetype = "dashed"), size = linesize) +
  geom_line(data = Suby0, aes(x = TimeseriesWeek, y = Dstor,
#                              colour = "blue",
                              linetype = "solid"), size = linesize) +
  geom_line(data = Suby0, aes(x = TimeseriesWeek, y = DurhamTargetStorageFraction,
#                              colour = "black",
                              linetype = "dotted"), size = linesize) +
  # scale_colour_manual(name = element_blank(),
  #                    labels = c("ROF Trigger Level", 
  #                               "Storage (no raw water transfers)", 
  #                               "Storage (raw water transfers)"),
  #                    values = c("red"="red", 
  #                               "blue"="blue", 
  #                               "black"="black")) +
  scale_linetype_manual(name = element_blank(),
                        values = c("dashed", "dotted", "solid"),
                        labels = c( 
                                   "Storage (raw water transfers)","ROF Trigger Level", 
                                   "Storage (no raw water transfers)")) +
  scale_x_continuous(breaks = seq(Suby0$TimeseriesWeek[1],Suby0$TimeseriesWeek[nrow(Suby0)], by = 52), 
                     labels = c("Year 1", "Year 2")) +
  theme(legend.position = c(0.06,0.05), legend.box = "horizontal",
        legend.title = element_blank(), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.justification = c(0,0),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.x = element_blank(),
        legend.key.width = unit(0.5, "in"),
        legend.key.height = unit(0.2, "in")) +
  ylab("Storage Fraction") +
  guides(linetype = guide_legend(ncol = 3), keywidth = 0.52, keyheight = 0.1, default.unit = "inch") + 
#  guides(colour = guide_legend(ncol = 3)) + 
  ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(12, 0.83), label = "Upstream", size = 8) 

gA <- ggplotGrob(plotD)
gB <- ggplotGrob(plotR)

png(paste(figurepath, "Fig1", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
grid::grid.newpage()
grid::grid.draw(rbind(gA, gB))
dev.off()

# FINAL FIGURE 2 ----------------------------------------------------------------------------------------------

RDMnum = 0
FORMULATION = 0
REALIZATION = 27
RANK = 0
datafolder = "infrastructuredata\\"

ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
InfData  = read.csv(paste(datafolder, "InfraBuilt", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)

Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),]
Inf0 = InfData[which(InfData$Realization == REALIZATION),]

StorageCapacitiesD = c(6349)
# JL allocation not counted toward storage
TriggerYearsD = c(1) * 52 - 51
MultYearsD    = c(rep(StorageCapacitiesD[1],2392))

StorageCapacitiesR = c(14700, 18800, 18800, 18800 + 3700)
# NRI doesn't up firm capacity but allows 16+ MGD of withdrawal 
TriggerYearsR = c(1, 25-6, 37-6, 50-6) * 52 - 51
MultYearsR    = c(rep(StorageCapacitiesR[1],TriggerYearsR[2]),
                  rep(StorageCapacitiesR[2],TriggerYearsR[3]-TriggerYearsR[2]),
                  rep(StorageCapacitiesR[3],TriggerYearsR[4]-TriggerYearsR[3]),
                  rep(StorageCapacitiesR[4],2392-TriggerYearsR[4]))

TotCap1R = Sub0$Rstor * MultYearsR
TotCap1D = Sub0$Dstor * MultYearsD

FORMULATION = 2

ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
InfData  = read.csv(paste(datafolder, "InfraBuilt", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)

Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),]
Inf0 = InfData[which(InfData$Realization == REALIZATION),]

StorageCapacitiesR = c(14700, 14700 + 2500*0.7, 14700 + 2500*0.7 + 4100, 
                       14700 + 2500*0.7 + 4100 + 5200*0.7, 14700 + 2500*0.7 + 4100 + 5200*0.7)
# NRI doesn't up firm capacity but allows 16+ MGD of withdrawal 
TriggerYearsR = c(1, 16-6, 28-6, 43-6, 51-6) * 52 - 51
MultYearsR    = c(rep(StorageCapacitiesR[1],TriggerYearsR[2]),
                  rep(StorageCapacitiesR[2],TriggerYearsR[3]-TriggerYearsR[2]),
                  rep(StorageCapacitiesR[3],TriggerYearsR[4]-TriggerYearsR[3]),
                  rep(StorageCapacitiesR[4],TriggerYearsR[5]-TriggerYearsR[4]),
                  rep(StorageCapacitiesR[5],2392-TriggerYearsR[5]))

TotCap2R = Sub0$Rstor * MultYearsR

StorageCapacitiesD = c(6349, 6349 + 2500*0.3, 6349 + 2500*0.3, 
                       6349 + 2500*0.3 + 5200*0.3)
# NRI doesn't up firm capacity but allows 16+ MGD of withdrawal 
TriggerYearsD = c(1, 16-6, 17-6, 43-6) * 52 - 51
MultYearsD    = c(rep(StorageCapacitiesD[1],TriggerYearsD[2]),
                  rep(StorageCapacitiesD[2],TriggerYearsD[3]-TriggerYearsD[2]),
                  rep(StorageCapacitiesD[3],TriggerYearsD[4]-TriggerYearsD[3]),
                  rep(StorageCapacitiesD[4],2392-TriggerYearsD[4]))

TotCap2D = Sub0$Dstor * MultYearsD

DR = melt(data.frame(RCap = TotCap1R, DCap = TotCap2R, Time = 1:2392), id = "Time")
DD = melt(data.frame(RCap = TotCap1D, DCap = TotCap2D, Time = 1:2392), id = "Time")

A = ggplot(DD) + geom_line(aes(x = Time, y = value, col = variable), size = 0.8) +
  xlab("") + ylab("Storage (MG)") + 
  theme(axis.title.x = element_blank(), axis.text.x = element_blank(), 
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(vjust = 1),
        lege) +
  scale_x_continuous(breaks = seq(1,2393, by = 52*5)) +
  scale_color_manual(values=c("blue", "green4")) +
  guides(col = F) +
  geom_text(data = data.frame(), aes(2050, 2500), label = "Upstream", size = 8)

B = ggplot(DR) + geom_line(aes(x = Time, y = value, col = variable), size = 0.8) +
  xlab("") + ylab("Storage (MG)") + 
  theme(axis.title.x = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) +
  scale_x_continuous(breaks = seq(1,2393, by = 52*5), labels = seq(2015,2060,5)) +
  scale_color_manual(values = c("blue", "green4"), 
                     labels = c("Business-as-usual    ", "Joint upstream development    ")) +
  guides(col = guide_legend(title.position = "top", direction = "vertical", label.position = "right", 
                            ncol = 1, keywidth = 0.3, keyheight = 0.2, default.unit = "inch",
                            title = "Infrastructure Scenario")) +
  theme(legend.position = c(0.185, 0.81), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(size = 13, face = "bold"),
        legend.title = element_text(size = 15, face = "bold")) +
  geom_text(data = data.frame(), aes(2050, 5000), label = "Downstream", size = 8)

library(gridExtra)
gA <- ggplotGrob(A)
gB <- ggplotGrob(B)

png(paste(figurepath, "Fig2", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
grid::grid.newpage()
grid::grid.draw(rbind(gA, gB))
dev.off()

## figure 4 -------------------------------------------------------------------------------------------
## Make parallel coordinate plot in R -----------------------------------------------------------------

library(ggplot2)

maxconst = c(5, 50, 4, 0.25, 0.1)
minconst = c(0, 10, 1, 0.10, 0.0)
objnames = c("Failures", "Restrictions", "Peak Debt", "Losses", "Treated Transfers")
useroverride = F
# user-selected objective bounds

D = read.csv(paste(figurepath,"ParallelPlotDatabyFormulationFULLSET.csv",sep = ""), header = T)
D = D[,c(1:5,7)]
sample1 = nrow(D)
# read data, select just columns i care about
# first column is for color sorting, all other m columns of n objective sets are objectives

D2 = D[c(c(91:135),c(91:135)+135,c(91:135)+270),]
sample2 = nrow(D2)

D3 = D[which(D[,2] <= 1.5),]
sample3 = nrow(D3)

D4 = D[c(104, 104+135, 104+270),]
sample4 = nrow(D4)
# take subsets

D = rbind(D, D2, D3, D4)
# put all data together

colnames(D) = c("group", paste("Obj ", 1:(ncol(D)-1), sep = ""))
# rename columns to generic values to be overwritten later

nobj = length(names(D))-1
# number of objectives 

DataMaxBound = apply(D[,2:ncol(D)], MARGIN = 2, FUN = max)
DataMinBound = apply(D[,2:ncol(D)], MARGIN = 2, FUN = min)
# identify data bounds on objectives

UserMaxBound = maxconst
UserMinBound = minconst
# user-selected data bounds for plotting

MaxBounds = c()
MinBounds = c()

if (useroverride)
{
  for (i in 1:length(DataMaxBound))
  {
    if (DataMaxBound[i] > UserMaxBound[i])
    {
      MaxBounds[i] = DataMaxBound[i]
    }
    else
    {
      MaxBounds[i] = UserMaxBound[i]
    }
    
    if (DataMinBound[i] < UserMinBound[i])
    {
      MinBounds[i] = DataMinBound[i]
    }
    else
    {
      MinBounds[i] = UserMinBound[i]
    }
  }
  # override user if you want entire range
  
} else
{
  MaxBounds = UserMaxBound
  MinBounds = UserMinBound
}

Dnorm = D$group
for (col in 1:(ncol(D)-1))
{
  Dnorm = cbind(Dnorm, (D[,col+1] - MinBounds[col]) / (MaxBounds[col] - MinBounds[col]))
}
# normalize the objectives to a 0-1 range each

Dnorm = as.data.frame(Dnorm)
colnames(Dnorm) = c("group", paste("Obj ", 1:(ncol(D)-1), sep = ""))
Dnorm$FacetSet  = c(rep("A", sample1), rep("B", sample2), 
                    rep("C", sample3), rep("D", sample4))
Dnorm$group = as.character(Dnorm$group)
Dnorm$Sim = 1:nrow(Dnorm)
# set labels again and determine which facet each set is plotted on

maxChars = paste(c("", "", "", "", ""), MaxBounds, sep = "")
maxChars = paste(maxChars, c("%", "%", "00% AVR", "", " MGD"), sep = "")

minChars = paste(c("", "", "", "", ""), MinBounds, sep = "")
minChars = paste(minChars, c("%", "%", "00% AVR", "", " MGD"), sep = "")

colnames(Dnorm)[2:(nobj+1)] = minChars
#colnames(Dnorm)[2:(nobj+1)] = maxChars
  # adjust the labels depending on x-axis label position

DNmelt = melt(Dnorm, id = c("FacetSet", "group", "Sim"))
DNmelt = DNmelt[with(DNmelt, order(FacetSet, group, Sim)),]
#DNmelt = DNmelt[DNmelt$FacetSet == "D",]

A = ggplot(DNmelt) + geom_line(aes(x = variable, y = value, group = Sim, color = group), 
                               size = 0.95, alpha = 1) + 
  facet_grid(FacetSet ~ .) + guides(col = T) + ylim(c(0,1)) +
  theme(axis.title   = element_blank(),
        axis.text.x  = element_text(size = 15, face = "bold"),
        axis.text.y  = element_blank(),
        axis.ticks.y = element_blank()) +
  scale_y_continuous(breaks = c(0,1), expand = c(0, 0)) +
  scale_x_discrete(expand = c(0.03, 0.03), position = "bottom") +
  guides(col = guide_legend(title.position = "top", direction = "horizontal", label.position = "right", 
                            ncol = 1, keywidth = 0.3, keyheight = 0.15, default.unit = "inch",
                            title = "Formulation")) +
  theme(legend.position = c(0.7,0.8), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text  = element_text(size = 9, face = "bold"),
        legend.title = element_text(size = 10, face = "bold"),
        panel.spacing = unit(0,"lines"),
        strip.text  = element_text(size = 12, colour = "black", 
                                   angle = 0, face = "bold"),
        panel.grid.major.y = element_line(colour = "black"),
        panel.grid.major.x = element_line(colour = "grey70"),
        panel.background   = element_blank(),
        plot.subtitle = element_text(size = 12, face = "bold"),
        plot.margin = unit(c(0.3,0.2,0.5,0.2), unit ="in"),
        axis.line.x = element_line(colour = "black")) +
  scale_color_manual(values = c("blue","red","green4"), 
                     breaks = c("0", "1", "2"),
                     labels = c("Business-as-usual", 
                                "Raw water transfers added", 
                                "Joint upstream development added")) +
  geom_hline(yintercept = 0)

ggsave(paste(figurepath,"Fig5what.png",sep = ""), 
       width = 6.5, height = 15, units = "in")

## figure 7 -----------------------------------------------------------------------------------------------
## ---------- IBTs figure ------------------------------------------------------------------------

RANK   = 104
RDMnum = 0
for (FORMULATION in 0:2)
{
  datafolder = paste("figure2data\\", FORMULATION, "\\", sep = "")
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  RSTdata  = read.csv(paste(datafolder, "RestrictionData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization, ALLTdata$Year), FUN = max, na.rm = T)
  # first aggregate sums to annual level, then decide how to display data
  # SWITCH THIS TO MAX TO GET WORST WEEK OF EACH REALIZATION
  Tdata = aggregate(x = Tdata,    by = list(Tdata$Realization), FUN = max, na.rm = T)
  Rdata = aggregate(x = RSTdata,  by = list(RSTdata$Realization), FUN = max, na.rm = T)
  # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  colnames(Tdata)[1] = "col1"
  
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

alldata1 = melt(alldata[,c("TTR0", "TTR1", "TTR2")])
trans = 0.35

alldata1sub = alldata1[which(alldata1$variable != "TTR2"),]

TTRdensplot = ggplot(alldata1) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(name = "Formulation",
                    labels = c("Business-as-usual",
                               "Raw water transfers added",
                               "Joint upstream development added"),
                    values = c("blue",
                               "red",
                               "green4")) +
  # scale_fill_manual(name = "Formulation",
  #                   labels = c("Business-as-usual",
  #                              "Raw water transfers added"),
  #                   values = c("blue",
  #                              "red")) +
  # scale_fill_manual(name = "Formulation",
  #                   labels = c("Business-as-usual"),
  #                   values = c("blue")) +
  ggtitle("Raleigh") +
  guides(colour = F, fill = guide_legend(ncol = 1)) + 
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 10, face = "bold"),
        legend.key.size = unit(0.25, "in"),
        legend.title = element_text(size = 14, face = "bold"),
        legend.spacing = unit(1, "in")) +
  xlab("Treated transfers in worst week (MGW)") + ylab("Probability") +
  scale_x_continuous(breaks = c(0, 10, 14, 21, 30, 40)) +
  geom_vline(xintercept = c(14,21), colour = c("black", "red")) +
  geom_text(data = data.frame(), label = "Average IBT Limit", aes(13,0.12), 
            angle = 90) + ylim(c(0,0.4)) +
  geom_text(data = data.frame(), label = "Maximum IBT Limit", aes(20,0.12), 
            color = "red", angle = 90)

ggsave(paste(figurepath, "Fig7all", ".png", sep = ""), 
       units = "in", width = 7.5, height = 6)


## -----------------------------------------------------------------------------------------------
## -----------------------------------------------------------------------------------------------
## ------------ build some defense figures -------------------------------------------------------

x = c(20,23) + 45 - 1
  # ROF triggers

RDMnum = 0
FORMULATION = 1
REALIZATION = 0
datafolder = "figure2data\\1\\"
# either pick for the spot or option contract 
firstrank = x[1]
Years = seq((2015 + 21 - 6 + 1), (2015 + 23 - 6 - 1), 1)

for (RANK in x)
{
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  ObjData  = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
  
  AllocFrac = ObjData[,58]
  Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                               "RaleighTargetStorageFraction")]
  Sub0 = Sub0[which(Sub0$Year < 23),]
  Sub0 = Sub0[which(Sub0$Year > 21),]
  Sub0 = Sub0[with(Sub0, order(Year, Week)),]
  Sub0$TimeseriesWeek = c(1:nrow(Sub0))
  # collect and subset storage level data
  
  if (RANK == firstrank)
  {
    Sub0 = Sub0[,c("TimeseriesWeek","RaleighTargetStorageFraction")]
  }
  else
  {
    Sub0 = Sub0[,c("TimeseriesWeek","RaleighTargetStorageFraction")]
  }
  
  AllocFrac = rep(AllocFrac, nrow(Sub0))
  
  alld = data.frame(ReleaseAllocationFraction = AllocFrac, Sub0)
  allmelt = melt(alld, id = c("TimeseriesWeek", "ReleaseAllocationFraction"))
  
  if (RANK == firstrank)
  {
    Data = allmelt
  }
  else
  {
    Data = rbind(Data, allmelt)
  }
}

Data$variable = factor(Data$variable, labels = c("ROF Storage Target"))
Data$ReleaseAllocationFraction = factor(Data$ReleaseAllocationFraction, labels = c("4%", "10%"))

#Data = Data[which(Data$ReleaseAllocationFraction == "4%"),]
  # take subset to make secondary animation

ROFlevels = ggplot() + theme_bw() +
  geom_line(data = Data, aes(x = TimeseriesWeek, y = value, colour = ReleaseAllocationFraction), size = 1) +
  ylab("Storage Fraction") + theme(legend.position = c(0.12,0.15), axis.title.x = element_blank(),
                                   axis.title = element_text(size = 15, face = "bold"),
                                   axis.text  = element_text(size = 12, face = "bold"),
                                   legend.text = element_text(size = 13, face = "bold"),
                                   legend.direction = "vertical", legend.title = element_blank(),
                                   panel.grid.minor = element_blank()) + 
  ggtitle("Risk-of-failure Storage Levels") + ylim(c(0,1)) + 
  scale_colour_discrete(guide = guide_legend(title = "ROF")) +
  scale_x_continuous(breaks = seq(0,52,4.5), labels = c("J","F","M","A","M","J","J","A","S","O","N","D"))

ggsave(paste(figurepath, "ROFfig4", ".png", sep = ""), 
       units = "in", width = 5, height = 4)


## -----------------------------------------------------------------------------------------------
## -----------------------------------------------------------------------------------------------
## ------------ infrastructure trigger fig -------------------------------------------------------

x = c(20) + 45 - 1
# ROF triggers

RDMnum = 0
FORMULATION = 1
REALIZATION = 0
datafolder = "figure2data\\1\\"
# either pick for the spot or option contract 
firstrank = x[1]
Years = seq((2015 + 10 - 6 + 1), (2015 + 30 - 6 - 1), 1)


ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
ObjData  = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)

AllocFrac = ObjData[,58]
Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),]
Sub1 = Sub0[which(Sub0$Year < 30),]
Sub1 = Sub1[which(Sub1$Year > 21),]
Sub1 = Sub1[with(Sub1, order(Year, Week)),]
Sub1$TimeseriesWeek = c(1:nrow(Sub1))
  # collect and subset storage level data



alld = data.frame(Sub0)
allmelt = melt(alld, id = c("TimeseriesWeek", "ReleaseAllocationFraction"))




# --------------------------------------------- figure 1 FOR THE DEFENSE ------------------------------------------------------
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

startyear = 20
endyear   = 23

Years = seq((2015 + startyear - 6 + 1), (2015 + endyear - 6 - 1), 1)

Suby0 = Sub0[which(Sub0$Year < endyear & Sub0$Year > startyear),]
Suby1 = Sub1[which(Sub1$Year < endyear & Sub1$Year > startyear),]

Suby0 = Suby0[with(Suby0, order(Year, Week)),]
Suby1 = Suby1[with(Suby1, order(Year, Week)),]

Suby0$TimeseriesWeek = c(1:nrow(Suby0))
Suby1$TimeseriesWeek = c(1:nrow(Suby1))

maxrel1 = max(Suby1$RRR)

Suby1$RRR = Suby1$RRR/max(Suby1$RRR)*0.30
Suby0$TTD = Suby0$TTD/max(Suby0$TTD)*0.30
Suby0$TTR = Suby0$TTR/max(Suby0$TTR)*0.30
# scale for plotting

datatomelt = data.frame(TimeseriesWeek = Suby1$TimeseriesWeek,
                        Rstor1 = Suby1$Rstor, Rstor0 = Suby0$Rstor,
                        RTSF = Suby0$RaleighTargetStorageFraction)

meltR = melt(datatomelt, id = "TimeseriesWeek")

datatomelt = data.frame(TimeseriesWeek = Suby1$TimeseriesWeek,
                        Dstor1 = Suby1$Dstor, Dstor0 = Suby0$Dstor,
                        DTSF = Suby0$DurhamTargetStorageFraction)

meltD = melt(datatomelt, id = "TimeseriesWeek")

meltRsub = meltR[which(meltR$variable != "RTSF"),]
meltDsub = meltD[which(meltD$variable != "DTSF"),]
meltRtar = meltR[which(meltR$variable == "RTSF"),]
meltDtar = meltD[which(meltD$variable == "DTSF"),]

linesize = 1.3
plotR = ggplot() +
  geom_line(data = meltRsub, aes(x = TimeseriesWeek, y = value, 
                              colour = variable), size = linesize) +
  geom_line(data = meltRtar, aes(x = TimeseriesWeek, y = value), 
                                 linetype = "dotted", size = linesize) +
  scale_colour_manual(name = element_blank(),
                      values = c("red", "blue"),
                      labels = c("Storage (With RWTs)", "Storage (W/O RWTs)")) +
  theme(legend.position = c(0.5, 0.05), legend.box = "horizontal",
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.title = element_text(face = "bold", size = 14),
        legend.justification = c(0,0),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.y = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold")) +
  guides(colour = guide_legend(ncol = 2), keywidth = 0.52, keyheight = 0.1, default.unit = "inch") +
  scale_linetype_manual(name = element_blank(),
                        values = c("dotted"),
                        labels = c("Target risk-of-failure")) +
  theme(legend.position = c(0.1, 0.05), legend.box = "horizontal",
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.title = element_text(face = "bold", size = 14),
        legend.justification = c(0,0),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.y = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold"),
        panel.grid.major.y = element_line(colour = "black"),
        panel.grid.major.x = element_line(colour = "black"),
        panel.background   = element_blank(),
        plot.margin = unit(c(0.3,0.3,0.3,0.3), unit ="in"),
        axis.line.x = element_blank()) +
  scale_y_continuous(limits = c(0,1), expand = c(0.01, 0.01), labels = c(0,1), breaks = c(0,1)) +
  guides(linetype = guide_legend(ncol = 1), keywidth = 0.52, keyheight = 0.1, default.unit = "inch") +
  ylab("Storage Fraction") + 
  scale_x_continuous(breaks = seq(Suby0$TimeseriesWeek[1],Suby0$TimeseriesWeek[nrow(Suby0)], by = 51), 
                     labels = c("Year 1", "Year 2", "Year 3"), expand = c(0.01,0.01)) +
  geom_text(data = data.frame(), aes(20, 0.2), label = "Downstream", size = 8) 

plotD = ggplot() +
  geom_line(data = meltDsub, aes(x = TimeseriesWeek, y = value, 
                                 colour = variable), size = linesize) +
  geom_line(data = meltDtar, aes(x = TimeseriesWeek, y = value), 
            linetype = "dotted", size = linesize) +
  scale_colour_manual(name = element_blank(),
                      values = c("red", "blue"),
                      labels = c("Storage (With RWTs)", "Storage (W/O RWTs)")) +
  theme(legend.position = c(0.5, 0.05), legend.box = "horizontal",
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.title = element_text(face = "bold", size = 14),
        legend.justification = c(0,0),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.y = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold")) +
  guides(colour = guide_legend(ncol = 2), keywidth = 0.52, keyheight = 0.1, default.unit = "inch") +
  scale_linetype_manual(name = element_blank(),
                        values = c("dotted"),
                        labels = c("Target risk-of-failure")) +
  theme(legend.position = c(0.1, 0.05), legend.box = "horizontal",
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.title = element_text(face = "bold", size = 14),
        legend.justification = c(0,0),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.y = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold"),
        panel.grid.major.y = element_line(colour = "black"),
        panel.grid.major.x = element_line(colour = "black"),
        panel.background   = element_blank(),
        plot.margin = unit(c(0.3,0.3,0.3,0.3), unit ="in"),
        axis.line.x = element_blank()) +
  scale_y_continuous(limits = c(0,1), expand = c(0.01, 0.01), labels = c(0,1), breaks = c(0,1)) +
  guides(linetype = guide_legend(ncol = 1), keywidth = 0.52, keyheight = 0.1, default.unit = "inch") +
  ylab("Storage Fraction") + 
  scale_x_continuous(breaks = seq(Suby0$TimeseriesWeek[1],Suby0$TimeseriesWeek[nrow(Suby0)], by = 51), 
                     labels = c("Year 1", "Year 2", "Year 3"), expand = c(0.01,0.01)) +
  geom_text(data = data.frame(), aes(20, 0.2), label = "Upstream", size = 8) 

plotD = ggplot() +
  geom_line(data = meltD, aes(x = TimeseriesWeek, y = value, 
                              colour = variable), size = linesize) +
  scale_colour_manual(name = element_blank(),
                     values = c("red", "blue", "black"),
                     labels = c("Storage (With RWTs)", "Storage (No RWTs)", "ROF Target")) +
  scale_x_continuous(breaks = seq(Suby0$TimeseriesWeek[1],Suby0$TimeseriesWeek[nrow(Suby0)], by = 52), 
                     labels = Years) +
  theme(legend.position = c(0.06,0.05), legend.box = "horizontal",
        legend.title = element_blank(), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.justification = c(0,0),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.x = element_blank(),
        legend.key.width = unit(0.5, "in"),
        legend.key.height = unit(0.2, "in")) +
  ylab("Storage Fraction") +
  guides(colour = guide_legend(ncol = length(unique(meltD$variable))), keywidth = 0.52, keyheight = 0.1, default.unit = "inch") + 
  ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(12, 0.83), label = "Upstream", size = 8) 

gA <- ggplotGrob(plotD)
gB <- ggplotGrob(plotR)

png(paste(figurepath, "Fig1sub2", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
grid::grid.newpage()
grid::grid.draw(rbind(gA, gB))
dev.off()


scale_colour_manual(name = element_blank(),
                    values = c("blue", "black"),
                    labels = c("Storage (No RWTs)", "ROF Target")) +
  scale_colour_manual(name = element_blank(),
                      values = c("blue"),
                      labels = c("Storage (No RWTs)")) +


  ### density plots for various comparisons --------------------------------------------------------------------------
### ----------------------------------------------------------------------------------------------------------------

### across formulation ---------------------------------------------------------------------------------------------

RANK = 104
for (FORMULATION in 0:2)
{
  datafolder = paste("figure2data\\", FORMULATION, "\\", sep = "")
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  RSTdata  = read.csv(paste(datafolder, "RestrictionData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization, ALLTdata$Year), FUN = sum, na.rm = T)
  # first aggregate sums to annual level, then decide how to display data
  # SWITCH THIS TO MAX TO GET WORST WEEK OF EACH REALIZATION
  Tdata = aggregate(x = Tdata,    by = list(Tdata$Realization), FUN = max, na.rm = T)
  Rdata = aggregate(x = RSTdata,  by = list(RSTdata$Realization), FUN = max, na.rm = T)
  # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  colnames(Tdata)[1] = "col1"
  
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

alldata1 = melt(alldata[,c("TTR0", "TTR1", "TTR2")])
alldata2 = melt(alldata[,c("TTD0", "TTD1", "TTD2")])
alldata3 = melt(alldata[,c("restR0", "restR1", "restR2")])
alldata4 = melt(alldata[,c("restD0", "restD1", "restD2")])

alldata1sub = alldata1[which(alldata1$variable != "TTR2"),]
alldata2sub = alldata2[which(alldata2$variable != "TTD2"),]
alldata3sub = alldata3[which(alldata3$variable != "restR2"),]
alldata4sub = alldata4[which(alldata4$variable != "restD2"),]

trans = 0.35

TTRdensplot = ggplot(alldata1sub) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  # scale_fill_manual(name = "Formulation",
  #                   labels = c("Business-as-usual",
  #                              "Raw water transfers added",
  #                              "Joint upstream development added"),
  #                   values = c("blue",
  #                              "red",
  #                              "green4")) +
  scale_fill_manual(name = "Formulation",
                    labels = c("Business-as-usual",
                               "Raw water transfers added"),
                    values = c("blue",
                               "red")) +
  ggtitle("Raleigh") + xlim(c(0,200)) + ylim(c(0,0.15)) +
  guides(colour = F, fill = guide_legend(ncol = 1)) + 
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 18, face = "bold"),
        legend.spacing = unit(1, "in")) + 
  xlab("Treated transfers in worst year (MGY)") 

TTDdensplot = ggplot(alldata2sub) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  # scale_fill_manual(name = "Formulation",
  #                   labels = c("Business-as-usual",
  #                              "Raw water transfers added",
  #                              "Joint upstream development added"),
  #                   values = c("blue",
  #                              "red",
  #                              "green4")) +
  scale_fill_manual(name = "Formulation",
                    labels = c("Business-as-usual",
                               "Raw water transfers added"),
                    values = c("blue",
                               "red")) +
  guides(colour = F, fill = F) + ggtitle("Durham") + xlim(c(4000,7300)) + ylim(c(0,0.002)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold")) + 
  xlab("Treated transfers in worst year (MGY)") + ylab("Probability") 

RestRdensplot = ggplot(alldata3sub) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  # scale_fill_manual(name = "Formulation",
  #                   labels = c("0 - Business-as-usual",
  #                              "1 - Raw water transfers added",
  #                              "2 - Joint upstream development added"),
  #                   values = c("blue",
  #                              "red",
  #                              "green4")) +
  scale_fill_manual(name = "Formulation",
                    labels = c("Business-as-usual",
                               "Raw water transfers added"),
                    values = c("blue",
                               "red")) +
  guides(colour = F, fill = F) + xlim(c(0,52)) + ylim(c(0,0.06)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) + 
  xlab("Weeks under restriction in worst year")

RestDdensplot = ggplot(alldata4sub) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) + 
  # scale_fill_manual(name = "Formulation",
  #                   labels = c("0 - Business-as-usual",
  #                              "1 - Raw water transfers added",
  #                              "2 - Joint upstream development added"),
  #                   values = c("blue",
  #                              "red",
  #                              "green4")) +
  scale_fill_manual(name = "Formulation",
                    labels = c("Business-as-usual",
                               "Raw water transfers added"),
                    values = c("blue",
                               "red")) +
  guides(colour = F, fill = F) + xlim(c(0,52)) + ylim(c(0,0.06)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) + 
  xlab("Weeks under restriction in worst year") + ylab("Probability") 

png(paste(figurepath, "Fig5", ".png", sep = ""), units = "in", width = 15, height = 10, res = 400)
multiplot(TTDdensplot, RestDdensplot, TTRdensplot, RestRdensplot, cols = 2)
dev.off()

























# --------------------------------------------- figure 2 ------------------------------------------------------
# single simulation distribution of releases/transfer/restriction use -----------------------------------------
# (looks like histograms on top of each other, show the shift between formulations) ---------------------------

RANK = 0
for (FORMULATION in 0:2)
{
  datafolder = paste("figure2data\\", FORMULATION, "\\", sep = "")
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  RSTdata  = read.csv(paste(datafolder, "RestrictionData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization, ALLTdata$Year), FUN = sum, na.rm = T)
    # first aggregate sums to annual level, then decide how to display data
  Tdata = aggregate(x = Tdata,    by = list(Tdata$Realization), FUN = max, na.rm = T)
  Rdata = aggregate(x = RSTdata,  by = list(RSTdata$Realization), FUN = mean, na.rm = T)
    # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  colnames(Tdata)[1] = "col1"
  
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

trans = 0.30

TTRdensplot = ggplot(alldata) + 
  geom_density(aes(TTR0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(TTR1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(TTR2, fill = "2", colour = "2"), alpha = trans) +
  scale_fill_discrete(name = "Formulation") + ggtitle("Raleigh") +
  guides(colour = F, fill = guide_legend(ncol = 3)) + 
#  xlim(c(0,150)) +
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 15, face = "bold"),
        legend.spacing = unit(1, "in")) + 
  xlab("Total realization transfers (MG)")  
#  geom_text(data = data.frame(), aes(500/10, 0.04), label = "Raleigh", size = 9, hjust = 0, vjust = 0)

TTDdensplot = ggplot(alldata) + 
  geom_density(aes(TTD0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(TTD1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(TTD2, fill = "2", colour = "2"), alpha = trans) +
#  scale_fill_discrete(name = "Formulation") +
  guides(colour = F, fill = F) + ggtitle("Durham") +
#  xlim(c(0,1000)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold")) + 
  xlab("Total realization transfers (MG)") + ylab("Probability") 
#  geom_text(data = data.frame(), aes(2000/10, 0.0015), label = "Durham", size = 9, hjust = 0, vjust = 0)

RestRdensplot = ggplot(alldata) + 
  geom_density(aes(restR0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(restR1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(restR2, fill = "2", colour = "2"), alpha = trans) +
#  scale_fill_discrete(name = "Formulation") +
  guides(colour = F, fill = F) +
#  xlim(c(0,52)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) + 
  xlab("Average weeks under restriction annually") 
#  geom_text(data = data.frame(), aes(25, 0.05), label = "Raleigh", size = 7)

RestDdensplot = ggplot(alldata) + 
  geom_density(aes(x = restD0, fill = "0", colour = "0"), alpha = trans) +
  geom_density(aes(x = restD1, fill = "1", colour = "1"), alpha = trans) +
  geom_density(aes(x = restD2, fill = "2", colour = "2"), alpha = trans) +
#  scale_fill_discrete(name = "Formulation") + 
  guides(colour = F, fill = F) + 
#  xlim(c(0,7)) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) + 
  xlab("Average weeks under restriction annually") + ylab("Probability") 
#  geom_text(data = data.frame(), aes(25, 0.055), label = "Durham", size = 7)

png(paste(figurepath, "Figure2", ".png", sep = ""), units = "in", width = 15, height = 10, res = 400)
multiplot(TTDdensplot, RestDdensplot, TTRdensplot, RestRdensplot, cols = 2)
dev.off()

# --------------------------------------------- figure 4 ------------------------------------------------------
# call out individual extreme pareto solutions and compare timeseries to show parameter differences -----------
# (transfers and releases under different ROF triggers, release/transfer caps, release allocation rule, etc) --
# (also show impact of different infrastructure build dates on timing of transfers and releases)

# First plot: effectiveness of releases under different ACE guidelines
# Take a single realization across several simulation runs for the same formulation (1) and repeat a plot
# like that in figure 1 but with different storage levels for each level of water supply release percentage

RDMnum = 0
FORMULATION = 1
REALIZATION = 10
datafolder = "figure2data\\1\\"
  # either pick for the spot or option contract 
firstrank = 90
Years = seq((2015 + 20 - 6 + 1), (2015 + 23 - 6 - 1), 1)

for (RANK in c(firstrank,c(29:31)+90))
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
  
  if (RANK == firstrank)
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
  
  if (RANK == firstrank)
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
  geom_line(data = Rdata, aes(x = TimeseriesWeek, y = value, colour = colorcol), size = 0.5) +
  scale_colour_manual(name = "% of RWTs allocated for downstream water supply",
                      labels = c("42.3%",
                                 "60%",
                                 "80%",
                                 "100%",
                                 "4% ROF Level"),
                      values = c("blue",
                                 "red",
                                 "green",
                                 "orange",
                                 "black")) +
  guides(colour = F) +
  ylab("Storage Fraction") + theme(legend.position = "none", axis.title.x = element_blank(),
                                   axis.text.x = element_blank(), axis.ticks.x = element_blank(),
                                   axis.title = element_text(size = 15, face = "bold"),
                                   axis.text  = element_text(size = 12, face = "bold")) +
  geom_text(data = data.frame(), aes(100, 0.125), label = "Raleigh", size = 7) + ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(1, 0.9), label = "(a)", size = 12)

DStorages$colorcol = DStorages$ReleaseAllocationFraction
DGC$colorcol = as.character(DGC$variable2)

Ddata = rbind(DGC, DStorages)

rm(ALLTdata, alld, Data, allmelt, DGC, RGC, ObjData, DStorages, RStorages, Sub0, 
   AllocFrac, GuideCurves, Storages)

fig4allocfraclevelsD = ggplot() + 
  geom_line(data = Ddata, aes(x = TimeseriesWeek, y = value, colour = colorcol), size = 0.5) + 
  ylab("Storage Fraction") +
  scale_colour_manual(name = "% of RWTs allocated for downstream water supply",
                    labels = c("42.3%",
                               "60%",
                               "80%",
                               "100%",
                               "4% ROF Level"),
                    values = c("blue",
                               "red",
                               "green",
                               "orange",
                               "black")) +
#  scale_colour_discrete(name = "% of releases allocated for water supply",
#                        breaks = c("0.423", "0.6", "0.8", "1", "Durham ROF Guide Curve"),
#                        labels = c("42.3", "60", "80", "100", "2% ROF Level")) +
  theme(legend.position = c(0.33, 0.15), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.x = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 15, face = "bold"),
        legend.spacing = unit(1, "in")) + 
  geom_text(data = data.frame(), aes(100, 0.125), label = "Durham", size = 7) + ylim(c(0,1)) +
  guides(colour = guide_legend(title.position = "top", direction = "horizontal", label.position = "bottom", ncol = 5,
                               keywidth = 0.52, keyheight = 0.1, default.unit = "inch")) +
  scale_x_continuous(breaks = seq(1,52*nrow(Ddata)/5/52, by = 52), 
                     labels = Years)

png(paste(figurepath, "Figure4a", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
multiplot(fig4allocfraclevelsR, fig4allocfraclevelsD, cols = 1)
dev.off()

# REPEAT CODE BUT CHANGE SLIGHTLY TO DO THE SAME FOR DIFFERENT RELEASE SIZES --------------------------------------------

RDMnum = 0
FORMULATION = 1
REALIZATION = 603
datafolder = "figure2data\\1\\"

Years = seq((2015 + 22 - 6 + 1), (2015 + 25 - 6 - 1), 1)

firstrank = 23 + 90
for (RANK in c(c(23,0,28)+90))
{
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  ObjData  = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
  
  RelCap = ObjData[,60]
  Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                               "Rstor", "Dstor", 
                                                               "RaleighTargetStorageFraction", "DurhamTargetStorageFraction")]
  Sub0 = Sub0[which(Sub0$Year < 25),]
  Sub0 = Sub0[which(Sub0$Year > 22),]
  Sub0 = Sub0[with(Sub0, order(Year, Week)),]
  Sub0$TimeseriesWeek = c(1:nrow(Sub0))
  # collect and subset storage level data
  
  if (RANK == 113)
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
  
  if (RANK == firstrank)
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
  scale_colour_manual(name = "Maximum RWT cap (MGW)",
                      labels = c("100",
                                 "500",
                                 "5,000",
                                 "4% ROF Level"),
                      values = c("blue",
                                 "red",
                                 "orange",
                                 "black")) +
  guides(colour = F) +
  ylab("Storage Fraction") + theme(legend.position = "none", axis.title.x = element_blank(),
                                   axis.text.x = element_blank(), axis.ticks.x = element_blank(),
                                   axis.title = element_text(size = 15, face = "bold"),
                                   axis.text  = element_text(size = 12, face = "bold")) +
  geom_text(data = data.frame(), aes(100, 0.125), label = "Raleigh", size = 7) + ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(1, 0.9), label = "(b)", size = 12)

DStorages$colorcol = DStorages$ReleaseCap
DGC$colorcol = as.character(DGC$variable2)

Ddata = rbind(DGC, DStorages)

fig4relcaplevelsD = ggplot() + 
  geom_line(data = Ddata, aes(x = TimeseriesWeek, y = value, colour = colorcol)) + 
  ylab("Storage Fraction") +
  scale_colour_manual(name = "Maximum RWT cap (MGW)",
                      labels = c("100",
                                 "500",
                                 "5,000",
                                 "4% ROF Level"),
                      values = c("blue",
                                 "red",
                                 "orange",
                                 "black")) +
  # scale_colour_discrete(name = "Release cap (MGW)",
  #                       breaks = c("100", "500", "5000", "Durham ROF Guide Curve"),
  #                       labels = c("100", "500", "5,000", "2% ROF Level")) +
  theme(legend.position = c(0.23, 0.15), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.x = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 15, face = "bold"),
        legend.spacing = unit(1, "in")) + 
  geom_text(data = data.frame(), aes(100, 0.125), label = "Durham", size = 7) + ylim(c(0,1)) +
  guides(colour = guide_legend(title.position = "top", direction = "horizontal", label.position = "bottom", 
                               ncol = 4,
                               keywidth = 0.52, keyheight = 0.1, default.unit = "inch")) +
  scale_x_continuous(breaks = seq(1,52*2, by = 52), 
                     labels = Years)

png(paste(figurepath, "Figure4b", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
multiplot(fig4relcaplevelsR, fig4relcaplevelsD, cols = 1)
dev.off()

# --------------------------------------------- figure 5 ------------------------------------------------------
# parallel axis plots of all important objectives (regional reliability, raleigh releases, raleigh transfers,
# durham transfers, debt and risk and regional restrictions) --------------------------------------------------

# Here, I will use the interactive tool at https://reed.cee.cornell.edu/parallel-axis-categories/parallel/
# which requires data in a specific format.  So, the following code accepts objective values from results and 
# reorganizes the data into a format that the plotting page will accept.

RDMnum     = 0
datafolder = "figure3data0.3\\"
allobjdata = c()
otherdatakeeper = c()
for (FORMULATION in 0:2)
{
  for (RANK in 0:134)
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
    
    otherdatakeeper = rbind(otherdatakeeper, ObjData[c(58,59,60,63,75)])
    
    allobjdata = rbind(allobjdata, c(TypeToggle, FORMULATION, ObjData))
  }
}

# for (RANK in 10:83)
# {
#   for (FORMULATION in 1:2)
#   {
#     ObjData = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
#     ObjData = as.numeric(as.character(ObjData[,1:105]))
#     
#     if (ObjData[76] > 0)
#     {
#       TypeToggle = "Option"
#     }
#     else
#     {
#       TypeToggle = "Spot"
#     }
#     
#     allobjdata = rbind(allobjdata, c(TypeToggle, FORMULATION, ObjData))
#   }
# }
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

FailuresMax    = as.numeric(as.character(apply(aodata[,c(3, 9, 15, 21)],  MARGIN = 1, FUN = max)))
RestrictionMax = as.numeric(as.character(apply(aodata[,c(7, 13, 19, 25)], MARGIN = 1, FUN = max)))
PeakDebtMax    = as.numeric(as.character(apply(aodata[,c(4, 10, 16, 22)], MARGIN = 1, FUN = max)))
TotalLossesMax = as.numeric(as.character(apply(aodata[,c(5, 11, 17, 23)], MARGIN = 1, FUN = max)))
RaleighRelease = as.numeric(as.character(aodata$ReleasesR))/365
DTransfer      = as.numeric(as.character(aodata$TransfersD))/365
RTransfer      = as.numeric(as.character(aodata$TransfersR))/365

WhichFailuresMax    = apply(aodata[,c(3, 9, 15, 21)],  MARGIN = 1, FUN = which.max)
WhichRestrictionMax = apply(aodata[,c(7, 13, 19, 25)], MARGIN = 1, FUN = which.max)
WhichPeakDebtMax    = apply(aodata[,c(4, 10, 16, 22)], MARGIN = 1, FUN = which.max)
WhichTotalLossesMax = apply(aodata[,c(5, 11, 17, 23)], MARGIN = 1, FUN = which.max)
  # tells which city is the minimax objective value 

assign.city = function(vectoritem)
{
  for (i in 1:length(vectoritem))
  {
    if (vectoritem[i] == 1)
    {
      vectoritem[i] = "Durham"
    }
    else if (vectoritem[i] == 2)
    {
      vectoritem[i] = "OWASA"
    }
    else if (vectoritem[i] == 3)
    {
      vectoritem[i] = "Raleigh"
    }
    else
    {
      vectoritem[i] = "Cary"
    }
  }
  
  return(vectoritem)
}

MinimaxInfo = data.frame(group = aodata[,2],
                         FailureCity = assign.city(WhichFailuresMax),
                         DebtCity = assign.city(WhichPeakDebtMax),
                         RestCity = assign.city(WhichRestrictionMax),
                         RiskCity = assign.city(WhichTotalLossesMax),
                         Releases     = RaleighRelease,
                         Buybacks     = aodata$ReleasesD,
                         RaleighTransfers = RTransfer,
                         DurhamTransfers  = DTransfer)

ParallelPlotData1 = data.frame(group = aodata[,2],
                               Reliability  = FailuresMax*100,
                               Restrictions = RestrictionMax*100,
                               PeakDebt     = PeakDebtMax,
                               TotalLosses  = TotalLossesMax,
                               Releases     = RaleighRelease,
                               RaleighTransfers = RTransfer,
                               DurhamTransfers  = DTransfer)

objnames = c("Failures (% in worst simulation year)", 
             "Restrictions (% of weeks in worst year)",
             "Debt ($MM average in worst year)",
             "Losses (99% VaR for the simulation)",
             "Releases (average MGD)",
             "Raleigh Transfers (average MGD)",
             "Durham Transfers (average MGD)")

colnames(ParallelPlotData1) = c("group", objnames)

x = c(12:23)
  # ROF triggers
x = c(24,25,1,26:29)
x = c(24,1,27,29)
  # RWT cap
x = c(1,30:32)
  # ACE frac
x = c(37,39,41,43,45)
  # JLM frac
x = c(104,104+135,104+270)
  # low development small set
x = c(1:405)
  # keep all realizations

xgroup = otherdatakeeper[x + 270 + 90,]
  # pull the simulations we want
  # first +135 is to change formulation
  # second additive adjusts to infrastructure options available for Raleigh
PPD = ParallelPlotData1[c(x + 270 + 90),]
PPD = ParallelPlotData1[x,]
  # select the subset I am looking for

PPD$group = xgroup[,1]
  # raleigh ROF triggers
PPD$group = xgroup[,2]
  # durham ROF triggers
PPD$group = xgroup[,3]
  # RWT size cap
PPD$group = xgroup[,4]
  # ACE frac
PPD$group = xgroup[,5]
  # JLM share frac

PPD = PPD[,c(1:3,5:8)]
  # remove debt objective
  # dont do this for JLM development results
PPD = PPD[,c(1:5,7)]
  # no durham transfer or raw transfer objectives

write.csv(PPD, paste(figurepath, "ParallelPlotDataFinalALL.csv", sep = ""), row.names = F)
  # ADJUST THIS BEFORE RUNNING SUBSET

write.csv(ParallelPlotData1, paste(figurepath, "ParallelPlotDatabyFormulationFULLSET.csv", sep = ""), row.names = F)

### density plots for various comparisons --------------------------------------------------------------------------
### ----------------------------------------------------------------------------------------------------------------

### across formulation ---------------------------------------------------------------------------------------------

RANK = 104
for (FORMULATION in 0:2)
{
  datafolder = paste("figure2data\\", FORMULATION, "\\", sep = "")
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  RSTdata  = read.csv(paste(datafolder, "RestrictionData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization, ALLTdata$Year), FUN = sum, na.rm = T)
    # first aggregate sums to annual level, then decide how to display data
    # SWITCH THIS TO MAX TO GET WORST WEEK OF EACH REALIZATION
  Tdata = aggregate(x = Tdata,    by = list(Tdata$Realization), FUN = max, na.rm = T)
  Rdata = aggregate(x = RSTdata,  by = list(RSTdata$Realization), FUN = max, na.rm = T)
    # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  colnames(Tdata)[1] = "col1"
  
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

alldata1 = melt(alldata[,c("TTR0", "TTR1", "TTR2")])
alldata2 = melt(alldata[,c("TTD0", "TTD1", "TTD2")])
alldata3 = melt(alldata[,c("restR0", "restR1", "restR2")])
alldata4 = melt(alldata[,c("restD0", "restD1", "restD2")])

trans = 0.35

TTRdensplot = ggplot(alldata1) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(name = "Formulation",
                    labels = c("Business-as-usual",
                               "Raw water transfers added",
                               "Joint upstream development added"),
                    values = c("blue",
                               "red",
                               "green4")) +
  ggtitle("Raleigh") +
  guides(colour = F, fill = guide_legend(ncol = 1)) + 
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 18, face = "bold"),
        legend.spacing = unit(1, "in")) + 
  xlab("Treated transfers in worst year (MGY)") 
#  geom_text(data = data.frame(), aes(500/10, 0.04), label = "Raleigh", size = 9, hjust = 0, vjust = 0)

TTDdensplot = ggplot(alldata2) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(name = "Formulation",
                    labels = c("Business-as-usual",
                               "Raw water transfers added",
                               "Joint upstream development added"),
                    values = c("blue",
                               "red",
                               "green4")) +
  guides(colour = F, fill = F) + ggtitle("Durham") +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold")) + 
  xlab("Treated transfers in worst year (MGY)") + ylab("Probability") 

RestRdensplot = ggplot(alldata3) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(name = "Formulation",
                    labels = c("0 - Business-as-usual",
                               "1 - Raw water transfers added",
                               "2 - Joint upstream development added"),
                    values = c("blue",
                               "red",
                               "green4")) +
  guides(colour = F, fill = F) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) + 
  xlab("Weeks under restriction in worst year")

RestDdensplot = ggplot(alldata4) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) + 
  scale_fill_manual(name = "Formulation",
                    labels = c("0 - Business-as-usual",
                               "1 - Raw water transfers added",
                               "2 - Joint upstream development added"),
                    values = c("blue",
                               "red",
                               "green4")) +
  guides(colour = F, fill = F) + 
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) + 
  xlab("Weeks under restriction in worst year") + ylab("Probability") 

png(paste(figurepath, "Fig5", ".png", sep = ""), units = "in", width = 15, height = 10, res = 400)
multiplot(TTDdensplot, RestDdensplot, TTRdensplot, RestRdensplot, cols = 2)
dev.off()

### across rank ---------------------------------------------------------------------------------------------

x = c(12:23)
x = c(12:15) + 45 - 1
x = c(16:19) + 45 - 1
x = c(20:23) + 45 - 1
  # ROF triggers
x = c(24,25,1,26:29)
x = c(24,1,27,29) + 90 - 1
  # RWT cap
  # formulation 1, limited infrastructure runs (+90)?
x = c(1,30:32) + 90 - 1
  # ACE frac
x = c(37,39,41,43,45) + 90 - 1
  # JLM frac

firstrank = x[1]
FORMULATION = 1
for (RANK in x)
{
  datafolder = paste("figure2data\\", FORMULATION, "\\", sep = "")
  
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  RSTdata  = read.csv(paste(datafolder, "RestrictionData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  
  Tdata = aggregate(x = ALLTdata, by = list(ALLTdata$Realization, ALLTdata$Year), FUN = sum, na.rm = T)
  # first aggregate sums to annual level, then decide how to display data
  Tdata = aggregate(x = Tdata,    by = list(Tdata$Realization), FUN = max, na.rm = T)
  Rdata = aggregate(x = RSTdata,  by = list(RSTdata$Realization), FUN = max, na.rm = T)
  # realization-level counts of weeks of restrictions, total MG of transfers to R and D, releases, buybacks
  
  colnames(Tdata)[1] = "col1"
  
  Tdata = Tdata[,c("Group.1", "TTR", "TTD", "RRR", "BBD")]
  Rdata = Rdata[,c("Group.1", "restR", "restD")]
  # just keep necessary data
  
  colnames(Tdata) = c("Realization", 
                      paste("TTR", RANK, sep = ""),
                      paste("TTD", RANK, sep = ""),
                      paste("RRR", RANK, sep = ""),
                      paste("BBD", RANK, sep = ""))
  colnames(Rdata) = c("Realization", 
                      paste("restR", RANK, sep = ""),
                      paste("restD", RANK, sep = ""))
  plotdata = merge(Tdata, Rdata, by = "Realization")
  # merge and rename
  
  if (RANK == firstrank)
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

alldata1 = melt(alldata[,paste("TTR", x, sep = "")])
alldata2 = melt(alldata[,paste("TTD", x, sep = "")])
alldata3 = melt(alldata[,paste("restR", x, sep = "")])
alldata4 = melt(alldata[,paste("restD", x, sep = "")])

trans = 0.40

TTRdensplot = ggplot(alldata1) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(
#                    name = "Maximum allowable weekly\nRWT (MGW)",
  #                  name = "% of RWTs allocated for\ndownstream water supply",
                     name = "ROF trigger",
#                    labels = c("100",
#                               "500",
#                               "1,000",
#                               "5,000"),
                    labels = c("4%", "6%", "8%", "10%"),
                    # labels = c("42.3%",
                    #            "60%",
                    #            "80%",
                    #            "100%"),
                    values = c("blue",
                               "red",
                               "green",
                               "orange")) +
  # scale_fill_manual(name = "% of joint Lake Michie\ndevelopment capacity for\nDurham (upstream)",
  #   labels = c("100%",
  #              "70%",
  #              "50%",
  #              "30%",
  #              "10%"),
  #   values = c("blue",
  #              "red",
  #              "green",
  #              "orange",
  #              "purple")) +
  ggtitle("Raleigh") +
  guides(colour = F, fill = guide_legend(ncol = 1)) + 
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 18, face = "bold"),
        legend.spacing = unit(1, "in")) + 
  xlab("Treated transfers in worst year (MGY)")  
#  geom_text(data = data.frame(), aes(500/10, 0.04), label = "Raleigh", size = 9, hjust = 0, vjust = 0)

TTDdensplot = ggplot(alldata2) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(name = "RWT downstream water supply\nallocation fraction",
                    labels = c("42.3%",
                               "60%",
                               "80%",
                               "100%"),
                    values = c("blue",
                               "red",
                               "green",
                               "orange")) +
  # scale_fill_manual(name = "% of joint Lake Michie\ndevelopment capacity for\nDurham (downstream)",
  #                   labels = c("100%",
  #                              "70%",
  #                              "50%",
  #                              "30%",
  #                              "10%"),
  #                   values = c("blue",
  #                              "red",
  #                              "green",
  #                              "orange",
  #                              "purple")) +
  guides(colour = F, fill = F) + ggtitle("Durham") +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold")) + 
  xlab("Treated transfers in worst year (MGY)") + ylab("Probability") 

RestRdensplot = ggplot(alldata3) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(name = "RWT downstream water supply\nallocation fraction",
                    labels = c("42.3%",
                               "60%",
                               "80%",
                               "100%"),
                    values = c("blue",
                               "red",
                               "green",
                               "orange")) +
  # scale_fill_manual(name = "% of joint Lake Michie\ndevelopment capacity for\nDurham (downstream)",
  #                   labels = c("100%",
  #                              "70%",
  #                              "50%",
  #                              "30%",
  #                              "10%"),
  #                   values = c("blue",
  #                              "red",
  #                              "green",
  #                              "orange",
  #                              "purple")) +
  guides(colour = F, fill = F) +
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) + 
  xlab("Average weeks under restriction in worst year") 

RestDdensplot = ggplot(alldata4) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) + 
  scale_fill_manual(name = "RWT downstream water supply\nallocation fraction",
                    labels = c("42.3%",
                               "60%",
                               "80%",
                               "100%"),
                    values = c("blue",
                               "red",
                               "green",
                               "orange")) +
  # scale_fill_manual(name = "% of joint Lake Michie\ndevelopment capacity for\nDurham (downstream)",
  #                   labels = c("100%",
  #                              "70%",
  #                              "50%",
  #                              "30%",
  #                              "10%"),
  #                   values = c("blue",
  #                              "red",
  #                              "green",
  #                              "orange",
  #                              "purple")) +
  guides(colour = F, fill = F) + 
  theme(legend.position = c(0.87, 0.91),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) + 
  xlab("Average weeks under restriction in worst year") + ylab("Probability") 

png(paste(figurepath, "Figure5BothROF", ".png", sep = ""), units = "in", width = 15, height = 10, res = 400)
multiplot(TTDdensplot, RestDdensplot, TTRdensplot, RestRdensplot, cols = 2)
dev.off()

## timeseries of variable data -------------------------------------------------------------

x = c(12:23)
x = c(12:15) + 45 - 1
x = c(16:19) + 45 - 1
x = c(20:23) + 45 - 1
  # ROF triggers

RDMnum = 0
FORMULATION = 1
REALIZATION = 10
datafolder = "figure2data\\1\\"
# either pick for the spot or option contract 
firstrank = x[1]
Years = seq((2015 + 20 - 6 + 1), (2015 + 23 - 6 - 1), 1)

for (RANK in x)
{
  ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
  ObjData  = read.csv(paste(datafolder, "simulationOutput", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = F)
  
  AllocFrac = ObjData[,59]
  Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),c("Year", "Week", 
                                                               "Rstor", "Dstor")]
  Sub0 = Sub0[which(Sub0$Year < 23),]
  Sub0 = Sub0[which(Sub0$Year > 20),]
  Sub0 = Sub0[with(Sub0, order(Year, Week)),]
  Sub0$TimeseriesWeek = c(1:nrow(Sub0))
  # collect and subset storage level data
  
  if (RANK == firstrank)
  {
    Sub0 = Sub0[,c("TimeseriesWeek","Rstor", "Dstor")]
  }
  else
  {
    Sub0 = Sub0[,c("TimeseriesWeek","Rstor", "Dstor")]
  }
  
  AllocFrac = rep(AllocFrac, nrow(Sub0))
  
  alld = data.frame(ReleaseAllocationFraction = AllocFrac, Sub0)
  allmelt = melt(alld, id = c("TimeseriesWeek", "ReleaseAllocationFraction"))
  
  if (RANK == firstrank)
  {
    Data = allmelt
  }
  else
  {
    Data = rbind(Data, allmelt)
  }
}

Data$variable2 = factor(Data$variable, labels = c("Raleigh Storage", "Durham Storage"))

Storages  = Data[which(Data$variable2 != "Raleigh ROF Guide Curve" & Data$variable2 != "Durham ROF Guide Curve"),]

RStorages = Storages[which(Storages$variable == "Rstor"),]
DStorages = Storages[which(Storages$variable == "Dstor"),]

RStorages$colorcol = as.character(RStorages$ReleaseAllocationFraction)
Rdata = RStorages

fig4allocfraclevelsR = ggplot() + 
  geom_line(data = Rdata, aes(x = TimeseriesWeek, y = value, colour = colorcol), size = 0.5) +
  scale_colour_manual(name = "Downstream ROF trigger",
                      labels = c("4%",
                                 "6%",
                                 "8%",
                                 "10%"),
                      values = c("blue",
                                 "red",
                                 "green",
                                 "orange")) +
  guides(colour = F) +
  ylab("Storage Fraction") + theme(legend.position = "none", axis.title.x = element_blank(),
                                   axis.text.x = element_blank(), axis.ticks.x = element_blank(),
                                   axis.title = element_text(size = 15, face = "bold"),
                                   axis.text  = element_text(size = 12, face = "bold")) +
  geom_text(data = data.frame(), aes(100, 0.125), label = "Raleigh", size = 7) + ylim(c(0,1)) +
  geom_text(data = data.frame(), aes(1, 0.9), label = "(d)", size = 12)

DStorages$colorcol = as.character(DStorages$ReleaseAllocationFraction)
Ddata = DStorages

fig4allocfraclevelsD = ggplot() + 
  geom_line(data = Ddata, aes(x = TimeseriesWeek, y = value, colour = colorcol), size = 0.5) + 
  ylab("Storage Fraction") +
  scale_colour_manual(name = "Upstream ROF trigger",
                      labels = c("4%",
                                 "6%",
                                 "8%",
                                 "10%"),
                      values = c("blue",
                                 "red",
                                 "green",
                                 "orange")) +
  #  scale_colour_discrete(name = "% of releases allocated for water supply",
  #                        breaks = c("0.423", "0.6", "0.8", "1", "Durham ROF Guide Curve"),
  #                        labels = c("42.3", "60", "80", "100", "2% ROF Level")) +
  theme(legend.position = c(0.18, 0.15), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.x = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 15, face = "bold"),
        legend.spacing = unit(1, "in")) + 
  geom_text(data = data.frame(), aes(100, 0.125), label = "Durham", size = 7) + ylim(c(0,1)) +
  guides(colour = guide_legend(title.position = "top", direction = "horizontal", label.position = "bottom", ncol = 4,
                               keywidth = 0.52, keyheight = 0.1, default.unit = "inch")) +
  scale_x_continuous(breaks = seq(1,52*nrow(Ddata)/5/52, by = 52), 
                     labels = Years)

png(paste(figurepath, "Figure4dBothROF", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
multiplot(fig4allocfraclevelsR, fig4allocfraclevelsD, cols = 1)
dev.off()

## Timeseries of capacity changes over a timeseries ----------------------------------------------------

RDMnum = 0
FORMULATION = 0
REALIZATION = 27
RANK = 0
datafolder = "infrastructuredata\\"

ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
InfData  = read.csv(paste(datafolder, "InfraBuilt", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)

Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),]
Inf0 = InfData[which(InfData$Realization == REALIZATION),]

StorageCapacitiesD = c(6349)
  # JL allocation not counted toward storage
TriggerYearsD = c(1) * 52 - 51
MultYearsD    = c(rep(StorageCapacitiesD[1],2392))

StorageCapacitiesR = c(14700, 18800, 18800, 18800 + 3700)
  # NRI doesn't up firm capacity but allows 16+ MGD of withdrawal 
TriggerYearsR = c(1, 25-6, 37-6, 50-6) * 52 - 51
MultYearsR    = c(rep(StorageCapacitiesR[1],TriggerYearsR[2]),
                  rep(StorageCapacitiesR[2],TriggerYearsR[3]-TriggerYearsR[2]),
                  rep(StorageCapacitiesR[3],TriggerYearsR[4]-TriggerYearsR[3]),
                  rep(StorageCapacitiesR[4],2392-TriggerYearsR[4]))

TotCap1R = Sub0$Rstor * MultYearsR
TotCap1D = Sub0$Dstor * MultYearsD

FORMULATION = 2

ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
InfData  = read.csv(paste(datafolder, "InfraBuilt", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)

Sub0 = ALLTdata[which(ALLTdata$Realization == REALIZATION),]
Inf0 = InfData[which(InfData$Realization == REALIZATION),]

StorageCapacitiesR = c(14700, 14700 + 2500*0.7, 14700 + 2500*0.7 + 4100, 
                       14700 + 2500*0.7 + 4100 + 5200*0.7, 14700 + 2500*0.7 + 4100 + 5200*0.7)
# NRI doesn't up firm capacity but allows 16+ MGD of withdrawal 
TriggerYearsR = c(1, 16-6, 28-6, 43-6, 51-6) * 52 - 51
MultYearsR    = c(rep(StorageCapacitiesR[1],TriggerYearsR[2]),
                  rep(StorageCapacitiesR[2],TriggerYearsR[3]-TriggerYearsR[2]),
                  rep(StorageCapacitiesR[3],TriggerYearsR[4]-TriggerYearsR[3]),
                  rep(StorageCapacitiesR[4],TriggerYearsR[5]-TriggerYearsR[4]),
                  rep(StorageCapacitiesR[5],2392-TriggerYearsR[5]))

TotCap2R = Sub0$Rstor * MultYearsR

StorageCapacitiesD = c(6349, 6349 + 2500*0.3, 6349 + 2500*0.3, 
                       6349 + 2500*0.3 + 5200*0.3)
# NRI doesn't up firm capacity but allows 16+ MGD of withdrawal 
TriggerYearsD = c(1, 16-6, 17-6, 43-6) * 52 - 51
MultYearsD    = c(rep(StorageCapacitiesD[1],TriggerYearsD[2]),
                  rep(StorageCapacitiesD[2],TriggerYearsD[3]-TriggerYearsD[2]),
                  rep(StorageCapacitiesD[3],TriggerYearsD[4]-TriggerYearsD[3]),
                  rep(StorageCapacitiesD[4],2392-TriggerYearsD[4]))

TotCap2D = Sub0$Dstor * MultYearsD

DR = melt(data.frame(RCap = TotCap1R, DCap = TotCap2R, Time = 1:2392), id = "Time")
DD = melt(data.frame(RCap = TotCap1D, DCap = TotCap2D, Time = 1:2392), id = "Time")

A = ggplot(DD) + geom_line(aes(x = Time, y = value, col = variable), size = 0.8) +
  xlab("") + ylab("Storage (MG)") + 
  theme(axis.title.x = element_blank(), axis.text.x = element_blank(), 
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(vjust = 1)) +
  scale_x_continuous(breaks = seq(1,2393, by = 52*5)) +
  scale_color_manual(values=c("blue", "green4")) +
  guides(col = F) +
  geom_text(data = data.frame(), aes(2050, 2500), label = "Upstream", size = 8)

B = ggplot(DR) + geom_line(aes(x = Time, y = value, col = variable), size = 0.8) +
  xlab("") + ylab("Storage (MG)") + 
  theme(axis.title.x = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold")) +
  scale_x_continuous(breaks = seq(1,2393, by = 52*5), labels = seq(2015,2060,5)) +
  scale_color_manual(values = c("blue", "green4"), 
                     labels = c("Business-as-\nusual", "Joint upstream\ndevelopment")) +
  guides(col = guide_legend(title.position = "top", direction = "vertical", label.position = "right", 
                            ncol = 1, keywidth = 0.52, keyheight = 0.5, default.unit = "inch",
                            title = "Infrastructure Scenario")) +
  theme(legend.position = c(0.20, 0.28), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(size = 13, face = "bold"),
        legend.title = element_text(size = 15, face = "bold")) +
  geom_text(data = data.frame(), aes(2050, 5000), label = "Downstream", size = 8)

library(gridExtra)
gA <- ggplotGrob(A)
gB <- ggplotGrob(B)

png(paste(figurepath, "MethodsInfrastructurePathsR", ".png", sep = ""), units = "in", width = 10, height = 6.5, res = 400)
grid::grid.newpage()
grid::grid.draw(rbind(gA, gB))
dev.off()

## -------------------------------------------------------------------------------------------
## quick TT vs RWT capacity comparison bar plot ----------------------------------------------

D = data.frame(Transfers  = c("Treated Water", "Raw Water"),
               Capacities = c(10.8 * 7, 500))
RaleighDemand = 60 * 7
  # MGW

D$Capacities = D$Capacities/RaleighDemand * 100

ggplot(D) + geom_bar(aes(x = Transfers, y = Capacities, fill = Transfers), stat = "identity") +
  ggtitle("Transfer Size") + ylab("Capacity (as % of Raleigh weekly demand)") + xlab("") + 
  guides(fill = F) + ylim(c(0,120)) + geom_hline(yintercept = c(100), colour = c("black")) +

ggsave(paste(figurepath, "TTvsRWTcomp", ".png", sep = ""), 
       units = "in", width = 3, height = 6)

## single parallel plot with animation --------------------------------------------------------

library(ggplot2)

maxconst = c(5, 50, 4, 0.25, 0.1)
minconst = c(0, 10, 1, 0.10, 0.0)
objnames = c("Failures", "Restrictions", "Peak Debt", "Losses", "Treated Transfers")
useroverride = F
# user-selected objective bounds

D = read.csv(paste(figurepath,"ParallelPlotDatabyFormulationFULLSET.csv",sep = ""), header = T)
D = D[,c(1:5,7)]
sample1 = nrow(D)
# read data, select just columns i care about
# first column is for color sorting, all other m columns of n objective sets are objectives

D1 = D
  # all 

D2 = D[c(c(91:135),c(91:135)+135,c(91:135)+270),]
sample2 = nrow(D2)
  # low-dev

D3 = D[which(D[,2] <= 1.5),]
sample3 = nrow(D3)
  # high reliability

D4 = D[c(104, 104+135, 104+270),]
sample4 = nrow(D4)
# take subsets

D = rbind(D, D2, D3, D4)
# put all data together

colnames(D) = c("group", paste("Obj ", 1:(ncol(D)-1), sep = ""))
# rename columns to generic values to be overwritten later

nobj = length(names(D))-1
# number of objectives 

DataMaxBound = apply(D[,2:ncol(D)], MARGIN = 2, FUN = max)
DataMinBound = apply(D[,2:ncol(D)], MARGIN = 2, FUN = min)
# identify data bounds on objectives

UserMaxBound = maxconst
UserMinBound = minconst
# user-selected data bounds for plotting

MaxBounds = c()
MinBounds = c()

if (useroverride)
{
  for (i in 1:length(DataMaxBound))
  {
    if (DataMaxBound[i] > UserMaxBound[i])
    {
      MaxBounds[i] = DataMaxBound[i]
    }
    else
    {
      MaxBounds[i] = UserMaxBound[i]
    }
    
    if (DataMinBound[i] < UserMinBound[i])
    {
      MinBounds[i] = DataMinBound[i]
    }
    else
    {
      MinBounds[i] = UserMinBound[i]
    }
  }
  # override user if you want entire range
  
} else
{
  MaxBounds = UserMaxBound
  MinBounds = UserMinBound
}

Dnorm = D$group
for (col in 1:(ncol(D)-1))
{
  Dnorm = cbind(Dnorm, (D[,col+1] - MinBounds[col]) / (MaxBounds[col] - MinBounds[col]))
}
# normalize the objectives to a 0-1 range each

Dnorm = as.data.frame(Dnorm)
colnames(Dnorm) = c("group", paste("Obj ", 1:(ncol(D)-1), sep = ""))
Dnorm$FacetSet  = c(rep("A", sample1), rep("B", sample2), 
                    rep("C", sample3), rep("D", sample4))
Dnorm$group = as.character(Dnorm$group)
Dnorm$Sim = 1:nrow(Dnorm)
# set labels again and determine which facet each set is plotted on

maxChars = paste(c("", "", "", "", ""), MaxBounds, sep = "")
maxChars = paste(maxChars, c("%", "%", "00% AVR", "", " MGD"), sep = "")

minChars = paste(c("", "", "", "", ""), MinBounds, sep = "")
minChars = paste(minChars, c("%", "%", "00% AVR", "", " MGD"), sep = "")

colnames(Dnorm)[2:(nobj+1)] = minChars
#colnames(Dnorm)[2:(nobj+1)] = maxChars
# adjust the labels depending on x-axis label position

DNmelt = melt(Dnorm, id = c("FacetSet", "group", "Sim"))
DNmelt = DNmelt[with(DNmelt, order(FacetSet, group, Sim)),]
DNmelt = DNmelt[DNmelt$FacetSet == "B" | DNmelt$FacetSet == "A",]
#DNmelt = DNmelt[DNmelt$FacetSet == "A",]

DNmelt$group[DNmelt$FacetSet == "A"] = 3
#DNmelt = DNmelt[DNmelt$group != "1" & DNmelt$group != "2",]
#DNmelt = DNmelt[DNmelt$group != "2",]
  # locate background set to be colored grey

A = ggplot(DNmelt) + geom_line(aes(x = variable, y = value, group = Sim, color = group), 
                               size = 1.5, alpha = 1) + guides(col = T) + ylim(c(0,1)) +
  theme(axis.title   = element_blank(),
        axis.text.x  = element_text(size = 15, face = "bold"),
        axis.text.y  = element_blank(),
        axis.ticks.y = element_blank()) +
  scale_y_continuous(breaks = c(0,1), expand = c(0, 0)) +
  scale_x_discrete(expand = c(0.03, 0.03), position = "bottom") +
  guides(col = guide_legend(title.position = "top", direction = "horizontal", label.position = "right", 
                            ncol = 1, keywidth = 0.3, keyheight = 0.15, default.unit = "inch",
                            title = "Formulation")) + 
  guides(col = F) +
  theme(legend.position = c(0.736,0.83), 
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text  = element_text(size = 9, face = "bold"),
        legend.title = element_text(size = 10, face = "bold"),
        panel.spacing = unit(0,"lines"),
        strip.text  = element_text(size = 12, colour = "black", 
                                   angle = 0, face = "bold"),
        panel.grid.major.y = element_line(colour = "black"),
        panel.grid.major.x = element_line(colour = "black"),
        panel.background   = element_blank(),
        plot.subtitle = element_text(size = 12, face = "bold"),
        plot.margin = unit(c(0.3,0.2,0.5,0.2), unit ="in"),
        axis.line.x = element_line(colour = "black")) +
  scale_color_manual(values = c("blue","red","green4","grey70"),
                   breaks = c("0", "1", "2", "3"),
                   labels = c("Business-as-usual",
                              "Raw water transfers added",
                              "Joint upstream development added",
                              "All simulations")) +
  #scale_color_manual(values = c("blue","red","green4"),
  #                   breaks = c("0", "1", "2"),
  #                   labels = c("Business-as-usual",
  #                              "Raw water transfers added",
  #                              "Joint upstream development added")) +
  #scale_color_manual(values = c("blue","grey70"), 
  #                   breaks = c("0","3"),
  #                   labels = c("Business-as-usual", 
  #                              "All simulations")) +
  #scale_color_manual(values = c("blue","red","grey70"), 
  #                   breaks = c("0", "1", "3"),
  #                   labels = c("Business-as-usual", 
  #                              "Raw water transfers added",
  #                              "All simulations")) +
  #scale_color_manual(values = c("blue","green4","grey70"),
  #                   breaks = c("0", "2", "3"),
  #                   labels = c("Business-as-usual",
  #                              "Joint upstream development added",
  #                              "All simulations")) +
  geom_hline(yintercept = 0)

ggsave(paste(figurepath,"MediumSubsetNOLEGEND.png",sep = ""), 
       width = 7, height = 5, units = "in")


