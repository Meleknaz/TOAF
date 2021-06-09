/*
 * 
 * Azot, Fosfor ve Potasyum ölçümü için Arduino'lu Toprak NPK Sensörü
 */
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// 128x64 olan i2c destekli Oled ekran modülü için 
#define SCREEN_WIDTH 128    // piksel OLED ekran genişliği,
#define SCREEN_HEIGHT 64    // OLED ekran yüksekliği, piksel cinsinden
#define OLED_RESET -1       // Sıfırlama pimi
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
#define RE 8
#define DE 7

// NPK sensörüne gönderilen Sorgulama çerçeveleri aşağıdadır.
//Azot, Fosfor ve Potasyum değerlerini okumak için
// nitro_inquiry_frame, phos_inquiry_frame ve pota_inquiry_frame adlarıyla üç dizi tanımladık
// Her sorgulama çerçevesinin 8 değeri vardır
const byte nitro_inquiry_frame[] = {0x01,0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};
const byte phos_inquiry_frame[] = {0x01,0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};
const byte pota_inquiry_frame[] = {0x01,0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};
 
byte values[11];
SoftwareSerial modbus(2,3);
 
void setup() {
  Serial.begin(9600);
  modbus.begin(9600);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //I2C adresi 0x3C (128x64) ile başlat
  delay(500);
  display.clearDisplay();
  display.setCursor(25, 15);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" NPK Sensor");
  display.setCursor(25, 35);
  display.setTextSize(1);
  display.print("Initializing");
  display.display();
  delay(2000);
}
 
void loop() {
  // değerlerini saklamak için byte türünde üç değişkene ihtiyacımız olacak
   // Azot, fosfor ve Potasyum. 
  byte nitrogen_val,phosphorus_val,potassium_val;

  nitrogen_val = nitrogen();
  delay(250);
  phosphorus_val = phosphorous();
  delay(250);
  potassium_val = potassium();
  delay(250);
  
  // Verileri seri monitöre göndermek için aşağıdaki kod kullanılır
   // ancak Bluetooth modülünü bağladığımız için veriyi şu adrese gönderecek:
   // Android cep telefonu Uygulaması
  
  Serial.print("Nitrogen_Val: ");
  Serial.print(nitrogen_val);
  Serial.println(" mg/kg");
  Serial.print("Phosphorous_Val: ");
  Serial.print(phosphorus_val);
  Serial.println(" mg/kg");
  Serial.print("Potassium_Val: ");
  Serial.print(potassium_val);
  Serial.println(" mg/kg");
  delay(2000);

// Verileri Oled ekranında görüntülemek için aşağıdaki kod kullanılır
  //Oled ekranını kullanmak istemiyorsanız aşağıdaki kodu silebilirsiniz.
  // Veya Oled ekran modülünü de bağlayabilirsiniz, bu şekilde verileri görüntüleyebilirsiniz.
  // Oled ekran modülünde ve veriler Android Uygulamasına da gönderilebilir.
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 5);
  display.print("N: ");
  display.print(nitrogen_val);
  display.setTextSize(1);
  display.print(" mg/kg");
 
  display.setTextSize(2);
  display.setCursor(0, 25);
  display.print("P: ");
  display.print(phosphorus_val);
  display.setTextSize(1);
  display.print(" mg/kg");
 
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print("K: ");
  display.print(potassium_val);
  display.setTextSize(1);
  display.print(" mg/kg");
 
  display.display();
}

/*
 * Şimdi Azot, Fosfor ve Potasyum değerlerini okumak için kullanıcı tanımlı üç fonksiyon oluşturmamız gerekecek
 * Bu kullanıcı tanımlı işlevler bayt türünde olacaktır.
 */
 
byte nitrogen(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(modbus.write(nitro_inquiry_frame,sizeof(nitro_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    // Sorgu çerçevesini NPK sensörüne gönderdiğimizde, yanıt çerçevesi ile yanıt verir.
     // şimdi yanıt çerçevesini okuyacağız ve değerleri[] dizisinde saklayacağız, bir for döngüsü kullanacağız.
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
   // Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4]; // yalnızca dizideki 4. konumda depolanan Azot değerini döndürür
}
 
byte phosphorous(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(modbus.write(phos_inquiry_frame,sizeof(phos_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
   // Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}
 
byte potassium(){
  digitalWrite(DE,HIGH);
  digitalWrite(RE,HIGH);
  delay(10);
  if(modbus.write(pota_inquiry_frame,sizeof(pota_inquiry_frame))==8){
    digitalWrite(DE,LOW);
    digitalWrite(RE,LOW);
    for(byte i=0;i<7;i++){
    //Serial.print(modbus.read(),HEX);
    values[i] = modbus.read();
    //Serial.print(values[i],HEX);
    }
    Serial.println();
  }
  return values[4];
}
