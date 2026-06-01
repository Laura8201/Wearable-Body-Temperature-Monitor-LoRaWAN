# Wearable Body Temperature Monitor — LoRaWAN
Arduino code developed as part of a thesis project on wearable technology. The system reads body temperature from a DS18B20 sensor and transmits the data every 10 seconds via LoRaWAN, using a Heltec CubaCell (HTCC-AB02A) development board. Data is visualized in real time on ThingSpeak.
# Hardware
Heltec CubaCell HTCC-AB02A  
DS18B20 waterproof temperature sensor (probe version)  
3.7V lithium battery  
# Requirements
Arduino IDE with Heltec CubaCell board support installed  
DallasTemperature library  
OneWire library  
# Configuration  
Before uploading the code, update the LoRaWAN credentials in the file with your own keys:  
devEui, appEui, appKey (for OTAA)  
nwkSKey, appSKey, devAddr (for ABP)  
The transmission interval is set to 10 seconds and can be changed by modifying:  
appTxDutyCycle = 10000;  
# Network
The system communicates via LoRaWAN. To transmit data, the device must be within range of a LoRaWAN gateway connected to a network server (such as The Things Network). Without gateway coverage in your area, the data will not be transmitted.  
# Data Visualization  
Data is transmitted via LoRaWAN and monitored through ThingSpeak, which provides real-time graphs of the temperature readings.
