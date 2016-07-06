## Output Plotting
## May 25th
## Make a variety of plots from new output data for Cornell presentation 
## -------------------------------------------------------------------------------------------

## ---------------------- ORGANIZE DATA ------------------------------------------------------

rm(list = ls())
setwd("C:\\Users\\David\\OneDrive\\UNC\\Research\\WSC\\Coding")

resultpath = paste("GorelickModelOutput\\ModelResults_", Sys.Date(), "\\", sep = "")
dir.create(resultpath)

figurepath = paste("GorelickModelOutput\\Figures_", Sys.Date(), "\\", sep = "")
dir.create(figurepath)

datafolder = "GorelickModelCopy0\\output\\"
numranks   = 16
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

plotthisbitch3comps = function(realization, rank1, rank2, rank3, startyear, numyears, titlestring,
                         datapath, figurep)
{
  Years = seq((2015 + startyear), (2015 + startyear + numyears - 1), 1)
  Blank = rep("", 3)
  
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
  
  data3     = read.csv(paste(datapath, "WeeklyData", rank3, ".csv", sep = ""), header = T)
  weekdata3 = data3[which(data3$Realization == realization),]
  rm(data3)
  
  pars1      = read.csv(paste(datapath, "RealizationParameters", rank1, ".csv", sep = ""), header = T)
  paramdata1 = pars1[which(pars1$Realization == realization),]
  rm(pars1)
  
  pars2      = read.csv(paste(datapath, "RealizationParameters", rank2, ".csv", sep = ""), header = T)
  paramdata2 = pars2[which(pars2$Realization == realization),]
  rm(pars2)
  
  pars3      = read.csv(paste(datapath, "RealizationParameters", rank3, ".csv", sep = ""), header = T)
  paramdata3 = pars3[which(pars3$Realization == realization),]
  rm(pars3)
  
  weekdata1 = weekdata1[with(weekdata1, order(Year, Week)),]
  weekdata1$TimeseriesWeek = c(1:nrow(weekdata1))
  # correctly order the timeseries
  
  weekdata2 = weekdata2[with(weekdata2, order(Year, Week)),]
  weekdata2$TimeseriesWeek = c(1:nrow(weekdata2))
  # correctly order the timeseries
  
  weekdata3 = weekdata3[with(weekdata3, order(Year, Week)),]
  weekdata3$TimeseriesWeek = c(1:nrow(weekdata3))
  # correctly order the timeseries
  
  inf1 = read.csv(paste(datapath, "InfrastructureSequencing", rank1, ".csv", sep = ""), header = T)
  inf1 = inf1[which(inf1$Realization == realization),]
  inf1 = inf1[with(inf1, order(Year)),]
  
  inf2 = read.csv(paste(datapath, "InfrastructureSequencing", rank2, ".csv", sep = ""), header = T)
  inf2 = inf2[which(inf2$Realization == realization),]
  inf2 = inf2[with(inf2, order(Year)),]
  
  inf3 = read.csv(paste(datapath, "InfrastructureSequencing", rank3, ".csv", sep = ""), header = T)
  inf3 = inf3[which(inf3$Realization == realization),]
  inf3 = inf3[with(inf3, order(Year)),]
  
  # ----------------------------------------------------------------------------------------
  # ------------------ make 3 year timeseries plots ----------------------------------------
  
  weeks1 = weekdata1[(startyear*52+1):((startyear+numyears)*52),]
  weeks2 = weekdata2[(startyear*52+1):((startyear+numyears)*52),]
  weeks3 = weekdata3[(startyear*52+1):((startyear+numyears)*52),]
  
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
  
  DataToPlot3 = data.frame(TimeseriesWeek  = weeks3$TimeseriesWeek,
                           Rank            = weeks3$Rank,
                           RRQ             = weeks3$ReleaseRequest,
                           BBQ             = weeks3$BuybackQuantity,
                           TTR             = weeks3$RaleighTransferVolume,
                           TTD             = weeks3$DurhamTransferVolume)
  
  DataToPlot = rbind(DataToPlot1, DataToPlot2)
  DataToPlot = rbind(DataToPlot, DataToPlot3)
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek", "Rank"))
  # organize just the data I want to plot
  # in a form better suited to ggplot
  
  DataMelted$variable2 = factor(DataMelted$variable, labels = c(paste("Releases", sep = ""),
                                                                paste("Buybacks", sep = ""),
                                                                paste("Transfers (Raleigh)", sep = ""),
                                                                paste("Transfers (Durham)", sep = "")))
  
  if (max(weekdata1$ReleaseRequest) == 0.0)
  {
    msg1 = "Case 1: No Releases Allowed"
  }
  if (max(weekdata1$ReleaseRequest) > 0.0)
  {
    msg1 = "Case 1: Releases Allowed"
  }
  
  if (max(weekdata2$ReleaseRequest) == 0.0)
  {
    msg2 = "Case 2: No Releases Allowed"
  }
  if (max(weekdata2$ReleaseRequest) > 0.0)
  {
    msg2 = "Case 2: Releases Allowed"
  }
  
  if (max(weekdata3$ReleaseRequest) == 0.0)
  {
    msg3 = "Case 3: No Releases Allowed"
  }
  if (max(weekdata3$ReleaseRequest) > 0.0)
  {
    msg3 = "Case 3: Releases Allowed"
  }
  
  DataMelted$Rank2 = factor(DataMelted$Rank, labels = c(msg1, msg2, msg3))
  
  allgraph = ggplot(DataMelted) + 
    geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
    facet_grid(variable2 ~ Rank2, scales = "free_y") +
    xlab("") + 
    ylab("Water Volume (MG)") + 
    theme(legend.position = "none") +
    ggtitle(paste(titlestring, sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/4), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_", rank3, "_3yearcomparison_", titlestring, ".png", sep = ""),
         units = "in", width = 16, height = 10)
  
  # ------------------ compare Raleigh storage ---------------------------------------------
  
  DataToPlot = data.frame(TimeseriesWeek   = weeks1$TimeseriesWeek,
                          RROF1            = weeks1$RaleighTargetStorageFraction,
                          Rsupply1         = weeks1$RaleighStorageRatio,
                          Rsupply2         = weeks2$RaleighStorageRatio,
                          Rsupply3         = weeks3$RaleighStorageRatio)
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
  # organize just the data I want to plot
  # in a form better suited to ggplot
  
  DataMelted$var = factor(DataMelted$variable, labels = c(paste("Release ROF Trigger", sep = ""),
                                                          paste("Storage (Case 1)", sep = ""),
                                                          paste("Storage (Case 2)", sep = ""),
                                                          paste("Storage (Case 3)")))
  
  allgraph = ggplot(DataMelted) + 
    geom_line(aes(x = TimeseriesWeek, y = value, colour = var)) + 
    ylim(c(0,1)) +
    xlab("") + 
    ylab("Storage Fraction") + 
    scale_colour_manual(values = c("black", "red", "blue", "green"), 
                        labels = c("Release ROF Trigger", "Storage (Case 1)", "Storage (Case 2)", "Storage (Case 3)")) +
    theme(legend.title = element_blank(), legend.position = c(0,0), legend.justification = c(0,0)) + 
    guides(colour = guide_legend(ncol = 4)) + 
    ggtitle(paste("Raleigh", sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/4), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_", rank3, "_3yearstoragesR_", titlestring, ".png", sep = ""),
         units = "in", width = 12, height = 5)
  
  # ------------------ compare Durham storage ---------------------------------------------
  
  DataToPlot = data.frame(TimeseriesWeek    = weeks1$TimeseriesWeek,
                          DROF1             = weeks1$DurhamTargetStorageFraction,
                          Dsupply1          = weeks1$DurhamStorageRatio,
                          Dsupply2          = weeks2$DurhamStorageRatio,
                          Dsupply3          = weeks3$DurhamStorageRatio)
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek"))
  # organize just the data I want to plot
  # in a form better suited to ggplot
  
  DataMelted$var = factor(DataMelted$variable, labels = c(paste("Buyback ROF Trigger", sep = ""),
                                                          paste("Storage (Case 1)", sep = ""),
                                                          paste("Storage (Case 2)", sep = ""),
                                                          paste("Storage (Case 3)")))
  
  allgraph = ggplot(DataMelted) + 
    geom_line(aes(x = TimeseriesWeek, y = value, colour = var)) + 
    ylim(c(0,1)) +
    xlab("") + 
    ylab("Storage Fraction") + 
    scale_colour_manual(values = c("black", "red", "blue", "green"), 
                        labels = c("Buyback ROF Trigger", "Storage (Case 1)", "Storage (Case 2)", "Storage (Case 3)")) +
    theme(legend.title = element_blank(), legend.position = c(0,0), legend.justification = c(0,0)) + 
    guides(colour = guide_legend(ncol = 4)) + 
    ggtitle(paste("Durham", sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/4), 
                       labels = line)
  
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_", rank3, "_3yearstoragesD_", titlestring, ".png", sep = ""),
         units = "in", width = 12, height = 5)
}

