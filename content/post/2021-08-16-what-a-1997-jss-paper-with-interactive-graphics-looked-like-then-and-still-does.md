---
title: What a 1997 JSS paper with interactive graphics looked like then and still does
author: Di Cook
date: '2021-08-15'
slug: content/post/2021-08-16-what-a-1995-jss-looked-like-with-interactive-graphics
categories: 
  - data visualisation
  - statistics
tags: 
  - R
  - S
  - statistical computing
  - statistical graphics
  - tour
  - animation
  - multivariate data
header:
  caption: ''
  image: ''
---

In 1997, I wrote a paper that was accepted for the newly fledged Journal of Statistical Software. That article is still available at https://www.jstatsoft.org/article/view/v002i06. It looks nothing like the original published paper. Papers today are only published in pdf format, unlike the original which were delivered as html, with pdf being a secondary format o help readers who preferred a printed copy.

My paper was titled "Calibrate your to Recognise High-Dimensional Shapes from their Low-Dimensional Projections". The ability to publish interactive graphics Interactive was an important part of the mission of JSS, and I was really keen to help with this effort. Thus, my paper had embedded animated gifs, and links that if your computer and its browser was appropriately set up could run the examples natively in [XGobi](https://en.wikipedia.org/wiki/GGobi) or [XLispStat](https://en.wikipedia.org/wiki/XLispStat). The data for the examples, and all the code to produce the data using [S](https://en.wikipedia.org/wiki/S_(programming_language)) were linked to the article. 

If you want to take a look at how it looked, [I'm posting it here](https://dicook.org/files/JSS-paper/paper.html). The animated gifs still work! They look very clunky, and I have no idea how I originally created them. Running the other examples using XGobi or XLispStat from the browser don't work, but you can still see the S code and download the data. With a bit of work you could regenerate them and run the examples using the [tourr](https://cran.r-project.org/web/packages/tourr/index.html) package. 


