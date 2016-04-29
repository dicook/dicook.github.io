# Read in the data, and take a quick peek at it
tips <- read.csv(file.choose())
tips
# Whoa! That's a whole lotta lines in the console. 
# Let's just look at the first few rows
head(tips)
# Hmm... the first column looks awkward, why is it named X, 
# and the same as the row number?
# Let's read it in by setting that column to be row labels
tips <- read.csv(file.choose(), row.names=1)
head(tips)
# Much better!
# How big is tips? What types of variables are in the columns?
str(tips)
# It looks like I have 3 numerical variables and 4 character variable
# Lets get a summary of these columns of my data set
summary(tips)
# Look at the way that cost of the total bill might effect the tip value
#   using base R function plot()
plot(tips$bill, tips$tip)
# Uggh, awful!
# Let's make the plot in style. Load the ggplot2 library.
library(ggplot2)
qplot(bill, tip, data=tips)
# What's the relationship? Fit a linear model.
lm(tips$tip ~ tips$bill)
# Let's plot the model on the data
qplot(bill, tip, data=tips, geom=c("point", "smooth"), method="lm")
# I don't like the grey error bar.
qplot(bill, tip, data=tips, geom=c("point", "smooth"), method="lm", se=F)
# I am really more interested in tip as a % though
# Create a new variable using two of the existing variable 
tips$rate <- tips$tip / tips$bill
head(tips)
summary(tips$rate)
qplot(rate, data=tips, geom="histogram")
# Let's see if there is a difference by gender
qplot(rate, data=tips, geom="histogram", facets=sex~smoker, fill=sex)
# Not much!
# Oh, by the way, we could have read the data directly from the internet
read.csv("http://www.ggobi.org/book/data/tips.csv", row.names=1)
head(tips)
