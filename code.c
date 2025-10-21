// ESP32-CAM (AI-Thinker) + DHT11 + Blynk + Cảm biến ánh sáng
// Stream MJPEG sử dụng ESPAsyncWebServer (chunked response)
// MJPEG stream: chuỗi ảnh JPEG liên tiếp, tạo hiệu ứng như video

#define BLYNK_TEMPLATE_ID "TMPL6x4hXdSRt"
#define BLYNK_TEMPLATE_NAME "Denthongminh"
#define BLYNK_AUTH_TOKEN "oEbxCG8r-NiXNKMLyJ-nR2GqfDn9lU4i"

#include "esp_camera.h"               // Thư viện điều khiển camera
#include <WiFi.h>                     // Kết nối WiFi
#include <AsyncTCP.h>                 // TCP bất đồng bộ
#include <ESPAsyncWebServer.h>       // Web server bất đồng bộ
#include <BlynkSimpleEsp32.h>        // Kết nối Blynk
#include "DHT.h"                      // Đọc cảm biến DHT11

// ==== Thông tin WiFi & Blynk ====
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "denthongminh";
char pass[] = "12345678";

// ==== Cấu hình chân camera (AI-Thinker) ====
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// ==== Cảm biến DHT11 ====
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ==== Cảm biến ánh sáng & điều khiển đèn ====
#define LIGHT_SENSOR_PIN 12
#define LED_PIN 2
int lightState = -1;   // 0 = sáng, 1 = tối
int ledState = 0;      // 0 = tắt, 1 = bật
int manualMode = 0;    // 0 = tự động, 1 = thủ công

// ==== Khởi tạo server bất đồng bộ ====
AsyncWebServer server(80);

// ==== Biến lưu trạng thái DHT ====
float lastTemp = -100;
float lastHum  = -100;
const unsigned long DHT_INTERVAL = 10000UL; // Đọc DHT mỗi 10 giây

// ==== Khởi tạo camera ====
bool initCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Kiểm tra PSRAM để chọn cấu hình phù hợp
  if (psramFound()) {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 15;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return false;
  }
  return true;
}

// ==== Tạo MJPEG stream gửi tới trình duyệt ====
size_t mjpegStreamGenerator(uint8_t *buffer, size_t maxLen, size_t index) {
  static camera_fb_t * fb = NULL;
  static size_t pos = 0;
  static char headerBuf[64];
  static size_t header_len = 0;
  static size_t frame_len = 0;

  if (fb == NULL) {
    fb = esp_camera_fb_get();
    if (!fb) {
      const char err[] = "--frame\r\nContent-Type: text/plain\r\n\r\nCamera capture failed\r\n";
      size_t l = strlen(err);
      if (l > maxLen) l = maxLen;
      memcpy(buffer, err, l);
      return l;
    }
    header_len = snprintf(headerBuf, sizeof(headerBuf),
                          "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n",
                          (unsigned)fb->len);
    frame_len = fb->len;
    pos = 0;
  }

  size_t written = 0;
  if (pos < header_len) {
    size_t hleft = header_len - pos;
    size_t toCopy = (hleft < maxLen) ? hleft : maxLen;
    memcpy(buffer + written, headerBuf + pos, toCopy);
    written += toCopy;
    pos += toCopy;
    if (written >= maxLen) return written;
  }

  if (pos >= header_len && (pos - header_len) < frame_len) {
    size_t img_sent = pos - header_len;
    size_t img_left = frame_len - img_sent;
    size_t toCopy = (img_left < (maxLen - written)) ? img_left : (maxLen - written);
    memcpy(buffer + written, fb->buf + img_sent, toCopy);
    written += toCopy;
    pos += toCopy;
    if (written >= maxLen) return written;
  }

  if ((pos - header_len) >= frame_len) {
    const char tail[] = "\r\n";
    size_t tail_pos = (pos - header_len) - frame_len;
    size_t tail_left = 2 - tail_pos;
    if (tail_left > 0) {
      size_t toCopy = (tail_left < (maxLen - written)) ? tail_left : (maxLen - written);
      memcpy(buffer + written, tail + tail_pos, toCopy);
      written += toCopy;
      pos += toCopy;
      if (written >= maxLen) return written;
    }
    if ((pos - header_len) >= (frame_len + 2)) {
      esp_camera_fb_return(fb);
      fb = NULL;
    }
  }
  return written;
}

// ==== Thiết lập các route cho web server ====
void setupRoutes() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    const char * html =
      "<!DOCTYPE html><html><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
      "<title>ESP32-CAM Stream</title></head><body><h3>ESP32-CAM Stream</h3>"
      "<img src=\"/stream\" style=\"max-width:100%\"></body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/stream", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginChunkedResponse(
      "multipart/x-mixed-replace; boundary=frame",
      [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
        return mjpegStreamGenerator(buffer, maxLen, index);
      });
    request->send(response);
  });
}

// ==== Cập nhật dữ liệu DHT lên Blynk ====
void updateDHT() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(t) && fabs(t - lastTemp) >= 1.0) {
    Blynk.virtualWrite(V1, t);
    lastTemp = t;
    Serial.print("Nhiệt độ: "); Serial.println(t);
  }
  if (!isnan(h) && fabs(h - lastHum) >= 1.0) {
    Blynk.virtualWrite(V2, h);
    lastHum = h;
    Serial.print("Độ ẩm: "); Serial.println(h);
  }
}

// ==== Cập nhật trạng thái cảm biến ánh sáng ====
void updateLightSensor() {
  int current = digitalRead(LIGHT_SENSOR_PIN); // 0 = sáng, 1 = tối
  if (current != lightState) {
    lightState = current;
    Blynk.virtualWrite(V0, !lightState);
   