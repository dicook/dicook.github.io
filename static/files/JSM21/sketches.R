load("data/sketches_train.rda")

sketches_pc <- prcomp(sketches[,1:784], scale=FALSE)$x[,1:6]
sketches_pc <- as.data.frame(sketches_pc)
sketches_pc$sketch <- sketches[,785]

library(tourr)
library(tidyverse)
sketches_pc_small <- sketches_pc %>%
  group_by(sketch) %>%
  sample_n(100) %>%
  ungroup()

ggplot(sketches_pc_small, aes(x=PC1, y=PC2, colour = sketch)) +
  geom_point() +
  scale_colour_viridis_d() +
  theme_bw()

animate_xy(sketches_pc_small[,1:6], col=sketches_pc_small$sketch)

render_gif(sketches_pc_small[,1:6], grand_tour(), 
           display_xy(axes = "bottomleft", 
                      col=sketches_pc_small$sketch,
                      half_range = 0.8),
           "sketches.gif",
           frames = 500)

