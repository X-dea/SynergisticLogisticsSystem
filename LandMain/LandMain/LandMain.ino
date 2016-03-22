#include <SPI.h>
#include <CTB_SIM900A.h>
#include <CTB_Stepper.h>
#include <PWMServoDriver.h>
int d[4]={0,0,0,0};//已占用货柜号
CTB_Stepper SM;
PWMServoDriver pwm = PWMServoDriver();
const int SMPinY=9,SMPinZ=7,PinY_CW=8,PinZ_CW=6,SPEED=100; //步进电机参数
//设置旋转角度
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  450 // this is the 'maximum' pulse length count (out of 4096)

void setup()
{
   Serial.begin(115200);
   SPI.begin();
   pwm.begin();
   SM.init(SMPinY,SMPinZ,PinY_CW,PinZ_CW,SPEED);
   Serial.println("16 channel Servo test!");
   pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}
void loop()
{
   char tel[11];
   char sms[50]={
    'Y','o','u',' ','h','a','v','e',' ','e','x','p','r','e','s','s','a','g','e','(','s',')',','
    ,'a','n','d',' ','t','h','e',' ','p','a','s','s','w','o','r','d',' ','i','s',':','*','*','*','*'};
   int c[4];//储存临时随机密码
   int x,y,i=0;//y：货柜号   st：载物台移动步数
   int st1,st2=5000;
   int servonum;
   CTB_SIM900A SIM;
   i=Serial.read();
   Serial.println(i);
   delay(1000);
   if(i==1)
   {
     //发送短信及取货密码
     SIM.init();//初始化SIM900A
     for(x=0;x<=10;x++)
     {
      tel[x]=Serial.read();//提取收件人电话号码
     }
     for(x=43;x<=46;x++)
     {
       sms[x]=random(1,8)+48;//产生随机密码
     }
     //暂存密码
     c[0]=sms[43];
     c[1]=sms[44];
     c[2]=sms[45];
     c[3]=sms[46];
     SIM.SendSMS(tel,sms);//发送短信
     y=random(1,4);//产生货柜号
     //存储货柜号
     for(x=0;x<=3;x++)
     {
       if(y==d[x])
       {
         y=random(1,4);
       }
     }y=1;
     for(x=0;x<=3;x++)
     {
       Serial.println(d[x]);
     }
     servonum=y-1;
     //取载物台移动步数
     if(y<=2)
     {
       st1=5000;
       st2=y*5000;
     }
     else
     {
       st1=5000;
       st2=y/2*5000;
     }
     //移动载物台至指定货柜
     for(x=0;x<=st1;x++)
     {
       SM.StepY(0);
     }
     for(x=0;x<=st2;x++)
     {
       SM.StepZ(0);
     }
     // Drive each servo one at a time
     Serial.print("servonum:");
     Serial.println(servonum);
     for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
       pwm.setPWM(0, 0, pulselen);
       pwm.setPWM(1, 0, pulselen);
     }
     delay(3000);
     for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
       pwm.setPWM(0, 0, pulselen);
       pwm.setPWM(1, 0, pulselen);
     }
     //载物台复位
     for(x=0;x<=st1;x++)
     {
       SM.StepY(1);
     }
     for(x=0;x<=st2;x++)
     {
       SM.StepZ(1);
     }
   }
}


String comdata = "";
void uart()
 {
     while (Serial.available() > 0)  
     {
         comdata += char(Serial.read());
         delay(2);
     }
 }