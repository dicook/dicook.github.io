      rm(list=ls())

      library(shiny)
      library(ggvis)
source("definitions.R")      
      
# Define UI for distribution application
    shinyUI(fluidPage(

# Application title
         titlePanel("Finding your way again"),

# Sidebar with controls to select subset
          fluidPage(

#  Display your plot created by GGvis        
            mainPanel(ggvis_output("my_plot"))

          )
        ))
