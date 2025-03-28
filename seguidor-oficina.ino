// Definição dos pinos
#define IN1 5
#define IN2 6
#define IN3 7
#define IN4 8
#define PwmA 9
#define PwmB 10

// Calibragem para deixar motores retos
#define velA 90
#define velB 150

// Pinos dos sensores
#define S1 A1
#define S2 A2
#define S3 A3
#define S4 A4
#define S5 A5

// Definindo pinos dos sensores
const int pinosSensores[5] = {S1, S2, S3, S4, S5};
int leituraDosSensores[5];

// Função para ler os sensores
void lerSensores() {
  for(int i = 0; i < 5; i++) {
    leituraDosSensores[i] = !digitalRead(pinosSensores[i]); // Mudando a lógica de leitura
  }
}

// Variável global para armazenar o erro anterior
float erroAnterior = 0;

// Função para calcular o erro
float calcularErro() {
  int pesos[5] = {-2, -1, 0, 1, 2};
  float erro = 0;
  int sensoresAtivos = 0;

  for (int i = 0; i < 5; i++) {
    erro = erro + (leituraDosSensores[i] * pesos[i]);
    sensoresAtivos += leituraDosSensores[i];
  }

  if (sensoresAtivos != 0) {
    erro = erro / (sensoresAtivos);
  }

  if (erro != 0) {
    erroAnterior = erro; // Atualizando o erro anterior
  }

  return erro;
}

// Função para controlar os motores
void controlarMotores(int VelD, int VelE) {
  // Restringindo velocidade dos motores
  analogWrite(PwmA, constrain(VelD, 1, 255));
  analogWrite(PwmB, constrain(VelE, 1, 255));
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN3, LOW);
}

// Função para seguir a linha com base no erro calculado
void seguirLinha(float erro) {
  int pmwMotorDireito = 0;
  int pmwMotorEsquerdo = 0;
  int kp = 50;

  pmwMotorDireito = velA + (erro * kp);
  pmwMotorEsquerdo = velB - (erro * kp);
  controlarMotores(pmwMotorDireito, pmwMotorEsquerdo);
}

// Função de setup
void setup() {
  Serial.begin(9600);

  for(int i = 0; i < 5; i++) {
    pinMode(pinosSensores[i], INPUT);
  }

  pinMode(IN2, OUTPUT); 
  pinMode(IN1, OUTPUT); 
  pinMode(IN4, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(PwmA, OUTPUT); 
  pinMode(PwmB, OUTPUT); 
}

// Função para identificar se há linha nos sensores
bool identificarLinha() {
  for (int i = 0; i < 5; i++) {
    if (leituraDosSensores[i]) return true;
  }
  return false;
}

// Função de loop
void loop() {
  // lerSensores();
  // Serial.print(leituraDosSensores[0]);
  // Serial.print(leituraDosSensores[1]);
  // Serial.print(leituraDosSensores[2]);
  // Serial.print(leituraDosSensores[3]);
  // Serial.print(leituraDosSensores[4]);
  // Serial.println("");

   if (identificarLinha()) {
     Serial.println("identificou linha");
     seguirLinha(calcularErro());
   } else {
     controlarMotores(0, 0);
     Serial.println("sem linha");
   }
}
