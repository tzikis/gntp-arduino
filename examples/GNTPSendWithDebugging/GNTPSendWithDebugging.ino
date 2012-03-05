#include <MD5.h>
#include <GNTP.h>
#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(192,168,1,96); // Your computer's IP
IPAddress ip(192,168,1, 177);  //Your Arduino's IP

// Initialize the Ethernet client library
// with the IP address of your computer
GNTP gntp("Arduino Door Controller", server);
GNTPNotification notif_open("door opened", "Attention!", "The door is now open");
void setup() 
{
  //In order to enable debugging messages on the serial monitor, you first need to
  //start the Arduino's Serial interface.
  Serial.begin(9600);
  // start the GNTP library, and specify a password to use for the notifications:
  gntp.setDebugging(true);
  gntp.begin();
  Ethernet.begin(mac, ip);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  //add our two notifications to our GNTP controller
  gntp.add(notif_open);
  //register our notifications (needed, once, before we can send any notification)
  gntp.registerNotifications();
  gntp.sendNotification(notif_open);
}

void loop()
{
}

