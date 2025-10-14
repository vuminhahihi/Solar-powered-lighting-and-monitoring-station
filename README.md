# 🏠 Smart Light System with ESP32-CAM, DHT11, and Blynk
<img src="https://user-images.githubusercontent.com/47092672/97660147-142f1f00-1ab4-11eb-9d14-48f30a666cdc.PNG" width="12%" align="right">

[![Platform - ESP32](https://img.shields.io/badge/Platform-ESP32-blue)](https://www.espressif.com/en/products/socs/esp32)
[![Library - Blynk](https://img.shields.io/badge/Library-Blynk-green)](https://blynk.io/)
[![License - MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## 🧩 Overview

This project demonstrates a **Smart Light IoT System** built using **ESP32-CAM**, **DHT11**, **LDR sensor**, and **Blynk IoT Cloud**.  
The system streams live camera video via Wi-Fi and intelligently controls lighting automatically or manually through the Blynk app.

---

## ⚙️ Features

- 📸 Real-time video streaming (MJPEG)
- 🌡️ Temperature & humidity monitoring (DHT11)
- 💡 Automatic and manual light control
- 🌞 Light-dependent switching (LDR)
- 🌐 Remote access via Blynk mobile app
- 🔁 Auto reconnect Wi-Fi & Cloud

---

## 🧰 Hardware Components

| Component | Description |
|------------|-------------|
| ESP32-CAM (AI-Thinker) | Main MCU + camera |
| DHT11 | Temperature & humidity sensor |
| LDR sensor | Light intensity detection |
| LED | Smart lighting device |
| Power | 5V DC (≥2A recommended) |

---

## ⚡ Circuit Overview

| Component | GPIO |
|------------|------|
| DHT11 | GPIO 14 |
| LDR | GPIO 12 |
| LED | GPIO 2 |
| Camera | Standard AI Thinker mapping |

---

## 🧩 Mechanical Design (SolidWorks)

All mechanical parts are designed in **SolidWorks** for 3D printing and assembly.  
The design includes the housing, camera cover, pin holders, and light frame.

| Hình ảnh | Tên chi tiết | Mô tả |
|:--:|:--|:--|
| ![Khớp bi cầu](images/khopbicau.PNG) | Khớp bi cầu | Dùng để điều chỉnh góc xoay của camera |
| ![LED housing](images/led.PNG) | Vỏ đèn LED | Bảo vệ và cố định module LED |
| ![Mạch chính](images/machchinh.PNG) | Mạch chính | Bo mạch chứa ESP32-CAM và module nguồn |
| ![Pin](images/pin.PNG) | Hộp pin | Chứa pin cấp nguồn cho toàn hệ thống |
| ![Thanh ngang](images/thanhngang.PNG) | Thanh ngang | Giá đỡ liên kết giữa hộp pin và khung chính |
| ![Trẹo đèn](images/treoden.PNG) | Trẹo đèn | Cố định và hướng đèn về khu vực mong muốn |
| ![Khung nhôm 220mm](images/220mm.PNG) | Thanh nhôm định hình 220mm | Khung chính lắp các module |
| ![Hộp camera](images/hopcam.PNG) | Hộp camera | Bảo vệ module ESP32-CAM |
| ![Hộp điện](images/hopdien.PNG) | Hộp điện | Chứa dây nối và module điện |
| ![Kê góc vuông](images/kegocvuong.PNG) | Kê góc vuông | Giữ cố định góc giữa các thanh nhôm |

👨‍💻 **Designer:** *Vũ Ngọc Minh*

---

## 🎬 Demo Video

🎥 Watch project demonstration on YouTube:  
[![Smart Light Video](https://img.youtube.com/vi/zmhjNo55GJg/0.jpg)](https://youtu.be/zmhjNo55GJg)

---

## 🧾 License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT).

---

## 📚 References

- [Blynk Cloud Platform](https://blynk.cloud/)
- [ESP32-CAM Documentation](https://randomnerdtutorials.com/esp32-cam-projects/)
- [ESPAsyncWebServer GitHub](https://github.com/me-no-dev/ESPAsyncWebServer)
