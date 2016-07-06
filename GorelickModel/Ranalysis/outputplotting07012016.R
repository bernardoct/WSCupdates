## Output plotting for July 1 update
## New transfers function and observation of Bernardo's model
## --------------------------------------------------------------------------------------

rm(list = ls())
setwd("C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\Coding")

resultpath = paste("GorelickModelOutput\\ModelResults_", Sys.Date(), "\\", sep = "")
dir.create(resultpath)

figurepath = paste("GorelickModelOutput\\Figures_", Sys.Date(), "\\", sep = "")
dir.create(figurepath)

datafolder = "GorelickModel\\output\\"
numranks   = 4
# location of output data and number of ranks run

for (RANK in 0:(numranks-1))
{
  TTdata = read.csv(paste(datafolder, "JLTreatedTransfers", RANK, ".csv", sep = ""), header = T)
  RRdata = read.csv(paste(datafolder, "RRfuncOutput", RANK, ".csv", sep = ""), header = T)
  # these csvs are of weekly data for every realization
  
  ALLRR = merge(TTdata, RRdata,  by = c("Rank", "Realization", "Year", "Week"))
  
  rm(TTdata, RRdata)
  write.csv(ALLRR, paste(resultpath, "WeeklyData", RANK, ".csv", sep = ""))
  # write combined data to location
  rm(ALLRR)
  
  INdata = read.csv(paste(datafolder, "InfraBuilt", RANK, "_0", ".csv", sep = ""), header = T)
  # collect the infrastructure timeline for all realizations
  
  write.csv(INdata, paste(resultpath, "InfrastructureSequencing", RANK, ".csv", sep = ""))
  rm(INdata)
  
  params = read.csv(paste(datafolder, "weeklyRiskParams", RANK, ".csv", sep = ""), header = T)
  # triggers and other set parameters for each realization
  
  write.csv(params, paste(resultpath, "RealizationParameters", RANK, ".csv", sep = ""))
  rm(params)
}
# CONSOLIDATE AND WRITE OUTPUT FILES

# plot single observation timeseries -----------------------------------------------------------------

