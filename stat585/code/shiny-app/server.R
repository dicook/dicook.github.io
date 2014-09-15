library(shiny)
library(datasets)

# Define server logic required to summarize and view the selected dataset
shinyServer(function(input, output) {
  
  inputData <- reactive({
    switch(input$dataset,
                "rock" = rock,
                "pressure" = pressure,
                "cars" = cars), n = input$obs)
  })
  
  # Generate a summary of the dataset
  output$summary <- renderPrint({
    dataset <- inputData()
    summary(dataset)
  })
  
  # Show the first "n" observations
  output$view <- renderTable({
    head(inputData())
  })
})