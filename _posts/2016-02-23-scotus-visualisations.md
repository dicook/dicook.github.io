---
layout: post
title: "SCOTUS over the years"
author: heike
tags: [politics,statistics,EDA,data mining,R,statistical computing,statistical graphics,data wrangling]
---
{% include JB/setup %}

In the days since the death of Justice Antonin Scalia, there has been a
lot of discussion on what is going to happen now - whether President
Obama should or should not nominate a candidate to fill the vacancy in
the supreme court. As I write this, FoxNews reports that Americans are
almost
[2:1](http://www.foxnews.com/politics/interactive/2016/02/18/fox-news-poll-national-presidential-race-february-18-2016/)
in favor of a nomination by President Obama, politifact has rated the
claim from the Republican rumour mill of an \`80 year old tradition to
not nominate a supreme court candidate during an election' as [half
right](http://www.politifact.com/truth-o-meter/article/2016/feb/17/misleading-notion-supreme-court-vacancy-hasnt-been/)
(which could also be read as half wrong, just to indicate my side of
things). So what better way to add to the general hubbub than to look at
some actual data?
<p>
Here goes: the first chart is an overview of the two major time series
in question: in black and white from left to right we have the Chief
Justice line and the ten Associate Justice (AJ) lines on a time line
since the establishment of the supreme court to the present day. The
width of the rectangle indicates the time served in office, the fill
colour helps with the distinction of whether the justice died while in
office (in black), retired or resigned (both in white). Currently
serving justices are also shown with white rectangles. It can be seen
immediately that since the early 19 hundreds it has become more and more
the norm for justices to retire or resign from their office than to
serve until death. It has also been pointed out by politifact that the
last supreme court justice to be vacated, nominated and confirmed in an
election year was 84 years ago in 1932 (when President Herbert Hoover
nominated Benjamin Cardozo as a replacement for retiring justice Oliver
Wendell Holmes).

![]({{ site.url }}/assets/SCOTUS_files/unnamed-chunk-2-1.png)

Russell Wheeler (Brookings Institution) is quoted saying "Justices in
the modern era rarely die in office." There does not seem to be a
readily available definition of modern era for the United States - but
if we define the modern era as starting post World War I or maybe with
the suffrage movement: women were first allowed to vote in the elections
of 1920, we can definitely agree with Wheeler. As shown below, we see
that the number of deaths while in office outnumbered the resignations
and retirements in the pre 1920 era, while since then only 11 justices
have died in office, and far more have retired (or resigned). Currently
serving justices are excluded from the chart - but note that some
justices are counted twice: whenever a justice serves into multiple
offices, so e.g. when an associate justice later becomes chief justice,
he is counted twice in the chart (e.g. William Rehnquist served as
Associate Justice from 1972 onwards until becoming Chief Justice in
1986. In the chart below, he shows up as having resigned from the AJ
seat, and later as having died while serving as CJ).

![]({{ site.url }}/assets/SCOTUS_files/unnamed-chunk-3-1.png)

Another question that we might be asking is, how often justices in the
modern era leave the office during an election year. This is shown in
the chart below, where the number of justices leaving the office is
broken down by the year of the presidential term. Distinguishing the pre
and post 1920 era, we see that there is actually a trend for justices in
the post 1920 era to leave office during the beginning of the
presidential term.

![]({{ site.url }}/assets/SCOTUS_files/unnamed-chunk-4-1.png)

Accordingly, the number of supreme court nominations a president makes
during his term in the post 1920 era is skewed towards the first years
of his presidency, as can be seen in the chart below.

![]({{ site.url }}/assets/SCOTUS_files/unnamed-chunk-5-1.png)

### Retirement plans? - above the party!

<!--Another question worth asking is, whether or not supreme court justices tend to leave office by retiring or resigning when a supporting party president is in office.-->
Another question worth asking is, whether supreme court justices tend to
leave office by retiring or resigning when a president from the same
party is in office as when they were nominated. Visually this means that
the white-filled rectangles start and end in an area of the same color.
And there are surprisingly few of them, i.e. there is not a lot of
evidence that supreme court justices are exercising this strategy when
retiring from office.

### Election year nomination? Eleventh hour nominations!!

Not only did former presidents nominate supreme court justices in their
last year of office, if need be, some of them did so during the last
days of their term: Presidents John Adams, Andrew Jackson, Van Buren,
John Tyler, John Fillmore, Benjamin Harrison, and Warren Harding all
made nominations for supreme court justices in their final days of
office, long after the next President was elected. All but President
Fillmore could get confirmations from the Senate for their candidate.

So rather than if - we should focus on who will President Barack Obama
nominate for the vacant seat?

### Data and such

All data was scraped from various website and I have taken care to not
fish out any inconsistencies and not introduce any mistakes. I still
might have, so if you find any problems with the data, just email and
we'll get the problems fixed. Here's the csv files for all of the data
used in the above charts:

-   [Supreme court justices and
    nominations](assets/SCOTUS_files/justices-nominations.csv)
-   [Presidents of the United
    States](assets/SCOTUS_files/presidents.csv)

Data and figures were scraped/processed/made with the R packages: knitr,
rvest, lubridate, ggplot2. Thank you!
