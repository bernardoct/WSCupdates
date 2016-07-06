## Output Plotting
## May 2016
## ------------------------------------------------------------------------------

# Code to build the .csv called below is found in outputplotting03212016.R
# this is just for the 4-rank results.  immediately below is code to read 
# 16-rank results

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

# Graph data from a single realization/rank combo -------------------------------
# first, build a function to spit out results ---------------------

plotsinglerealization = function(realization = REAL, Rank = RANK, datapath = resultpath, figurep = figurepath)
{
  data     = read.csv(paste(datapath, "WeeklyData", Rank, ".csv", sep = ""), header = T)
  weekdata = data[which(data$Realization == realization),]
  rm(data)
  
  pars      = read.csv(paste(datapath, "RealizationParameters", Rank, ".csv", sep = ""), header = T)
  paramdata = pars[which(pars$Realization == realization),]
  rm(pars)
  
  # plot all the data i want --------------------------
  
  WeeksPerRealization = nrow(weekdata)
  WeekOfRealization   = c(1:WeeksPerRealization)
  
  Years = seq(2015, 2055, 5)
  Blank = rep("", 4)
  
  line = NULL
  for (yr in 1:length(Years))
  {
    line = c(line, Years[yr], Blank)
  }
  
  line = c(line, "2060")
  rm(Years, Blank, yr)
  
  weekdata = weekdata[with(weekdata, order(Year, Week)),]
  weekdata$TimeseriesWeek = c(1:nrow(weekdata))
    # correctly order the timeseries
  
  if (max(weekdata$ReleaseMaxLimit) > 0.0)
  {
    msg   = paste("Releases Allowed, ROF Trigger = ", paramdata$RaleighRRtrigger, sep = "")
    marker = "RR"
    
    DataToPlot = data.frame(TimeseriesWeek   = weekdata$TimeseriesWeek,
                             RSupply         = weekdata$RaleighActualStorage,
                             DSupply         = weekdata$DurhamActualStorage,
                             RRQ             = weekdata$ReleaseRequest,
                             BBQ             = weekdata$BuybackQuantity,
                             TTR             = weekdata$RaleighTransferVolume,
                             TTD             = weekdata$DurhamTransferVolume)
    
    DataMelted = melt(DataToPlot, id = "TimeseriesWeek")
      # organize just the data I want to plot
      # in a form better suited to ggplot
    
    DataMelted$variable2 = factor(DataMelted$variable, labels = c(paste("Raleigh Supply", sep = ""),
                                                                  paste("Durham Supply", sep = ""),
                                                                  paste("Releases", sep = ""),
                                                                  paste("Buybacks", sep = ""),
                                                                  paste("Transfers (Raleigh)", sep = ""),
                                                                  paste("Transfers (Durham)", sep = "")))
  }
  else 
  {
    msg    = "No Releases Allowed"
    marker = "NR"
    
    DataToPlot = data.frame(TimeseriesWeek  = weekdata$TimeseriesWeek,
                            RSupply         = weekdata$RaleighActualStorage,
                            DSupply         = weekdata$DurhamActualStorage,
                            RRQ             = weekdata$ReleaseRequest,
                            BBQ             = weekdata$BuybackQuantity,
                            TTR             = weekdata$RaleighTransferVolume,
                            TTD             = weekdata$DurhamTransferVolume)
    
    DataMelted = melt(DataToPlot, id = "TimeseriesWeek")
    # organize just the data I want to plot
    # in a form better suited to ggplot
    
    DataMelted$variable2 = factor(DataMelted$variable, labels = c(paste("Raleigh Supply", sep = ""),
                                                                  paste("Durham Supply", sep = ""),
                                                                  paste("Releases", sep = ""),
                                                                  paste("Buybacks", sep = ""),
                                                                  paste("Transfers (Raleigh)", sep = ""),
                                                                  paste("Transfers (Durham)", sep = "")))
  }
  
  allgraph = ggplot(DataMelted) + 
    geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
    facet_grid(variable2 ~ ., scales = "free_y") +
    xlab("") + 
    ylab("Water Volume (MG)") + 
    scale_fill_discrete(name = "ROF Trigger") + 
    theme(legend.position = "none") + 
    guides(colour = guide_legend(ncol = 4)) + 
    ggtitle(paste(msg, ", Transfer ROF Trigger = ", paramdata$RaleighTTtrigger, sep = "")) +
    scale_x_continuous(breaks = seq(1,nrow(weekdata),52), labels = line)
  
  ggsave(filename = paste(figurep, RANK, "_", REAL, "_", "everyvariable", marker, ".png", sep = ""),
         units = "in", width = 9, height = 12)
  
  rm(weekdata, paramdata, allgraph, DataMelted, DataToPlot, msg, marker, line)
}

# build another function to plot differences ---------------------

