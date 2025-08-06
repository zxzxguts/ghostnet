#include <WiFi.h>
#include <WebServer.h>
#include "tvcodes.h"

#define IRLED 26
#define LED 15

// Configuração do ponto de acesso
const char* ssid = "TV-B-GONE-AP";
const char* password = "desligatvs";

WebServer server(80);
bool isAttacking = false;

void sendIRCode(int region, int codeIndex) {
  const int *protocols;
  const unsigned short *irCodes;
  int numProtocols = 0;

  if(region == 0) { // EU
    protocols = codes_eu;
    irCodes = codes2_eu;
    numProtocols = sizeof(codes_eu) / (3 * sizeof(int));
  } else { // US
    protocols = codes_us;
    irCodes = codes2_us;
    numProtocols = sizeof(codes_us) / (3 * sizeof(int));
  }

  if(codeIndex >= numProtocols) return;

  const int *proto = protocols + (3 * codeIndex);
  int freq = pgm_read_dword(proto);
  const unsigned short *code = irCodes + pgm_read_dword(proto + 2);
  int codeLength = pgm_read_dword(proto + 1);

  for (int k = 0; k < codeLength; k += 2) {
    uint16_t ontime = pgm_read_word(code + k);
    uint16_t offtime = pgm_read_word(code + k + 1);
    
    unsigned long startTime = micros();
    if(freq) {
      // Geração manual de PWM (38kHz)
      while(micros() - startTime < ontime * 10) {
        digitalWrite(IRLED, HIGH);
        delayMicroseconds(8);
        digitalWrite(IRLED, LOW);
        delayMicroseconds(18);
      }
    } else {
      // Sinal contínuo
      digitalWrite(IRLED, HIGH);
      delayMicroseconds(ontime * 10);
      digitalWrite(IRLED, LOW);
    }
    delayMicroseconds(offtime * 10);
  }
}

void startAttack() {
  if(isAttacking) return;
  isAttacking = true;c:\Users\Gusta\Downloads\projetor\BenQ (1).h c:\Users\Gusta\Downloads\projetor\BenQ (2).h c:\Users\Gusta\Downloads\projetor\BenQ (3).h c:\Users\Gusta\Downloads\projetor\BenQ (4).h c:\Users\Gusta\Downloads\projetor\BenQ (5).h c:\Users\Gusta\Downloads\projetor\BenQ (6).h c:\Users\Gusta\Downloads\projetor\BenQ (7).h c:\Users\Gusta\Downloads\projetor\BenQ (8).h c:\Users\Gusta\Downloads\projetor\BenQ (9).h c:\Users\Gusta\Downloads\projetor\BenQ (10).h c:\Users\Gusta\Downloads\projetor\BenQ (11).h
  digitalWrite(LED, HIGH);

  // Primeiro: enviar todos os códigos EU
  int numEUCodes = sizeof(codes_eu) / (3 * sizeof(int));
  for(int i = 0; i < numEUCodes; i++) {
    sendIRCode(0, i); // Região EU
    delay(100); // Pequeno delay entre códigos
  }

  // Segundo: enviar todos os códigos US
  int numUSCodes = sizeof(codes_us) / (3 * sizeof(int));
  for(int i = 0; i < numUSCodes; i++) {
    sendIRCode(1, i); // Região US
    delay(100);
  }

  digitalWrite(LED, LOW);
  isAttacking = false;
}

