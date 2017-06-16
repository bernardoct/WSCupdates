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

## ---------- manuscript RWT figure -------------------------------------------------------------

RANK = 0
REALIZATION = 0
FORMULATION = 0

datafolder  = "figure1data\\zero\\"
ALLTdata = read.csv(paste(datafolder, "ALLtransferData", RANK, "_", FORMULATION , "_", RDMnum, ".csv", sep = ""), header = T)
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

rm(datatomelt, meltD, meltR, Sub0, Sub1, Suby0, Suby1, ALLTdata, FORMULATION, REALIZATION, Years)

# make the plot --------------------------------------------------------------------

linesize = 1.3
plotR = ggplot() +
  geom_line(data = meltRsub, aes(x = TimeseriesWeek, y = value, 
                              colour = variable), size = linesize) +
  geom_line(data = meltRtar, aes(x = TimeseriesWeek, y = value, 
                                 linetype = variable), size = linesize) +
  scale_colour_manual(name = "Storage",
                      values = c("red", "blue"),
                      labels = c("With RWTs", "Without")) +
  guides(colour   = guide_legend(ncol = 1), default.unit = "inch",
         linetype = F) +
  scale_linetype_manual(name = element_blank(),
                        values = c("dotted"),
                        labels = c("Target ROF")) +
  theme(legend.position = c(0.95, 0.05), legend.box = "horizontal",
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.title = element_text(face = "bold", size = 14),
        legend.justification = c(1,0),
        legend.key = element_rect(fill = "grey90", colour = "grey90"),
        legend.key.width = unit(0.5,"in"), legend.key.height = unit(0.2,"in"),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.y = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold"),
        axis.ticks = element_blank(),
        panel.grid.major.y = element_blank(),
        panel.grid.major.x = element_blank(),
        panel.background   = element_blank(),
        plot.margin = unit(c(0.3,0.3,0.3,0.3), unit ="in"),
        axis.line.x = element_blank()) +
  theme(axis.title.x = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        axis.ticks = element_blank(),
        legend.position = c(0.97,0.05), legend.justification = c(1,0),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(size = 13, face = "bold"),
        legend.title = element_text(size = 15, face = "bold"),
        legend.key = element_rect(fill = "grey90", colour = "grey90"),
        axis.line = element_blank(),
        panel.grid.major = element_blank(),
        panel.border = element_blank()) +
  scale_y_continuous(limits = c(0,1), expand = c(0.01, 0.01), labels = c("Empty", "Full"), breaks = c(0,1)) +
  ylab("Storage") + 
  scale_x_continuous(breaks = seq(7,98,by = 104/8), 
                     labels = rep(c("Winter","Spring","Summer","Fall"),2), expand = c(0.01,0.01)) +
  geom_vline(xintercept = c(1,52), colour = c("black","grey60"), size = c(1,0.5)) +
  geom_hline(yintercept = c(0),  colour = c("black"), size = c(1))

plotD = ggplot() +
  geom_line(data = meltDsub, aes(x = TimeseriesWeek, y = value, 
                                 colour = variable), size = linesize) +
  geom_line(data = meltDtar, aes(x = TimeseriesWeek, y = value, 
            linetype = variable), size = linesize) +
  scale_colour_manual(name = "Storage",
                      values = c("red", "blue"),
                      labels = c("With RWTs", "Without")) +
  scale_linetype_manual(name = element_blank(),
                        values = c("dotted"),
                        labels = c("Target ROF")) +
  guides(linetype = guide_legend(ncol = 1), default.unit = "inch",
         colour = F) +
  theme(legend.position = c(0.95, 0.05), legend.box = "horizontal",
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(face = "bold", size = 12),
        legend.title = element_blank(),
        legend.justification = c(1,0),
        legend.key = element_rect(fill = "grey90", colour = "grey90"),
        legend.key.width = unit(0.5,"in"), legend.key.height = unit(0.2,"in"),
        axis.text = element_text(size = 12, face = "bold"),
        axis.title.x = element_blank(),
        axis.text.y = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(size = 15, face = "bold"),
        axis.ticks = element_blank(),
        panel.grid.major = element_blank(),
        panel.background   = element_blank(),
        plot.margin = unit(c(0.3,0.3,0.3,0.3), unit ="in"),
        axis.line.x = element_blank()) +
  theme(axis.title.x = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        axis.ticks = element_blank(),
        legend.position = c(0.97,0.05), legend.justification = c(1,0),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(size = 13, face = "bold"),
        legend.title = element_text(size = 15, face = "bold"),
        legend.key = element_rect(fill = "grey90", colour = "grey90"),
        axis.line = element_blank(),
        panel.grid.major = element_blank(),
        panel.border = element_blank()) +
  scale_y_continuous(limits = c(0,1), expand = c(0.01, 0.01), 
                     labels = c("Empty", "Full"), breaks = c(0,1)) +
  ylab("Storage") + 
  scale_x_continuous(breaks = seq(7,98,by = 104/8), 
                     labels = rep(c("Winter","Spring","Summer","Fall"),2), expand = c(0.01,0.01)) +
  geom_vline(xintercept = c(1,52), colour = c("black","grey60"), size = c(1,0.5)) +
  geom_hline(yintercept = c(0),  colour = c("black"), size = c(1))
  
