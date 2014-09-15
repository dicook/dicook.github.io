##' get the average spectrum from same group
##'
##' This function calculates the average spectrum from group 'gname'
##'   in which there are serveal spectra
##' @author Jelly Bean
##' @param data long format data frame with 4 or 5 variables: group, (sample), spec.no, WtNo and Intensity
##' @param gname group name
##' @param sname sample name
##' @examples load('spectra1.rda')
##' load('spectra2.rda')
##' sp_average1 <- average_sp(sp1, gname = 'TYPE1')
##' sp_average1 <- average_sp(sp1)
##' sp_average2 <- average_sp(sp2, gname = 'TYPE2')
average_sp <- function(data, gname = gname, sname = "all") {
    library("ggplot2")
    data_orig <- data
    if (length(data$sample) == 0) {
        flag <- (data_orig$group == data_orig$group[1]) & (data_orig$spec.no == 
            data_orig$spec.no[1])
        data <- subset(data, group == gname)
        data <- t(cbind(data$WtNo, data$Intensity))
        wt_no <- data_orig$WtNo[flag]
        n <- length(wt_no)
        Intensity <- data[2, ]
        spec_no <- dim(data)[2]/length(wt_no)
        average <- as.vector(matrix(0, 1, n))
        sum <- matrix(0, 1, n)
        for (i in 1:n) {
            for (j in 1:spec_no) {
                sum[i] <- sum[i] + Intensity[(i + (j - 1) * n)]
            }
            average[i] <- sum[i]/spec_no
        }
        print(qplot(wt_no, average, main = paste("Average Spectrum for", 
            gname), geom = "line"))
        cat("Number of groups for", gname, ":", spec_no, "\n")
    } else if (length(data$sample) != 0 & sname == "all") {
        flag <- (data_orig$group == data_orig$group[1]) & (data_orig$spec.no == 
            data_orig$spec.no[1]) & (data_orig$sample == data_orig$sample[1])
        data <- subset(data, group == gname)
        data <- t(cbind(data$WtNo, data$Intensity))
        wt_no <- data_orig$WtNo[flag]
        n <- length(wt_no)
        Intensity <- data[2, ]
        spec_no <- dim(data)[2]/length(wt_no)
        average <- matrix(0, 1, n)
        sum <- matrix(0, 1, n)
        for (i in 1:n) {
            for (j in 1:spec_no) {
                sum[i] <- sum[i] + Intensity[(i + (j - 1) * n)]
            }
            average[i] <- sum[i]/spec_no
        }
        cat(dim(average), "\n")
        print(qplot(wt_no, average, main = paste("Average Spectrum for", 
            gname), geom = "line"))
        cat("Number of groups for", gname, ":", spec_no, "\n")
    } else {
        flag <- (data_orig$group == data_orig$group[1]) & (data_orig$spec.no == 
            data_orig$spec.no[1]) & (data_orig$sample == data_orig$sample[1])
        data <- subset(data, group = gname, sample == sname)
        data <- t(cbind(data$WtNo, data$Intensity))
        wt_no <- data_orig$WtNo[flag]
        n <- length(wt_no)
        Intensity <- data[2, ]
        spec_no <- dim(data)[2]/length(wt_no)
        average <- matrix(0, 1, n)
        sum <- matrix(0, 1, n)
        for (i in 1:n) {
            for (j in 1:spec_no) {
                sum[i] <- sum[i] + Intensity[(i + (j - 1) * n)]
            }
            average[i] <- sum[i]/spec_no
        }
        print(qplot(wt_no, average, main = paste("Average Spectrum for", 
            gname), geom = "line"))
        cat("Number of groups for", gname, sname, ":", spec_no, "\n")
    }
    
    data <- rbind(wt_no, average)
    data <- t(data)
    colnames(data) <- c("WtNo", "Average Intensity")
    data <- as.data.frame(data)
    return(data)
} 
