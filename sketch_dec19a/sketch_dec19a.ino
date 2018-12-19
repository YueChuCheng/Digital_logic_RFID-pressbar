#define led_pin 33
#define fsr_pin 32

#include <SPI.h>
#include <MFRC522.h> 
#define RST_PIN      22       // 讀卡機的重置腳位
#define SS_PIN       21        // 晶片選擇腳位
 MFRC522 mfrc522(SS_PIN, RST_PIN);  // 建立MFRC522物件

void setup()
{
  //Serial.begin(115200);
  //pinMode(led_pin, OUTPUT);
  //ledcSetup(0,5000,8);
  //ledcAttachPin(led_pin,0);
  
  Serial.begin(9600);
  Serial.println("RFID reader is ready!");
  SPI.begin();
  mfrc522.PCD_Init();   //初始化
}

void loop()
{
  /*int fsr_value = analogRead(fsr_pin); // 讀取FSR
  int led_value = map(fsr_value, 0, 1023, 0, 255); // 從0~1023映射到0~255
  ledcWrite(0, led_value); // 改變LED亮度
  Serial.println(fsr_value);
  Serial.println(led_value);
  Serial.println("-------------");
  delay(100);*/

   // 確認是否有新卡片
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
  
}
