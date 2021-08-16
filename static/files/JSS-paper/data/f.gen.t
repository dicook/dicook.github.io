f.gen.t<-function(n = 100, p = 5, mu = 5)
{
        x <- matrix(rnorm(n * p), ncol = p)
        x<-t(apply(x,1,norm.vec))
        y <- rt(n, mu)  #    x<-t(apply(x,1,norm.vec))
        for(i in 1:n)
                x[i,  ] <- x[i,  ]*y[i]
        return(x)
}
