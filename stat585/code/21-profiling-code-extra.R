stopifnot <- function (...) 
{
  k <- length(ll <- list(...))
  if (k == 0L) 
    return(invisible())
  mc <- match.call()
  for (i in 1L:k) if (!(is.logical(r <- ll[[i]]) && !any(is.na(r)) && 
                          all(r))) {
    ch <- deparse(mc[[i + 1]], width.cutoff = 60L)
    if (length(ch) > 1L) 
      ch <- paste(ch[1L], "....")
    stop(paste(ch, " is not ", if (length(r) > 1L) 
      "all ", "TRUE", sep = ""), call. = FALSE)
  }
  invisible()
}

stopifnot2 <- function (...) 
{
  browser()
  k <- length(ll <- list(...))
  if (k == 0L) 
    return(invisible())
  mc <- match.call()
  for (i in 1L:k) if (!(is.logical(r <- ll[[i]]) && !any(is.na(r)) && 
                          all(r))) {
    ch <- deparse(mc[[i + 1]], width.cutoff = 60L)
    if (length(ch) > 1L) 
      ch <- paste(ch[1L], "....")
    stop(paste(ch, " is not ", if (length(r) > 1L) 
      "all ", "TRUE", sep = ""), call. = FALSE)
  }
  invisible()
}

stopifnot3 <- function (...) 
{
  k <- length(ll <- list(...))
  if (k == 0L) 
    return(invisible())
  mc <- match.call()
  for (i in 1L:k) if (!(is.logical(r <- ll[[i]]) && !any(is.na(r)) && 
                          all(r))) {
    browser()
    ch <- deparse(mc[[i + 1]], width.cutoff = 60L)
    if (length(ch) > 1L) 
      ch <- paste(ch[1L], "....")
    stop(paste(ch, " is not ", if (length(r) > 1L) 
      "all ", "TRUE", sep = ""), call. = FALSE)
  }
  invisible()
}

x <- "TRUE"
stopifnot(x == x, 1+1==2, c(7+5==11.99999, 1+1==2))
stopifnot2(x == x, 1+1==2, c(7+5==11.99999, 1+1==2))
stopifnot3(x == x, 1+1==2, c(7+5==11.99999, 1+1==2))

f <- function(x) { 
  w(x)
  g(h(x)) 
  w(x)
} 
g <- function(x) {
  a <- 10
  x
} 

h <- function(x) {
  w(x) 
  w(x) 
}

w <- function(x) { 
  if (sample(10, 1) == 1) stop("This is an error!")
}

f() 
traceback()

options(error=recover)
f() 
options(error=NULL)
options(warn=2)
options(warn=-1)

larger <- function(x, y) { 
  y.is.bigger <- y > x 
  x[y.is.bigger] <- y[y.is.bigger] 
  x
} 
larger(c(1, 5, 10), c(2, 4, 11)) 
larger(c(1, 5, 10), 6)

debug(larger)
larger(c(1, 5, 10), 6)
undebug(larger)