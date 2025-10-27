//Nama : Sultan Naufal Al Faiq
// NIM : 24/544318/SV/25383
#include <Wire.h> //ini memanggil library yang digunakan pada program ini
#include <ESP32Servo.h> //ini juga
#include "MPU6050.h"// ini juga

MPU6050 mpu;

//ini mendefinisikan kalau servo 1 dan seterusnya terhubung ke pin 25,32 dan seterusnya
#define SERVO1 25
#define SERVO2 32 
#define SERVO3 17 
#define SERVO4 16 
#define SERVO5 33

Servo s1, s2, s3, s4, s5;// membuat variable bernama s1 dan seterusnya

// Posisi awal servo 0 derajat = tegak lurus
int initialPos = 90;  

void setup() {
  Serial.begin(115200);// memulai serial begin di serial monitor
  Wire.begin();
  mpu.initialize();

  //ini menginisialisasikan bahwa variable yang sudah dibuat tadi memuat input SERVO1, dan seterusnya
  s1.attach(SERVO1);
  s2.attach(SERVO2);
  s3.attach(SERVO3);
  s4.attach(SERVO4);
  s5.attach(SERVO5);

  //ini digunakan untuk menginisialisasi kalau semua servo dimulai dari set awal di initialPos
  s1.write(initialPos);
  s2.write(initialPos);
  s3.write(initialPos);
  s4.write(initialPos);
  s5.write(initialPos);

  Serial.println("Sistem siap");// tampilan awal serial monitro
}

void loop() {
  // ini membaca data dari MPU6050
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  //ini menghitung sudut dari sensor mpu5060nya
  float roll  = atan2(ay, az) * 180 / PI;
  float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / PI;
  float yaw   = gz / 131.0;

  //RULE 1: Roll (Servo 1 & 2 berlawanan arah)
  int servoRoll = map(roll, -90, 90, 0, 180);
  s1.write(servoRoll); // lawan arah
  s2.write(180 - servoRoll);

  //RULE 2: Pitch (Servo 3 & 4 searah)
  int servoPitch = map(pitch, -90, 90, 0, 180);
  s3.write(servoPitch);
  s4.write(servoPitch);

  //RULE 3: Yaw (Servo 5 mengikuti, lalu kembali)
  int servoYaw = map(yaw, -90, 90, 0, 180);
  s5.write(servoYaw);
  delay(1000); //diam 1 detik
  s5.write(initialPos); //kembali ke posisi awal

  //RULE 4: Gerakan eksternal
  // Misalnya threshold percepatan > 15000
  if (abs(ax) > 15000 || abs(ay) > 15000 || abs(az) > 15000) {
    Serial.println("Gerakan eksternal terdeteksi!");
    //Semua servo ke posisi bebas (misal 45 derajat)
    s1.write(45);
    s2.write(45);
    s3.write(135);
    s4.write(135);
    s5.write(90);
    delay(1000);
    //Kembali ke posisi awal
    s1.write(initialPos);
    s2.write(initialPos);
    s3.write(initialPos);
    s4.write(initialPos);
    s5.write(initialPos);
  }

  delay(50); //loop stabil
}