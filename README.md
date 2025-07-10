# ESP32-Based Automatic Medicine Dispenser

An IoT-based smart medicine dispenser using ESP32, designed to automatically dispense medicines at scheduled times or on user request. It features a real-time clock (via Wi-Fi), 360° servo motors, an LCD display, buzzer alerts, and IR sensors for medicine detection and user confirmation.

---

## 🔧 Features

- ⏰ **Time-based dispensing** using Wi-Fi synced clock
- ⚙️ **Two 360° servo motors** to dispense up to two different medicines
- 🧠 **IR sensor-based confirmation** to check if the user has taken the medicine
- 🔊 **Buzzer alerts** until medicine is taken
- 🌐 **Web interface** to trigger dispensing manually
- 💡 **RGB LEDs** for visual feedback
- 📟 **1602A I2C LCD** for real-time status display

---
## 📷 Image – Medicine Dispensing

Here's a photo showing the actual dispensing mechanism in action:

![Medicine Dispensing](https://drive.google.com/uc?export=view&id=1rSmcthVzO44-cye45sX7V2FF3ip7n5bf)

---

## 🎥 Demo Video

Watch the full working demo of the automatic medicine dispenser:

🔗 [Click to watch demo](https://drive.google.com/file/d/1GtldoK9w6m0OWXqOJZqCajpK8JYOP-7j/view?usp=sharing)


---

## 🧰 Hardware Used

- [x] ESP32 Dev Module  
- [x] 2x 360° Servo Motors  
- [x] IR Proximity Sensor (LM393)  
- [x] 1602A LCD with I2C module  
- [x] TBM12A05 Buzzer  
- [x] RGB LEDs (Common Cathode)  
- [x] LM2596 Buck Converter  
- [x] Breadboard + jumper wires  
- [x] 12V adapter  

---

## 🛠️ Software and Libraries

- Arduino IDE  
- Libraries used:
  - `WiFi.h`  
  - `Wire.h`  
  - `LiquidCrystal_I2C.h`  
  - `WebServer.h`  
  - `time.h`  
  - `ESP32Servo.h`

---


## 🚀 How It Works

1. ESP32 connects to Wi-Fi and syncs time via NTP  
2. At predefined times, the servo rotates to drop a medicine  
3. LCD displays status; RGB LEDs glow based on which medicine is dispensed  
4. IR sensor checks if the user has taken the medicine  
5. If not taken, buzzer rings continuously until detected  
6. Web interface also allows manual dispensing

---

## 🧪 Future Improvements

- Add push notifications via Telegram/IFTTT  
- Add pill count tracking using load cell  
- Integrate real-time mobile app interface

---

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🤝 Acknowledgments

Special thanks to **Robotic Society of kalyani Government Engineering college ** for the mentorship and resources.

---

## 🔗 Developed By

**Ankush Mondal**  
[GitHub](https://github.com/TFSIIFURY) • [LinkedIn](www.linkedin.com/in/ankush-mondal-kgec)



