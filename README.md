# Arduino-2.8-TFT-Multi-Band-Receiver
Arduino LoRa BlueTooth Wifi Serial GSM receiver 

# Recommendation:

It is recommended, really, that you use the following Scripts from 
```MCUFRIEND_kbv``` Library to appropriately Calibrate your TFT Sheild and Arduino:

* Touch_shield_new 
* TouchScreen_Calibr_native

Copy and paste the outputs from those into your scripts: eg:
```
// Touch screen pins and thresholds
const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=891,TS_RT=140,TS_TOP=100,TS_BOT=915;
```

