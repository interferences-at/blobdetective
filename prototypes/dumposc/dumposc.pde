/**
 * oscP5sendreceive by andreas schlegel
 * example shows how to send and receive osc messages.
 * oscP5 website at http://www.sojamo.de/oscP5
 */
 
import oscP5.*;
import netP5.*;
  
OscP5 oscP5;
NetAddress myRemoteLocation;

void setup()
{
  size(100, 100);
  frameRate(25);
  /* start oscP5, listening for incoming messages at port 12000 */
  oscP5 = new OscP5(this, 31340);
  
}

void draw()
{
  background(0);  
}

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage)
{
  /* print the address pattern and the typetag of the received OscMessage */
  print("### received an osc message.");
  print(" addrpattern: " + theOscMessage.addrPattern());
  println(" typetag: " + theOscMessage.typetag());
  if (theOscMessage.checkAddrPattern("/blob"))
  {
    println("args: " + theOscMessage.get(0).intValue()
      + " " + theOscMessage.get(1).floatValue()
      + " " + theOscMessage.get(2).floatValue()
      + " " + theOscMessage.get(3).floatValue()
      );
  }
}