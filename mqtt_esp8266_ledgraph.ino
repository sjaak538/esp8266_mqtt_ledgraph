
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            14

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 50; // delay for half a second





// Update these with values suitable for your network.

const char* ssid = "ssid";
const char* password = "passwd";
const char* mqtt_server = "hostname mqtt server";
IPAddress ip(192, 168, 3, 209); //your static IP address
IPAddress gateway(192, 168, 3, 1); //your gateway
IPAddress subnet(255, 255, 255, 0);//your subnet


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
int col = 1; 
int var= 5;
String modes;
String inData;
String ledHex;
String ledColor; 
long memr[NUMPIXELS];
long memg[NUMPIXELS];
long memb[NUMPIXELS];
char* hexstring = "#ffffff";

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet); //sets the connection to static

  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
            inData += (char)payload[i]; 
  }
  Serial.println();

                  ledHex=inData.substring(0,6);
                    long number = strtol( &ledHex[0], NULL, 16);
                    // Split the hex number into values
                  long ledNoHex = number >> 16;
                   int ledNo = inData.substring(0,2).toInt();                     
                        //Serial.print(ledNo);
            modes=inData.substring(2,4);
            ledColor=inData.substring(4,10);
                    number = strtol( &ledColor[0], NULL, 16);

                    // Split them up into r, g, b values
                    long r = number >> 16;
                    long g = number >> 8 & 0xFF;
                    long b = number & 0xFF;
Serial.println("");
Serial.println("Modes :");
Serial.println(modes);
Serial.println("Color :");
Serial.println(ledColor);
Serial.println("LedNo :");
Serial.println(ledNo);
Serial.println("Number :");
Serial.println(number);
Serial.println("");

 inData = "";

            if(modes=="00") {
                            dark(0);
          }
            if(modes=="01") {
              // set light color and put in memory
                             pixels.setPixelColor(ledNo, pixels.Color(r,g,b)); // Moderately bright green color.
                             pixels.show();
                             memr[ledNo] = r;
                             memg[ledNo] = g;
                             memb[ledNo] = b;
                             Serial.println("Ready"); 
          }
            if(modes=="02") {               
            // Clear all leds
                for(int i=0;i<NUMPIXELS;i++){            
                  pixels.setPixelColor(i, pixels.Color(r,g,b)); 
                  pixels.show(); 
                } 
                Serial.println("Ready");      
            }
            if(modes=="03") {
              // recall in memory leds
                           recall(memr,memg,memb);
                          Serial.println("Ready");
            } 
            if(modes=="04") {
              // flashing led longer time and put in memory
                           int flashamount=10;
                           flash(ledNo,flashamount,r,g,b);
                             memr[ledNo] = r;
                             memg[ledNo] = g;
                             memb[ledNo] = b;
                           recall(memr,memg,memb);
                           Serial.println("Ready"); 
           }
           if(modes=="05") {
            // flashing led short time
                           int flashamount=3;
                           ledglow(ledNo,flashamount,r,g,b);
                             memr[ledNo] = r;
                             memg[ledNo] = g;
                             memb[ledNo] = b;
                           recall(memr,memg,memb);
                           Serial.println("Ready"); 
           }           

            if(modes=="06") {               
            // graph leds single led
            Serial.println("Temp changed to :");
            Serial.println("LedNo :");
            Serial.println(ledNo);
                for(int i=0;i<NUMPIXELS;i++){            
                  if ((ledNo-10) == i) {
                    pixels.setPixelColor(i, pixels.Color(r,g,b)); 
                    pixels.show(); 
                  } else {
                    
                    pixels.setPixelColor(i, pixels.Color(0,0,0)); 
                    pixels.show(); 
                    
                  }
                } 
                Serial.println("Ready");      
            }
            
            if(modes=="07") {               
           // graph leds with background color
            Serial.println("Temp changed to :");
            Serial.println("LedNo :");
            Serial.println(ledNo);
                for(int i=0;i<NUMPIXELS;i++){            
                  if ((ledNo-10) < i) {
                    pixels.setPixelColor(i, pixels.Color(10,0,0)); 
                    pixels.show(); 
                  } else {
                    
                    pixels.setPixelColor(i, pixels.Color(r,g,b)); 
                    pixels.show(); 
                    
                  }
                } 
                Serial.println("Ready");      
            }
            
            
           
            if(modes=="10") {
                            rainbow(40);
                            recall(memr,memg,memb);
          }  
            if(modes=="11") {
                            rainbowCycle(20);
                            recall(memr,memg,memb);
          }
            if(modes=="12") {
                            theaterChaseRainbow(20);
                            recall(memr,memg,memb);
          } 
            if(modes=="13") {
                            oneloop(20);
                            recall(memr,memg,memb);
          }                      
            if(modes=="FF") {               
            // Clear all leds
                for(int i=0;i<NUMPIXELS;i++){            
                  pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
                  pixels.show(); // This sends the updated pixel color to the hardware.
                }      
            }

delay(500);


}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic2", "hello world");
      // ... and resubscribe
      client.subscribe("/ledringgraph");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  pixels.begin(); // This initializes the NeoPixel library.
  
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 15000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "Value execute #%ld", value);
    Serial.print("Publish message: ");
  
    Serial.println(msg);
  //  client.publish("outTopic2", msg);
  }

  
}

void recall(long memr[],long memg[],long memb[]){
                                       for (int i = 0; i < NUMPIXELS; i = i + 1) {
                                         //Serial.print(i);
                                         //Serial.print(memr[i]);
                                         //Serial.print(memg[i]);
                                         //Serial.println(memb[i]);
                                         pixels.setPixelColor(i, pixels.Color(memr[i],memg[i],memb[i])); // Moderately bright green color.
                                         pixels.show();
                                        }
   
}


void flash(uint8_t ledno,uint8_t amount,long r,long g,long b) {
  int r1=0;
  int b1;
  b1=255;
   if (b > 100) {
            b1=0;
            r1=254;
            };
  for (int j=0; j < amount; j++) {     // cycle all 256 colors in the wheel
    pixels.setPixelColor(ledno, pixels.Color(r1,0,b1)); // Moderately bright green color.
    pixels.show();
    delay(150);
    pixels.setPixelColor(ledno, pixels.Color(r,g,b)); // Moderately bright green color.
    pixels.show();
    delay(150);
  }

}

void ledglow(uint8_t ledno,uint8_t amount,long r,long g,long b) {
  for(int j=0; j<256*amount; j++) { // 5 cycles of all colors on wheel
    for(int i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(ledno, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(5);
  }
    pixels.setPixelColor(ledno, pixels.Color(r,g,b)); // Moderately bright green color.
    pixels.show();
}
void oneloop(uint8_t wait) {
  for (int j=0; j < 255; j++) {     // cycle all 256 colors in the wheel

    for (int q=0; q < NUMPIXELS; q++) {
      for (int i=0; i < pixels.numPixels(); i=i+NUMPIXELS) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixels.show();

      delay(40);

      for (int i=0; i < pixels.numPixels(); i=i+NUMPIXELS) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
        j=j+10;
  }
  dark(0);
  Serial.println("Ready"); 
}



void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixels.show();

      delay(40);

      for (int i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  dark(0);
  Serial.println("Ready"); 
}


void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
  dark(0);
  Serial.println("Ready");
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
  dark(0);
  Serial.println("Ready"); 
}

void dark(uint8_t wait) {
       for(int i=0; i< pixels.numPixels(); i++) {
              pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.

          pixels.show();
      }
  }



// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
