#include "LoRaWan_APP.h" 
#include <DallasTemperature.h> 
#include <Wire.h>                
#include "HT_SSD1306Wire.h" 

SSD1306Wire  Display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED); 
// addr , freq , SDA, SCL, resolution , rst 

// Data wire is plugged into GPIO2 on the ESPR32 Board 
#define ONE_WIRE_BUS 2 

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire); 
/* OTAA para*/ 
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0x20, 0x37 }; 
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; 
uint8_t appKey[] = {0xA4, 0x2A, 0x26, 0xDE, 0xFB, 0x33, 0xBD, 0x26, 0xA4, 0xA8, 0x45, 
0x93, 0xC8, 0x53, 0x8D, 0x40 }; 

/* ABP para*/ 
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 
0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 }; 
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 
0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 }; 
uint32_t devAddr =  ( uint32_t )0x007e6ae1; 

/*LoraWan channelsmask, default channels 0-7*/  
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 }; 

/*LoraWan region, select in arduino IDE tools*/ 
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION; 

/*LoraWan Class, Class A and Class C are supported*/ 
DeviceClass_t  loraWanClass = CLASS_A; 

/*the application data transmission duty cycle.  value in [ms].*/ 
uint32_t appTxDutyCycle = 10000; 

 
/*OTAA or ABP*/ 
bool overTheAirActivation = true; 
/*ADR enable*/ 
bool loraWanAdr = true; 
/* Indicates if the node is sending confirmed or unconfirmed messages */ 
bool isTxConfirmed = true; 
/* Application port */ 
uint8_t appPort = 2; 
uint8_t confirmedNbTrials = 4; 
/* Prepares the payload of the frame */ 
static void prepareTxFrame( uint8_t port ) 
{ 
  pinMode(Vext, OUTPUT); 
  digitalWrite(Vext, LOW); 
 
  sensors.requestTemperatures(); // Send the command to get temperatures 
  float temperature = (float) (sensors.getTempCByIndex(0)); 
  int int_temp=temperature*100; 
   
  digitalWrite(Vext, HIGH); 
   
  appDataSize = 2; 
  appData[0] = int_temp >> 8; 
  appData[1] = int_temp; 
   
  Serial.print("Temperature is: "); 
  Serial.println(temperature); 
  Display.clear(); 
  Display.drawString(0,0,"Temperature: "+ String(temperature)+" °C"); 
  Display.display(); 
   
} 
 
void setup() { 
  Serial.begin(115200); 
  Display.init(); 
 
  Display.setFont(ArialMT_Plain_10); 
  sensors.begin(); 
  Mcu.begin(); 
   deviceState = DEVICE_STATE_INIT; 
} 
void loop() 
{ 
  switch( deviceState ) 
  { 
    case DEVICE_STATE_INIT: 
    { 
 

 
#if(LORAWAN_DEVEUI_AUTO) 
      LoRaWAN.generateDeveuiByChipID(); 
 
#endif 
      
      LoRaWAN.init(loraWanClass,loraWanRegion); 
      break; 
    } 
    case DEVICE_STATE_JOIN: 
    { 
      LoRaWAN.join(); 
      break; 
    } 
    case DEVICE_STATE_SEND: 
    { 
      prepareTxFrame( appPort ); 
      LoRaWAN.send(); 
      deviceState = DEVICE_STATE_CYCLE; 
      break; 
    } 
    case DEVICE_STATE_CYCLE: 
    { 
      // Schedule next packet transmission 
      txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, 
      APP_TX_DUTYCYCLE_RND ); 
      LoRaWAN.cycle(txDutyCycleTime); 
      deviceState = DEVICE_STATE_SLEEP; 
      break; 
    } 
    case DEVICE_STATE_SLEEP: 
    { 
      LoRaWAN.sleep(loraWanClass); 
      break; 
    } 
    default: 
    { 
      deviceState = DEVICE_STATE_INIT; 
      break; 
    } 
  } 
} 