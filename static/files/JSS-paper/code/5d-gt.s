# 5D gt is shown as parallel coordinate plots
motif()
# Read in your data
cube9<-matrix(scan("cube9"),ncol=9,byrow=T)
# Read in projection vectors
x<-matrix(scan("gt_proj"),ncol=5,byrow=T)
par(mfrow=c(5,5),pty="s")
for (k in 1:6)
{
  for (i in ((k-1)*25+1):(k*25))
  {
    y<-cbind(cube9%*%x[((i-1)*9+1):(i*9),1:5],rep(NA,512))
    y<-as.vector(t(y))
    plot(rep(c(1:5,NA),512),y,xlab="",ylab="",
      type="l",ylim=c(-3,3))
  }
  cat("ready for new plot? \n")
  n<-scan()
}
