# Addition
2 + 2
# Subtraction
15 - 7
# Multiplication
109*23452
# Division
3/7
# Integer division
7 %/% 2
# Modulo operator (Remainder)
7 %% 2
# Powers
1.5^3
# Roots
sqrt(25)
25^.5
25^(1/2)
# constant e, and powers of e
exp(1)
exp(2)
# same as
exp(1)^2
# logarithms
# log gives natural log
log(exp(2))
# log10 gives log base 10
log10(100)
# log base 2
log2(10)
# Can't do it for every base
log3(10) 
# gives error, use this
log(10,base = 3)
# Trigonometric functions
pi
sin(pi)
cos(pi)
tan(pi)
# Cleaning up 
round(2.579)
round(-2.579)
round(-2.579, digits = 2)
signif(2.579, 2)
signif(23511,2)
# Basic stats
sum(c(1, 5, 7)) # c means collect these elements together
mean(c(1, 5, 7))
?sum

# Indexing elements of a vector or matrix
# Generate a sample of size 50 from a normal distribution
x <- rnorm(50)
x
# 10th element
x[10]
# 10-15th elements
x[10:15]
# every second element
x[seq(1, 100, 2)]
# Generate another factor variable
y <- c(rep("male", 15), rep("female", 35))
# Subset x by y
x[y=="male"]
# Find the sample quantiles/percentiles of x
quantile(x, seq(0, 1, 0.1))
