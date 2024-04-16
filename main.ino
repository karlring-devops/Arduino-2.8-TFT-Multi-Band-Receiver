

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

const int MINPRESSURE = 200;
const int MAXPRESSURE = 1000;

// TFT LCD colors
#define BLACK   0x0000
#define WHITE   0xFFFF
#define BLUE    0x001F
#define RED     0xF800

// Touch screen pins and thresholds
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=891,TS_RT=140,TS_TOP=100,TS_BOT=915;

// Button dimensions
#define BUTTON_WIDTH  150
#define BUTTON_HEIGHT 40
#define BUTTON_MARGIN 10
#define BUTTON_X_START 85
#define BUTTON_Y_START 50

// Define TFT LCD object
MCUFRIEND_kbv tft;   //(LCD_CS, LCD_RS, LCD_WR, LCD_RD, LCD_RST);

// Define touch screen object
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Define button labels
const char* buttonLabels[] = {"LoRa", "WiFi", "BlueTooth", "Serial", "", "Return"};
const int numButtons = sizeof(buttonLabels) / sizeof(buttonLabels[0]);

// Button objects
Adafruit_GFX_Button buttons[numButtons];

int pixel_x, pixel_y;
bool Touch_getXY(void) {
    TSPoint p = ts.getPoint();
    pinMode(YP, OUTPUT);
    pinMode(XM, OUTPUT);
    digitalWrite(YP, HIGH);
    digitalWrite(XM, HIGH);
    bool pressed = (p.z > MINPRESSURE && p.z < MAXPRESSURE);
    if (pressed) {
        pixel_x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
        pixel_y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    }
    return pressed;
}

void displayTemplateScreen(const char* buttonLabels[]){

  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);

  // Initialize buttons
  int yPos = BUTTON_Y_START;
  for (int i = 0; i < numButtons; i++) {
    uint16_t buttonColor = (i == numButtons - 1) ? RED : BLUE;
    // buttons[i].initButton(&tft, BUTTON_X_START, yPos, BUTTON_WIDTH, BUTTON_HEIGHT, buttonColor, buttonColor, WHITE, buttonLabels[i], 2);
    if ( buttonLabels[i] == "" ) { buttonColor = BLACK; };
    buttons[i].initButton(&tft, 120, yPos, BUTTON_WIDTH, BUTTON_HEIGHT, buttonColor, buttonColor, WHITE, buttonLabels[i], 2);
    buttons[i].drawButton();
    yPos += BUTTON_HEIGHT + BUTTON_MARGIN;
    Serial.println("Button ["+String(i)+"](x,y,w,h,label) 120,"+ String(yPos)
                    +", "+BUTTON_WIDTH+", "+BUTTON_HEIGHT+", "+buttonLabels[i]);
  }
}

void displayTerminalHeader(const char* headerLabel){
  initScreen();
  // Draw header section
  tft.fillRect(1, 1, 240, 15, RED);
  tft.setTextColor(WHITE);
  tft.setTextSize(0.5);
  tft.setCursor(1, 1);
  tft.println("Scanning: " + String(headerLabel) );
}

void initScreen(){
  // Initialize TFT LCD
  tft.begin(0x9341);
  tft.setRotation(0);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
}

void serialLog(String MESSAGE ){
    Serial.println("["+String(MESSAGE)+"])");
}

void WiFiListener(const char* headerLabel){

  if (headerLabel == "WiFi") {
    const char* DEMO_MESSAGES[] = {
      "[AA:AA:AA:AA:AA:AA] ASG 2.4G QoS=87-100  SigLvl=27-100"
      ,"[AA:AA:AA:AA:AA:AA] TPS_2.4GHz QoS=100-100  SigLvl=17-100"
      ,"[AA:AA:AA:AA:AA:AA] Jamana 2.4 QoS=93-100  SigLvl=14-100"
      ,"[AA:AA:AA:AA:AA:AA] FOAD_2.4GHz QoS=100-100  SigLvl=17-100"
      ,"[AA:AA:AA:AA:AA:AA] HLGH QoS=40-100  SigLvl=8-100"
      ,"[AA:AA:AA:AA:AA:AA] DINGDONG_2.4G QoS=13-100  SigLvl=13-100"
      ,"[AA:AA:AA:AA:AA:AA] LAAPSAAP QoS=97-100  SigLvl=14-100"
    };

    int x_wifi = 1;
    int y_wifi = 35;
    int i_wifi;
    tft.setCursor(x_wifi, y_wifi);

    for (i_wifi = 0; i_wifi <= 6; i_wifi++) {
        String message = DEMO_MESSAGES[i_wifi];
        tft.println(message+"\n");
        if ( message.length() > 40 ) {
          y_wifi = y_wifi + 20;
        } else {
          y_wifi = y_wifi + 10;
        }
      delay(1300);
    }
  }
}

// void sensorListener(const char* headerLabel, char** MESSAGES[]){

//   int X_LSNR = 1;
//   int Y_LSNR = 35;
//   int i;

//   tft.setCursor(X_LSNR, Y_LSNR);

//   int MESSAGES_TOTAL = sizeof(MESSAGES) / sizeof(MESSAGES[0]);

//   for (i = 0; i <= MESSAGES_TOTAL; i++) {
//       String message = MESSAGES[i];
//       tft.println(message+"\n");
//       Serial.println(message+"\n");
//       if ( message.length() > 40 ) {
//         Y_LSNR = Y_LSNR + 20;
//       } else {
//         Y_LSNR = Y_LSNR + 10;
//       }
//     delay(1300);
//   }
// }