gA <- ggplotGrob(plotD)
gB <- ggplotGrob(plotR)

png(paste(figurepath, "RWTfigure1", ".png", sep = ""), units = "in", width = 8, height = 6, res = 400)
grid::grid.newpage()
grid::grid.draw(rbind(gA, gB))
dev.off()

## manuscript figure 2 (infrastructure) ---------------------------------------------------------------

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

A = ggplot(DD) + theme_bw() +
  geom_line(aes(x = Time, y = value, col = variable), size = 0.8) +
  xlab("") + ylab("Storage (MG)") + 
  theme(axis.title.x = element_blank(), axis.text.x = element_blank(), 
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        axis.title.y = element_text(vjust = 1),
        axis.ticks = element_blank(),
        panel.grid.minor.x = element_blank(),
        panel.grid.major = element_line(colour = "grey90", size = 0.6),
        plot.margin = unit(c(0.3,0.3,0.3,0.3), unit ="in"),
        legend.key.width = unit(0.5,"in"), legend.key.height = unit(0.2,"in"),
        axis.line = element_blank(),
        panel.border = element_blank()) +
  scale_x_continuous(breaks = seq(1,2393, by = 52*5),
                     expand = c(0,0)) +
  scale_y_continuous(breaks = c(0,3000,6000,9000), 
                     labels = c(0,3000,6000,9000),
                     limits = c(0,9000),
                     expand = c(0.03,0.0)) +
  scale_color_manual(values=c("blue", "green4")) +
  guides(col = F) +
  geom_vline(xintercept = 1, colour = "black", size = 1) +
  geom_hline(yintercept = 0, colour = "black", size = 1)

B = ggplot(DR) + theme_bw() +
  geom_line(aes(x = Time, y = value, col = variable), size = 0.8) +
  xlab("") + ylab("Storage (MG)") + 
  theme(axis.title.x = element_blank(),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        axis.ticks = element_blank(),
        panel.grid.minor.x = element_blank(),
        panel.grid.major = element_line(colour = "grey90", size = 0.6),
        legend.position = c(0.97,0.05), legend.justification = c(1,0),
        plot.margin = unit(c(0.3,0.3,0.3,0.3), unit ="in"),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text = element_text(size = 13, face = "bold"),
        legend.title = element_text(size = 15, face = "bold"),
        legend.key = element_rect(fill = "grey90", colour = "grey90"),
        legend.key.width = unit(0.5,"in"), legend.key.height = unit(0.2,"in"),
        axis.line = element_blank(),
        panel.border = element_blank()) +
  scale_x_continuous(breaks = seq(1,2393, by = 52*5), 
                     labels = seq(2015,2060,5),
                     expand = c(0,0)) +
  scale_y_continuous(breaks = c(0,5000,10000,15000,20000,25000), 
                     labels = c(0,5000,10000,15000,20000,25000),
                     limits = c(0,25000),
                     expand = c(0.03,0.0)) +
  scale_color_manual(values = c("blue", "green4"), 
                     labels = c("Business-as-usual      ", "Joint upstream development   ")) +
  guides(col = guide_legend(title.position = "top", direction = "vertical", label.position = "right", 
                            ncol = 1, default.unit = "inch",
                            title = "Infrastructure Formulation")) +
  geom_vline(xintercept = 1, colour = "black", size = 1) +
  geom_hline(yintercept = 0, colour = "black", size = 1)

