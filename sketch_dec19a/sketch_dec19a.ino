//壓力感測器
#define led_pin 33
#define fsr_pin 32

//rfid
#include <SPI.h>
#include <MFRC522.h> 
#define RST_PIN      22       // 讀卡機的重置腳位
#define SS_PIN       21        // 晶片選擇腳位
MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件

//超距感測器
const int trigPin = 26; // The pin to which the TRIG is connected
const int echoPin = 25 ; // The pin to which the ECHO is connected
const int ledOnTime = 1000; // The time that the LED stays on, after detecting the motion (in milliseconds, 1000ms = 1s)
const int trigDistance = 20; // The distance (and lower than it) at which the sensor is triggered (in centimeters)
int duration;
int distance;

void setup()
{
  //壓力感測器
  Serial.begin(115200);
  pinMode(led_pin, OUTPUT);
  ledcSetup(0,5000,8);
  ledcAttachPin(led_pin,0);
 
  //rfid
  Serial.begin(9600);
  Serial.println("RFID reader is ready!");
  SPI.begin();
  mfrc522.PCD_Init();   //初始化
 
 //超距感測器
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()
{
  //壓力感測器
  int fsr_value = analogRead(fsr_pin); // 讀取FSR
  int led_value = map(fsr_value, 0, 1023, 0, 255); // 從0~1023映射到0~255
  ledcWrite(0, led_value); // 改變LED亮度
  Serial.println(fsr_value);
  Serial.println(led_value);
  Serial.println("-------------");
  delay(100);

  //rfid 確認是否有新卡片
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      byte *id = mfrc522.uid.uidByte;   // 取得卡片的UID
      byte idSize = mfrc522.uid.size;   // 取得UID的長度
 
      Serial.print("PICC type: ");      // 顯示卡片類型
      // 根據卡片回應的SAK值（mfrc522.uid.sak）判斷卡片類型
      MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
      Serial.println(mfrc522.PICC_GetTypeName(piccType));
 
      Serial.print("UID Size: ");       // 顯示卡片的UID長度值
      Serial.println(idSize);
 
      for (byte i = 0; i < idSize; i++) {  // 逐一顯示UID碼
        Serial.print("id[");
        Serial.print(i);
        Serial.print("]: ");
        Serial.println(id[i], HEX);       // 以16進位顯示UID值
      }
      Serial.println();
 
      mfrc522.PICC_HaltA();  // 讓卡片進入停止模式
    } 

//超距感測器
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH);
  delay(1);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
 
  if (distance <= trigDistance) {
    digitalWrite(led_pin, HIGH);
    delay(ledOnTime);
    digitalWrite(led_pin, LOW);
  }
  delay(100);
  
}