plotthisbitch = function(realization = 0, rank1 = 0, startyear = 0, numyears = 10, titlestring = "",
                         datapath = resultpath, figurep = figurepath)
{
  Years = seq((2015 + startyear), (2015 + startyear + numyears - 1), 1)
  
  if (numyears > 5)
  {
    seasons = 2
    Blank = rep("", (seasons - 1))
  }
  if (numyears <= 5)
  {
    seasons = 4
    Blank = rep("", (seasons - 1))
  }
  if (numyears >= 20)
  {
    seasons = 1
    Blank = rep("", (seasons - 1))
  }
  if (numyears >= 30)
  {
    seasons = 0.20
    Blank = character(0)
    Years = seq((2015 + startyear), (2015 + startyear + numyears - 1), 1/seasons)
  }
  
  line = NULL
  for (yr in 1:length(Years))
  {
    line = c(line, Years[yr], Blank)
  }
  
  line = c(line, (2015 + startyear + numyears))
  rm(Years, Blank, yr)
  
  ## --------------------- all the plots compare at least one rank/realization ------------
  ## --------------------- read in the data here ------------------------------------------
  
  data1     = read.csv(paste(datapath, "WeeklyData", rank1, ".csv", sep = ""), header = T)
  weekdata1 = data1[which(data1$Realization == realization),]
  rm(data1)
  
  pars1      = read.csv(paste(datapath, "RealizationParameters", rank1, ".csv", sep = ""), header = T)
  paramdata1 = pars1[which(pars1$Realization == realization),]
  rm(pars1)
  
  weekdata1 = weekdata1[with(weekdata1, order(Year, Week)),]
  weekdata1$TimeseriesWeek = c(1:nrow(weekdata1))
  # correctly order the timeseries
  
  inf1 = read.csv(paste(datapath, "InfrastructureSequencing", rank1, ".csv", sep = ""), header = T)
  inf1 = inf1[which(inf1$Realization == realization),]
  inf1 = inf1[with(inf1, order(Year)),]
  
  # ----------------------------------------------------------------------------------------
  # ------------------ make 3 year timeseries plots ----------------------------------------
  
  weeks1 = weekdata1[(startyear*52+1):((startyear+numyears)*52),]
  
  # ------------------ compare releases and transfers --------------------------------------
  
  DataToPlot1 = data.frame(TimeseriesWeek  = weeks1$TimeseriesWeek,
                           RRQ             = weeks1$ReleaseRequest,
                           BBQ             = weeks1$BuybackQuantity,
                           TTRD            = weeks1$RaleighDirectTransferVolume,
                           TTRI            = weeks1$RaleighIndirectTransferVolume,
                           TTDD            = weeks1$DurhamDirectTransferVolume,
                           TTDI            = weeks1$DurhamIndirectTransferVolume,
                           Rstor           = weeks1$RaleighActualStorage,
                           Dstor           = weeks1$DurhamActualStorage)
  
  DataToPlot = DataToPlot1
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
  # organize just the data I want to plot
  # in a form better suited to ggplot
  
  DataMelted$variable2 = factor(DataMelted$variable, labels = c(paste("Releases", sep = ""),
                                                                paste("Buybacks", sep = ""),
                                                                paste("Direct Transfers (Raleigh)", sep = ""),
                                                                "Indirect (Raleigh)",
                                                                paste("Direct Transfers (Durham)", sep = ""),
                                                                "Indirect (Durham)",
                                                                "Storage (Raleigh)",
                                                                "Storage (Durham)"))

  allgraph = ggplot(DataMelted) + 
    geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
    facet_grid(variable2 ~ ., scales = "free_y") +
    xlab("") + 
    ylab("Water Volume (MG)") + 
    theme(legend.position = "none") + 
    ggtitle(paste(titlestring, sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/seasons), 
                       labels = line)
  
  if (numyears < 10)
  {
    ggsave(filename = paste(figurep, realization, "_", rank1, "_", numyears, "years", "_singlecomp_", titlestring, ".png", sep = ""),
           units = "in", width = 7, height = 15)
  }
  else
  {
    ggsave(filename = paste(figurep, realization, "_", rank1, "_", numyears, "years", "_singlecomp_", titlestring, ".png", sep = ""),
           units = "in", width = (numyears), height = 15)
  }
  
  ## PRINT SOME STATISTICS FROM EACH TIMESERIES ------------------------------------------------------
  ## -------------------------------------------------------------------------------------------------
  
  ## Case 1 --------------------
  
  RRweeks1  = length(weekdata1$ReleaseRequest[which(weekdata1$ReleaseRequest > 0)])
  BBweeks1  = length(weekdata1$BuybackQuantity[which(weekdata1$BuybackQuantity > 0)])
  TTRDweeks1 = length(weekdata1$RaleighDirectTransferVolume[which(weekdata1$RaleighDirectTransferVolume > 0)])
  TTDDweeks1 = length(weekdata1$DurhamDirectTransferVolume[which(weekdata1$DurhamDirectTransferVolume > 0)])
  TTRIweeks1 = length(weekdata1$RaleighIndirectTransferVolume[which(weekdata1$RaleighIndirectTransferVolume > 0)])
  TTDIweeks1 = length(weekdata1$DurhamIndirectTransferVolume[which(weekdata1$DurhamIndirectTransferVolume > 0)])
  
  RRsums1  = sum(weekdata1$ReleaseRequest[which(weekdata1$ReleaseRequest > 0)])
  BBsums1  = sum(weekdata1$BuybackQuantity[which(weekdata1$BuybackQuantity > 0)])
  TTRDsums1 = sum(weekdata1$RaleighDirectTransferVolume[which(weekdata1$RaleighDirectTransferVolume > 0)])
  TTDDsums1 = sum(weekdata1$DurhamDirectTransferVolume[which(weekdata1$DurhamDirectTransferVolume > 0)])
  TTRIsums1 = sum(weekdata1$RaleighIndirectTransferVolume[which(weekdata1$RaleighIndirectTransferVolume > 0)])
  TTDIsums1 = sum(weekdata1$DurhamIndirectTransferVolume[which(weekdata1$DurhamIndirectTransferVolume > 0)])
  
  ViolationWeeks = nrow(weekdata1[which(weekdata1$RaleighIndirectTransferVolume > 0.0 && weekdata1$DurhamIndirectTransferVolume > 0.0),])
  
  cat(paste("This is realization ", realization, " of solution (rank) ", rank1, ". \n", sep = ""))
  cat("Basic statistics for this case: \n")
  
  cat(paste(paste("RR weeks", RRweeks1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("BB weeks", BBweeks1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTRD weeks", TTRDweeks1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTDD weeks", TTDDweeks1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTRI weeks", TTRIweeks1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTDI weeks", TTDIweeks1, sep = "\t"), "\n", sep = ""))
  
  cat(paste(paste("RR total", RRsums1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("BB total", BBsums1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTRD total", TTRDsums1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTDD total", TTDDsums1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTRI total", TTRIsums1, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTDI total", TTDIsums1, sep = "\t"), "\n", sep = ""))
  
  cat(paste(paste("Violations", ViolationWeeks, sep = "\t"), "\n", sep = ""))
}

# plot this bitch ------------------------------

for (real in 10:15)
{
  plotthisbitch(realization = real, rank1 = 0, startyear = 0, numyears = 20, titlestring = "With Releases")
  plotthisbitch(realization = real, rank1 = 1, startyear = 0, numyears = 20, titlestring = "Release ROF Trigger = 0.05")
  plotthisbitch(realization = real, rank1 = 2, startyear = 0, numyears = 20, titlestring = "Transfer ROF Trigger = 0.05")
  plotthisbitch(realization = real, rank1 = 3, startyear = 0, numyears = 20, titlestring = "Without Releases")
}

# check what the difference is with where i calculate ROF ------------

plotthisbitch(realization = 45, rank1 = 0, startyear = 0, numyears = 20, titlestring = "New Method",
              datapath = "GorelickModelOutput\\ModelResults_2016-07-02\\")
plotthisbitch(realization = 45, rank1 = 0, startyear = 0, numyears = 20, titlestring = "Old Method",
              datapath = "GorelickModelOutput\\ModelResults_2016-07-01\\")
plotthisbitch(realization = 45, rank1 = 3, startyear = 0, numyears = 20, titlestring = "New Method",
              datapath = "GorelickModelOutput\\ModelResults_2016-07-02\\")
plotthisbitch(realization = 45, rank1 = 3, startyear = 0, numyears = 20, titlestring = "Old Method",
              datapath = "GorelickModelOutput\\ModelResults_2016-07-01\\")

# plot the buyback curves --------------------------------------------

realization = 11
rank1 = 0
startyear = 0
numyears = 5
titlestring = "buybackcurves"
figurep = figurepath
datapath = resultpath

Years = seq((2015 + startyear), (2015 + startyear + numyears - 1), 1)

if (numyears > 5)
{
  seasons = 2
  Blank = rep("", (seasons - 1))
}
if (numyears <= 5)
{
  seasons = 4
  Blank = rep("", (seasons - 1))
}
if (numyears >= 20)
{
  seasons = 1
  Blank = rep("", (seasons - 1))
}
if (numyears >= 30)
{
  seasons = 0.20
  Blank = character(0)
  Years = seq((2015 + startyear), (2015 + startyear + numyears - 1), 1/seasons)
}

line = NULL
for (yr in 1:length(Years))
{
  line = c(line, Years[yr], Blank)
}

line = c(line, (2015 + startyear + numyears))
rm(Years, Blank, yr)

## --------------------- all the plots compare at least one rank/realization ------------
## --------------------- read in the data here ------------------------------------------

data1     = read.csv(paste(datapath, "WeeklyData", rank1, ".csv", sep = ""), header = T)
weekdata1 = data1[which(data1$Realization == realization),]
rm(data1)

pars1      = read.csv(paste(datapath, "RealizationParameters", rank1, ".csv", sep = ""), header = T)
paramdata1 = pars1[which(pars1$Realization == realization),]
rm(pars1)

weekdata1 = weekdata1[with(weekdata1, order(Year, Week)),]
weekdata1$TimeseriesWeek = c(1:nrow(weekdata1))
# correctly order the timeseries

inf1 = read.csv(paste(datapath, "InfrastructureSequencing", rank1, ".csv", sep = ""), header = T)
inf1 = inf1[which(inf1$Realization == realization),]
inf1 = inf1[with(inf1, order(Year)),]

# ----------------------------------------------------------------------------------------
# ------------------ make 3 year timeseries plots ----------------------------------------

weeks1 = weekdata1[(startyear*52+1):((startyear+numyears)*52),]

# ------------------ compare releases and transfers --------------------------------------

DataToPlot1 = data.frame(TimeseriesWeek  = weeks1$TimeseriesWeek,
                         BB1  = weeks1$DurhamTargetStorageFraction,
                         BB2  = weeks1$DurhamTargetBuybackLevel)

DataToPlot = DataToPlot1

DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
# organize just the data I want to plot
# in a form better suited to ggplot

DataMelted$variable2 = factor(DataMelted$variable, labels = c("Release Cutoff Floor", "Buyback Ceiling"))

allgraph = ggplot(DataMelted) + 
  geom_line(aes(x = TimeseriesWeek, y = value, colour = variable2)) + 
#  scale_colour_manual(values = c("red", "blue"), labels = c("Release Cutoff Floor", "Buyback Ceiling")) +
  theme(legend.title = element_blank(), legend.position = c(0,0), legend.justification = c(0,0)) +
  guides(colour = guide_legend(ncol = 2)) + 
  xlab("") + ylim(c(0,1)) +
  ylab("Storage Fraction") + 
  ggtitle(paste(titlestring, sep = "")) +
  scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/seasons), 
                     labels = line)

if (numyears < 10)
{
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", numyears, "years", "_singlecomp_", titlestring, ".png", sep = ""),
         units = "in", width = 7, height = 3)
}
else
{
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", numyears, "years", "_singlecomp_", titlestring, ".png", sep = ""),
         units = "in", width = (numyears), height = 15)
}