## --------------------------------------------------------------------------------------
## --------------------- MAKE SOME PLOTS ------------------------------------------------
## --------------------- ONLY COMPARE TWO -----------------------------------------------

plotthisbitch2comps = function(realization, rank1, rank2, startyear, numyears, titlestring,
                         datapath, figurep)
{
  Years = seq((2015 + startyear), (2015 + startyear + numyears - 1), 1)
  Blank = rep("", 3)
  
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
  
  if (max(weekdata1$ReleaseRequest) == 0.0)
  {
    msg1 = "Case 1: No Releases Allowed"
  }
  if (max(weekdata1$ReleaseRequest) > 0.0)
  {
    msg1 = "Case 1: Releases Allowed"
  }
  
  if (max(weekdata2$ReleaseRequest) == 0.0)
  {
    msg2 = "Case 2: No Releases Allowed"
  }
  if (max(weekdata2$ReleaseRequest) > 0.0)
  {
    msg2 = "Case 2: Releases Allowed"
  }
  
  DataMelted$Rank2 = factor(DataMelted$Rank, labels = c(msg1, msg2))
  
  allgraph = ggplot(DataMelted) + 
    geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
    facet_grid(variable2 ~ Rank2, scales = "free_y") +
    xlab("") + 
    ylab("Water Volume (MG)") + 
    theme(legend.position = "none") + 
    ggtitle(paste(titlestring, sep = "")) +
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/4), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_3year2comps_", titlestring, ".png", sep = ""),
         units = "in", width = 16, height = 10)
  
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
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/4), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_3yearstorages2compsR_", titlestring, ".png", sep = ""),
         units = "in", width = 12, height = 5)
  
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
    scale_x_continuous(breaks = seq(weeks1$TimeseriesWeek[1],weeks1$TimeseriesWeek[nrow(weeks1)]+1,52/4), 
                       labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_3yearstorages2compsD_", titlestring, ".png", sep = ""),
         units = "in", width = 12, height = 5)
}

