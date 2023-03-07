/*
  ------------------------------------------Carrinho Autônomo SR 2------------------------------------------------

*/ 
#include <Servo.h>     // Adiciona a biblitoeca Servo

//Definindo os pinos
#define trigPin A0     //Pino TRIG do sensor no pino analógico A0
#define echoPin A1     //Pino ECHO do sensor no pino analógico A1

// motor um            // Ligação dos pinos da Ponte H L298N
#define enA  11        //pino enA na porta digital 10
#define in1  9         //pino in1 na porta digital 9
#define in2  8         //pino in2 na porta digital 8

// motor dois          // Ligação dos pinos da Ponte H L298N
#define enB  5         //pino enB na porta digital 5
#define in3  7         //pino in3 na porta digital 7
#define in4  6         //pino in4 na porta digital 6

Servo servoSensor;       // Crie um objeto Servo para controlar o Servo.

//função para procurar obtasculo a todo o tempo
int Procurar (void) {
  float duracao = 0.0;              // variavel para guartar a duração do retorno do som
  float CM = 0.0;                   // variavel para guartar a distancia
  digitalWrite(trigPin, LOW);       //não envia som
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);      //envia som
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);       //não envia o som e espera o retorno do som enviado
  duracao = pulseIn(echoPin, HIGH); //Captura a duração em tempo do retorno do som.
  CM = (duracao / 58.8);            //Calcula a distância em centimetros
  Serial.print("Distancia em CM: ");
  Serial.println(CM);               //Imprimi no monitor serial a distancia
  return CM;                        // Return to CM.
}

//Variaveis
int DistanciaDireita, DistanciaEsquerda;  // variavel de Distâncias de ambos os lados
float Distancia = 0.00;                   // variavel para guardar a distancia

//Velocidades dos motores (você pode calibrar cada motor colocando os valores de 0 a 254)
int velocidadeMotorUm = 180;
int velocidadeMotorDois = 195;

// Função que é executado na inicialização do Arduino
void setup() {
  Serial.begin(9600); // inicializa a comunicação serial para mostrar dados
  
  //Define o servo na porta 13
  servoSensor.attach(13); 
  
  // Definir todos os pinos de controle do motor como saídas
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
       
  //Configuraçõs do sensor ultrassonico
  pinMode(trigPin, OUTPUT);     //define o pino TRIG como saída
  pinMode(echoPin, INPUT);      //define o pino ECHO como entrada

}

// Função principal do Arduino
void loop() {
  servoSensor.write (90);                           // Gira o Servo com o sensor a 90 graus
  delay (100);                                      // Aguarda 100 milesugodos
  Distancia = Procurar ();                          // Medindo a Distancia em CM.
  if (Distancia < 40) {                             // Se há obstáculo encontrado a menos de 40cm.
   direcao ();                                      // Se Frente estiver bloqueado, mude de direção
  }
  else if (Distancia >= 40)  {                      // Se o obstáculo for encontrado entre a mais de 40cm 
    Frente ();                                      // Robô se move para a direção da Frente.
  }
  
}

// Função para pegar as distancias de cada direção
void direcao () {        
  Parar ();                                         // O Carrinho Para
  ParaTras ();                                      // O Carrinho vai para tras
  Parar ();                                         // O Carrinho Para
  servoSensor.write (180);                          // Gira o Servo com o sensor a 180 graus
  delay (1000);              
  DistanciaEsquerda = Procurar ();                  // Defina a Distancia da Esquerda 
  delay (500);               
  servoSensor.write (0);                            // Gira o Servo com o sensor a 0 graus
  delay (500);               
  DistanciaDireita = Procurar ();                   // Defina a Distancia da Direita
  delay (500);               
  servoSensor.write (90);                           // Gira o Servo com o sensor a 90 graus
  delay (500);              
  CompareDistance ();                               // Encontre a distância mais longa.
}

// Função para calcular qual a distancia é melhor para o Carrinho ir
void CompareDistance () {                   
  if (DistanciaDireita > DistanciaEsquerda) {       // Se a direita está menos obstruída.
    Vireadireita ();                                // O Carrinho vai virar a direita 
  }
  else if (DistanciaEsquerda > DistanciaDireita) {  // Se Esquerda estiver menos obstruída.
    VireaEsquerda ();                               // Carrinho Vire na direção esquerda.
  }
  else {                                            // Se ambos estiverem igualmente obstruídos.
    Retorne ();                                     // Carrinho Vire-se.
  }
}

// Função para fazer o Carrinho parar
void Parar()
{
  Serial.println("Carrinho: Parar ");
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(100);                                       //aguarda um tempo
}

// Função para fazer o Carrinho andar para frente
void Frente()
{
  Serial.println("Carrinho: Frente ");
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

// Função que faz o Carrinho andar para trás e emite som quando ele dá ré
void ParaTras()
{
  Serial.println("Carrinho: Ré ");
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  delay(300);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

// Função que faz o Carrinho virar à direita, 
void Vireadireita()
{
  Serial.println("Carrinho: Direita ");
  digitalWrite(in1, LOW);                           //Configurar a ponte h 
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(300);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

// Função que faz o Carrinho virar à esquerda
void VireaEsquerda()
{
  Serial.println("Carrinho: Esquerda ");                
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay(300);                                       //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         
}

//Função para o Carrinho virar para o outro lado
void Retorne () {    
  Serial.println("Carrinho: Girar ");      
  digitalWrite(in1, HIGH);                          //Configurar a ponte h 
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  delay (700);                                      //aguarda um tempo
  analogWrite(enA, velocidadeMotorUm);              // Defina a velocidade do motor Um
  analogWrite(enB, velocidadeMotorDois);            // Defina a velocidade do motor Dois                         

  digitalWrite(ledVermelho 12,HIGH);//acende o led.
  delay(2000);// 2segundos
  digitalWrite(ledVermelho12,LOW);//apaga o led.
}

