#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

// Credenciais da rede industrial
const char* ssid = "guilhermeTeste";
const char* password = "maracuja";

// URL de teste (JSONPlaceholder é um serviço comum para testes de API)
const char* serverName = "https://jsonplaceholder.typicode.com/todos/1";

const int PINO_LED_VERDE = 18;
const int PINO_LED_VERMELHO = 19;

void setup() {
  Serial.begin(115200);

  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado com sucesso!");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Criamos o objeto cliente seguro
    WiFiClientSecure *client = new WiFiClientSecure;

    if(client) {
      // Em um cenário real, você usaria client->setCACert(rootCACertificate);
      // Para este exemplo simples, vamos ignorar a validação do certificado:
      client->setInsecure();

      HTTPClient https;

      Serial.print("[HTTPS] Iniciando requisição...\n");
      if (https.begin(*client, serverName)) {

        // Realiza o GET
        int httpCode = https.GET();

        // httpCode será positivo se o servidor responder
        if (httpCode > 0) {
          Serial.printf("[HTTPS] Código de resposta: %d\n", httpCode);

          if (httpCode == HTTP_CODE_OK) {
            String payload = https.getString();
            Serial.println("Conteúdo recebido:");
            Serial.println(payload);

            if (httpCode == 200) {
              digitalWrite(PINO_LED_VERDE, HIGH); //Acende led verde
              digitalWrite(PINO_LED_VERMELHO, LOW); //Apaga led vermelho
            } else {
              digitalWrite(PINO_LED_VERMELHO, HIGH); //Acende led vermelho
              digitalWrite(PINO_LED_VERDE, LOW); //Apaga led verde
            }

          }

        } else {
          Serial.printf("[HTTPS] Falha na requisição, erro: %s\n", https.errorToString(httpCode).c_str());
          digitalWrite(PINO_LED_VERMELHO, HIGH); //Acende led vermelho
          digitalWrite(PINO_LED_VERDE, LOW); //Apaga led verde
        }

        https.end();
      } else {
        Serial.printf("[HTTPS] Não foi possível conectar ao servidor\n");
        digitalWrite(PINO_LED_VERMELHO, HIGH); //Acende led vermelho
        digitalWrite(PINO_LED_VERDE, LOW); //Apaga led verde
      }

      delete client; // Libera memória
    }
  }

  // Aguarda 15 segundos para a próxima leitura (evita sobrecarga no servidor)
  delay(15000);
}