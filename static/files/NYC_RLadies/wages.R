# Explore NLSY79 wages data
# 
# Install the data package
# remotes::install_github("numbats/yowie")

# Let's get started
# Load the library and read the explanation
library(yowie)
# help(package = "yowie")

library(tidyverse)
glimpse(wages_hs_do)

# Primary question: 
# How have wages of high school dropouts changed over time?
# Would like to think about this from an individual perspective,
# what is the individual experience.

# Other questions:
# Is there a difference in experience based on education, gender, race
# What is the highest wage earned? Lowest?
# How many records for individuals?

# ---- STOP ----
# What might you expect to see?
# Gradual increase over time?
# Gender, education, race gaps

# Let's take a look
# Checking the data
wages_hs_do %>% 
  key_size() %>%
  summary()

wages_hs_do %>% 
  key_size() %>%
  as_tibble() %>%
  ggplot(aes(x=value)) + 
    geom_histogram(breaks = seq(4.5, 28.5, 1), 
                   colour="white") 

# Examining the individual experience over time
# Long series
wages_hs_do_ids <- key_data(wages_hs_do) %>% 
  mutate(n = key_size(wages_hs_do))
long <- wages_hs_do_ids %>%
  filter(n == 28)

wages_hs_do %>%
  filter(id %in% long$id) %>%
  ggplot(aes(x=year, y=mean_hourly_wage)) +
  geom_point() +
  geom_smooth(method="lm", se=FALSE) +
  facet_wrap(~id) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage")

# Short series
short <- wages_hs_do_ids %>%
  filter(n == 5)

wages_hs_do %>%
  filter(id %in% short$id) %>%
  ggplot(aes(x=year, y=mean_hourly_wage)) +
  geom_point() +
  geom_smooth(method="lm", se=FALSE) +
  facet_wrap(~id) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage")

# Keep only individuals who have at least 10 or more measurements
# The rationale is that individuals only measured briefly
# provide limited information about experiences.
# We can come back and examine these again later, in particular
# check that the number of records per individual is not related
# to other demographic variables
keep <- wages_hs_do_ids %>%
  filter(n > 9)

longer <- wages_hs_do %>%
  filter(id %in% keep$id) 
  
ggplot(longer, aes(x=year, y=mean_hourly_wage, group=id)) +
  geom_line(alpha=0.1) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage")

# Add mouse over to examine id's of outliers
library(plotly)
ggplotly()

# Take out the few extremes
avwages <- longer %>% as_tibble() %>%
  group_by(id) %>%
  summarise(avwage = mean(mean_hourly_wage)) 
# To examine outliers, a boxplot is better than a histogram
ggplot(avwages, aes(x=avwage)) +
  geom_boxplot() 
# Now take a look at shape
avwages %>% 
  filter(avwage < 35) %>%
  ggplot(aes(x=avwage)) +
  geom_histogram(binwidth=5, colour="white") 
# Try different bin widths, think 1 is good
avwages %>% 
  filter(avwage < 35) %>%
  ggplot(aes(x=avwage)) +
  geom_histogram(colour="white") +
  scale_x_log10() # scale_x_sqrt() re-think based on long'al
# For modelling it could be useful to use log scale
# to use normal error assumption

# Look longitudinally again
longer <- longer %>%
  filter(id %in% avwages$id[avwages$avwage < 35])

ggplot(longer) +
  geom_line(aes(x=year, y=mean_hourly_wage, group=id), alpha=0.1) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage")

# Try the transformations to symmetrise wages
ggplot(longer) +
  geom_line(aes(x=year, y=mean_hourly_wage, group=id), alpha=0.1) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  scale_y_sqrt() # Log is too strong!

ggplot(longer) +
  geom_line(aes(x=year, y=mean_hourly_wage, group=id), alpha=0.1) +
  geom_smooth(aes(x=year, y=mean_hourly_wage), se=F) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  scale_y_sqrt() # Log is too strong!
# Relationship is not quite linear, overall
# Starting to plateau, maybe some retirement age
# Appears to be a lot of individual variation

# STOP: Now think about how to summarise everybody
# Summarise individual patterns using a trend and variation
# by fitting simple linear model to each person
lm_stats <- function(x) {
  x$year0 <- x$year-1979
  m <- lm(mean_hourly_wage~year0, x)
  tibble(slope = coef(m)[2], 
         r2 = summary(m)$r.squared)
}

