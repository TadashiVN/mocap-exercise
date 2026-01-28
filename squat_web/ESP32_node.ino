#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

/* ================= USER CONFIG ================= */
#define NODE_ID 1
// 1 = head
// 2 = left_wrist
// 3 = right_wrist
// 4 = left_ankle
// 5 = right_ankle

#define SAMPLE_RATE_HZ 50
#define SAMPLE_INTERVAL_MS (1000 / SAMPLE_RATE_HZ)
#define ERROR_LED_PIN 2   

/* ================= GLOBAL ================= */
Adafruit_MPU6050 mpu;

unsigned long startTime = 0;
unsigned long nextSampleTime = 0;
uint32_t seq = 0;

bool mpuReady = false;
uint32_t errorCount = 0;

/* ================= SETUP ================= */
void setup() {
  pinMode(ERROR_LED_PIN, OUTPUT);
  digitalWrite(ERROR_LED_PIN, LOW);

  Serial.begin(115200);
  delay(200);

  Serial.println();
  Serial.println("# ================================");
  Serial.println("# ESP32 MPU6050 Motion Capture");
  Serial.println("# ================================");
  Serial.print("# NODE_ID: ");
  Serial.println(NODE_ID);
  Serial.print("# Sample Rate: ");
  Serial.print(SAMPLE_RATE_HZ);
  Serial.println(" Hz");

  /* I2C */
  Wire.begin();
  Wire.setClock(400000);

  /* MPU INIT */
  Serial.println("# Initializing MPU6050...");
  if (!mpu.begin()) {
    Serial.println("# ERROR: MPU6050 not found!");
    blinkError();
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("# MPU6050 READY");

  delay(100);

  Serial.println("# Calibrating... Keep sensor still");
  calibrateSensor();

  Serial.println("# STREAM START");
  Serial.println("# Format:");
  Serial.println("# NODE_ID,SEQ,LOCAL_TS,AX,AY,AZ,GX,GY,GZ");
  Serial.println("# ================================");

  startTime = millis();
  nextSampleTime = startTime;
  mpuReady = true;
}

/* ================= LOOP ================= */
void loop() {
  if (!mpuReady) return;

  unsigned long now = millis();

  // Fixed-rate scheduler (NO DRIFT)
  if ((long)(now - nextSampleTime) >= 0) {
    nextSampleTime += SAMPLE_INTERVAL_MS;
    sampleAndSend();
  }
}

/* ================= SAMPLE ================= */
void sampleAndSend() {
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  if (!isValidData(accel, gyro)) {
    errorCount++;
    if (errorCount % 20 == 0) blinkOnce();
    return;
  }

  seq++;
  unsigned long localTs = millis() - startTime;

  sendCSV(
    NODE_ID,
    seq,
    localTs,
    accel.acceleration.x,
    accel.acceleration.y,
    accel.acceleration.z,
    gyro.gyro.x,
    gyro.gyro.y,
    gyro.gyro.z
  );
}

/* ================= CSV OUTPUT ================= */
void sendCSV(int id, uint32_t seq, unsigned long ts,
             float ax, float ay, float az,
             float gx, float gy, float gz) {

  Serial.print(id); Serial.print(",");
  Serial.print(seq); Serial.print(",");
  Serial.print(ts); Serial.print(",");
  Serial.print(ax, 3); Serial.print(",");
  Serial.print(ay, 3); Serial.print(",");
  Serial.print(az, 3); Serial.print(",");
  Serial.print(gx, 3); Serial.print(",");
  Serial.print(gy, 3); Serial.print(",");
  Serial.println(gz, 3);
}

/* ================= VALIDATION ================= */
bool isValidData(sensors_event_t &a, sensors_event_t &g) {
  if (isnan(a.acceleration.x) || isnan(a.acceleration.y) || isnan(a.acceleration.z)) return false;
  if (isnan(g.gyro.x) || isnan(g.gyro.y) || isnan(g.gyro.z)) return false;

  if (abs(a.acceleration.x) > 100) return false;
  if (abs(a.acceleration.y) > 100) return false;
  if (abs(a.acceleration.z) > 100) return false;

  if (abs(g.gyro.x) > 50) return false;
  if (abs(g.gyro.y) > 50) return false;
  if (abs(g.gyro.z) > 50) return false;

  return true;
}

/* ================= CALIBRATION ================= */
void calibrateSensor() {
  const int N = 100;
  for (int i = 0; i < N; i++) {
    sensors_event_t a, g, t;
    mpu.getEvent(&a, &g, &t);
    delay(10);
  }
  Serial.println("# Calibration done");
}

/* ================= LED ================= */
void blinkOnce() {
  digitalWrite(ERROR_LED_PIN, HIGH);
  delay(5);
  digitalWrite(ERROR_LED_PIN, LOW);
}

void blinkError() {
  while (1) {
    digitalWrite(ERROR_LED_PIN, HIGH);
    delay(200);
    digitalWrite(ERROR_LED_PIN, LOW);
    delay(200);
  }
}
