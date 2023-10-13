/* 
 * |||||||||||||||||Components||||||||||||||||||||
 * ||                                           ||
 * ||1.ESP8266(NODEMCU);                        ||
 * ||2.LCD DISPLAY 20x4                         ||
 * ||3. i2c Lcd module                          ||
 * ||4.Some Jumper Wires;                       ||              
 * ||                                           ||
 * |||||||||||||||||||||||||||||||||||||||||||||||
 *    
 * 
 * |||||||||||||||||||||||Pinout|||||||||||||||||||||
 * ||                                              ||
 * ||i2c LCD module          TO         NODEMCU    ||
 * ||                                              ||
 * ||VCC                     =           VIN;      ||
 * ||GND                     =           GND;      ||
 * ||SCL                     =           D1;       ||
 * ||SDA                     =           D2;       ||
 * ||                                              ||
 * ||||||||||||||||||||||||||||||||||||||||||||||||||
 *
 * Github Repo Link: https://github.com/pollob1563/IOT-CLOCK   

 */

// include the library code:
#include <ESP8266WiFi.h>    // initialize the library by associating any needed ESP8266(NODEMCU) interface pin

#include <LiquidCrystal_I2C.h>

#include <NTPClient.h>

#include <WiFiUdp.h>


     /**//**//**//**//**//**//**//**//**//**//*Fill Up Your Information/**//**//**//**//**//**//**//**//**//**//***//**//***/
    /**/               const char* ssid = "******";              // Your WiFi SSID name                                 /**/
   /**/                const char* password = "************" ;  // Your WiFi password                                  /**/
  /**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**//**/

WiFiClient client; //Declear WiFiClient
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

float timeZone = 6; //BST 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", timeZone * 3600, 60000); //Udp& udp, server name, timezone, updateinterval

String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int flag = 0;
void setup() {
    lcd.init(); // initialize the lcd 
    lcd.backlight();
    lcd.begin(20, 4); //LCD
    lcd.clear();

    Serial.begin(115200); // Baud rate of Serial Monitor

    lcd.setCursor(0, 0);
    lcd.print("WiFi:"); //Print WiFi Name at LCD display
    lcd.print(ssid);

    Serial.print("WiFi: "); //Print WiFi Name at Serial Monitor
    Serial.println(ssid);

    delay(2000);

    WiFi.begin(ssid, password); //Connecting to WiFi
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("WiFi Connecting");
    Serial.println("WiFi Connecting"); //Print WiFi Status at Serial Monitor
    delay(8000); //Waitng For Connecting
    lcd.clear();

    while (WiFi.status() != WL_CONNECTED) {

        lcd.setCursor(0, 0);
        lcd.print("NOT CONNECTED"); //Print WiFi Status at LCD display (Confarmation Messege)

        Serial.println("WiFi NOT CONNECTED"); //Print WiFi Status at Serial Monitor(Confarmation Messege)
        Serial.println();
        delay(500);
        Serial.println("Wait a few Seconds or check your WiFi SSID and Password(In Credential Section)"); //If WiFi Not CONNECTED then the instruction print at Serial Monitor 
        delay(1000);
        lcd.clear();
    }

    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print("WiFi Connected!"); //Print WiFi Status at LCD Display(Confarmation Messege)
    Serial.println("Wifi Connected!"); //Print WiFi Status at Serial Monitor(Confarmation Messege)
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP()); //Print Gateway IP at LCD Display
    delay(100);
    Serial.print("Default Gateway: "); //Gateway IP
    Serial.println(WiFi.localIP()); //Print Gateway IP at Serial Monitor

    delay(2000);

    lcd.clear();

    timeClient.begin();

}

