## Output Plotting for June Update
## results with only historic streamflows and different Durham infrastructure options
## -----------------------------------------------------------------------------------

## ---------------------- ORGANIZE DATA ------------------------------------------------------

rm(list = ls())
setwd("C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\Coding")

resultpath = paste("GorelickModelOutput\\ModelResults_", Sys.Date(), "\\", sep = "")
dir.create(resultpath)

figurepath = paste("GorelickModelOutput\\Figures_", Sys.Date(), "\\", sep = "")
dir.create(figurepath)

datafolder = "GorelickModelCopy0\\output\\"
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
  
  INdata = read.csv(paste(datafolder, "InfraBuilt", RANK, ".csv", sep = ""), header = T)
  # collect the infrastructure timeline for all realizations
  
  write.csv(INdata, paste(resultpath, "InfrastructureSequencing", RANK, ".csv", sep = ""))
  rm(INdata)
  
  params = read.csv(paste(datafolder, "weeklyRiskParams", RANK, ".csv", sep = ""), header = T)
  # triggers and other set parameters for each realization
  
  write.csv(params, paste(resultpath, "RealizationParameters", RANK, ".csv", sep = ""))
  rm(params)
}
  # CONSOLIDATE AND WRITE OUTPUT FILES

## --------------------------------------------------------------------------------------
## --------------------- MAKE SOME PLOTS ------------------------------------------------
## --------------------- ONLY COMPARE TWO -----------------------------------------------

