
// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_WIFI_POINT
#include <WiFi.h>

#include <WiFiUdp.h>

#include <NTPClient.h>               // Include NTPClient library

#include <TimeLib.h>                 // Include Arduino time library

#include <LiquidCrystal_I2C.h>       // Include LiquidCrystal_I2C library

#include <RemoteXY.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Configure LiquidCrystal_I2C library with 0x27 address, 16 columns and 2 rows

const char *ssid     = "Rewan";

const char *password = "w2yekvft";

WiFiUDP ntpUDP;

// 'time.nist.gov' is used UTC with +2 hour offset (7200 seconds) 

NTPClient timeClient(ntpUDP, "time.nist.gov", 18000, 60000);

char Hour[] = "00";
char Min[] = "00";
char Sec[] = "00";
char Year[] = "2000";
char Month[] = "00";
char Day[] = "00";


byte last_second, second_, minute_, hour_, day_, month_;

int year_;

// RemoteXY connection settings 
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 158 bytes
  { 255,0,0,88,0,151,0,16,174,0,67,4,3,26,25,13,2,26,11,67,
  4,4,42,24,13,2,26,11,67,4,32,26,16,13,2,26,11,67,4,53,
  26,16,13,2,26,11,67,4,75,26,16,13,2,26,11,67,4,32,42,16,
  13,2,26,11,67,4,52,42,16,13,2,26,11,67,4,72,42,27,13,2,
  26,11,129,0,49,28,3,8,17,58,0,129,0,70,28,3,8,17,58,0,
  129,0,49,44,3,10,17,47,0,129,0,28,29,3,8,17,58,0,129,0,
  68,44,3,10,17,47,0,129,0,29,44,3,8,17,58,0,129,0,19,9,
  65,10,78,73,110,116,101,114,110,101,116,32,99,108,111,99,107,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // output variables
  char time1[11];  // string UTF8 end zero 
  char date1[11];  // string UTF8 end zero 
  char hour1[11];  // string UTF8 end zero 
  char minute1[11];  // string UTF8 end zero 
  char second1[11];  // string UTF8 end zero 
  char day1[11];  // string UTF8 end zero 
  char month1[11];  // string UTF8 end zero 
  char year1[11];  // string UTF8 end zero 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup() 
{
  RemoteXY_Init (); 
  
  
    
  // TODO you setup code
  Serial.begin(115200);

  lcd.begin(16,2);                   // Initialize I2C LCD module 
  lcd.backlight();                   // Turn backlight ON

  WiFi.begin(ssid, password);

  Serial.print("Connecting.");

  while ( WiFi.status() != WL_CONNECTED && millis() <=5000  ) {

    delay(500);

    Serial.print(".");

  }

  Serial.println("connected");

  timeClient.begin();
  
  last_second=0;
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  timeClient.update();

  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server

 

  second_ = second(unix_epoch);

  if (last_second != second_) {

 

    minute_ = minute(unix_epoch);

    hour_   = hour(unix_epoch);

    day_    = day(unix_epoch);

    month_  = month(unix_epoch);

    year_   = year(unix_epoch);

 

    Sec[1] = second_ % 10 + 48;

    Sec[0] = second_ / 10 + 48;

   
    Min[1]  = minute_ % 10 + 48;

    Min[0]  = minute_ / 10 + 48;


    Hour[1]  = hour_   % 10 + 48;

    Hour[0]  = hour_   / 10 + 48;

    Day[0]  = day_   / 10 + 48;

    Day[1]  = day_   % 10 + 48;

    Month[0]  = month_  / 10 + 48;

    Month[1]  = month_  % 10 + 48;

    Year[2] = (year_   / 10) % 10 + 48;

    Year[3] = year_   % 10 % 10 + 48;
 
   // print the time and date in serial monitor

   Serial.print("Time:");
   Serial.print(Hour);
   Serial.print(":");
   Serial.print(Min );
   Serial.print(":");
   Serial.println(Sec);
   Serial.print("Date:");
   Serial.print(Day );
   Serial.print("/");
   Serial.print(Month );
   Serial.print("/");
   Serial.println(Year);
   

    // Display time and date on the 16x2 LCD

    lcd.setCursor(0, 0);
    lcd.print("Time:");
    lcd.setCursor(5, 0);
    lcd.print(Hour);
     lcd.setCursor(7, 0);
    lcd.print(":");
    lcd.setCursor(8, 0);
    lcd.print(Min);
     lcd.setCursor(10, 0);
    lcd.print(":");
     lcd.setCursor(11, 0);
    lcd.print(Sec);
    
    lcd.setCursor(0, 1);
    lcd.print("Date:");
    lcd.setCursor(5, 1);
    lcd.print(Day);
     lcd.setCursor(7, 1);
    lcd.print("/");
    lcd.setCursor(8, 1);
    lcd.print(Month);
     lcd.setCursor(10,1);
    lcd.print("/");
     lcd.setCursor(11, 1);
    lcd.print(Year);

    last_second = second_;

  }

   strcpy  (RemoteXY.time1,"Time");
   
   strcpy  (RemoteXY.hour1,Hour);
   strcpy  (RemoteXY.minute1,Min);
   strcpy  (RemoteXY.second1,Sec);

   strcpy  (RemoteXY.date1,"Date");
   
   strcpy  (RemoteXY.year1,Year);
   strcpy  (RemoteXY.month1,Month);
   strcpy  (RemoteXY.day1,Day);

  delay(200);



}