realizationcomparerank = function(realization = REAL, rank1 = 0, rank2 = 8, 
                                  datapath = resultpath, figurep = figurepath)
{
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
  
  # make comparison statistics --------------------------------
  
  DataToPlot1 = data.frame(TimeseriesWeek  = weekdata1$TimeseriesWeek,
                           Rank            = weekdata1$Rank,
                           RSupply         = weekdata1$RaleighActualStorage,
                           DSupply         = weekdata1$DurhamActualStorage,
                           RRQ             = weekdata1$ReleaseRequest,
                           BBQ             = weekdata1$BuybackQuantity,
                           TTR             = weekdata1$RaleighTransferVolume,
                           TTD             = weekdata1$DurhamTransferVolume)
  
  DataToPlot2 = data.frame(TimeseriesWeek  = weekdata2$TimeseriesWeek,
                           Rank            = weekdata2$Rank,
                           RSupply         = weekdata2$RaleighActualStorage,
                           DSupply         = weekdata2$DurhamActualStorage,
                           RRQ             = weekdata2$ReleaseRequest,
                           BBQ             = weekdata2$BuybackQuantity,
                           TTR             = weekdata2$RaleighTransferVolume,
                           TTD             = weekdata2$DurhamTransferVolume)
  
  DataToPlot = rbind(DataToPlot1, DataToPlot2)
  
  DataMelted = melt(DataToPlot, id = c("TimeseriesWeek", "Rank"))
    # organize just the data I want to plot
    # in a form better suited to ggplot
  
  DataMelted$variable2 = factor(DataMelted$variable, labels = c(paste("Raleigh Supply", sep = ""),
                                                                paste("Durham Supply", sep = ""),
                                                                paste("Releases", sep = ""),
                                                                paste("Buybacks", sep = ""),
                                                                paste("Transfers (Raleigh)", sep = ""),
                                                                paste("Transfers (Durham)", sep = "")))
  
  if (max(weekdata1$ReleaseRequest) == 0.0)
  {
    msg1 = paste("No Releases Allowed,", 
                 " Release Fraction = ", min(weekdata1$ReleaseToFLSupplyFraction), sep = "")
  }
  if (max(weekdata1$ReleaseRequest) > 0.0)
  {
    msg1 = paste("Releases Allowed (ROF Trigger = ", paramdata1$RaleighRRtrigger, "),", 
                 " Release Fraction = ", min(weekdata1$ReleaseToFLSupplyFraction), sep = "")
  }
  
  if (max(weekdata2$ReleaseRequest) == 0.0)
  {
    msg2 = paste("No Releases Allowed,", 
                 " Release Fraction = ", min(weekdata2$ReleaseToFLSupplyFraction), sep = "")
  }
  if (max(weekdata2$ReleaseRequest) > 0.0)
  {
    msg2 = paste("Releases Allowed (ROF Trigger = ", paramdata2$RaleighRRtrigger, "),",
                 " Release Fraction = ", min(weekdata2$ReleaseToFLSupplyFraction), sep = "")
  }
  
  DataMelted$Rank2 = factor(DataMelted$Rank, labels = c(msg1, msg2))
  
  # plot the comparison -------------------------------------------------
  
  Years = seq(2015, 2055, 5)
  Blank = rep("", 4)
  
  line = NULL
  for (yr in 1:length(Years))
  {
    line = c(line, Years[yr], Blank)
  }
  
  line = c(line, "2060")
  rm(Years, Blank, yr)
  
  allgraph = ggplot(DataMelted) + 
    geom_bar(aes(x = TimeseriesWeek, y = value, fill = variable2), stat = "identity") + 
    facet_grid(variable2 ~ Rank2, scales = "free_y") +
    xlab("") + 
    ylab("Water Volume (MG)") + 
    scale_fill_discrete(name = "ROF Trigger") + 
    theme(legend.position = "none") + 
    guides(colour = guide_legend(ncol = 4)) + 
    ggtitle(paste("Comparison across Simulations", sep = "")) +
    scale_x_continuous(breaks = seq(1,nrow(weekdata1),52), labels = line)
  
  ggsave(filename = paste(figurep, realization, "_", rank1, "_", rank2, "_tworankcomparison", ".png", sep = ""),
         units = "in", width = 18, height = 12)
}

# run the function(s) to generate plots -----------------------

library(ggplot2)
library(reshape2)

for(RANK in 0:(1))
{
  for (REAL in 0:15)
  {
    plotsinglerealization(REAL, RANK, resultpath, figurepath)
    #realizationcomparerank(REAL, rank1 = RANK, rank2 = (RANK + 5), resultpath, figurepath)
    #realizationcomparerank(REAL, rank1 = RANK, rank2 = (RANK + 10), resultpath, figurepath)
      # comparing cases with different release fractions for FL
    #realizationcomparerank(REAL, rank1 = RANK, rank2 = 15, resultpath, figurepath)
      # comparing with and without releases
  }
}

## -----------------------------------------------------------------------------------
## Investigate some statistics for all realizations of a single rank -----------------
## -----------------------------------------------------------------------------------

numRealizations = 100

meanstats = NULL
for (RANK in 0:4)
{
  Rank = RANK
  
  data = read.csv(paste(resultpath, "WeeklyData", Rank, ".csv", sep = ""), header = T)
  pars = read.csv(paste(resultpath, "RealizationParameters", Rank, ".csv", sep = ""), header = T)
  
  Rank = Rank + 10
  
  data2 = read.csv(paste(resultpath, "WeeklyData", Rank, ".csv", sep = ""), header = T)
  pars2 = read.csv(paste(resultpath, "RealizationParameters", Rank, ".csv", sep = ""), header = T)
  
  allstats  = NULL
  for (REAL in 0:(numRealizations-1))
  {
    # data from RR realization ---------------------------------------------
    
    yeardata  = data[which(data$Realization == REAL),]
    # pull a single realization
    
    weeksofreleases  = which(yeardata$ReleaseRequest > 0, arr.ind = T)
    numberofreleases = length(weeksofreleases)
    totalreleases    = sum(yeardata$ReleaseRequest)
    # weeks releases were used
    
    weeksofbuybacks  = which(yeardata$BuybackQuantity > 0, arr.ind = T)
    numberofbuybacks = length(weeksofbuybacks)
    totalbuybacks    = sum(yeardata$BuybackQuantity)
    # weeks buybacks were used
    
    weeksofTTR  = which(yeardata$RaleighTransferVolume > 0, arr.ind = T)
    numberofTTR = length(weeksofTTR)
    totalTTR    = sum(yeardata$RaleighTransferVolume)
    # weeks treated transfers to raleigh were used
    
    weeksofTTD  = which(yeardata$DurhamTransferVolume > 0, arr.ind = T)
    numberofTTD = length(weeksofTTD)
    totalTTD    = sum(yeardata$DurhamTransferVolume)
    # weeks treated transfers to durham were used
    
    RRstats = c(numberofreleases, numberofbuybacks,
                totalreleases, totalbuybacks,
                numberofTTR, numberofTTD,
                totalTTR, totalTTD)
    
    # data from NR realization ---------------------------------------------
    
    yeardata  = data2[which(data2$Realization == REAL),]
    # pull a single realization
    
    weeksofTTR  = which(yeardata$RaleighTransferVolume > 0, arr.ind = T)
    numberofTTR = length(weeksofTTR)
    totalTTR    = sum(yeardata$RaleighTransferVolume)
    # weeks treated transfers to raleigh were used
    
    weeksofTTD  = which(yeardata$DurhamTransferVolume > 0, arr.ind = T)
    numberofTTD = length(weeksofTTD)
    totalTTD    = sum(yeardata$DurhamTransferVolume)
    # weeks treated transfers to durham were used
    
    NRstats = c(numberofTTR, numberofTTD,
                totalTTR, totalTTD)
    
    # save the differences for each realization ---------------------------
    
    allstats = rbind(allstats, c(RRstats, NRstats))
    
  }
  allstats = as.data.frame(allstats)
  averages = colMeans(allstats)
  
  pars = pars[which(pars$Realization == REAL),]
    # same for every realization
  
  tojoin = c(averages, pars[,2:ncol(pars)])
  
  meanstats = rbind(meanstats, tojoin)
}
colnames(meanstats) = c("ReleaseWeeks", "BuybackWeeks", "ReleaseTotals", "BuybackTotals",
                        "RRTransferWeeksR", "RRTransferWeeksD", "RRTotalTransfersR", "RRTotalTransfersD",
                        "NRTransferWeeksR", "NRTransferWeeksD", "NRTotalTransfersR", "NRTotalTransfersD",
                        names(pars[,2:ncol(pars)]))

meanstats = as.data.frame(meanstats)
  # each row are summary stats for 100 realization sets
  # both RR and NR ranks together

write.csv(meanstats, 
          paste(figurepath, "overallstats", ".csv", sep = ""))

## ------------------------------------------------------------------------------------------
## Read and look at infrastructure sequencing -----------------------------------------------

INdata1 = read.csv(paste(datafolder, "InfraBuilt", RANK, ".csv", sep = ""), header = T)
  # anytime LM small expansion occurs, the large expansion is also triggered?

## ------------------------------------------------------------------------------------------
## ------------------------------------------------------------------------------------------

# A few interesting tradeoffs are going to be between maximum allowable release amounts,
# buyback threshold for durham, treated transfer triggers for both raleigh and durham,
# release trigger, release amount used for water supply.  Some ideas for objectives:
#   minimize the maximum weekly release cap
#   minimize treated transfers to raleigh
#     constrained to see what it would take to get raleigh under the IBT threshold?
#   maximize the fraction of water used for water supply in FL
#     bounded reasonably
#   minimize profit due to release/buyback contract
#     (aiming for the costs of releases/buybacks/treated transfers to "zero" out)
#     (alternatively: find out what a contract would look like that would
#      satisfy raleigh's needs to reduce IBTs and adjust the price of buybacks 
#      to reflect this)
#   minimize contract price for raleigh 
#   minimize contract cost for durham (total buybacks)
#     utilities unlikely to want to have at-large capital 











