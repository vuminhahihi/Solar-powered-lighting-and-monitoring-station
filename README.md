# 🏠 Smart Light System with ESP32-CAM, DHT11, and Blynk
<img src="https://user-images.githubusercontent.com/47092672/97660147-142f1f00-1ab4-11eb-9d14-48f30a666cdc.PNG" width="12%" align="right">

[![Platform - ESP32](https://img.shields.io/badge/Platform-ESP32-blue)](https://www.espressif.com/en/products/socs/esp32)
[![Library - Blynk](https://img.shields.io/badge/Library-Blynk-green)](https://blynk.io/)
[![License - MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---

## 🧩 Overview

This project implements a **Smart Lighting System** using an **ESP32-CAM (AI-Thinker)** board combined with:
- **DHT11 sensor** (temperature & humidity)
- **Light sensor (LDR)**
- **Blynk IoT app** for remote monitoring and control  
- **ESPAsyncWebServer** for live MJPEG video streaming via Wi-Fi  

It supports **manual and automatic lighting modes**, and periodically uploads sensor data to **Blynk Cloud**.

---

## ⚙️ Features

✅ Real-time video streaming (MJPEG via HTTP)  
✅ DHT11 temperature & humidity monitoring  
✅ Automatic or manual LED lighting control  
✅ Light detection using LDR sensor  
✅ Remote monitoring & control via Blynk mobile app  
✅ Local web interface for camera preview  
✅ Auto reconnect to Wi-Fi and Blynk  

---

## 🧰 Hardware Requirements

| Component | Description |
|------------|-------------|
| ESP32-CAM (AI-Thinker) | Main board with camera module |
| DHT11 | Temperature & humidity sensor |
| LDR + Resistor | Light sensor circuit |
| LED | Controlled lighting device |
| Power Supply | 5V (≥2A recommended) |

---

## 🧠 Pin Configuration

| Component | GPIO Pin |
|------------|----------|
| DHT11 Data | GPIO 14 |
| LDR Sensor | GPIO 12 |
| LED Output | GPIO 2 |
| Camera Pins | AI-Thinker default configuration |

> The code uses the standard AI-Thinker camera pin mapping.

---

## 📦 Software Dependencies

Install the following libraries in **Arduino IDE** or **PlatformIO**:

- `esp_camera`
- `WiFi`
- `AsyncTCP`
- `ESPAsyncWebServer`
- `BlynkSimpleEsp32`
- `DHT sensor library`

---

## 🔧 Configuration

### 🔑 Blynk Setup
Create a new template in **Blynk Cloud**:
- **Template ID:** `"TMPL6x4hXdSRt"`
- **Template Name:** `"Denthongminh"`
- **Auth Token:** (replace with your token)

Assign the following **Virtual Pins**:
| Virtual Pin | Function |
|--------------|-----------|
| V0 | Light state indicator |
| V1 | Temperature |
| V2 | Humidity |
| V3 | Device IP |
| V4 | Manual LED control |
| V5 | Mode selection (Auto / Manual) |

---

## 🚀 How to Upload

1. Open the `.ino` or `.cpp` file in **Arduino IDE**.  
2. Select **Board:** “AI Thinker ESP32-CAM”.  
3. Choose the correct **Port**.  
4. Click **Upload**.  
5. Open the Serial Monitor (`115200 baud`) to check logs.  
6. Once connected, the ESP32-CAM will:
   - Connect to your Wi-Fi and Blynk
   - Display its IP in Serial/Blynk
   - Start the HTTP streaming server

---

## 🌐 Web Streaming Access

After booting, open your browser and go to:

