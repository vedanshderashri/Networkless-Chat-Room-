Networkless ESP8266 Self-Hosted Wi-Fi Chat Server

A **standalone, offline, real-time chat application** built using **NodeMCU (ESP8266)**.  
The ESP8266 acts as a **Wi-Fi Access Point, HTTP server, and WebSocket server**, enabling multiple devices to communicate in real time **without internet, cloud services, sensors, or external modules**.


📌 Project Summary

This project demonstrates how an embedded device can fully replace a traditional server and router for local communication.  
Users connect directly to the Wi-Fi network created by the NodeMCU and open a browser-based chat interface hosted entirely on the ESP8266.

✅ No Internet  
✅ No Cloud  
✅ No External Hardware  
✅ Pure Embedded Networking


🚀 Key Features

- ✅ Standalone Wi-Fi Access Point (AP Mode)
- ✅ Real-time chat using WebSockets
- ✅ Multiple client support
- ✅ Browser-based UI (Mobile & Desktop)
- ✅ Zero dependency on cloud or router
- ✅ Fully offline operation

🏗️ System Architecture
[ Phone / Laptop Browser ]
│
▼
Wi-Fi (AP Mode)
│
▼
NodeMCU (ESP8266)
├── HTTP Server (UI Hosting)
├── WebSocket Server (Real-time Chat)
└── Message Broadcast Engine

🧠 How It Works

1. ESP8266 starts in **Access Point (AP) mode**
2. Devices connect to the ESP’s Wi-Fi network
3. ESP hosts:
   - Chat UI via HTTP
   - Real-time messaging via WebSockets
4. Messages are broadcast to all connected clients instantly

The ESP8266 functions as a **complete embedded chat server**.


🛠️ Tech Stack

#Hardware
- NodeMCU (ESP8266)

#Software
- Arduino Framework (C++)
- HTML, CSS, JavaScript (Frontend)

#Libraries
- `ESP8266WiFi`
- `ESP8266WebServer`
- `WebSocketsServer` by Markus Sattler (Links2004)


📋 Requirements

- Arduino IDE
- ESP8266 Board Package
- WebSockets Library (Links2004)
- USB cable & NodeMCU board


⚙️ Installation & Setup

1️⃣ Install ESP8266 Board Package
Add this URL in **Arduino IDE → Preferences**:
http://arduino.esp8266.com/stable/package_esp8266com_index.json

Then install **ESP8266 by ESP8266 Community** from Boards Manager.

2️⃣ Install WebSockets Library
- Open **Library Manager**
- Search **WebSockets**
- Install **WebSockets by Markus Sattler (Links2004)**

3️⃣ Upload Code
- Select board: `NodeMCU 1.0 (ESP-12E Module)`
- Select correct COM port
- Upload the sketch

📡 How to Use

1. Power the NodeMCU
2. Connect to Wi-Fi:
3. Open browser and go to: http://192.168.4.1
4. Enter your name and start chatting
5. Connect multiple devices to test real-time messaging

🎓 Use Cases

- Offline classroom communication
- Lab or campus internal chat
- Disaster or emergency networks
- Secure local communication
- Embedded networking demonstrations


🔮 Future Enhancements

- 🔐 User authentication
- 💾 Message history storage (SPIFFS)
- 🔔 Join/leave notifications
- 🎨 Jarvis / AI-themed UI
- 🔄 OTA firmware updates
- 🛡️ Message encryption

🎯 Learning Outcomes

- Embedded web servers
- WebSocket-based real-time systems
- ESP8266 Wi-Fi architecture
- IoT system design without sensors
- Client–server communication

👨‍💻 Author

**Vedansh Derashri**  
Web Developer & UI/UX/Graphic Designer



