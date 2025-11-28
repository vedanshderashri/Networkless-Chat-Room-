#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>

const char* ssid     = "ESP-Chat";
const char* password = "12345678"; 

ESP8266WebServer server(80); 
WebSocketsServer webSocket(81); 

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8" />
  <title>ESP Chat Room</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      background: #02040a;
      color: #e6f1ff;
      font-family: Arial, sans-serif;
      display: flex;
      flex-direction: column;
      height: 100vh;
    }
    header {
      padding: 10px 15px;
      border-bottom: 1px solid #1f2933;
      display: flex;
      justify-content: space-between;
      align-items: center;
      background: linear-gradient(90deg, #020617, #02040a);
    }
    .brand {
      font-size: 16px;
      letter-spacing: 2px;
      text-transform: uppercase;
      color: #38bdf8;
    }
    .status {
      font-size: 12px;
      color: #9ca3af;
    }
    .status span {
      display: inline-block;
      width: 8px;
      height: 8px;
      margin-right: 5px;
      border-radius: 50%;
      background: #ef4444;
    }
    .status.connected span {
      background: #22c55e;
    }
    #chat {
      flex: 1;
      padding: 10px;
      overflow-y: auto;
      background: radial-gradient(circle at top, #0b1120, #020617);
    }
    .msg {
      margin-bottom: 8px;
      padding: 6px 10px;
      border-radius: 6px;
      background: rgba(15, 23, 42, 0.85);
      border: 1px solid #1f2937;
      font-size: 14px;
      word-wrap: break-word;
    }
    .msg.me {
      border-color: #38bdf8;
      background: rgba(15, 23, 42, 0.95);
    }
    .meta {
      font-size: 11px;
      color: #9ca3af;
      margin-bottom: 2px;
    }
    .content {
      font-size: 14px;
    }
    #inputBar {
      padding: 8px;
      border-top: 1px solid #1f2933;
      background: #020617;
      display: flex;
      flex-direction: column;
      gap: 6px;
    }
    #nameInput, #msgInput {
      width: 100%;
      padding: 8px 10px;
      border-radius: 6px;
      border: 1px solid #1f2937;
      background: #020617;
      color: #e5e7eb;
      font-size: 14px;
    }
    #controls {
      display: flex;
      gap: 6px;
    }
    #msgInput {
      flex: 1;
    }
    button {
      padding: 8px 12px;
      border-radius: 6px;
      border: 1px solid #38bdf8;
      background: #020617;
      color: #e0f2fe;
      font-size: 14px;
      cursor: pointer;
      transition: background 0.15s, transform 0.05s;
      white-space: nowrap;
    }
    button:active {
      transform: scale(0.97);
    }
    button:disabled {
      opacity: 0.4;
      cursor: default;
    }
    @media (min-width: 600px) {
      #inputBar {
        flex-direction: column;
      }
      #controls {
        flex-direction: row;
      }
    }
  </style>
</head>
<body>
  <header>
    <div class="brand">ESP CHAT ROOM</div>
    <div id="status" class="status"><span></span>Connecting...</div>
  </header>

  <div id="chat"></div>

  <div id="inputBar">
    <input id="nameInput" type="text" placeholder="Enter your name (e.g. Vedansh)" />
    <div id="controls">
      <input id="msgInput" type="text" placeholder="Type a message and press Enter..." />
      <button id="sendBtn">Send</button>
    </div>
  </div>

  <script>
    let ws;
    let myName = "";
    const chat      = document.getElementById("chat");
    const statusEl  = document.getElementById("status");
    const nameInput = document.getElementById("nameInput");
    const msgInput  = document.getElementById("msgInput");
    const sendBtn   = document.getElementById("sendBtn");

    function setStatus(online, text) {
      statusEl.classList.toggle("connected", online);
      const span = statusEl.querySelector("span");
      statusEl.innerHTML = "";
      statusEl.appendChild(span);
      span.outerHTML = "<span></span>";
      statusEl.innerHTML = statusEl.innerHTML + (text || (online ? "Connected" : "Disconnected"));
    }

    function addMessage(name, text, me=false, system=false) {
      const box = document.createElement("div");
      box.className = "msg" + (me ? " me" : "");
      if(system) box.style.opacity = "0.75";

      const meta = document.createElement("div");
      meta.className = "meta";
      const now = new Date();
      const timeStr = now.toLocaleTimeString();
      meta.textContent = system ? "[SYSTEM] " + timeStr : name + " · " + timeStr;

      const content = document.createElement("div");
      content.className = "content";
      content.textContent = text;

      box.appendChild(meta);
      box.appendChild(content);
      chat.appendChild(box);
      chat.scrollTop = chat.scrollHeight;
    }

    function connectWS() {
      const host = window.location.hostname;
      ws = new WebSocket("ws://" + host + ":81/");

      ws.onopen = function() {
        setStatus(true, "Connected");
        addMessage("SYSTEM", "You joined the room.", false, true);
        sendBtn.disabled = false;
      };

      ws.onclose = function() {
        setStatus(false, "Disconnected (auto-retrying...)");
        sendBtn.disabled = true;
        setTimeout(connectWS, 2000);
      };

      ws.onmessage = function(event) {
        try {
          const data = JSON.parse(event.data);
          const from = data.name || "Anon";
          const msg  = data.msg || "";
          const me   = (from === myName && myName !== "");
          addMessage(from, msg, me, false);
        } catch(e) {
          addMessage("SYSTEM", event.data, false, true);
        }
      };
    }

    function sendMessage() {
      if(!ws || ws.readyState !== WebSocket.OPEN) return;
      const name = nameInput.value.trim() || "Guest";
      const text = msgInput.value.trim();
      if(!text) return;
      myName = name;
      const payload = JSON.stringify({ name: name, msg: text });
      ws.send(payload);
      msgInput.value = "";
    }

    sendBtn.addEventListener("click", sendMessage);
    msgInput.addEventListener("keydown", function(e) {
      if(e.key === "Enter") {
        sendMessage();
      }
    });

    window.addEventListener("load", function() {
      connectWS();
    });
  </script>
</body>
</html>
)rawliteral";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      String msg = "Client " + String(num) + " connected from " + ip.toString();
      Serial.println(msg);
     
      webSocket.sendTXT(num, "{\"name\":\"SYSTEM\",\"msg\":\"Connected to ESP Chat\"}");
      break;
    }
    case WStype_DISCONNECTED: {
      Serial.printf("Client %u disconnected\n", num);
      break;
    }
    case WStype_TEXT: {
      String text = String((char *)payload);
      Serial.printf("Got: %s\n", text.c_str());
      webSocket.broadcastTXT(text);
      break;
    }
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println("Starting ESP Chat AP...");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  Serial.println("HTTP server started");

 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started on port 81");
}

void loop() {
  webSocket.loop();
  server.handleClient();
}
