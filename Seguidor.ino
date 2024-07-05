#include <Servo.h> // libreria para los servo motores 

// 115 horizontal MAXIMO
Servo horizontal; // declaramos el valor para el servo horizontal
int servoh = 40;        

int servohLimitHigh = 115; // limite en posicion superior
int servohLimitLow = 40; // limite para posicion inferior

Servo vertical; // vertical servo 
int servov = 5;    
int servovLimitHigh = 20;
int servovLimitLow = 5;

int ldr_tr = 0; // conexion a las resistencias LDR
int ldr_tl = 1; 
int ldr_dr = 3; 
int ldr_dl = 4; 

void setup() {
  horizontal.attach(9); // servo horizontal pin digital 9
  vertical.attach(3); // servo vertical pin digital 3
  horizontal.write(servohLimitLow);// establecer posicion inicial para servo horizontal
  vertical.write(servovLimitLow); // establecer posicion inicial para servo vertical
  delay(3000);
}
//funcion para mover servos
void smoothMove(Servo &servo, int &currentPos, int targetPos, int delayTime) {
  if (currentPos < targetPos) {
    for (int pos = currentPos; pos <= targetPos; pos++) {
      servo.write(pos);
      delay(delayTime);
    }
  } else {
    for (int pos = currentPos; pos >= targetPos; pos--) {
      servo.write(pos);
      delay(delayTime);
    }
  }
  currentPos = targetPos;
}

void loop() {
  int tl = analogRead(ldr_tl); // top left (superior izquierdo)
  int tr = analogRead(ldr_tr); // top right (superior derecho)
  int dl = analogRead(ldr_dl); // down left (inferior izquierdo)
  int dr = analogRead(ldr_dr); // down right (inferior derecho)
  
  int dtime = 10;// tiempo de espera entre cada lectura
  int tol = 50; // tolerancia 
  
  int vmt = (tl + tr) / 2; // valor arriba maximo
  int vmd= (dl + dr) / 2; // valor abajo maximo
  int vml= (tl + dl) / 2; // valor izquierdo maximo
  int vmr= (tr + dr) / 2; // valor derecho maximo

  int dvert = vmd - vmt; // diferencia entre abajo y arriba
  int dhoriz = vml - vmr; // diferencia entre izquierdo y derecho
  
  // Movimiento suave del servo vertical
  if (abs(dvert) > tol) { // revisar la diferencia para cambiar al angulo vertical
    int targetv = servov;
    if (vmd > vmt) {
      targetv++;
      if (targetv > servovLimitHigh) {
        targetv = servovLimitHigh;
      }
    } else if (vmd < vmt) {
      targetv--;
      if (targetv < servovLimitLow) {
        targetv = servovLimitLow;
      }
    }
    smoothMove(vertical, servov, targetv, 10); // ajustar para cambiar la suavidad del movimiento
  }

  // Movimiento suave del servo horizontal
  if (abs(dhoriz) > tol) { // revisar la diferencia para cambiar al angulo horizontal
    int targeth = servoh;
    if (vml > vmr) {
      targeth--;
      if (targeth < servohLimitLow) {
        targeth = servohLimitLow;
      }
    } else if (vml < vmr) {
      targeth++;
      if (targeth > servohLimitHigh) {
        targeth = servohLimitHigh;
      }
    }
    smoothMove(horizontal, servoh, targeth, 2); // ajustar para cambiar la suavidad del movimiento
  }

  delay(dtime);
}