library(gridExtra)
gA <- ggplotGrob(A)
gB <- ggplotGrob(B)

png(paste(figurepath, "INFRAfigure2", ".png", sep = ""), units = "in", width = 8, height = 6, res = 400)
grid::grid.newpage()
grid::grid.draw(rbind(gA, gB))
dev.off()

## parallel plot facet figure ---------------------------------------------------------------------------

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
sampleX = nrow(D1)
# all 

D2 = D[c(c(91:135),c(91:135)+135,c(91:135)+270),]
sample2 = nrow(D2)
# low-dev

D3 = D[which(D[,2] <= 2),]
sample3 = nrow(D3)
# high reliability

D4 = D[c(104, 104+135, 104+270),]
sample4 = nrow(D4)
# take subsets

D = rbind(D1, D2,D1, D3,D1, D4,D1)
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
Dnorm$FacetSet  = c(rep("A", sample1), 
                    rep("B", sample2), rep("BX", sample1),
                    rep("C", sample3), rep("CX", sample1),
                    rep("D", sample4), rep("DX", sample1))
Dnorm$group = as.character(Dnorm$group)
Dnorm$Sim = 1:nrow(Dnorm)
# set labels again and determine which facet each set is plotted on

maxChars = paste(c("", "", "", "", ""), MaxBounds, sep = "")
maxChars = paste(maxChars, c("%", "%", "00% AVR", "", " MGD"), sep = "")

minChars = paste(c("", "", "", "", ""), MinBounds, sep = "")
minChars = paste(minChars, c("%", "%", "00% AVR", "", " MGD"), sep = "")

colnames(Dnorm)[2:(nobj+1)] = minChars
colnames(Dnorm)[2:(nobj+1)] = maxChars
# adjust the labels depending on x-axis label position

DNmelt = melt(Dnorm, id = c("FacetSet", "group", "Sim"))
DNmelt = DNmelt[which(DNmelt$FacetSet != "B" & DNmelt$FacetSet != "BX"),]

for (code in c("AX", "BX", "CX", "DX")) {DNmelt$group[DNmelt$FacetSet == code] = 3}
for (code in c("AX", "BX", "CX", "DX")) {DNmelt$FacetSet[DNmelt$FacetSet == code] = substr(code,1,1)}

#DNmelt$groupnew = factor(as.numeric(DNmelt$group)*-1, levels = c(0,-1,-2,-3))
DNmelt = DNmelt[with(DNmelt, order(FacetSet, group, Sim)),]

DNmeltbase = DNmelt[which(DNmelt$group == 3),]
DNmeltsets = DNmelt[which(DNmelt$group != 3),]

DNmeltsets$groupnew = as.numeric(DNmeltsets$group)*-1
DNmeltsets = DNmeltsets[with(DNmeltsets, order(FacetSet, groupnew, Sim)),]

DNmeltsets$FacetSet[which(DNmeltsets$FacetSet == "C" | DNmeltsets$FacetSet == "CX")] = "B"
DNmeltsets$FacetSet[which(DNmeltsets$FacetSet == "D" | DNmeltsets$FacetSet == "DX")] = "C"
DNmeltbase$FacetSet[which(DNmeltbase$FacetSet == "C" | DNmeltbase$FacetSet == "CX")] = "B"
DNmeltbase$FacetSet[which(DNmeltbase$FacetSet == "D" | DNmeltbase$FacetSet == "DX")] = "C"
  # remove one of the 4 facets and re-letter the facets alphabetically

#DNmeltplot = DNmelt[DNmelt$FacetSet == "A" | DNmelt$FacetSet == "X",]
#DNmeltplot = DNmelt[DNmelt$FacetSet == "A",]
#DNmelt = DNmelt[DNmelt$group != "1" & DNmelt$group != "2",]
#DNmelt = DNmelt[DNmelt$group != "2",]

