//Pemanggilan library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>
#define BATAS_SENSORLDR 200
#define BATAS_SENSORHUJAN 900
#define WAKTU_PUTARMOTOR 100
#define KECEPATAN_MOTOR 85

//sistem pengaturan varibel dan penentuan sistem kerja variabel
#define BLYNK_PRINT Serial
#define ESP8266_BAUD 9600
#define DHTPIN 2   //Pin yangdigunakan untuk sensor suhu
#define DHTTYPE DHT22   //Menentukan jenis sensor suhu DHT11,DHT22,DHT21
#define lenght 13.0   //Panjang bargrapig
ESP8266 wifi(&Serial); 
DHT dht (DHTPIN, DHTTYPE);   //Inisialisasi sensor suhu

//Menentukan addres komunikasi I2C lcd dan jenis lcd
LiquidCrystal_I2C lcd (0x27, 20, 4);   

//koneksi jaringan
char auth[] = "6a54f48a31c9422f89b53bcbb289f310";
char ssid[] = "Ozan";
char pass[] = "";

//Deklarasi pin dan penggunaan variable
int buton1 = 10;
int buton2 = 9;
const byte pwml = 5;
const byte pwmr = 6;
const byte ml1 = 3;
const byte ml2 = 4;
const byte mr1 = 7;
const byte mr2 = 8;
byte hold1 = 0; // tahan putara motor1 jemur
byte hold2 = 0; // tahan putara motor1 balik
byte Stop = 0; // setop putaran motor1
byte hold11 = 0; // tahan putara motor2 jemur
byte hold22 = 0; // tahan putara motor2 balik
byte Stop1 = 0; // setop putaran motor2
byte hold111 = 0; // tahan putara motor2 jemur
byte hold222 = 0; // tahan putara motor2 balik
byte Stop11 = 0; // setop putaran motor2
int kondisi1;
int kondisi2;
int nilaitombolVirtual1;
int nilaitombolVirtual2;
double percent = 100.0;
unsigned char b;
unsigned int peace;
unsigned long sensor_hujan;
unsigned long sensor_ldr;
unsigned long sensor_suhu;
unsigned long manual1;
unsigned long manual2;

//Gambar bargrapig
byte p1[8] = {
  0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10
  };
byte p2[8] = {
  0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18
  };
byte p3[8] = {
  0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C
  };
byte p4[8] = {
  0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E
  };
byte p5[8] = {
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F
  };
  
//Gambar icon termometer
byte termometer[8] = {
  B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110
};
//Gambar icon kelembabpan
byte kelembabpan[8] = {
  B00100, B00100, B01010, B01010, B10001, B11011, B11111, B01110
};

void jemur1();
void kembali1();
void berhenti1();
void jemur2();
void kembali2();
void berhenti2();
void jemur22();
void kembali22();
void berhenti22();

