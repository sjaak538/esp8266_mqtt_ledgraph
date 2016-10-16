# esp8266_mqtt_ledgraph
creating MQTT neopixels graph


Show a neopixel ledring or strip as a graph with MQTT
Also some nice ledpixels play arounds.

send code xxaabbccnn to mqtt subscribe to /ledringgraph
xx Mode
aabbcc = Color
nn = number led


Modes 01 // set 1 single led and color and numberled and put in memory

Modes 02 // set all leds in color

Modes 03 // recall in memory leds

Modes 04 // set 1 single led flashing longer time and put in memory

Modes 05 // set 1 single led flashing short time and put in memory

Modes 06 // graph leds set color and background is light red number

Modes 07 // graph leds single led


Modes 10 // Rainbow

modes 11 // rainbowCycle

Modes 12 //theaterChaseRainbow

Please help me to develope more features and clean up the code.