longer_lm <- longer %>%
  as_tibble() %>%
  group_by(id) %>% 
  summarise(lm_stats(cur_data())) 
ggplot(longer_lm, aes(x=r2, y=slope, label=id)) +
  geom_hline(yintercept = 0, size=2, colour="white") +
  geom_point() +
  theme(aspect.ratio = 1)
ggplotly()

increasing <- longer_lm %>%
  arrange(desc(slope)) %>%
  slice(1:12)
longer %>%
  filter(id %in% increasing$id) %>%
  ggplot() +
    geom_point(aes(x=year, y=mean_hourly_wage)) +
    geom_smooth(aes(x=year, y=mean_hourly_wage), 
                method = "lm", se=F) +
    facet_wrap(~id, ncol=4) +
    scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
    ylab("Hourly wage")

decreasing <- longer_lm %>%
  arrange(slope) %>%
  slice(1:12) 
longer %>%
  filter(id %in% decreasing$id) %>%
  ggplot() +
  geom_point(aes(x=year, y=mean_hourly_wage)) +
  geom_smooth(aes(x=year, y=mean_hourly_wage), 
              method = "lm", se=F) +
  facet_wrap(~id, ncol=4) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage")

varied <- longer_lm %>%
  arrange(r2) %>%
  slice(1:12)
longer %>%
  filter(id %in% varied$id) %>%
  ggplot() +
  geom_point(aes(x=year, y=mean_hourly_wage)) +
  geom_smooth(aes(x=year, y=mean_hourly_wage), 
              method = "lm", se=F) +
  facet_wrap(~id, ncol=4) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage")

smooth <- longer_lm %>%
  arrange(desc(r2)) %>%
  slice(1:12)
longer %>%
  filter(id %in% smooth$id) %>%
  ggplot() +
  geom_point(aes(x=year, y=mean_hourly_wage)) +
  geom_smooth(aes(x=year, y=mean_hourly_wage), 
              method = "lm", se=F) +
  facet_wrap(~id, ncol=4) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage")

ggplot(longer_lm, aes(x=r2)) +
  geom_histogram(breaks = seq(0, 1, 0.1), colour="white")
ggplot(longer_lm, aes(x=slope)) +
  geom_histogram(breaks = seq(-0.5, 4, 0.25), colour="white")
quantile(longer_lm$slope, seq(0, 1, 0.25))

# STOP: Now examine demographic variables, 
# relative to wage and time
# Education
ggplot(longer, aes(x=hgc)) +
  geom_bar() + coord_flip() +
  xlab("")

ggplot(longer, aes(x=hgc_i)) +
  geom_bar() + coord_flip() +
  xlab("")

# Too few observations in anything but 12th grade
# Combine categories
library(forcats)
longer <- longer %>%
  mutate(hgc12 = fct_collapse(hgc, 
              `9-11TH` = c("9TH GRADE","10TH GRADE","11TH GRADE"),
               `12TH` = "12TH GRADE")) %>%
  mutate(hgc12 = factor(hgc12))

longer %>%
  ggplot() +
  geom_line(aes(x=year, y=mean_hourly_wage, group=id), alpha=0.1) +
  geom_smooth(aes(x=year, y=mean_hourly_wage), se=F) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  facet_wrap(~hgc12, ncol=2) +
  scale_y_sqrt()

longer %>%
  ggplot() +
  geom_smooth(aes(x=year, y=mean_hourly_wage, 
                  colour=hgc12), se=F) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  scale_colour_brewer("educ", palette = "Dark2") +
  scale_y_sqrt()

# Gender
# NOTE: This is as the database defines gender, and names the variable. 
# There are only two values in the database, and the meaning
# is not necessarily self-defined gender
longer %>%
  ggplot() +
  geom_bar(aes(x=gender))
# What!

# Check the counts for the full data
wages_hs %>%
  ggplot() +
  geom_bar(aes(x=gender))

# Oh, notice that timing of data collection changes
# around 1995, to two year intervals
wages_hs %>%
  ggplot() +
  geom_bar(aes(x=year, fill = gender), 
           position="fill") +
  ylab("Hourly wage") +
  scale_fill_brewer("educ", palette = "Dark2") 
