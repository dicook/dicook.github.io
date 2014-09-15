# clear memory & load packages    
        rm(list=ls())

        library(shiny)
        library(ggvis)
 source("definitions.R")        
        
# Define server logic for distribution application

        shinyServer(function(input, output, session) {

# Reactive expression to generate the subset.      
  vals <- reactiveValues(dataset=us.cities)

hover <- function(x) {
  isolate({
  idx <- which(vals$dataset$long==x[1] & vals$dataset$lat==x[2])
  hstring <-   paste(vals$dataset$name[idx], collapse=",")
  })

  hstring
}

clicked <- function(x) {
  isolate({
    idx <- which(vals$dataset$long==x[1] & vals$dataset$lat==x[2])
    vals$dataset$tour[idx] <- "included"
    vals$dataset$order[idx] <- max(vals$dataset$order)+1
    print(paste(vals$dataset$name[idx], collapse=","))
  })
}
# Generate your plot using GGvis and your reactive inputs      
          gv <- reactive({ 
            p2 <- ggvis(vals$dataset,
                  props(x = ~long,
                        y = ~lat,
                        stroke = ~tour,
                        fillOpacity := 0.2,
                        fillOpacity.hover := 0.7)) +
              layer_point(props(fill = ~tour)) + 
              tooltip(hover) + 
              click_tooltip(clicked) +
              layer_path(props(stroke := 'steelblue'), 
                         data = subset(vals$dataset[order(vals$dataset$order),], order > 0)[])
          p1 +p2
          })


# necessary additions for ggvis integration to shiny        
          output$controls <- renderControls(gv)
          observe_ggvis(gv, "my_plot", session)               
        })
        