void setup() {
  lcd.begin(); //Inisialisasi lcd
  dht.begin();
  pinMode (buton1, INPUT_PULLUP);
  pinMode (buton2, INPUT_PULLUP);
  pinMode (pwml, OUTPUT);
  pinMode (pwmr, OUTPUT);
  pinMode (ml1, OUTPUT);
  pinMode (ml2, OUTPUT);
  pinMode (mr1, OUTPUT);
  pinMode (mr2, OUTPUT);
  lcd.createChar (1, termometer);
  lcd.createChar (2, kelembabpan);
  lcd.createChar (3, p1);
  lcd.createChar (4, p2);
  lcd.createChar (5, p3);
  lcd.createChar (6, p4);
  lcd.createChar (7, p5);
  lcd.setCursor (0, 0);
  lcd.print ("NAMA :              ");
  lcd.setCursor (0, 1);
  lcd.print ("NPM  :              ");
  lcd.setCursor (0, 2);
  lcd.print ("       Info 1       ");
  lcd.setCursor (0, 3);
  lcd.print ("       Info 2       ");
  delay (3000);
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print ("connecting..........");
  // Set ESP8266 baud rate
  Serial.begin(ESP8266_BAUD);
  delay(10);
  
  Blynk.begin(auth, wifi, ssid, pass);
  lcd.setCursor (0, 0);
  lcd.print ("connected...........");
  sensor_suhu = millis();
  sensor_ldr = millis();
  lcd.createChar (1, termometer);
  lcd.createChar (2, kelembabpan);
  lcd.clear();
  sensor_suhu = millis();
  sensor_hujan = millis();
  sensor_ldr = millis();
  manual1 = millis();
  manual2 = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  if ((millis() - sensor_suhu) >= 1000) {
    lcd.setCursor (1, 1); lcd.write (1);
    lcd.setCursor (2, 1); lcd.print (t,1); lcd.print ((char)223); lcd.print ("C");
    lcd.setCursor (13, 1); lcd.write (2);
    lcd.setCursor (14, 1); lcd.print (h,1); lcd.print ("%");
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);
    sensor_suhu = millis();
  }
  
  unsigned int data_sensor_hujan1 = analogRead(A0);
  percent = data_sensor_hujan1/1024.0*100.0;
  if ((millis() - sensor_hujan) >= 1000) {
    lcd.setCursor (15, 2); lcd.print(percent,1); lcd.print("%");
  Blynk.virtualWrite(V5, percent); 
    lcd.setCursor (0, 2);
    double a = lenght/100*percent;
    if (a >= 1) {
      for (int i = 0; i < a; i++) {
        lcd.write(7);
        b = i;
        }
       a = a-b;
      }
      peace = a*5;
      switch (peace) {
        case 0: break;
        case 1: lcd.print((char)3); break;
        case 2: lcd.write(4); break;
        case 3: lcd.write(5); break;
        case 4: lcd.write(6); break;
           }
       for (int i = 0; i < (lenght-b); i++) {
           lcd.print (" ");
       }
     sensor_hujan = millis();
  }
  
  int data_sensor_ldr = analogRead(A2);
  if ((millis() - sensor_ldr) >= 1000) {
    lcd.setCursor (0, 0); lcd.print (" Suhu  Cahaya Lembab"); 
    lcd.setCursor (9, 1); lcd.print (data_sensor_ldr); lcd.print(" ");
  Blynk.virtualWrite(V2, data_sensor_ldr); 
    sensor_suhu = millis();
  }
  
  if (!digitalRead(buton1)) {
    kondisi1++;
  }
  if (kondisi1 == 1) {
    lcd.setCursor(0,3);
    lcd.print("             Jemur2 ");
    if (hold1 == 0) {
      jemur1();
    }
    if (Stop == 1) {
      
    }
  }
  if ( kondisi1 == 2) {
    lcd.setCursor(0,3);
    lcd.print("            Kembali2");
    if (hold2 == 0) {
      kembali1();
    }
    if (Stop == 1) {
      
    }
    kondisi1 = 0;
  }

  if (!digitalRead(buton2)) {
    kondisi2++;
  }
  if (kondisi2 == 1) {
    lcd.setCursor(0,3);
    lcd.print(" Jemur1             ");
    if (hold11 == 0) {
      jemur2();
    }
    if (Stop1 == 1) {
      
    }
  }
  if ( kondisi2 == 2) {
    lcd.setCursor(0,3);
    lcd.print("kembali1            ");
    if (hold22 == 0) {
      kembali2();
    }
    if (Stop1 == 1) {
      
    }
    kondisi2 = 0;
  }
  
  if (data_sensor_ldr >= BATAS_SENSORLDR && data_sensor_hujan1 >= BATAS_SENSORHUJAN ) {
    lcd.setCursor(0,3);
    lcd.print(" Jemur1      Jemur2 ");
    if (hold111 == 0) {
      jemur22();
    }
    if (Stop11 == 1) {
      
    }
  }
  if (data_sensor_ldr <= BATAS_SENSORLDR || data_sensor_hujan1 <= BATAS_SENSORHUJAN ) {
    lcd.setCursor(0,3);
    lcd.print("kembali1    Kembali2");
    if (hold222 == 0) {
      kembali22();
    }
    if (Stop11 == 1) {
      
    }
  }
}

BLYNK_WRITE (3) {
  int nilaitombolVirtual1 = param.asInt();
  if (nilaitombolVirtual1 == 1) {
    kondisi1++;
  }
  if (kondisi1 == 1) {
    lcd.setCursor(0,3);
    lcd.print("             Jemur2 ");
         if (hold1 == 0) {
          jemur1();
          }
         if (Stop == 1) {
          
          }
  }
  if ( kondisi1 == 2) {
    lcd.setCursor(0,3);
    lcd.print("            Kembali2");
         if (hold2 == 0) {
          kembali1();
          }
         if (Stop == 1) {
          
          }
       kondisi1 = 0;
  }
}


