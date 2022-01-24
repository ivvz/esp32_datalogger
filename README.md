# ESP32 temperature datalogger

ESP32 based type k thermocouple temperature data logger

## Devices

- ESP32 lolin lite board
- OLED Screen SSD1306
- RTC Module DS3231
- Microsd moodule
- Max6675 module
- MT3608 boost dc-dc module
- Two push buttons

## Pinout

### Microsd
- MISO  --- Pin 19
- MOSI --- Pin 23
- SCK --- Pin 18
- CS --- Pin 5
- VCC --- MT3608 VOUT Pin 
- GND --- GND 

### SSD1306 (I2C)
- SCL -- Pin 32
- SDA --- Pin 33
- VCC --- MT3608 VOUT Pin                                                 
- GND --- GND     

### RTC DS3131 (I2C)
- SCL -- Pin 32                                                           
- SDA --- Pin 33 
- VCC --- MT3608 VOUT Pin                                                 
- GND --- GND 

### Thermocouple modules (MAX6675)
- DO (MISO) --- Pin 23                                                    - SCK --- Pin 4                                                          
- CS1 --- Pin 16
- CS2 --- Pin 15  
- CS3 --- Pin 13  
- VCC --- MT3608 VOUT Pin                                                 
- GND --- GND

# Schematics
![schematics](/diagram/datalogger_diagram.png) 
