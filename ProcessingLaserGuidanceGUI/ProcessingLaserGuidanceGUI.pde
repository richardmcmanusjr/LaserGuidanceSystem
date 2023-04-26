import controlP5.*; //import ControlP5 library
import processing.serial.*;

Serial port;
float val;
String stringVal;
PImage MP;
float[] zForce = new float[4];
String[] strings = new String[8];
float totalForce;
float x;
float y;
float XOFFSET = 43;
float YOFFSET = 36;

void settings(){
  size(900,700);
}

void setup() 
{  
  noStroke();
  background(90,90,90);

  port = new Serial(this, "/dev/cu.usbmodem101", 9600);  
 
  MP = loadImage("MP.png");
  MP.loadPixels();
  image(MP,XOFFSET,YOFFSET);
}

void draw(){  //same as loop in arduino
  background(90,90,90);
  image(MP,XOFFSET,YOFFSET);
  
  if ( port.available() > 0) {  // If data is available,
   stringVal = port.readStringUntil('\n');
    if (stringVal != null) {
      strings = stringVal.split(",");
      x = parseFloat(strings[0]); // read it and store it in val
      y = parseFloat(strings[1]); // read it and store it in val
    }
  } 
  print(x);
  print(", ");
  println(y);
  if (totalForce > 10){
    x = (x*814) + XOFFSET;
    y = (y*627) + YOFFSET;
    fill(255,0,0);
    circle(x,y,20);
  }
  
}
