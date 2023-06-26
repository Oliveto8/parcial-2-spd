// OLIVETO GOÑI AGUSTÍN
#include <IRremote.h>
#include <Servo.h>
#include <LiquidCrystal.h>

#define RED 13
#define GREEN 12
#define sensor_temperatura A0

#define sensor_ir 11

#define tecla_on_off 0xFF00BF00
Servo myservo;  // Creamos un objeto servo 

int angulo;
int temperatura;
int lectura_sensor;

bool prendido_apagado = false;


IRrecv irrecv(sensor_ir);
decode_results valor;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup()
{
  myservo.attach(9);  // Asignamos el pin 3 al servo.

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  lcd.begin(16, 2);
  irrecv.enableIRIn();
}

void loop()
{
  if (prendido_apagado)
  {
      int lectura_sensor = analogRead(sensor_temperatura);
      float temperatura = (lectura_sensor * 5.0 / 1024.0 - 0.5) * 100.0; //cuenta a grados C°
		
      detectar_temperatura(temperatura);

      if(temperatura > 55)
      {
        lcd.setCursor(0, 0);
        lcd.print("Temperatura: ");
        lcd.print(temperatura);
        delay(20);
        digitalWrite(GREEN, LOW);
        mover_servo();
      }
      else
      {
        detener_servo();
        digitalWrite(GREEN, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Temperatura: ");
        lcd.print(temperatura);
        delay(200);
      }

        if (IrReceiver.decode()) 
        {
            prender();
        }
  }
  else
  {
    if (IrReceiver.decode()) 
    {
        apagar();
    }
  }
}


void luzEmergencia(int led_rojo) {
  
  for (int i = 0; i < 1; i++) {
    digitalWrite(led_rojo, HIGH);
    delay(100);
    digitalWrite(led_rojo, LOW);
    delay(100);
  }
  digitalWrite(led_rojo, LOW);

}


void detectar_temperatura(int temperatura)
{
  lcd.setCursor(0, 1);
  lcd.print("");
  
  if (temperatura <= 15)
  {
    lcd.print("Invierno");
  }
  else if (temperatura > 15 && temperatura <= 20)
  {
    lcd.print("Otonio");
  }
  else if (temperatura > 21 && temperatura <= 28)
  {
    lcd.print("Primavera");
  }
  else if(temperatura >= 29 && temperatura <= 55)
  {
    lcd.print("Verano");
  }
  else
  {
    lcd.print("Incendio");
  }
  lcd.print("                ");
}
  
// Mover servo
void mover_servo()
{
  for (angulo = 0; angulo <= 180; angulo += 1)
    {
      myservo.write(angulo);
      digitalWrite(RED, HIGH);
      delay(10);
      digitalWrite(RED, LOW);
      
      
    }
    for (angulo = 180; angulo >= 0; angulo -= 1)
    {
      myservo.write(angulo);
      digitalWrite(RED, HIGH);
      delay(10);
      digitalWrite(RED, LOW);
    }
}

void detener_servo()
{
  angulo = 90;
  myservo.write(angulo);
  delay(15);
}


void prender()
{
  Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
            IrReceiver.resume();
            switch (IrReceiver.decodedIRData.decodedRawData)
            {
                case 4278238976:
                prendido_apagado = false;
                lcd.setCursor(0, 0);
                lcd.print("                ");
                lcd.setCursor(0, 1);
                lcd.print("                ");
                break;
            }
            delay(10);
            irrecv.resume();
}

void apagar()
{
  Serial.print(IrReceiver.decodedIRData.decodedRawData, HEX);
        IrReceiver.resume();
        switch (IrReceiver.decodedIRData.decodedRawData)
        {
            case 4278238976:
              prendido_apagado = true;
        }
        delay(10);
        irrecv.resume();
}