# Proportions roughly the same for the full data

# Race
longer %>%
  ggplot() +
  geom_bar(aes(x=race)) +
  xlab("")

longer %>%
  ggplot() +
  geom_line(aes(x=year, y=mean_hourly_wage, group=id), alpha=0.1) +
  geom_smooth(aes(x=year, y=mean_hourly_wage), se=F) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  facet_wrap(~race, ncol=3) +
  scale_y_sqrt()

longer %>%
  ggplot() +
  geom_smooth(aes(x=year, y=mean_hourly_wage, 
                  colour=race), se=F) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  scale_colour_brewer("race", palette = "Dark2") +
  scale_y_sqrt()

longer %>%
  ggplot() +
  geom_smooth(aes(x=year, y=mean_hourly_wage, 
                  colour=hgc12), se=F) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  facet_wrap(~race, ncol=3) +
  scale_colour_brewer("educ", palette = "Dark2") +
  scale_y_sqrt()

# STOP: Can we make statements like "Education matters!"
# Randomisation to check the strength of effects
# library(nullabor) # try for simpler sample designs
# If there is no difference in average wages by education
# it wouldn't matter if we "scrambled the education label"
# Need to take the id's and scramble the education variable
longer_demog <- longer %>%
  as_tibble() %>%
  select(id, hgc12) %>%
  distinct()
longer_demog_p <- longer_demog %>%
  mutate(hgc12 = sample(hgc12))
# table(longer_demog$hgc12, longer_demog_p$hgc12)

longer_lineup <- longer %>%
  as_tibble() %>%
  select(id, year, mean_hourly_wage) %>%
  left_join(longer_demog_p, by="id")

# Plot permuted data
longer_lineup %>% 
  ggplot() +
  geom_smooth(aes(x=year, y=mean_hourly_wage, 
                  colour=hgc12), se=F) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  scale_colour_brewer("educ", palette = "Dark2") +
  scale_y_sqrt()

# Make a lot of sets of permuted data
longer_lineup <- NULL
for (i in 1:12) {
  longer_demog_p <- longer_demog %>%
    mutate(hgc12 = sample(hgc12), 
           set = i)
  l <- longer %>%
    as_tibble() %>%
    select(id, year, mean_hourly_wage) %>%
    left_join(longer_demog_p, by="id")
  longer_lineup <- bind_rows(longer_lineup, l)
}
# Randomly choose a location for the data
pos <- sample(1:12, 1)
d <- longer %>%
  as_tibble() %>%
  select(id, year, mean_hourly_wage, hgc12) %>%
  mutate(set = pos)
longer_lineup[longer_lineup$set == pos,] <- 
  d
# Plot lineup
longer_lineup %>% 
  ggplot() +
  geom_smooth(aes(x=year, y=mean_hourly_wage, 
                  colour=hgc12), se=F) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage") +
  scale_colour_brewer("educ", palette = "Dark2") +
  scale_y_sqrt() +
  theme(legend.position = "none") +
  facet_wrap(~set, ncol=4)
# is the data plot detectable?

# STOP: Adding more sophisticated interaction
# tsibbletalk
library(tsibbletalk)
library(feasts)
longer_shared <- as_shared_tsibble(longer, spec = id)
longer_shared_lm <- longer_shared %>%
  features(mean_hourly_wage, feat_stl)
p_l <- longer_shared %>% 
  ggplot(aes(x=year, y=mean_hourly_wage, group=id)) +
  geom_line(alpha=0.1) +
  scale_x_continuous("", breaks = seq(1980, 2020, 10),
                     labels = c("80", "90", "00", "10", "20")) +
  ylab("Hourly wage")
p_f <- longer_shared_lm %>% 
  ggplot() +
  geom_point(aes(x=trend_strength, y=spikiness, group=id)) +
  scale_y_log10()
subplot(
  ggplotly(p_l, tooltip = "id", width = 700, height = 400),
  ggplotly(p_f, tooltip = "id", width = 700, height = 400),
  nrows = 1) %>%
  highlight(dynamic = TRUE)

# That's all for now - thanks for listening
# I hope you learned a bit about the wage volatility 
# for high school dropouts, and also a few insights
# into exploring your data
