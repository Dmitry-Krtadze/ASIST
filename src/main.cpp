#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL45sRQ_T41"
#define BLYNK_TEMPLATE_NAME "ASSIST"
#define BLYNK_AUTH_TOKEN "3TBu1BrlTtNW7ZniYHa89NI-iALbuDk_"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "3TBu1BrlTtNW7ZniYHa89NI-iALbuDk_";
char ssid[] = "Triade";
char pass[] = "TriadeWell";

BlynkTimer timer;

int soilMoisturePin1 = 34;  // Аналоговый пин для первого датчика влажности почвы
int soilMoisturePin2 = 35;  // Аналоговый пин для второго датчика влажности почвы

bool buttonState = false;

// Функция для чтения данных с датчиков влажности
void sendSensorData() {
  if (buttonState) {  // Проверка состояния кнопки
    int soilMoistureValue1 = analogRead(soilMoisturePin1);
    float voltage1 = soilMoistureValue1 * (3.3 / 4095.0);
    int moisturePercent1 = map(soilMoistureValue1, 0, 4095, 0, 100);

    int soilMoistureValue2 = analogRead(soilMoisturePin2);
    float voltage2 = soilMoistureValue2 * (3.3 / 4095.0);
    int moisturePercent2 = map(soilMoistureValue2, 0, 4095, 0, 100);

    Blynk.virtualWrite(V1, moisturePercent1);  // Отправка данных первого датчика на виртуальный пин V1
    Blynk.virtualWrite(V2, moisturePercent2);  // Отправка данных второго датчика на виртуальный пин V2
  }
}

// Функция для обработки нажатия виртуальной кнопки
BLYNK_WRITE(V4) {
  buttonState = param.asInt();
  if (buttonState) {
    digitalWrite(4, HIGH);  // Включение реле
    Serial.println("Button ON");
  } else {
    digitalWrite(4, LOW);  // Выключение реле
    Serial.println("Button OFF");
  }
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(4, OUTPUT);  // Настройка 4-го пина как выход
  digitalWrite(4, LOW);  // Изначально реле выключено
  timer.setInterval(1000L, sendSensorData);  // Отправка данных каждую секунду
}

void loop() {
  Blynk.run();
  timer.run();
}