void loop() {
    timeClient.update();

    time_t epochTime = timeClient.getEpochTime();
    Serial.print("Epoch Time: ");
    Serial.println(epochTime);

    String formattedTime = timeClient.getFormattedTime();
    Serial.print("Formatted Time: ");
    Serial.println(formattedTime);

    int currentHour = timeClient.getHours();
    Serial.print("Hour: ");
    Serial.println(currentHour);

    int currentMinute = timeClient.getMinutes();
    Serial.print("Minutes: ");
    Serial.println(currentMinute);

    int currentSecond = timeClient.getSeconds();
    Serial.print("Seconds: ");
    Serial.println(currentSecond);

    String weekDay = weekDays[timeClient.getDay()];
    Serial.print("Week Day: ");
    Serial.println(weekDay);

    //Get a time structure
    struct tm * ptm = gmtime((time_t * ) & epochTime);

    int currentDate = ptm -> tm_mday;
    Serial.print("Date: ");
    Serial.println(currentDate);

    int currentMonth = ptm -> tm_mon + 1;
    Serial.print("Month: ");
    Serial.println(currentMonth);

    int currentYear = ptm -> tm_year + 1900;
    Serial.print("Year: ");
    Serial.println(currentYear);

    if (currentHour == 0 && flag == 0) {
        flag = 1;
        lcd.clear();
    }

    if (currentHour == 0) {
        currentHour = 12;

        //if(currentMinute<10) {String currentMinute = "0"+String(currentMinute);
        //if(currentSecond<10) String currentSecond = "0"+String(currentSecond);

        if (currentMinute < 10) {
            String currentMinuteStr = "00";
            currentMinuteStr[1] = char(currentMinute + 48);
            String currentMinute = currentMinuteStr;
        }
        if (currentSecond < 10) {
            String currentSecondStr = "00";
            currentSecondStr[1] = char(currentSecond + 48);
            String currentSecond = currentSecondStr;
        }

        lcd.setCursor(0, 0);
        lcd.print("Time: " + String(currentHour) + ":" + String(currentMinute) + ":" + String(currentSecond) + " AM");

        //if(currentDate<10) String currentDate = "0"+String(currentDate);
        //if(currentMonth<10) String currentMonth = "0"+String(currentMonth);
        if (currentDate < 10) {
            String currentDateStr = "00";
            currentDateStr[1] = char(currentDate + 48);
            String currentDate = currentDateStr;
        }
        if (currentMonth < 10) {
            String currentMonthStr = "00";
            currentMonthStr[1] = char(currentMonth + 48);
            String currentMonth = currentMonthStr;
        }

        lcd.setCursor(0, 1);
        lcd.print("Date: " + String(currentDate) + "/" + String(currentMonth) + "/" + String(currentYear));

        lcd.setCursor(0, 2);
        lcd.print("Day: " + weekDay);

    } 
    
    else if (currentHour > 12) {
        flag = 0;
        currentHour = currentHour - 12;

        //if(currentMinute<10) String currentMinute = "0"+String(currentMinute);
        //if(currentSecond<10) String currentSecond = "0"+String(currentSecond);
        if (currentMinute < 10) {
            String currentMinuteStr = "00";
            currentMinuteStr[1] = char(currentMinute + 48);
            String currentMinute = currentMinuteStr;
        }
        if (currentSecond < 10) {
            String currentSecondStr = "00";
            currentSecondStr[1] = char(currentSecond + 48);
            String currentSecond = currentSecondStr;
            //Serial.println("Before  :" + String(currentSecond)+'\n' );
        }
        

        lcd.setCursor(0, 0);
        lcd.print("Time: " + String(currentHour) + ":" + String(currentMinute) + ":" + String(currentSecond) + " PM");

        //if(currentDate<10) String currentDate = "0"+String(currentDate);
        //if(currentMonth<10) String currentMonth = "0"+String(currentMonth);
        if (currentDate < 10) {
            String currentDateStr = "00";
            currentDateStr[1] = char(currentDate + 48);
            String currentDate = currentDateStr;
        }
        if (currentMonth < 10) {
            String currentMonthStr = "00";
            currentMonthStr[1] = char(currentMonth + 48);
            String currentMonth = currentMonthStr;
        }

        lcd.setCursor(0, 1);
        lcd.print("Date: " + String(currentDate) + "/" + String(currentMonth) + "/" + String(currentYear));

        lcd.setCursor(0, 2);
        lcd.print("Day: " + weekDay);
    } 
    else {
        //if(currentHour<10) String currentHour = "0"+String(currentHour);
        //if(currentMinute<10) String currentMinute = "0"+String(currentMinute);
        //if(currentSecond<10) String currentSecond = "0"+String(currentSecond);
        if (currentHour < 10) {
            String currentHourStr = "00";
            currentHourStr[1] = char(currentHour + 48);
            String currentHour = currentHourStr;
        }
        if (currentMinute < 10) {
            String currentMinuteStr = "00";
            currentMinuteStr[1] = char(currentMinute + 48);
            String currentMinute = currentMinuteStr;
        }
        if (currentSecond < 10) {
            String currentSecondStr = "00";
            currentSecondStr[1] = char(currentSecond + 48);
            String currentSecond = currentSecondStr;
        }

        lcd.setCursor(0, 0);
        lcd.print("Time: " + String(currentHour) + ":" + String(currentMinute) + ":" + String(currentSecond) + " AM");

        //    if(currentDate<10) String currentDate = "0"+String(currentDate);
        //    if(currentMonth<10) String currentMonth = "0"+String(currentMonth);
        if (currentDate < 10) {
            String currentDateStr = "00";
            currentDateStr[1] = char(currentDate + 48);
            String currentDate = currentDateStr;
        }
        if (currentMonth < 10) {
            String currentMonthStr = "00";
            currentMonthStr[1] = char(currentMonth + 48);
            String currentMonth = currentMonthStr;
        }

        lcd.setCursor(0, 1);
        lcd.print("Date: " + String(currentDate) + "/" + String(currentMonth) + "/" + String(currentYear));

        lcd.setCursor(0, 2);
        lcd.print("Day: " + weekDay);

    }

    lcd.setCursor(0, 3);
    int hourLeft = 24 - timeClient.getHours() - 1;
    int minuteLeft = 60 - timeClient.getMinutes();
    int secondLeft = 60 - timeClient.getSeconds();

    //  if(hourleft<10) String hourleft = "0"+String(hourleft);
    //  if(minuteleft<10) String minuteleft = "0"+String(minuteleft);
    //  if(secondleft<10) String secondleft = "0"+String(secondleft);
    if (hourLeft < 10) {
        String hourLeftStr= "00";
        hourLeftStr[1] = char(hourLeft + 48);
        String hourLeft = hourLeftStr;
    }
    if (minuteLeft < 10) {
        String minuteLeftStr= "00";
        minuteLeftStr[1] = char(minuteLeft + 48);
        String minuteLeft = minuteLeftStr;
    }
    if (secondLeft < 10) {
        String secondLeftStr= "00";
        secondLeftStr[1] = char(secondLeft + 48);
        String secondLeft = secondLeftStr;
    }

    lcd.print("Time Left: " + String(hourLeft) + ":" + String(minuteLeft) + ":" + String(secondLeft));

    delay(1000);
}
