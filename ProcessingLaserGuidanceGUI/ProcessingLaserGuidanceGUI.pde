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

  port = new Serial(this, "/dev/cu.usbserial-110", 9600);  
 
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
      zForce[0] = parseFloat(strings[1]); // read it and store it in val
      zForce[1] = parseFloat(strings[2]); // read it and store it in val
      zForce[2] = parseFloat(strings[3]); // read it and store it in val
      zForce[3] = parseFloat(strings[4]); // read it and store it in val
    }
  } 
  println(zForce [0]);
  totalForce = zForce[0] + zForce[1] + zForce[2] + zForce[3];
  if (totalForce > 10){
    x = (((zForce[1]+zForce[2])/totalForce)*814) + XOFFSET;
    y = (((zForce[0]+zForce[1])/totalForce)*627) + YOFFSET;
    fill(255,0,0);
    circle(x,y,20);
  }
  
}