BLYNK_WRITE(4) {
  int nilaitombolVirtual2 = param.asInt();
  if (nilaitombolVirtual2 == 1) {
    kondisi2++;
  }
  if (kondisi2 == 1) {
    lcd.setCursor(0,3);
    lcd.print(" Jemur1             ");
        if (hold11 == 0) {
            jemur2();
            }
        if (Stop1 == 1) {
      
           }
         }
  if ( kondisi2 == 2) {
    lcd.setCursor(0,3);
    lcd.print("kembali1            ");
        if (hold22 == 0) {
            kembali2();
            }
        if (Stop1 == 1) {
      
            }
        kondisi2 = 0;
       }
}

void jemur1() {
  Stop = 0;
  for (int gerak = 0; gerak < WAKTU_PUTARMOTOR; gerak++) {
    analogWrite (pwml, KECEPATAN_MOTOR);
    digitalWrite (ml1, LOW);
    digitalWrite (ml2, HIGH);
    delay(10);
    if (gerak == WAKTU_PUTARMOTOR - 1) {
      hold1 = 1;
      hold2 = 0;
      berhenti1();
    }
  }
}

void kembali1() {
  Stop = 0;
  for (int gerak = 0; gerak < WAKTU_PUTARMOTOR; gerak++) {
    analogWrite (pwml, KECEPATAN_MOTOR);
    digitalWrite (ml1, HIGH);
    digitalWrite (ml2, LOW);
    delay(10);
    if (gerak == WAKTU_PUTARMOTOR - 1) {
      hold1 = 0;
      hold2 = 1;
      berhenti1();
    }
  }
}

void berhenti1() {
  analogWrite (pwml, 0);
  digitalWrite (ml1, LOW);
  digitalWrite (ml2, HIGH);
  Stop = 1;
  delay(10);
}
void jemur2() {
  Stop1 = 0;
  for (int gerak1 = 0; gerak1 < WAKTU_PUTARMOTOR; gerak1++) {
    analogWrite (pwmr, KECEPATAN_MOTOR);
    digitalWrite (mr1, LOW);
    digitalWrite (mr2, HIGH);
    delay(10);
    if (gerak1 == WAKTU_PUTARMOTOR - 1) {
      hold11 = 1;
      hold22 = 0;
      berhenti2();
    }
  }
}

void kembali2() {
  Stop1 = 0;
  for (int gerak1 = 0; gerak1 < WAKTU_PUTARMOTOR; gerak1++) {
    analogWrite (pwmr, KECEPATAN_MOTOR);
    digitalWrite (mr1, HIGH);
    digitalWrite (mr2, LOW);
    delay(10);
    if (gerak1 == WAKTU_PUTARMOTOR - 1) {
      hold11 = 0;
      hold22 = 1;
      berhenti2();
    }
  }
}

void berhenti2() {
  analogWrite (pwmr, 0);
  digitalWrite (mr1, LOW);
  digitalWrite (mr2, HIGH);
  Stop1 = 1;
  delay(10);
}


void jemur22() {
  Stop11 = 0;
  for (int gerak11 = 0; gerak11 < WAKTU_PUTARMOTOR; gerak11++) {
    analogWrite (pwmr, KECEPATAN_MOTOR);
    digitalWrite (mr1, LOW);
    digitalWrite (mr2, HIGH);
    analogWrite (pwml, KECEPATAN_MOTOR);
    digitalWrite (ml1, LOW);
    digitalWrite (ml2, HIGH);
    delay(10);
    if (gerak11 == WAKTU_PUTARMOTOR - 1) {
      hold111 = 1;
      hold222 = 0;
      berhenti22();
    }
  }
}

void kembali22() {
  Stop11 = 0;
  for (int gerak11 = 0; gerak11 < WAKTU_PUTARMOTOR; gerak11++) {
    analogWrite (pwmr, KECEPATAN_MOTOR);
    digitalWrite (mr1, HIGH);
    digitalWrite (mr2, LOW);
    analogWrite (pwml, KECEPATAN_MOTOR);
    digitalWrite (ml1, HIGH);
    digitalWrite (ml2, LOW);
    delay(10);
    if (gerak11 == WAKTU_PUTARMOTOR - 1) {
      hold111 = 0;
      hold222 = 1;
      berhenti22();
    }
  }
}

void berhenti22() {
  analogWrite (pwmr, 0);
  digitalWrite (mr1, LOW);
  digitalWrite (mr2, HIGH);
  analogWrite (pwml, 0);
  digitalWrite (ml1, LOW);
  digitalWrite (ml2, HIGH);
  Stop11 = 1;
  delay(10);
}

