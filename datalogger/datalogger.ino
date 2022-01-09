#include "FS.h"
#include "SD.h"
#include "max6675.h"
#include <RTClib.h> 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>




unsigned long tiempo_lectura = 2000; // intervalo de lectura general (sin ejecutar funcion de registro de datos)
unsigned long tiempo_sin_registro;

int thermoDO = 17;
int thermoCS = 16;
int thermoCLK = 4;

MAX6675 termopar1(thermoCLK, thermoCS, thermoDO);


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup(){
  Wire.begin(33, 32);
  Serial.begin(115200);
  
  checkOLED();
  display.display();
  delay(2000); // Pause for 2 seconds
  
  checkSD();
  


  
  writeFile(SD, "/hello00.txt", "Hello ");
  appendFile(SD, "/hello00.txt", "Worldllllllllllllllllllllllllllllllllllllllllll!\n");
  readFile(SD, "/hello00.txt");
  renameFile(SD, "/hello00.txt", "/f99.txt");
  readFile(SD, "/f99.txt");
  Serial.println("si jala mijo");

}

void loop(){
  
  if(millis() - tiempo_sin_registro >= tiempo_lectura){
     //hora = getTime();
     Serial.println(" Termopar 1 = " + String(termopar1.readCelsius()) + " C");
     //Serial.println(hora + " Termopar 2 = " + String(termopar2.readCelsius()) + " C");
     display.clearDisplay();
     display.setTextSize(1);             
     display.setTextColor(WHITE);        
     display.setCursor(0,10);             
     display.print("Tp 1");
     display.setTextSize(2);  
     display.setCursor(30,10);             
     display.print(String(termopar1.readCelsius()) + " C");

     display.setTextSize(1);
     display.setCursor(0,30);             
     display.print("Tp 2");
     display.setTextSize(2);  
     display.setCursor(30,30);             
     display.print(String(termopar1.readCelsius()+10) + " C");
     display.display();

     display.setTextSize(1);
     display.setCursor(0,50);             
     display.print("Tp 3");
     display.setTextSize(2);  
     display.setCursor(30,50);             
     display.print(String(termopar1.readCelsius()+16) + " C");
     display.display();
     
     
     tiempo_sin_registro = millis();  
    }
}



String createString(){
  // crea el string para csv con las temperaturas
    String dataString = ""; 
    float lectura1 = termopar1.readCelsius();
    float lectura2 = termopar1.readCelsius() + 21;
    float lectura3 = termopar1.readCelsius() + 14;
    dataString += String(millis()) + ",";
    dataString += String(lectura1);
    dataString += ","; 
    dataString  += String(lectura2);
    dataString += ","; 
    dataString  += String(lectura3);

  return dataString;
}

 File createFile(String nombre){
  writeFile(SD, "/hello00.txt", "Hello ");
  //File dataFile = SD.open(nombre + ".csv", FILE_WRITE);
  String header = ("hora,termopar 1,termopar 2,termopar 3");
  if (dataFile) {
    dataFile.println(header);
    dataFile.close();
    // print to the serial port too:
    Serial.println(header);
    delay(5000);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error creating datafile");
  }
  return dataFile;
}


void checkSD(){
    display.setTextSize(2);             
    display.setTextColor(WHITE);        
    display.setCursor(2,20);             
    display.println("REVISANDO MICRO SD!");
    display.display();
    delay(2000);
    display.clearDisplay();
    
    if(!SD.begin(5)){
    Serial.println("Card Mount Failed");
    display.setTextSize(2);             
    display.setTextColor(WHITE);        
    display.setCursor(10,20);             
    display.println(F("FALL0 SD!"));
    display.display();
    delay(2000);
    display.clearDisplay();
    return;
  }else{  
    display.setTextSize(3);             
    display.setTextColor(WHITE);        
    display.setCursor(40,10);             
    display.println("SD");
    display.setCursor(40,40);
    display.println("OK!");
    display.display();
    delay(2000);
    display.clearDisplay();
  }
  
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("No SE DETECTO UNA TARJETA");
    return;
  }
  }

void checkOLED(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
}





































void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
    Serial.println("Failed to open file for appending");
    return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
      size_t toRead = len;
      if(toRead > 512){
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}
