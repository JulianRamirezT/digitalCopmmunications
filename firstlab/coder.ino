#include <TimerOne.h>

const int signalPin = 13;  // Pin de salida para la señal digital
volatile unsigned long signalValue = 0;  // Variable para almacenar los bits capturados

void setup() {
  pinMode(signalPin, OUTPUT);
  Serial.begin(9600);  // Iniciar comunicación serial
  
  // Configurar temporizador para generar interrupción cada 166 microsegundos (aproximadamente 6000 bps)
  Timer1.initialize(166);
  Timer1.attachInterrupt(captureBits);
  
  // Iniciar la generación de la señal
  generateSignal();
}

void loop() {
  // Decodificar signalValue en I y Q
  int I, Q;

  switch (signalValue) {
    case B000:  // 000
      I = 1;
      Q = 1;
      break;
    case B001:  // 001
      I = 1;
      Q = -1;
      break;
    case B010:  // 010
      I = -1;
      Q = 1;
      break;
    case B011:  // 011
      I = -1;
      Q = -1;
      break;
    case B100:  // 100
      I = 2;
      Q = 2;
      break;
    case B101:  // 101
      I = 2;
      Q = -2;
      break;
    case B110:  // 110
      I = -2;
      Q = 2;
      break;
    case B111:  // 111
      I = -2;
      Q = -2;
      break;
    default:
      // Manejar un caso por defecto si es necesario
      break;
  }

  // Utilizar I y Q para tus necesidades, por ejemplo, imprimir los valores
  Serial.print("I: ");
  Serial.print(I);
  Serial.print(", Q: ");
  Serial.println(Q);

  // Continuar con el resto del código principal si es necesario
}

void generateSignal() {
  while (true) {
    for (int i = 0; i < 6000; i++) {
      int randomBit = random(2);  // Generar bit aleatorio (0 o 1)
      digitalWrite(signalPin, randomBit);
      delayMicroseconds(166);  // Retardo para obtener una tasa de 6000 bps
    }
  }
}

void captureBits() {
  static int bitCount = 0;
  static byte capturedBits = 0;

  int currentBit = digitalRead(signalPin);
  capturedBits |= (currentBit << bitCount);

  bitCount++;

  if (bitCount == 3) {
    // Almacenar los tres bits capturados en la variable signalValue
    signalValue = capturedBits;

    // Reiniciar variables para el próximo conjunto de tres bits
    bitCount = 0;
    capturedBits = 0;
  }
}