A = ggplot() + 
  geom_line(data = DNmeltbase, aes(x = variable, y = value, group = Sim), 
            size = 1.2, alpha = 1, color = "grey70") + 
  geom_line(data = DNmeltsets, aes(x = variable, y = value, group = Sim, color = group), 
            size = 1.5, alpha = 1) + 
  facet_grid(FacetSet ~ .) +
  theme(axis.title   = element_blank(),
        axis.text.x  = element_text(size = 15, face = "bold"),
        axis.text.y  = element_blank(),
        axis.ticks.y = element_blank()) +
  scale_y_continuous(breaks = c(0,1), expand = c(0,0), limits = c(0,1)) +
  scale_x_discrete(expand = c(0.0, 0.0), position = "top") +
  guides(col = guide_legend(title.position = "top", direction = "horizontal", label.position = "right", 
                            ncol = 1, keywidth = 0.3, keyheight = 0.15, default.unit = "inch",
                            title = "Formulation")) + 
  theme(legend.position = c(0.985,0.990), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        legend.text  = element_text(size = 9, face = "bold"),
        legend.title = element_text(size = 10, face = "bold"),
        legend.key = element_rect(fill = "grey90", colour = "grey90"),
        panel.spacing = unit(0,"lines"), 
        strip.background = element_rect(fill = "grey90", colour = "grey90", size = 0.5),
        strip.text  = element_text(size = 12, colour = "black", angle = 0, face = "bold"),
        panel.grid.major.y = element_line(colour = "black"),
        panel.grid.major.x = element_line(colour = "grey20", size = 0.6),
        panel.background   = element_blank(),
        plot.subtitle = element_text(size = 12, face = "bold"),
        plot.margin = unit(c(0.3,0.3,0.3,0.3), unit ="in"),
        axis.line.x = element_line(colour = "black")) +
  scale_color_manual(values = c("blue","red","green4"),
                     breaks = c("0", "1", "2"),
                     labels = c("Business-as-usual",
                                "Raw water transfers added",
                                "Joint upstream development added")) +
  geom_hline(yintercept = c(0,1), color = "black", size = 1)

ggsave(paste(figurepath,"PARALLELfigure3toplabel2perc.png",sep = ""), 
       width = 6.5, height = 8, units = "in")

## subset realization distributions -------------------------------------------------------------------

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

trans = 0.45

TTRdensplot = ggplot(alldata1) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(name = "Formulation",
                    labels = c("Business-as-usual",
                               "Raw water transfers added",
                               "Joint upstream development added"),
                    values = c("blue",
                               "red",
                               "green4")) + 
  guides(colour = F, fill = guide_legend(ncol = 1)) + 
  xlab("Treated transfers in worst year (MGY)") +
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(), 
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.3, "in"), legend.key = element_rect(colour = "grey90"),
        legend.title = element_text(size = 18, face = "bold"),
        legend.spacing = unit(1, "in"),
        plot.background = element_rect(fill = "white"),
        panel.border = element_blank(), panel.grid.major = element_line(colour = "grey70"),
        panel.grid.minor = element_blank()) + 
  geom_vline(xintercept = 0, colour = "black", size = 1) +
  geom_hline(yintercept = 0, colour = "black", size = 1) +
  scale_y_continuous(expand = c(0,0), limits = c(0,0.15), breaks = c(0, 0.05, 0.1, 0.15)) +
  scale_x_continuous(expand = c(0,0), limits = c(0,100))

