f.T2.confidence<-function(x,n=100,cl=0.95){
  xm<-apply(x,2,mean)
  p<-dim(x)[2]
  xn<-dim(x)[1]
  xv<-var(x)
  ev<-eigen(xv)
  sph<-matrix(rnorm(n*p),ncol=p)
  cntr<-t(apply(sph,1,f.norm.vec))
  cntr<-cntr%*%diag(sqrt(ev$values))%*%t(ev$vectors)
  cntr<-cntr*sqrt(p*(xn-1)*qf(cl,p,xn-p)/(xn*(xn-p)))
  cntr<-cntr+matrix(rep(xm,n),nrow=n,byrow=T)
  return(cntr)
}

f.norm.vec<-function(x){
  nrm<-sqrt(sum(x^2))
  return(x/nrm)
}