plotthisbitch2comps = function(realization, rank1, rank2, startyear, numyears, titlestring,
                               datapath, figurep)
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
  
  data2     = read.csv(paste(datapath, "WeeklyData", rank2, ".csv", sep = ""), header = T)
  weekdata2 = data2[which(data2$Realization == realization),]
  rm(data2)
  
  pars1      = read.csv(paste(datapath, "RealizationParameters", rank1, ".csv", sep = ""), header = T)
  paramdata1 = pars1[which(pars1$Realization == realization),]
  rm(pars1)
  
  pars2      = read.csv(paste(datapath, "RealizationParameters", rank2, ".csv", sep = ""), header = T)
  paramdata2 = pars2[which(pars2$Realization == realization),]
  rm(pars2)
  
  weekdata1 = weekdata1[with(weekdata1, order(Year, Week)),]
  weekdata1$TimeseriesWeek = c(1:nrow(weekdata1))
  # correctly order the timeseries
  
  weekdata2 = weekdata2[with(weekdata2, order(Year, Week)),]
  weekdata2$TimeseriesWeek = c(1:nrow(weekdata2))
  # correctly order the timeseries
  
  inf1 = read.csv(paste(datapath, "InfrastructureSequencing", rank1, ".csv", sep = ""), header = T)
  inf1 = inf1[which(inf1$Realization == realization),]
  inf1 = inf1[with(inf1, order(Year)),]
  
  inf2 = read.csv(paste(datapath, "InfrastructureSequencing", rank2, ".csv", sep = ""), header = T)
  inf2 = inf2[which(inf2$Realization == realization),]
  inf2 = inf2[with(inf2, order(Year)),]
  
  # ----------------------------------------------------------------------------------------
  # ------------------ make 3 year timeseries plots ----------------------------------------
  
  weeks1 = weekdata1[(startyear*52+1):((startyear+numyears)*52),]
  weeks2 = weekdata2[(startyear*52+1):((startyear+numyears)*52),]
  
  # ------------------ compare releases and transfers --------------------------------------
  
  DataToPlot1 = data.frame(TimeseriesWeek  = weeks1$TimeseriesWeek,
                           Rank            = weeks1$Rank,
                           RRQ             = weeks1$ReleaseRequest,
                           BBQ             = weeks1$BuybackQuantity,
                           TTR             = weeks1$RaleighTransferVolume,
                           TTD             = weeks1$DurhamTransferVolume)
  
  DataToPlot2 = data.frame(TimeseriesWeek  = weeks2$TimeseriesWeek,
                           Rank            = weeks2$Rank,
                           RRQ             = weeks2$ReleaseRequest,
                           BBQ             = weeks2$BuybackQuantity,
                           TTR             = weeks2$RaleighTransferVolume,
                           TTD             = weeks2$DurhamTransferVolume)
  
  DataToPlot = rbind(DataToPlot1, DataToPlot2)
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek", "Rank"))
  # organize just the data I want to plot
  # in a form better suited to ggplot
  
  DataMelted$variable2 = factor(DataMelted$variable, labels = c(paste("Releases", sep = ""),
                                                                paste("Buybacks", sep = ""),
                                                                paste("Transfers (Raleigh)", sep = ""),
                                                                paste("Transfers (Durham)", sep = "")))
  
  if (length(unique(weekdata1$DsupplyCapacity[1:520])) > 1)
  {
    msg1 = "Case 1: Early Durham Capacity Expansion"
  }
  if (length(unique(weekdata1$DsupplyCapacity[1:520])) == 1)
  {
    msg1 = "Case 1: No Early Capacity Expansion"
  }
  
  if (length(unique(weekdata2$DsupplyCapacity[1:520])) > 1)
  {
    msg2 = "Case 2: Early Durham Capacity Expansion"
  }
  if (length(unique(weekdata2$DsupplyCapacity[1:520])) == 1)
  {
    msg2 = "Case 2: No Early Capacity Expansion"
  }
  
  DataMelted$Rank2 = factor(DataMelted$Rank, labels = c(msg1, msg2))
  
  allgraph = ggplot(DataMelted) + 
    geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
    facet_grid(variable2 ~ Rank2, scales = "free_y") +
    xlab("") + 
    ylab("Water Volume (MG)") + 
    theme(legend.position = "none") + 
    ggtitle(paste(titlestring, sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/seasons), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_", numyears, "years", "_2comps_", titlestring, ".png", sep = ""),
         units = "in", width = 17, height = 10)
  
  # ------------------ compare Raleigh storage ---------------------------------------------
  
  DataToPlot = data.frame(TimeseriesWeek   = weeks1$TimeseriesWeek,
                          RROF1            = weeks1$RaleighTargetStorageFraction,
                          Rsupply1         = weeks1$RaleighStorageRatio,
                          Rsupply2         = weeks2$RaleighStorageRatio)
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
  # organize just the data I want to plot
  # in a form better suited to ggplot
  
  DataMelted$var = factor(DataMelted$variable, labels = c(paste("Release ROF Trigger", sep = ""),
                                                          paste("Storage (Case 1)", sep = ""),
                                                          paste("Storage (Case 2)", sep = "")))
  
  allgraph = ggplot(DataMelted) + 
    geom_line(aes(x = TimeseriesWeek, y = value, colour = var)) + 
    ylim(c(0,1)) +
    xlab("") + 
    ylab("Storage Fraction") + 
    scale_colour_manual(values = c("black", "red", "blue"), 
                        labels = c("Release ROF Trigger", "Storage (Case 1)", "Storage (Case 2)")) +
    theme(legend.title = element_blank(), legend.position = c(0,0), legend.justification = c(0,0)) + 
    guides(colour = guide_legend(ncol = 3)) + 
    ggtitle(paste("Raleigh", sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/seasons), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_", numyears, "years", "_storages2compsR_", titlestring, ".png", sep = ""),
         units = "in", width = 25, height = 10)
  
  # ------------------ compare Durham storage ---------------------------------------------
  
  DataToPlot = data.frame(TimeseriesWeek    = weeks1$TimeseriesWeek,
                          DROF1             = weeks1$DurhamTargetStorageFraction,
                          Dsupply1          = weeks1$DurhamStorageRatio,
                          Dsupply2          = weeks2$DurhamStorageRatio)
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
  # organize just the data I want to plot
  # in a form better suited to ggplot
  
  DataMelted$var = factor(DataMelted$variable, labels = c(paste("Buyback ROF Trigger", sep = ""),
                                                          paste("Storage (Case 1)", sep = ""),
                                                          paste("Storage (Case 2)", sep = "")))
  
  
  allgraph = ggplot(DataMelted) + 
    geom_line(aes(x = TimeseriesWeek, y = value, colour = var)) + 
    ylim(c(0,1)) +
    xlab("") + 
    ylab("Storage Fraction") + 
    scale_colour_manual(values = c("black", "red", "blue"), 
                        labels = c("Buyback ROF Trigger", "Storage (Case 1)", "Storage (Case 2)")) +
    theme(legend.title = element_blank(), legend.position = c(0,0), legend.justification = c(0,0)) + 
    guides(colour = guide_legend(ncol = 3)) + 
    ggtitle(paste("Durham", sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/seasons), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_", numyears, "years", "_storages2compsD_", titlestring, ".png", sep = ""),
         units = "in", width = 25, height = 10)
  
  # ------------------ compare both actual storages ---------------------------------------------
  
  DataToPlot = data.frame(TimeseriesWeek   = weeks1$TimeseriesWeek,
                          Rsupply1         = weeks1$RaleighActualStorage,
                          Dsupply1         = weeks1$DurhamActualStorage,
                          Rsupply2         = weeks2$RaleighActualStorage,
                          Dsupply2         = weeks2$DurhamActualStorage
                          )
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
  # organize just the data I want to plot
  # in a form better suited to ggplot
  
  DataMelted$var = factor(DataMelted$variable, labels = c(paste("Raleigh Storage", sep = ""),
                                                          paste("Durham Storage", sep = ""),
                                                          paste("Raleigh Storage", sep = ""),
                                                          paste("Durham Storage")))
  
  if (length(unique(weekdata1$DsupplyCapacity[1:520])) > 1)
  {
    msg1 = "Case 1: Early Durham Capacity Expansion"
  }
  if (length(unique(weekdata1$DsupplyCapacity[1:520])) == 1)
  {
    msg1 = "Case 1: No Early Capacity Expansion"
  }
  
  if (length(unique(weekdata2$DsupplyCapacity[1:520])) > 1)
  {
    msg2 = "Case 2: Early Durham Capacity Expansion"
  }
  if (length(unique(weekdata2$DsupplyCapacity[1:520])) == 1)
  {
    msg2 = "Case 2: No Early Capacity Expansion"
  }
  
  DataMelted$Rank2 = factor(DataMelted$variable, labels = c(msg1, msg1, msg2, msg2))
  
  allgraph = ggplot(DataMelted) + 
    geom_bar(aes(x = TimeseriesWeek, y = value, fill = var), stat = "identity") + 
    facet_grid(var ~ Rank2, scales = "free_y") +
    xlab("") + 
    ylab("Storage Fraction") + 
    theme(legend.position = "none") + 
    ggtitle(paste(titlestring, sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/seasons), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_", numyears, "years", "_storages2compsBOTH_", titlestring, ".png", sep = ""),
         units = "in", width = 25, height = 10)
  
  ## PRINT SOME STATISTICS FROM EACH TIMESERIES ------------------------------------------------------
  ## -------------------------------------------------------------------------------------------------
  
  ## Case 1 --------------------
  
  RRweeks1  = length(weekdata1$ReleaseRequest[which(weekdata1$ReleaseRequest > 0)])
  BBweeks1  = length(weekdata1$BuybackQuantity[which(weekdata1$BuybackQuantity > 0)])
  TTRweeks1 = length(weekdata1$RaleighTransferVolume[which(weekdata1$RaleighTransferVolume > 0)])
  TTDweeks1 = length(weekdata1$DurhamTransferVolume[which(weekdata1$DurhamTransferVolume > 0)])
  
  RRsums1  = sum(weekdata1$ReleaseRequest[which(weekdata1$ReleaseRequest > 0)])
  BBsums1  = sum(weekdata1$BuybackQuantity[which(weekdata1$BuybackQuantity > 0)])
  TTRsums1 = sum(weekdata1$RaleighTransferVolume[which(weekdata1$RaleighTransferVolume > 0)])
  TTDsums1 = sum(weekdata1$DurhamTransferVolume[which(weekdata1$DurhamTransferVolume > 0)])
  
  ## Case 2 --------------------
  
  RRweeks2  = length(weekdata2$ReleaseRequest[which(weekdata2$ReleaseRequest > 0)])
  BBweeks2  = length(weekdata2$BuybackQuantity[which(weekdata2$BuybackQuantity > 0)])
  TTRweeks2 = length(weekdata2$RaleighTransferVolume[which(weekdata2$RaleighTransferVolume > 0)])
  TTDweeks2 = length(weekdata2$DurhamTransferVolume[which(weekdata2$DurhamTransferVolume > 0)])
  
  RRsums2  = sum(weekdata2$ReleaseRequest[which(weekdata2$ReleaseRequest > 0)])
  BBsums2  = sum(weekdata2$BuybackQuantity[which(weekdata2$BuybackQuantity > 0)])
  TTRsums2 = sum(weekdata2$RaleighTransferVolume[which(weekdata2$RaleighTransferVolume > 0)])
  TTDsums2 = sum(weekdata2$DurhamTransferVolume[which(weekdata2$DurhamTransferVolume > 0)])
  
  cat("Basic statistics for each case: \n")
  cat("Statistic \tCase 1 \tCase 2\t\n")
  
  cat(paste(paste("RR weeks", RRweeks1, RRweeks2, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("BB weeks", BBweeks1, BBweeks2, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTR weeks", TTRweeks1, TTRweeks2, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTD weeks", TTDweeks1, TTDweeks2, sep = "\t"), "\n", sep = ""))
  
  cat(paste(paste("RR total", RRsums1, RRsums2, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("BB total", BBsums1, BBsums2, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTR total", TTRsums1, TTRsums2, sep = "\t"), "\n", sep = ""))
  cat(paste(paste("TTD total", TTDsums1, TTDsums2, sep = "\t"), "\n", sep = ""))
}

## ----------------- RUN THE FUNCTION ----------------------------------------------------------------

library(ggplot2)
library(reshape2)

## ----- comparing scenarios when LM is expanded at different times -----------------------

plotthisbitch2comps(realization = 0,
                    rank1 = 0,
                    rank2 = 1,
                    startyear = 0,
                    numyears = 30,
                    titlestring = "Lake Michie Expansion Scenarios",
                    datapath = resultpath,
                    figurep = figurepath)
