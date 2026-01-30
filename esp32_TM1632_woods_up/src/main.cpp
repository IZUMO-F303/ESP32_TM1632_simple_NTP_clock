#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <TM1637Display.h>
#include "wifi_credentials.h"



// NTP
const char* ntpServer1 = "ntp.nict.jp";
const char* ntpServer2 = "time.google.com";
const char* ntpServer3 = "ntp.jst.mfeed.ad.jp";
const long gmtOffset_sec = 9 * 3600; // JST (GMT+9)
const int daylightOffset_sec = 0;
unsigned long lastSyncTime = 0;
const unsigned long syncInterval = 3600000; // 1 hour

// TM1637
#define CLK 17
#define DIO 16
TM1637Display display(CLK, DIO);

void syncTime() {
  Serial.println("Starting WiFi and time sync...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.printf("Connecting to %s ", ssid);
  
  int connect_wait = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    connect_wait++;
    if (connect_wait > 20) { // 10秒待っても接続できなかったら
      Serial.println(" WiFi connection failed.");
      WiFi.mode(WIFI_OFF);
      return; // Wi-Fi接続に失敗したら同期処理を中断
    }
  }
  Serial.println(" CONNECTED");

  // NTP設定
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2, ntpServer3);
  
  // 時刻が取得できるまで少し待つ
  struct tm timeinfo;
  int retry_count = 0;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Waiting for time sync...");
    delay(1000);
    retry_count++;
    if (retry_count > 10) { // 10秒待っても同期できなかったら
        Serial.println("Time sync failed.");
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        return; // 同期に失敗したら中断
    }
  }
  Serial.println("Time successfully synced.");
  
  // WiFi切断
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  Serial.println("WiFi disconnected.");
  lastSyncTime = millis();
}

void adjustBrightness(struct tm* timeinfo) {
  uint8_t hour = timeinfo->tm_hour;
  if (hour >= 7 && hour < 19) {
    display.setBrightness(0x0f); // Daytime
  } else {
    display.setBrightness(0x08); // Nighttime
  }
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  adjustBrightness(&timeinfo);
  
  uint8_t hours = timeinfo.tm_hour;
  uint8_t minutes = timeinfo.tm_min;

  uint8_t segment_data[] = {
    display.encodeDigit(hours / 10),    // Hour Tens
    display.encodeDigit(hours % 10),    // Hour Units
    display.encodeDigit(minutes / 10),  // Minute Tens
    display.encodeDigit(minutes % 10)   // Minute Units
  };

  // 2桁目（時と分の間）にコロンを点灯させる
  segment_data[1] |= 0x80; // DP_BIT (or colon for 4-digit displays)

  display.setSegments(segment_data);
}

void setup()
{
  Serial.begin(115200);

  // 初回時刻同期
  syncTime();

  // 初回時刻表示
  printLocalTime();
}

void loop()
{
  // 1時間ごとに時刻を同期
  if (millis() - lastSyncTime > syncInterval) {
    syncTime();
  }

  // 1秒ごとに表示を更新
  printLocalTime();
  delay(1000);
}