void BTListener(char* headerLabel){

  if (headerLabel == "BlueTooth") {
   char* DEMO_MESSAGES[] = { 
    "bluetooth]# scan on", "Discovery started", "[CHG] Controller AA:AA:AA:AA:AA:AA Discovering: yes"
  , "[NEW] Device AA:AA:AA:AA:AA:AA AA:AA:AA:AA:AA:AA", "[CHG] Controller AA:AA:AA:AA:AA:AA Discovering: yes"
  , "[NEW] Device AA:AA:AA:AA:AA:AA AA:AA:AA:AA:AA:AA", "[NEW] Device AA:AA:AA:AA:AA:AA Pulse Oximeter"
  , "[NEW] Device AA:AA:AA:AA:AA:AA AA:AA:AA:AA:AA:AA", "[NEW] Device AA:AA:AA:AA:AA:AA 4D-D4-1B-FA-8F-C1"
  };
  // sensorListener(headerLabel, DEMO_MESSAGES);
  int x = 1;
  int y = 35;

  tft.setCursor(x, y);
  int i;

  for (i = 0; i <= 8; i++) {
      String message = DEMO_MESSAGES[i];
      tft.println(message+"\n");
      if ( message.length() > 40 ) {
        y = y + 20;
      } else {
        y = y + 10;
      }
    delay(1300);
  }
 }
}


void LoRaListener(char* headerLabel){

  if (headerLabel == "LoRa") {
    String MESSAGE = "[Lora] A21B8F9 - ok!";
  
  // sensorListener(headerLabel, DEMO_MESSAGES);
  int x = 1;
  int y = 35;

  tft.setCursor(x, y);
  int i;

  for (i = 0; i <= 8; i++) {
      String message = MESSAGE;
      tft.println(message+"\n");
      if ( message.length() > 40 ) {
        y = y + 20;
      } else {
        y = y + 10;
      }
    delay(1300);
  }
 }
}

// these must remain outside SerialListener()
// or does not progress next line !
int Y_RST_SERIAL = 35;
int YS;
int XS = 0;

void SerialListener(const char* SOURCE){

    if (SOURCE == "Serial") {
      YS = Y_RST_SERIAL;
      while (Serial.available() > 0) {
        String message = Serial.readStringUntil('\n');
        if ( message != "<<EOF>>" ) {
          if ( message.length() > 40 ) {
            YS = YS + 20;
          } else if ( message.length() < 40 ) {
            YS = YS + 10;
          } 
          Serial.println("["+String(XS)+","+String(YS)+"] "+message+"\n");
          tft.setCursor(XS, YS);
          tft.println("["+String(XS)+","+String(YS)+"] "+message+"\n");
          Y_RST_SERIAL = YS;
        } else {
            setup();
        }
      }
    }
}

void displayTerminalSerial(const char* headerLabel){
  serialLog("displayTerminalSerial("+String(headerLabel)+")");
  initScreen();
  // Draw header section
  displayTerminalHeader(headerLabel);
  Serial.println("Serial.available("+String(Serial.available())+")");
      
}

void displayTerminalSession(const char* headerLabel){
  serialLog("displayTerminalSession("+String(headerLabel)+")");
  initScreen();
  // Draw header section
  displayTerminalHeader(headerLabel);
}

void serialLogTouch(int x, int y ){
  Serial.print("[pixel] (x,y) : ");
  Serial.print(pixel_x);
  Serial.print(",");
  Serial.print(pixel_y);
  Serial.print("  [TSPoint] (x,y) : ");
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
}


const char* DISPLAY_SCREEN;



void setup() {
  Serial.begin(9600);
  // Serial.println("");
  // Serial.println("#############################################");
  initScreen();
  displayTemplateScreen(buttonLabels);
}

void loop(){
  // Check for touch input
  TSPoint touch = ts.getPoint();
  bool STATUS = Touch_getXY();
  // If a touch is detected
  if (touch.z > 0) {
    serialLogTouch(touch.x, touch.y );
    // Check if any button is pressed
    if (buttons[0].contains(pixel_x, pixel_y)) {
          DISPLAY_SCREEN = "LoRa";
          serialLog("loop("+String(DISPLAY_SCREEN)+")");
          displayTerminalSession(DISPLAY_SCREEN);
    } else
    if (buttons[1].contains(pixel_x, pixel_y)) {
          DISPLAY_SCREEN = "WiFi";
          serialLog("loop("+String(DISPLAY_SCREEN)+")");
          displayTerminalSession(DISPLAY_SCREEN);
    } else
    if (buttons[2].contains(pixel_x, pixel_y)) {
          DISPLAY_SCREEN = "BlueTooth";
          displayTerminalSession(DISPLAY_SCREEN);
    } else
    if (buttons[3].contains(pixel_x, pixel_y)) {
        DISPLAY_SCREEN = "Serial";
        displayTerminalSerial(DISPLAY_SCREEN);
    } else
    if (buttons[4].contains(pixel_x, pixel_y)) {
        DISPLAY_SCREEN = "Return";
        setup();
    }
    //############################################################
  }
      // ------------ LISTENERS --------------------
      LoRaListener(DISPLAY_SCREEN);
      BTListener(DISPLAY_SCREEN);
      SerialListener(DISPLAY_SCREEN);
      WiFiListener(DISPLAY_SCREEN);
      // -------------------- ------- --------------
      bool pressed = Touch_getXY();
      if (pressed){
        serialLog("listenerReturn("+String(DISPLAY_SCREEN)+")");
      }
}




