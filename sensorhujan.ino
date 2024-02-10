#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// Kredensial jaringan WiFi
#define WIFI_SSID "****nama wifi****"
#define WIFI_PASSWORD "*****Passwordwifi****"
#define BOT_TOKEN "#########Token#############"
#define CHAT_ID "******idbot********"

// Tentukan pin sensor hujan
#define RAIN_SENSOR_PIN 35  // Ganti ini dengan pin digital sesuai koneksi sensor hujan

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
// Variabel penanda apakah sedang hujan atau tidak
bool isRaining = false;

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Mencoba untuk terhubung ke jaringan WiFi:
  Serial.print("Menghubungkan ke jaringan WiFi dengan SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Tambahkan sertifikat root untuk api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi terhubung. ");


  // Persiapan tambahan untuk sensor hujan
  pinMode(RAIN_SENSOR_PIN, INPUT);
  Serial.println("Persiapan sensor hujan selesai.");

  // Mengirim pesan saat bot dijalankan ke ID obrolan yang ditentukan
  bot.sendMessage(CHAT_ID, "Selamat datang di channel Pasti Dadi", "");
}

void loop() {
  // Membaca nilai digital dari sensor hujan
  int nilaiSensorHujan = digitalRead(RAIN_SENSOR_PIN);
  Serial.print("Nilai Sensor Hujan: ");
  Serial.println(nilaiSensorHujan);
  // Jika sensor mendeteksi hujan dan sebelumnya tidak hujan, kirim pesan
  if (nilaiSensorHujan == HIGH && !isRaining) {
    Serial.println("Hujan Berhenti! Mengirim pesan...");
    String welcome = "*Hujan telah berhenti??.*\n";
    welcome += "_Anda bisa keluar sekarang._\n";
    bot.sendMessage(CHAT_ID, welcome, "Markdown");
    isRaining = true;
    delay(20000);  // Jeda selama 5 menit untuk menghindari pengiriman pesan berulang dengan cepat
  }
  // Jika hujan sudah berhenti dan sebelumnya hujan, kirim pesan
  else if (nilaiSensorHujan == LOW && isRaining) {
    Serial.println("Hujan Turun! Mengirim pesan...");
    String welcome = "*Hujan Turun!!.*\n";
    welcome += "_Cari tempat berteduh!._\n";
    bot.sendMessage(CHAT_ID, welcome, "Markdown");
    isRaining = false;
    delay(20000);  // Jeda selama 5 menit untuk menghindari pengiriman pesan berulang dengan cepat
  }
}