TTDdensplot = ggplot(alldata2) + 
  geom_density(aes(value, fill = variable), alpha = trans, colour = NA) +
  scale_fill_manual(name = "Formulation",
                    labels = c("0 - Business-as-usual",
                               "1 - Raw water transfers added",
                               "2 - Joint upstream development added"),
                    values = c("blue",
                               "red",
                               "green4")) +
  guides(colour = F, fill = F) +  
  theme(legend.position = c(0.87, 0.91), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(), 
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 18, face = "bold"),
        legend.spacing = unit(1, "in"),
        plot.background = element_rect(fill = "white"),
        panel.border = element_blank(), panel.grid.major = element_line(colour = "grey70"),
        panel.grid.minor = element_blank()) + 
  geom_vline(xintercept = 4000, colour = "black", size = 1) +
  geom_hline(yintercept = 0, colour = "black", size = 1) +
  xlab("Treated transfers in worst year (MGY)") + ylab("Probability") +
  scale_y_continuous(expand = c(0,0), limits = c(0,0.002)) +
  scale_x_continuous(expand = c(0,0), limits = c(4000,7300))

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
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(), 
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 18, face = "bold"),
        legend.spacing = unit(1, "in"),
        plot.background = element_rect(fill = "white"),
        panel.border = element_blank(), panel.grid.major = element_line(colour = "grey70"),
        panel.grid.minor = element_blank()) + 
  geom_vline(xintercept = 0, colour = "black", size = 1) +
  geom_hline(yintercept = 0, colour = "black", size = 1) +
  xlab("Weeks under restriction in worst year") +
  scale_y_continuous(expand = c(0,0), limits = c(0,0.06)) +
  scale_x_continuous(expand = c(0,0.0), limits = c(0,52))

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
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title.y = element_blank(), 
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        legend.text = element_text(size = 14, face = "bold"),
        legend.key.size = unit(0.5, "in"),
        legend.title = element_text(size = 18, face = "bold"),
        legend.spacing = unit(1, "in"),
        plot.background = element_rect(fill = "white"),
        panel.border = element_blank(), panel.grid.major = element_line(colour = "grey70"),
        panel.grid.minor = element_blank()) + 
  geom_vline(xintercept = 0, colour = "black", size = 1) +
  geom_hline(yintercept = 0, colour = "black", size = 1) +
  xlab("Weeks under restriction in worst year") + ylab("Probability") +
  scale_y_continuous(expand = c(0,0), limits = c(0,0.06)) +
  scale_x_continuous(expand = c(0.0,0), limits = c(0,52))

png(paste(figurepath, "DENSITYfigure4", ".png", sep = ""), units = "in", width = 15, height = 10, res = 400)
multiplot(TTDdensplot, RestDdensplot, TTRdensplot, RestRdensplot, cols = 2)
dev.off()

## ------ IBT figure -----------------------------------------------------------------------------------

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
  guides(colour = F, fill = guide_legend(ncol = 1)) + guides(fill = F) +
  theme(legend.position = c(0.95, 0.95), legend.justification = c(1,1),
        legend.background = element_rect(fill = "grey90", colour = "black"),
        axis.title = element_text(size = 15, face = "bold"),
        axis.text  = element_text(size = 12, face = "bold"),
        plot.title = element_text(size = 20, face = "bold"),
        axis.ticks = element_blank(),
        legend.text = element_text(size = 10, face = "bold"),
        legend.key.size = unit(0.25, "in"), axis.line = element_blank(),
        legend.title = element_text(size = 14, face = "bold"),
        legend.spacing = unit(1, "in"), legend.key = element_rect(colour = "grey90"),
        plot.background = element_rect(fill = "white"),
        panel.border = element_blank(), panel.grid.major = element_line(colour = "grey70"),
        panel.grid.minor = element_blank()) +
  xlab("Treated transfers in worst week (MGW)") + ylab("Probability") +
  scale_x_continuous(breaks = c(0, 10, 14, 21, 30, 40), expand = c(0.02,0), limits = c(0,40)) +
  scale_y_continuous(breaks = c(0, 0.1, 0.2, 0.3, 0.4), expand = c(0.02,0), limits = c(0,0.4)) +
  geom_vline(xintercept = c(0,14,21), colour = c("black", "black", "red"), size = c(2,2,2)) +
  geom_hline(yintercept = c(0), colour = c("black"), size = c(2)) +
  geom_text(data = data.frame(), label = "Average IBT Limit", aes(13,0.14), 
            angle = 90, size = 6) + 
  geom_text(data = data.frame(), label = "Maximum IBT Limit", aes(20,0.14), 
            color = "red", angle = 90, size = 6)

ggsave(paste(figurepath, "IBTfigure5NOLEGEND", ".png", sep = ""), 
       units = "in", width = 6.5, height = 6.5)

















