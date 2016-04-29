library(plyr)
tips.new <- ddply(tips[c("bill","tip","sex","rate")], "sex", mean)
tips.new <- ddply(tips, "sex", summarize, mean.bill=mean(bill),
                  mean.tip=mean(tip),
                  mean.rate=mean(rate))
# This is a function that calculates a 95\% confidence interval for a vector of data values.
ci95 <- function(x) {
  xmn <- mean(x)
  xsd <- sd(x)
  qt <- qt(0.975, length(x))
  return(data.frame(lower=xmn-qt*xsd/sqrt(length(x)), upper=xmn+qt*xsd/sqrt(length(x))))
}
ci95(tips$rate)
# But this throws an error if we try to run it on anything but a numeric vector
ci95(tips$day)
# An improved function that has safety checks
ci95 <- function(x) {
  if (is.numeric(x)) {
    xmn <- mean(x)
    xsd <- sd(x)
    qt <- qt(0.975, length(x))
    return(data.frame(lower=xmn-qt*xsd/sqrt(length(x)), upper=xmn+qt*xsd/sqrt(length(x))))
  }
  else
    cat("x needs to be a numeric vector\n")
}
ci95(tips$rate)
ci95(tips$day)
# Allowing users to change the % value
ci95 <- function(x, pct=0.95) {
  if (is.numeric(x)) {
    xmn <- mean(x)
    xsd <- sd(x)
    qt <- qt((1-pct)/2, length(x))
    return(data.frame(lower=xmn-qt*xsd/sqrt(length(x)), upper=xmn+qt*xsd/sqrt(length(x))))
  }
  else
    cat("x needs to be a numeric vector\n")
}
ci95(tips$rate)
ci95(tips$rate, pct=0.8)