## ----------------- RUN THE FUNCTION ----------------------------------------------------------------

library(ggplot2)
library(reshape2)

## ----- comparing varied RROF -------------------------------------------------

plotthisbitch2comps(realization = 2,
              rank1 = 2,
              rank2 = 7,
              startyear = 13,
              numyears = 3,
              titlestring = "Varied release risk-of-failure trigger values",
              datapath = resultpath,
              figurep = figurepath)

## ----- comparing varied DROF -------------------------------------------------

plotthisbitch2comps(realization = 2,
              rank1 = 0,
              rank2 = 4,
              startyear = 13,
              numyears = 3,
              titlestring = "Varied buyback risk-of-failure trigger values",
              datapath = resultpath,
              figurep = figurepath)

## ----- comparing varied RR cap -------------------------------------------------

plotthisbitch2comps(realization = 2,
              rank1 = 0,
              rank2 = 6,
#              rank3 = 7,
              startyear = 13,
              numyears = 3,
              titlestring = "Varied weekly release cap",
              datapath = resultpath,
              figurep = figurepath)

## ----- comparing varied TT triggers -------------------------------------------------

plotthisbitch2comps(realization = 2,
              rank1 = 0,
              rank2 = 5,
              startyear = 13,
              numyears = 3,
              titlestring = "Varied treated transfer ROF triggers",
              datapath = resultpath,
              figurep = figurepath)

## ----- comparing varied FLSP fraction -------------------------------------------------

plotthisbitch2comps(realization = 2,
              rank1 = 0,
              rank2 = 2,
              startyear = 13,
              numyears = 3,
              titlestring = "Varied supply pool release fraction",
              datapath = resultpath,
              figurep = figurepath)

