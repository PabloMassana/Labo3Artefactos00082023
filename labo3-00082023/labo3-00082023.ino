#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define ANCHO 128
#define ALTO 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(ANCHO, ALTO, &Wire, OLED_RESET);

const int pinLM35 = A0; 
const unsigned long intervalo = 1000;
unsigned long ultimaMillis = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) { /* espera en algunas placas */ }
  delay(50);
  Serial.println("LM35 - Lectura simple");

  // Inicializar OLED
  if (!display.begin(0x3C, true)) { // dirección I2C 0x3C (común en SH1106)
    Serial.println(F("No se encontró la pantalla OLED"));
    for (;;); // se queda detenido
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  unsigned long ahora = millis();
  if (ahora - ultimaMillis >= intervalo) {
    ultimaMillis = ahora;

    // Lectura del LM35
    int lectura = analogRead(pinLM35); // 0..1023
    const float Vref = 5.0; // ajustar si tu referencia ADC es distinta
    float voltaje = lectura * (Vref / 1023.0); // voltaje en V
    float tempC = voltaje * 100.0; // LM35: 10 mV/°C -> factor 100

    // Mostrar en Serial
    Serial.print("Temperatura: ");
    Serial.print(tempC, 2); // 2 decimales
    Serial.println(" °C");

    // Mostrar en OLED
    display.clearDisplay();

    // Texto fijo
    display.setTextSize(2);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 10);
    display.print("Temp:");

    // Texto de temperatura con decimales
    display.setTextSize(2);
    display.setCursor(0, 40);
    display.print(tempC, 2); // muestra con 2 decimales
    display.print(" C");

    display.display();
  }
}