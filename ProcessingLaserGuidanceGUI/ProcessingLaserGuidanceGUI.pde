import controlP5.*; //import ControlP5 library
import processing.serial.*;

Serial port;
float val;
String stringVal;
PImage img;
float[] zForce = new float[4];
String[] strings = new String[8];
float totalForce;
float x;
float y;
float XOFFSET = 43;
float YOFFSET = 36;

void settings(){
  size(814,806);
}

void setup() 
{  
  noStroke();
  background(90,90,90);

  port = new Serial(this, "/dev/cu.usbmodem101", 9600);  
 
  img = loadImage("CrossHair.jpg");
  img.loadPixels();
  image(img,0,0);
}

void draw(){  //same as loop in arduino
  background(90,90,90);
  image(img,0,0);
  
  if ( port.available() > 0) {  // If data is available,
   stringVal = port.readStringUntil('\n');
    if (stringVal != null) {
      strings = stringVal.split(",");
      x = parseFloat(strings[0]); // read it and store it in val
      y = parseFloat(strings[1]); // read it and store it in val
    }
  } 
  x = (x*814);
  y = (y*806);
  fill(255,0,0);
  circle(x,y,20);
}
