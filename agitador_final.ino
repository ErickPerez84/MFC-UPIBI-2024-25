#include <AccelStepper.h>

#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define maxsp 2000
#define potpin A0

// Crear una instancia de la clase AccelStepper:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() 
{
  Serial.begin(9600);
  // Establecer la velocidad y aceleración máximas:
  stepper.setMaxSpeed(2000); // Aumenta el límite máximo según sea necesario
}

void loop() {
  // Leer el valor del potenciómetro
  int potValue = analogRead(potpin);
  Serial.print("\nPotenciómetro: ");
  Serial.print(potValue);

  // Mapear el valor del potenciómetro al rango deseado de velocidad
  float speed = map(potValue, 0, 1023, 0, 2000);

  // Establecer la velocidad del motor
  stepper.setSpeed(speed);

  // Ejecutar el motor a la velocidad establecida
  stepper.runSpeed();
}