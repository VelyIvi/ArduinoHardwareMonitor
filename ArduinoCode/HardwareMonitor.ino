https://github.com/VelyIvi/ArduinoHardwareMonitor/

#include <TFT_HX8357.h> // Hardware-specific library

TFT_HX8357 tft = TFT_HX8357();       // Invoke custom library

const int verticalSpacing = 48;

int gpuTemp = 0;
int gpuUtil = 0;

int cpuTemp = 0;
int cpuUtil = 0;

int memUse = 0;
int memUtil = 0;

String readString, data;

void setup(void) {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(0x0000);
  UpdateAll();
  Serial.begin(9600);
  delay(2000);  
  Serial.println("Hardware visualizer active");
}

void memPartial(){
  tft.setCursor(tft.textWidth("Used: ", 2)+4, tft.height()/2+verticalSpacing+4);
  tft.fillRect(tft.textWidth("Used: ", 2)+4, tft.height()/2+verticalSpacing+4, tft.textWidth("0000",2), tft.fontHeight(2), TFT_BLACK);
  tft.print(memUse);
  tft.print("GB"); 

  tft.setCursor(tft.textWidth("Utilization: ", 2)+4, tft.height()/2+verticalSpacing*2+4);
  tft.fillRect(tft.textWidth("Utilization: ", 2)+4, tft.height()/2+verticalSpacing*2+4, tft.textWidth("0000",2), tft.fontHeight(2), TFT_BLACK);
  tft.print(memUtil);
  tft.print("%");
}

void memoryUpdate(){
  tft.drawRect(1, tft.height()/2+1, tft.width()/2-2, tft.height()/2-2, TFT_GREEN);

  tft.setCursor(4, tft.height()/2+4);
  tft.print("Memory");

  tft.setCursor(4, tft.height()/2+verticalSpacing+4);
  tft.print("Used: ");
  
  tft.setCursor(4, tft.height()/2+verticalSpacing*2+4);
  tft.print("Utilization: ");

  memPartial();
}

void cpuPartial(){
  tft.setCursor(tft.width()/2 + 4 + tft.textWidth("Temperature: ", 2), 4+verticalSpacing);
  tft.fillRect(tft.width()/2 + 4 + tft.textWidth("Temperature: ", 2), 4+verticalSpacing, tft.textWidth("0000",2), tft.fontHeight(2), TFT_BLACK);

  tft.print(cpuTemp);
  tft.print("C"); 

  tft.setCursor(tft.width()/2 + 4  + tft.textWidth("Utilization: ", 2), 4+verticalSpacing*2);
  tft.fillRect(tft.width()/2 + 4 + tft.textWidth("Utilization: ", 2), 4+verticalSpacing*2, tft.textWidth("0000",2), tft.fontHeight(2), TFT_BLACK);

  tft.print(cpuUtil);
  tft.print("%"); 
}

void cpuUpdate(){
  tft.drawRect(tft.width()/2+1, 1, tft.width()/2-2, tft.height()/2-2, TFT_RED);
  tft.setCursor(tft.width()/2 + 4, 4);
  tft.print("CPU");

  tft.setCursor(tft.width()/2 + 4, 4+verticalSpacing);
  tft.print("Temperature: "); 

  tft.setCursor(tft.width()/2 + 4, 4+verticalSpacing*2);
  tft.print("Utilization: ");

  cpuPartial();
}

void gpuPartial(){
  tft.setCursor(4+tft.textWidth("Temperature: ", 2), verticalSpacing+4);
  tft.fillRect(4+tft.textWidth("Temperature: ", 2), verticalSpacing+4,tft.textWidth("0000",2), tft.fontHeight(2), TFT_BLACK);

  tft.print(gpuTemp);
  tft.print("C");

  tft.setCursor(4 + tft.textWidth("Utilization: ", 2), verticalSpacing*2+4);
  tft.fillRect(4 + tft.textWidth("Utilization: ", 2), verticalSpacing*2+4,tft.textWidth("0000",2), tft.fontHeight(2), TFT_BLACK);

  tft.print(gpuUtil);
  tft.print("%"); 
}

void gpuUpdate(){
  tft.drawRect(1, 1, tft.width()/2-2, tft.height()/2-2, TFT_CYAN);
  tft.setCursor(4, 4);
  tft.println("GPU");

  tft.setCursor(4, verticalSpacing+4);
  tft.print("Temperature: "); 

  tft.setCursor(4, verticalSpacing*2+4);
  tft.print("Utilization: ");

  gpuPartial();
}


void UpdateAll(){
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  
  // tft.setTextSize(2);
  tft.setTextFont(2);
  tft.setTextSize(2);

  gpuUpdate();
  cpuUpdate();
  memoryUpdate();
}

void loop() {
  if(Serial.available()){
    char c = Serial.read();  //gets one byte from serial buffer
    if (c == ',') { 
      if(readString.indexOf("DIS") >= 0){
        cpuTemp = 0;
        cpuUtil = 0;
        gpuTemp = 0;
        gpuUtil = 0;
        memUtil = 0;
        memUse = 0;
        UpdateAll();
      }
      if(readString.indexOf("CT") >=0) {
        readString=readString.substring(2);
        if(cpuTemp != readString.toInt()){
          cpuTemp = readString.toInt();
          cpuPartial();
        }
      }
      if(readString.indexOf("CU") >=0) {
        readString=readString.substring(2);
        if(cpuUtil != readString.toInt()){
          cpuUtil = readString.toInt();
          cpuPartial();
        }
      }
      if(readString.indexOf("GT") >=0) {
        readString=readString.substring(2);
        if(gpuTemp != readString.toInt()){
          gpuTemp = readString.toInt();
          gpuPartial();
        }
      }
       if(readString.indexOf("GU") >=0) {
        readString=readString.substring(2);
        if(gpuUtil != readString.toInt()){
          gpuUtil = readString.toInt();
          gpuPartial();
        }
      }
      if(readString.indexOf("MT") >=0) {
        readString=readString.substring(2);
        if(memUtil != readString.toInt()){
          memUtil = readString.toInt();
          memPartial();
        }
      }
      if(readString.indexOf("MS") >=0) {
        readString=readString.substring(2);
        if(memUse != readString.toInt()){
          memUse = readString.toInt();
          memPartial();
        }
      }

      readString=""; //clears variable for new input

    }  
    else {
      readString += c; //makes the string readString
    }
  }
}