const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html lang="pt">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>TV-B-GONE | Ataque IR</title>
  <style>
    * {
      margin: 0;
      padding: 0;
      box-sizing: border-box;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    }
    
    body {
      background: linear-gradient(135deg, #1a1a2e, #16213e);
      min-height: 100vh;
      display: flex;
      justify-content: center;
      align-items: center;
      padding: 20px;
    }
    
    .container {
      background: rgba(0, 0, 0, 0.7);
      border-radius: 20px;
      box-shadow: 0 15px 35px rgba(0, 0, 0, 0.5);
      width: 100%;
      max-width: 500px;
      overflow: hidden;
      text-align: center;
    }
    
    .header {
      background: rgba(255, 77, 77, 0.9);
      padding: 30px 20px;
      position: relative;
    }
    
    h1 {
      color: white;
      font-size: 2.5rem;
      margin-bottom: 10px;
      text-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
    }
    
    .status {
      color: rgba(255, 255, 255, 0.9);
      font-size: 1.1rem;
    }
    
    .content {
      padding: 40px 20px;
    }
    
    .attack-btn {
      background: linear-gradient(45deg, #ff4d4d, #ff1a1a);
      border: none;
      border-radius: 50px;
      color: white;
      padding: 20px 50px;
      font-size: 1.8rem;
      font-weight: bold;
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: 0 10px 25px rgba(255, 77, 77, 0.4);
      position: relative;
      overflow: hidden;
    }
    
    .attack-btn:hover {
      transform: translateY(-5px);
      box-shadow: 0 15px 30px rgba(255, 77, 77, 0.6);
    }
    
    .attack-btn:active {
      transform: translateY(0);
      box-shadow: 0 5px 15px rgba(255, 77, 77, 0.4);
    }
    
    .attack-btn:disabled {
      background: linear-gradient(45deg, #777, #555);
      transform: none;
      box-shadow: none;
      cursor: not-allowed;
    }
    
    .pulse {
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: rgba(255, 255, 255, 0.3);
      border-radius: 50px;
      animation: pulse 2s infinite;
    }
    
    @keyframes pulse {
      0% { transform: scale(1); opacity: 1; }
      100% { transform: scale(1.5); opacity: 0; }
    }
    
    .instructions {
      margin-top: 30px;
      color: #aaa;
      font-size: 1rem;
      line-height: 1.6;
    }
    
    .footer {
      padding: 20px;
      color: #777;
      font-size: 0.9rem;
    }
    
    .status-indicator {
      display: inline-block;
      width: 12px;
      height: 12px;
      border-radius: 50%;
      margin-right: 8px;
      background: #4CAF50;
    }
    
    .status-text {
      display: flex;
      align-items: center;
      justify-content: center;
      margin-top: 20px;
      font-size: 1.1rem;
      color: #ddd;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">
      <h1>TV-B-GONE</h1>
      <p class="status">Controle Remoto de Desligamento</p>
    </div>
    
    <div class="content">
      <button id="attackBtn" class="attack-btn" onclick="startAttack()">
        <span id="btnText">INICIAR ATAQUE</span>
        <span id="pulse" class="pulse" style="display: none;"></span>
      </button>
      
      <div class="status-text">
        <span id="statusIcon" class="status-indicator"></span>
        <span id="statusText">Pronto para atacar</span>
      </div>
      
      <div class="instructions">
        <p>Posicione o dispositivo apontado para as TVs e clique no botão acima para iniciar o ataque infravermelho.</p>
        <p>O dispositivo enviará sequencialmente todos os códigos de desligamento para TVs de todas as marcas.</p>
      </div>
    </div>
    
    <div class="footer">
      Dispositivo: TV-B-GONE | IP: %IP%
    </div>
  </div>
  
  <script>
    function startAttack() {
      const btn = document.getElementById('attackBtn');
      const btnText = document.getElementById('btnText');
      const pulse = document.getElementById('pulse');
      const statusText = document.getElementById('statusText');
      const statusIcon = document.getElementById('statusIcon');
      
      // Desativar botão durante o ataque
      btn.disabled = true;
      btnText.textContent = 'ATAQUE EM ANDAMENTO';
      pulse.style.display = 'block';
      statusText.textContent = 'Enviando códigos de desligamento...';
      statusIcon.style.background = '#FFC107';
      
      // Enviar requisição para iniciar o ataque
      fetch('/attack')
        .then(response => response.text())
        .then(data => {
          btn.disabled = false;
          btnText.textContent = 'INICIAR ATAQUE';
          pulse.style.display = 'none';
          statusText.textContent = 'Ataque completo! TVs devem estar desligadas';
          statusIcon.style.background = '#4CAF50';
          
          // Resetar status após 5 segundos
          setTimeout(() => {
            statusText.textContent = 'Pronto para novo ataque';
          }, 5000);
        })
        .catch(error => {
          console.error('Error:', error);
          btn.disabled = false;
          btnText.textContent = 'INICIAR ATAQUE';
          pulse.style.display = 'none';
          statusText.textContent = 'Erro no ataque! Tente novamente';
          statusIcon.style.background = '#F44336';
        });
    }
    
    // Atualizar IP na página
    window.onload = function() {
      const ipElement = document.querySelector('.footer');
      ipElement.innerHTML = ipElement.innerHTML.replace('%IP%', location.host);
    };
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  String html = htmlPage;
  html.replace("%IP%", WiFi.softAPIP().toString());
  server.send(200, "text/html", html);
}

void handleAttack() {
  if (!isAttacking) {
    // Iniciar o ataque em segundo plano
    startAttack();
    server.send(200, "text/plain", "Ataque iniciado");
  } else {
    server.send(200, "text/plain", "Ataque já em progresso");
  }
}

void setup() {
  pinMode(IRLED, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(IRLED, LOW);
  digitalWrite(LED, LOW);

  // Iniciar ponto de acesso
  WiFi.softAP(ssid, password);
  
  // Configurar servidor web
  server.on("/", handleRoot);
  server.on("/attack", handleAttack);
  server.begin();
}

void loop() {
  server.handleClient();
}