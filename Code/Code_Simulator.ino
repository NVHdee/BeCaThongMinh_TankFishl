#include <Wire.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
RTC_DS1307 rtc ;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo_1;

#define led_feed 8
#define led_DC 13
#define Lm35 A0
#define btn_Feed 12
#define DC 7
#define Trig_HCR04 4
#define Echo_HCR04 3
int i = 0;
boolean flags = true;

void setup()
{
    if (!rtc.begin()  ){
        while(1);
    }
    if(!rtc.isrunning()){
    }
    //
    pinMode(Trig_HCR04,OUTPUT);
    pinMode(Echo_HCR04,INPUT);
    //
   // pinMode(btn_Feed,INPUT_PULLUP);
    //
    servo_1.attach(6);
    servo_1.write(0);

    //
    pinMode(DC,OUTPUT);
    //
   // pinMode(led_DC,OUTPUT);
    //pinMode(led_feed,OUTPUT);
    rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
}

void active_servo(int pos){
    servo_1.write(pos);
    delay(200);
    servo_1.write(pos);
    delay(500);
    servo_1.write(0);
}

int level_water(){
    float duration = pulseIn(Echo_HCR04,1,30000);
    float s=(duration*0.034/2);
    int muc = map(s, 0 , 100,0,100);
    return muc;
}

bool check_time_feed(){
  
    DateTime now = rtc.now();
    int h = now.hour();
    int m = now.minute();
    int s = now.second();
    if (h ==12 || h==0){
        if (m == 0 && s == 00){
            return true;
        }
        return false;
    }
    return false;
}

String time_now(){
    String res = "";
    DateTime now = rtc.now();
    if(now.hour()<=9){
        res+="0"+String(now.hour())+":";
    }else{
        res+=String(now.hour())+":";
    }
    if(now.minute()<=9){
        res+="0"+String(now.minute())+":";
    }else{
        res+=String(now.minute())+":";
    }
    if(now.second()<=9){
        res+="0"+String(now.second());
    }else{
        res+=String(now.second());
    }
    return res;
}

void scr_normal(){
    String str1 = "Nhiet Do: "+String(res_Template());
    String str2 = time_now();
    show_res(str1,str2);
}

void check_MucNuoc(){
    digitalWrite(Trig_HCR04,0);
    delayMicroseconds(2);
    digitalWrite(Trig_HCR04,1);
    delayMicroseconds(10);
    digitalWrite(Trig_HCR04,1);
    int muc = level_water();
    while (muc>=6){
       // digitalWrite(led_DC,HIGH);
        digitalWrite(DC,1);
        show_res("Bom Dang Hoat Dong!","...Wait!");
        muc = level_water();
    }
    //digitalWrite(led_DC,LOW);
    digitalWrite(DC,0);
    scr_normal();
}

void Feed_Fish(String STAT){
    //digitalWrite(led_feed,HIGH);
    show_res("Dang Cho Can An: ",STAT+" !");
    active_servo(30);
    delay(1000);
    //digitalWrite(led_feed,LOW);
}

void show_res(String str1,String str2){
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(str1);
    lcd.setCursor(0, 1);
    lcd.print(str2);
}

float res_Template(){
    float lm_val = analogRead(Lm35);
    float res_cal = (lm_val * 500) / 1023;
    return res_cal;
}

void loop()
{
  
    scr_normal();
    check_MucNuoc();
    if (check_time_feed()){
            Feed_Fish("Tu Dong");
    }
    
    
  /*
    //Kiem Tra Xem Nut bam Co sang hay ko
    if (digitalRead(btn_Feed) == LOW){
        scr_normal();
        check_MucNuoc();
        if (check_time_feed()){
           Feed_Fish("Tu Dong");
        }
    }else{
        Feed_Fish("Binh Thuong");
    }*/
 }
