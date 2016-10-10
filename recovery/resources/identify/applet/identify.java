import processing.core.*; 
import processing.xml.*; 

import java.applet.*; 
import java.awt.*; 
import java.awt.image.*; 
import java.awt.event.*; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class identify extends PApplet {

// some variables that are being used throughout the program
int kMargin = 25;
int bgColor = 255; //white //#D9E8FD; // medium grey
PFont font;

int n;
int highlight=-1;

// map coordinates
int[] mapX, mapY;
float[] gpsX, gpsY;
int[] group;
float maxX, minX, maxY, minY;
int map_n;

// city coordinates
int city_n, kSize=5;
int[] ctX, ctY;
float[] gpsCtX, gpsCtY,  ctPop;
String[] ctName;

public void readCity(String filename) {
  // load data from local file
  String lines[] = loadStrings(filename);

  println("there are " + lines.length + " cities");
  city_n = lines.length;    
  
  ctX = new int[city_n];
  ctY = new int[city_n];  
  gpsCtX = new float[city_n];
  gpsCtY = new float[city_n];
  ctName = new String[city_n];
  ctPop = new float[city_n];
  
  // read data from file
  for (int i=0; i < lines.length; i++) {
//        println(lines[i]);
    String list[] = split(lines[i], ',')   ; 
    // list[0] longitude
    // list[1] latitude
    // list[3] order (values should be in order)

    ctName[i] = list[0];
    ctPop[i] = PApplet.parseFloat(list[2]);
    gpsCtX[i] = PApplet.parseFloat(list[4]);
    gpsCtY[i] = PApplet.parseFloat(list[3]);
    
//println(i+" "+gpsX[i]+" "+gpsY[i]);
  }
//println("max: "+maxX+", min: "+minX);
//println("max: "+maxY+", min: "+minY);
// get screen coordina tes
  for (int i = 1; i < city_n; i++) {
    ctX[i] = getXCoord(gpsCtX[i]);
    ctY[i] = getYCoord(gpsCtY[i]);
//println(i+" "+mapX[i]+" "+mapY[i]);
  }  

}

public void readMap(String filename) {
  // load data from local file
  String lines[] = loadStrings(filename);

  println("there are " + lines.length + " lines");
  map_n = lines.length;    

  mapX = new int[map_n];
  mapY = new int[map_n];  
  gpsX = new float[map_n];
  gpsY = new float[map_n];
  group = new int[map_n];
  
  // read data from file
  for (int i=0; i < lines.length; i++) {  
    // list[3] order (values should be in order)
    String list[] = split(lines[i], ',')   ; 
    
    gpsX[i] = PApplet.parseFloat(list[0]);
    gpsY[i] = PApplet.parseFloat(list[1]);
    group[i] = PApplet.parseInt(list[2]);
    
    if (i == 1) {
      maxX = minX = gpsX[1];
      maxY = minY = gpsY[1];
    }
    
    if (gpsX[i] < minX) minX = gpsX[i];
    if (gpsX[i] > maxX) maxX = gpsX[i];

    if (gpsY[i] < minY) minY = gpsY[i];
    if (gpsY[i] > maxY) maxY = gpsY[i];
//println(i+" "+gpsX[i]+" "+gpsY[i]);
  }
//println("max: "+maxX+", min: "+minX);
//println("max: "+maxY+", min: "+minY);
// get screen coordinates
  for (int i = 1; i < map_n; i++) {
    mapX[i] = getXCoord(gpsX[i]);
    mapY[i] = getYCoord(gpsY[i]);
//println(i+" "+mapX[i]+" "+mapY[i]);
  }  
}

public void drawMap() {
  // draw shape between lower and upper quartile
  fill(200);
  stroke(255);
//  noStroke();
  beginShape();
  int curGroup=group[1];

  for (int i = 1; i < map_n; i++) {
    if (group[i] != curGroup) {
      curGroup=group[i];
      endShape(CLOSE);
      beginShape();
    }
    vertex(mapX[i],mapY[i]);
  }
  endShape(CLOSE);

}

public void drawCity() {
  // draw shape between lower and upper quartile
  fill(204,0,0);
  noStroke();

  for (int i = 1; i < city_n; i++) {
    ellipse(ctX[i], ctY[i], kSize, kSize);
  }
}


public void setup() {
  size(600, 400);
  background(bgColor);

//  readMap("iowa.csv");
  readMap("IA-counties.csv");
  readCity("IA-cities.csv");

//  font = createFont("CourierNew", 36); //
  font = createFont("FFScala", 32);
  textAlign(CENTER);

  // Set the font and its size (in units of pixels)
  textFont(font, 18);
}

public int getXCoord (float xval) {
  int w = width - 2*kMargin;
  return(kMargin+round(1.0f*(xval-minX)/(maxX-minX)*w));
}

public int getYCoord (float yval) {
  int h = height - 2*kMargin;
  return(kMargin + h- round(1.0f*(yval-minY)/(maxY-minY)*h));
}

public int onCircle() {
  for (int i = 1; i < city_n; i++) {
    if (overCircle(ctX[i], ctY[i], 2*kSize)) return(i);
  }

  return(-1);
}

public boolean overCircle(int x, int y, int diameter) {
  float disX = x - mouseX;
  float disY = y - mouseY;
  if(sqrt(sq(disX) + sq(disY)) < diameter/2 ) {
    return true;
  } 
  else {
    return false;
  }
}

public void draw() {
  // return index of data, if mouse is on one of the points  
//  highlight = onCircle();
  fill(bgColor);
  rect(0,0,width, height);
  
  fill(204,0,0);
  stroke(0);
  drawMap();
  drawCity();

  highlight=onCircle();
  if (highlight>-1) {
    fill(50);
    ellipse(ctX[highlight], ctY[highlight], 2*kSize, 2*kSize);
    
// add additional text here
    fill(50);
    int ctx = 20;
    int cty = height-50;
    textAlign(LEFT);
    text(ctName[highlight], ctx, cty);
    text("Award Amount: "+ctPop[highlight], ctx, cty+20);
//    text(ctName[highlight], ctX[highlight], ctY[highlight]);
//    text("Population: "+ctPop[highlight], ctX[highlight], ctY[highlight]+20);
  } 

  // Hello Mouse
//  fill(0);
//  ellipse(mouseX, mouseY, 3,3);
}

public void mousePressed() {
  fill(bgColor);
  rect(0,0,width, height);
}





  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#c0c0c0", "identify" });
  }
}
