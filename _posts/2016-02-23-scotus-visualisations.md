---
layout: post
title: "SCOTUS over the years"
author: Heike
tags: [data visualization,statistics,EDA,data mining,R,statistical computing,statistical graphics,data wrangling]
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

![](2016-02-23-scotus-visualisations_files/figure-markdown_strict/unnamed-chunk-2-1.png)

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

<img src="2016-02-23-scotus-visualisations_files/figure-markdown_strict/unnamed-chunk-3-1.png" title="" alt="" width="600" />

Another question that we might be asking is, how often justices in the
modern era leave the office during an election year. This is shown in
the chart below, where the number of justices leaving the office is
broken down by the year of the presidential term. Distinguishing the pre
and post 1920 era, we see that there is actually a trend for justices in
the post 1920 era to leave office during the beginning of the
presidential term.

<img src="2016-02-23-scotus-visualisations_files/figure-markdown_strict/unnamed-chunk-4-1.png" title="" alt="" width="600" />

Accordingly, the number of supreme court nominations a president makes
during his term in the post 1920 era is skewed towards the first years
of his presidency, as can be seen in the chart below.

<img src="2016-02-23-scotus-visualisations_files/figure-markdown_strict/unnamed-chunk-5-1.png" title="" alt="" width="600" />

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

<!--
  - John Adams nominated John Marshall on January 20 1801, fifteen days before Thomas Jefferson took office - and the senate approved the nomination on January 27 for fear "lest another not so qualified, and more disgusting to the Bench, should be substituted, and because it appeared that this gentleman [Marshall] was not privy to his own nomination" (Senator Jonathan Dayton). Not exactly a "delay, delay, delay" tactic, even though the seven days were already seen as delaying the confirmation.

  - President Andrew Jackson nominated two(!) supreme court justices on his last full day in office. Catron accepted the nomination and was confirmed by the Congress five days later.

  - President Van Buren nominated Peter Vivan Daniel during his last week as president, and Daniel was confirmed by the senate on March 2, 1841.
  
  - On February 4, 1845, Nelson was nominated by President John Tyler to a seat as an Associate Justice on the Supreme Court, it took the Senate only a couple of days to confirm his appointment.
  
  - President Fillmore attempted to fill Justice McKinley's seat four times, but only his successor was successful in that.
  
  - Howell Edmunds Jackson's  nomination for the supreme court was announced on February 2, 1893 by outgoing president B. Harrison.
  
  - President Warren Harding nominated Sanford to the Supreme Court on January 24, 1923, to the seat vacated by Mahlon Pitney. Sanford was confirmed by the Senate, and received his commission, on January 29, 1923.
  



```
##                          Judge   President.x President.y
## 15          Bushrod Washington      J. Adams       Adams
## 16                Alfred Moore      J. Adams       Adams
## 17                    John Jay      J. Adams       Adams
## 18               John Marshall      J. Adams   Jefferson
## 28              Robert Trimble   J. Q. Adams       Adams
## 29          John J. Crittenden   J. Q. Adams       Adams
## 36                 John Catron       Jackson       Buren
## 37               William Smith       Jackson       Buren
## 38               John McKinley     Van Buren       Buren
## 39         Peter Vivian Daniel     Van Buren       Tyler
## 47               Samuel Nelson         Tyler        Polk
## 48                John M. Read         Tyler        Polk
## 54        George Edmund Badger      Fillmore      Pierce
## 55            William C. Micou      Fillmore      Pierce
## 58           Jeremiah S. Black      Buchanan     Lincoln
## 64              Henry Stanbery    A. Johnson     Johnson
## 75            Stanley Matthews         Hayes      Arthur
## 76            Stanley Matthews      Garfield      Arthur
## 82         David Josiah Brewer   B. Harrison    Harrison
## 83        Henry Billings Brown   B. Harrison    Harrison
## 84          George Shiras, Jr.   B. Harrison    Harrison
## 85      Howell Edmunds Jackson   B. Harrison   Cleveland
## 92  Oliver Wendell Holmes, Jr.  T. Roosevelt   Roosevelt
## 93              William R. Day  T. Roosevelt   Roosevelt
## 94         William Henry Moody  T. Roosevelt   Roosevelt
## 108       Edward Terry Sanford       Harding    Coolidge
## 114                 Hugo Black  F. Roosevelt   Roosevelt
## 115        Stanley Forman Reed  F. Roosevelt   Roosevelt
## 116          Felix Frankfurter  F. Roosevelt   Roosevelt
## 117         William O. Douglas  F. Roosevelt   Roosevelt
## 118               Frank Murphy  F. Roosevelt   Roosevelt
## 119            Harlan F. Stone  F. Roosevelt   Roosevelt
## 120            James F. Byrnes  F. Roosevelt   Roosevelt
## 121          Robert H. Jackson  F. Roosevelt   Roosevelt
## 122      Wiley Blount Rutledge  F. Roosevelt   Roosevelt
## 135                 Abe Fortas    L. Johnson     Johnson
## 136          Thurgood Marshall    L. Johnson     Johnson
## 137                 Abe Fortas    L. Johnson     Johnson
## 138           Homer Thornberry    L. Johnson     Johnson
## 151               David Souter G. H. W. Bush        Bush
## 152            Clarence Thomas G. H. W. Bush        Bush
## 155            John G. Roberts    G. W. Bush        Bush
## 156            John G. Roberts    G. W. Bush        Bush
## 157              Harriet Miers    G. W. Bush        Bush
## 158               Samuel Alito    G. W. Bush        Bush
```

```
## , , OutAlive = Death, modern2 = pre 1920
## 
##                factor(Party.y)
## factor(Party.x) Democrat Republican Whig
##      Democrat          2          7    4
##      Republican        5          9    0
## 
## , , OutAlive = Resignation/Retirement, modern2 = pre 1920
## 
##                factor(Party.y)
## factor(Party.x) Democrat Republican Whig
##      Democrat          2          6    0
##      Republican        2         12    0
## 
## , , OutAlive = Death, modern2 = post 1920
## 
##                factor(Party.y)
## factor(Party.x) Democrat Republican Whig
##      Democrat          3          2    0
##      Republican        3          2    0
## 
## , , OutAlive = Resignation/Retirement, modern2 = post 1920
## 
##                factor(Party.y)
## factor(Party.x) Democrat Republican Whig
##      Democrat          5          8    0
##      Republican        8         10    0
```
-->
### Data and such

All data was scraped from various website and I have taken care to not
fish out any inconsistencies and not introduce any mistakes. I still
might have, so if you find any problems with the data, just email and
we'll get the problems fixed. Here's the csv files for all of the data
used in the above charts:

-   [Supreme court justices and nominations](justices-nominations.csv)
-   [Presidents of the United States](presidents.csv)
