import controlP5.*; //import ControlP5 library
import processing.serial.*;
import java.util.*;



Serial port;
float val;
String stringVal;
float[] zForce = new float[4];
String[] strings = new String[8];
float totalForce;
float x;
float y;
int WIDTH = 800;
int HEIGHT = 800;
int TICK_WIDTH = 10;
int TICK_SPACING = 50;
int numPoints = 0;

void settings(){
  size(WIDTH,HEIGHT);
}

void setup() 
{  
  noStroke();
  background(90,90,90);
  ArrayList<String> portNames = new ArrayList<String>(Arrays.asList(Serial.list()));
  String portName = null;
  if (portNames.contains("/dev/cu.usbmodem1101"))
    portName = "/dev/cu.usbmodem1101";
  else if (portNames.contains("/dev/cu.usbmodem101"))
    portName = "/dev/cu.usbmodem101";
  if (portName != null)
    port = new Serial(this, portName, 115200);
  else
    for(;;);
}

void draw(){  //same as loop in arduino
  //background(90,90,90);
  drawCrosshairs();
  if ( port.available() > 0) {  // If data is available,
   stringVal = port.readStringUntil('\n');
    if (stringVal != null) {
      strings = stringVal.split(",");
      x = parseFloat(strings[0]); // read it and store it in val
      y = parseFloat(strings[1]); // read it and store it in val
      numPoints++;
      println(numPoints);
    }
  } 
  x = (x*WIDTH);
  y = (y*HEIGHT);
  fill(255,0,0);
  circle(x,y,20);
}

void drawCrosshairs(){
  stroke(0);
  line(WIDTH/2, 0, WIDTH/2, HEIGHT);
  line(0, HEIGHT/2, WIDTH, HEIGHT/2);
  int i;
  for (i = 0; i < WIDTH; i += TICK_SPACING){
    if (i != 0)
      line(i, HEIGHT/2 + TICK_WIDTH, i, HEIGHT/2 - TICK_WIDTH);
  }  
  for (i = 0; i < HEIGHT; i += TICK_SPACING){
    if (i != 0)
      line(WIDTH/2 + TICK_WIDTH, i, WIDTH/2 - TICK_WIDTH, i);
  }  
  noFill();
  stroke(124,252,0);
  circle(WIDTH/2, HEIGHT/2, 0.3*WIDTH);
  stroke(0);
}
