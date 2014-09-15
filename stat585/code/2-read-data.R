
## ----setup, include=FALSE------------------------------------------------
opts_chunk$set(cache=TRUE)
options(width=90)


## ----, eval=FALSE--------------------------------------------------------
## # A function which calculates HotellingsT2 statistic for testing a hypothesis about a multivariate mean vector.
## f.HotellingsT2<-function(x,mu0){
## xm<-apply(x,2,mean)
## xv<-var(x)
## ev<-eigen(xv)
## xvinv<-ev$vectors%*%diag(1/ev$values)%*%t(ev$vectors)
## t2<-t(as.matrix(xm-mu0))%*%xvinv%*%as.matrix(xm-mu0)
## t2<-t2*nrow(x)
## return(t2)
## }


## ----, eval=FALSE--------------------------------------------------------
## # A function which calculates HotellingsT2 statistic for testing a hypothesis about a
## # multivariate mean vector.
## f.HotellingsT2 <- function(x, mu0) {
##     xm <- apply(x, 2, mean)
##     xv <- var(x)
##     ev <- eigen(xv)
##     xvinv <- ev$vectors %*% diag(1/ev$values) %*% t(ev$vectors)
##     t2 <- t(as.matrix(xm - mu0)) %*% xvinv %*% as.matrix(xm - mu0)
##     t2 <- t2 * nrow(x)
##     return(t2)
## }


## ----, eval =FALSE-------------------------------------------------------
## # get the current working directory
## getwd()
## setwd("path to working directory") # Windows users beware the tricky use of \\


