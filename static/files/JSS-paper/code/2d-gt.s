# 2D gt is shown as scatterplots
motif()
# Read in your data
cube9<-matrix(scan("cube9"),ncol=9,byrow=T)
# Read in projection vectors
x<-matrix(scan("gt_proj"),ncol=2,byrow=T)
par(mfrow=c(5,5),pty="s")
for (k in 1:6)
{
  for (i in ((k-1)*25+1):(k*25))
    plot(cube9%*%x[((i-1)*9+1):(i*9),1:2],axes=F,xlab="",ylab="",pch=16)
  cat("ready for new plot? \n")
  n<-scan()
}
