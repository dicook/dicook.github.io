data(us.cities, package="maps")
akhi <- which (us.cities$country.etc=="AK"| us.cities$country.etc=="HI" |
                 us.cities$country.etc=="ma")
us.cities <- us.cities[-akhi,]
us.cities$country.etc <- factor(us.cities$country.etc)
us.cities$tour <- "not in tour"
us.cities$tour[which(us.cities$name=="Ames IA")] <- "included"
us.cities$order <- 0
us.cities$order[which(us.cities$name=="Ames IA")] <- 1

library(maps)
library(ggplot2)
states <- map_data("state")
library(ggvis)

p1 <- ggvis(props(x = ~long, y = ~lat, stroke:="grey")) 
for (i in unique(states$group)) {
p1 <- p1 + layer_path(data=states[states$group==i,])
}