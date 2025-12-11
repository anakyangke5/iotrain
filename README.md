# 🌧️ Rain Monitoring System with ESP32

Sistem monitoring curah hujan real-time menggunakan ESP32, tipping bucket rain gauge, dengan integrasi Firebase dan Web Server lokal.

## 📋 Daftar Isi
- [Fitur](#fitur)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Cara Kerja](#cara-kerja)
- [Instalasi](#instalasi)
- [Konfigurasi](#konfigurasi)
- [Upload ke ESP32](#upload-ke-esp32)
- [Penggunaan](#penggunaan)
- [Troubleshooting](#troubleshooting)

---

## ✨ Fitur

- ✅ **Real-time monitoring** curah hujan
- ✅ **Web interface** dengan auto-refresh
- ✅ **Firebase integration** untuk cloud logging
- ✅ **Buzzer alert** setiap tipping terdeteksi
- ✅ **Akurasi 0.2mm** per tipping
- ✅ **Debouncing protection** anti false trigger
- ✅ **WiFi connectivity** untuk remote monitoring

---

## 🛠️ Hardware Requirements

| Komponen | Spesifikasi | Jumlah |
|----------|-------------|--------|
| ESP32 Development Board | Any ESP32 board | 1 |
| Tipping Bucket Rain Gauge | 0.2mm per tip (standar) | 1 |
| Buzzer | Active/Passive 5V | 1 |
| Kabel Jumper | Male-Female | Secukupnya |
| Power Supply | 5V USB atau adapter | 1 |

### 📌 Koneksi Pin

```
ESP32          →  Komponen
----------------------------
GPIO 27        →  Rain Sensor (Signal)
GPIO 26        →  Buzzer (+)
GND            →  Rain Sensor (GND) & Buzzer (-)
3.3V/5V        →  Rain Sensor (VCC)
```

**Diagram Koneksi:**
```
     ┌─────────────┐
     │    ESP32    │
     │             │
GPIO27├──────────┐  │
     │          │  │
GPIO26├────────┐ │  │
     │        │ │ │  │
  GND ├──┬───┬─┘ │ │
     │  │   │   │ │  │
  3.3V├──┼───┘   │ │  │
     └──┼────────┼─┼──┘
        │        │ │
        │    ┌───▼─▼────┐
        │    │  Buzzer  │
        │    └──────────┘
        │
    ┌───▼──────────┐
    │ Rain Sensor  │
    │ (Tipping     │
    │  Bucket)     │
    └──────────────┘
```

---

## 💻 Software Requirements

### 1. Arduino IDE
Download dan install dari [arduino.cc](https://www.arduino.cc/en/software)

### 2. ESP32 Board Manager
- Buka Arduino IDE
- File → Preferences
- Tambahkan URL ini di "Additional Board Manager URLs":
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- Tools → Board → Boards Manager
- Cari "ESP32" dan install

### 3. Library yang Diperlukan

Install via **Library Manager** (Sketch → Include Library → Manage Libraries):

| Library | Versi | Fungsi |
|---------|-------|--------|
| Firebase ESP Client | Latest | Integrasi Firebase |
| WiFi | Built-in | Koneksi WiFi |
| WebServer | Built-in | Web server lokal |

**Cara Install Firebase ESP Client:**
1. Library Manager → Cari "Firebase ESP Client"
2. Pilih by **Mobizt**
3. Install

---

## 🔧 Cara Kerja

### 1. **Tipping Bucket Mechanism**
```
Hujan turun → Bucket terisi → Tip (terbalik) → Trigger interrupt
→ Count++ → Buzzer bunyi → Kirim ke Firebase
```

### 2. **Interrupt System**
- Setiap kali bucket tip, pin GPIO27 akan **FALLING edge**
- ISR (Interrupt Service Routine) langsung eksekusi
- Debouncing 100ms mencegah double counting

### 3. **Data Flow**
```
Rain Sensor → ESP32 ISR → Counter → [Web Server + Firebase]
                              ↓
                          Buzzer Alert
```

---

## 📥 Instalasi

### Step 1: Clone/Download Kode
```bash
# Download file iotrain_copy_20251211161554.ino
```

### Step 2: Buka di Arduino IDE
- File → Open
- Pilih file `.ino`

---

## ⚙️ Konfigurasi

Edit bagian ini di kode:

### 1. WiFi Credentials
```cpp
#define WIFI_SSID "Arusbawah"          // ← Ganti dengan nama WiFi kamu
#define WIFI_PASSWORD "samono33"        // ← Ganti dengan password WiFi
```

### 2. Firebase Credentials
```cpp
#define API_KEY "AIzaSyDe9W577rTNSZpL8v3rMI9iNGWehULcKz8"  // ← API Key dari Firebase Console
#define DATABASE_URL "https://iotesp-e752f-default-rtdb.asia-southeast1.firebasedatabase.app/"  // ← Database URL dari Firebase
```

> **Note:** Firebase sudah dikonfigurasi dan siap digunakan. Jika ingin setup Firebase sendiri, hubungi admin project.

### 3. Pin Configuration (opsional)
```cpp
#define RAIN_PIN   27     // Pin untuk rain sensor
#define BUZZER_PIN 26     // Pin untuk buzzer
```

### 4. Kalibrasi Sensor (opsional)
```cpp
const float MM_PER_TIP = 0.2;  // Sesuaikan dengan spesifikasi sensor
```

---

## 📤 Upload ke ESP32

### 1. Koneksi Hardware
- Hubungkan ESP32 ke komputer via USB
- Pastikan driver CH340/CP2102 sudah terinstall

### 2. Pilih Board & Port
- Tools → Board → ESP32 Arduino → **ESP32 Dev Module**
- Tools → Port → Pilih COM port ESP32 (contoh: COM3)

### 3. Konfigurasi Upload
```
Upload Speed: 921600
CPU Frequency: 240MHz
Flash Frequency: 80MHz
Flash Mode: QIO
Flash Size: 4MB
Partition Scheme: Default 4MB
```

### 4. Upload
- Klik tombol **Upload** (→)
- Tunggu sampai "Hard resetting via RTS pin..."
- Jika gagal, tekan tombol BOOT di ESP32 saat uploading

### 5. Monitor Serial
- Tools → Serial Monitor
- Set baud rate: **115200**
- Lihat output:
```
Connecting WiFi....
✅ WiFi Connected
✅ Firebase Ready
✅ Web Server Active
192.168.1.100
```

---

## 🚀 Penggunaan

### 1. Akses Web Interface
- Buka browser
- Ketik IP address ESP32 (lihat di Serial Monitor)
- Contoh: `http://192.168.1.100`
- Halaman akan auto-refresh setiap 3 detik

**Tampilan Web:**
```
┌────────────────────────────┐
│     RAIN MONITOR           │
│                            │
│  Tipping: 125              │
│  Curah Hujan: 25.00 mm     │
└────────────────────────────┘
```

### 2. Monitor Firebase
1. Buka Firebase Console
2. Realtime Database
3. Lihat struktur data:
```json
{
  "rain": {
    "tipping": 125,
    "mm": 25.0
  }
}
```

### 3. Testing Sensor
- Tuang air ke rain gauge
- Dengar bunyi buzzer setiap tip
- Cek web interface & Firebase terupdate

---

## 🐛 Troubleshooting

### ❌ WiFi Tidak Connect
**Gejala:** Stuck di "Connecting WiFi..."
**Solusi:**
- Cek SSID dan password benar
- Pastikan WiFi 2.4GHz (ESP32 tidak support 5GHz)
- Coba pindah lebih dekat ke router
- Reset ESP32 dan coba lagi

### ❌ Firebase Error
**Gejala:** Data tidak masuk Firebase
**Solusi:**
- Pastikan API Key dan Database URL benar
- Cek Firebase Rules (set ke test mode dulu)
- Cek koneksi internet
- Lihat error di Serial Monitor

### ❌ Sensor Tidak Terdeteksi
**Gejala:** Tipping count tidak naik
**Solusi:**
- Cek koneksi kabel ke GPIO27
- Pastikan rain sensor dapat power (3.3V/5V)
- Test dengan multimeter (resistance berubah saat tip)
- Coba ganti pin dan update `#define RAIN_PIN`

### ❌ Buzzer Tidak Bunyi
**Gejala:** Counter naik tapi buzzer silent
**Solusi:**
- Cek polaritas buzzer (+/-)
- Pastikan buzzer dapat power
- Ganti buzzer (mungkin rusak)
- Test dengan digitalWrite manual

### ❌ Double Counting
**Gejala:** 1 tip terhitung 2x atau lebih
**Solusi:**
- Sudah ada debouncing 100ms, tapi bisa dinaikkan:
```cpp
if (now - lastInterrupt > 200) {  // Naikkan jadi 200ms
```

### ❌ Web Page Error
**Gejala:** Cannot access web page
**Solusi:**
- Pastikan ESP32 dan device di network yang sama
- Cek IP address di Serial Monitor
- Ping IP ESP32 dari command prompt
- Disable firewall sementara

### ❌ Serial Monitor Karakter Aneh
**Gejala:** Output Serial berantakan
**Solusi:**
- Set baud rate ke 115200
- Tekan tombol RESET di ESP32
- Cek kabel USB (ganti jika perlu)

---

## 📊 Data Structure Firebase

```json
{
  "rain": {
    "tipping": <integer>,    // Jumlah tipping total
    "mm": <float>            // Curah hujan dalam milimeter
  }
}
```

### Query Data (JavaScript - Web)
```javascript
firebase.database().ref('rain').on('value', (snapshot) => {
  const data = snapshot.val();
  console.log('Tipping:', data.tipping);
  console.log('Rainfall:', data.mm, 'mm');
});
```

---

## 📈 Improvement Ideas

### 1. Reset Counter
Tambah button reset atau reset otomatis setiap midnight:
```cpp
if (hour == 0 && minute == 0) {
  tipCount = 0;  // Reset harian
}
```

### 2. Timestamp
Simpan waktu setiap tip:
```cpp
Firebase.RTDB.setString(&fbdo, "/rain/lastTip", getTimestamp());
```

### 3. Rate Calculation
Hitung intensitas hujan per jam:
```cpp
float rainfallRate = (recentTips * MM_PER_TIP) / timeWindowHours;
```

### 4. Alert System
Kirim notifikasi jika hujan lebat:
```cpp
if (rainfallRate > 10) {  // >10mm/jam = hujan lebat
  sendAlert("Heavy Rain Detected!");
}
```

### 5. Data Logging
Simpan historical data dengan timestamp untuk grafik.

---

## 📝 Notes

- **Reset Counter**: Saat ini counter tidak reset otomatis. Untuk reset, restart ESP32 atau tambah fitur reset manual.
- **Power Loss**: Data counter akan hilang saat restart. Pertimbangkan simpan di EEPROM untuk persistensi.
- **Akurasi**: Tergantung kualitas tipping bucket. Kalibrasi dengan mengukur air actual vs reading.
- **Outdoor Use**: Gunakan waterproof enclosure untuk ESP32 dan komponen elektronik.

---

## 📜 License

Free to use untuk project personal dan educational.

---

## 👤 Author

erlindaannisa20@gmail.com

---

## 🤝 Contributing

Feel free to fork, modify, dan improve project ini!

---

**Happy Monitoring! 🌧️📊**
