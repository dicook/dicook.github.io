library(reshape)
library(plyr)
library(ggplot2)
library(maps)
library(ggmap)

temp.all<-read.table("../data/9641C_201112_raw.avg")
dim(temp.all)
colnames(temp.all)<-c("year", "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec", "ann")
temp.all$stn<-temp.all$year%/%100000
temp.all$year<-temp.all$year%%10000
head(temp.all)

stn.all<-read.table("http://cdiac.ornl.gov/ftp/ushcn_v2_monthly/ushcn-stations.txt", fill=T)
stn.all<-stn.all[,c(1:3,5:6)]
colnames(stn.all)<-c("stn", "lat", "lon", "state", "town")
stn.all$name<-paste(stn.all$town, stn.all$state)
head(stn.all)

temp.all2<-merge(temp.all, stn.all, by="stn")
dim(temp.all)
dim(temp.all2)
head(temp.all2)

iowa <- subset(temp.all2, state=="IA")
dim(iowa)
head(iowa)

iowa.melt <- melt(iowa[,-c(15, 19)], 
    id=c("stn", "year", "lat", "lon", "name", "state"))

colnames(iowa.melt)[7]<-"month"
colnames(iowa.melt)[8]<-"temp"
iowa.melt$temp <- as.numeric(as.character(iowa.melt$temp))
iowa.melt$temp[iowa.melt$temp==-9999] <- NA
iowa.melt$temp<-iowa.melt$temp/10

iowa.melt$state <- factor(iowa.melt$state)
iowa.melt$name <- factor(iowa.melt$name)
iowa.melt$stn <- factor(iowa.melt$stn)

summary(iowa.melt)
summary(iowa.melt$month)

temp.min <- min(iowa.melt$temp, na.rm=T)
subset(iowa.melt, temp == temp.min)

iowa.rngs <- ddply(iowa.melt, "stn", summarise, 
                   mintemp = min(temp, na.rm=T), 
                   maxtemp = max(temp, na.rm=T), 
                   lon = unique(lon), 
                   lat = unique(lat),
                   name = unique(name))
dim(iowa.rngs)
head(iowa.rngs)
qplot(lon, lat, data=iowa.rngs, colour=mintemp)
qmap(location = 'iowa', zoom = 6, 
     maptype = 'watercolor', source = 'stamen') +
  geom_point(data=iowa.rngs, 
             mapping=aes(x=lon, y=lat, colour=mintemp), size=5) 

# Iowa prairies 
newpbi <- read.csv("../data/newpbi.csv")
dim(newpbi)
newpbi[1,]
newpbi.m <- melt(newpbi, id.var = c("Site", "Type"))
head(newpbi.m)
colnames(newpbi.m)[3] <- "Species"
colnames(newpbi.m)[4] <- "Count"
head(newpbi.m)
qplot(Species, Count, data=newpbi.m) + coord_flip()

# html
library(XML)
src ="http://www.realclearpolitics.com/epolls/2012/president/us/republican_presidential_nomination-1452.html"
tables <- readHTMLTable(src)
library(plyr)
ldply(tables, dim)


## ------------------------------------------------------------------------
polls <- tables[[15]]
head(polls) # still some work to do ...


## ------------------------------------------------------------------------
library(foreign)
knights <- read.spss("../data/knightfoundation2008sotcdata.por")
str(knights)


## ------------------------------------------------------------------------
library(devtools)
install_github("cbapi", "heike")
library(cbapi)
setkey("7f784587c3918611ad6ca67188d9b269b3558dd4") # my key - if you want to use this, get your own :)
# population based on 2010 Census:
popstate <- read.census(sprintf("http://api.census.gov/data/2010/sf1?key=%s&get=P0010001,NAME&for=state:*", getkey()))
head(popstate)

