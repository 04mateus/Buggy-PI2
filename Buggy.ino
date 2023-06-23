#include "Motores.h"
#include "Ultrassom.h"
#include "Encoder.h"
#include "Linha.h"
#include "Baterias.h"

/* Instâncias das classe dos sensores/atuadores */
Motores motores;
Ultrassom ultrassom_1(Ultrassom::ECHO1, Ultrassom::TRIG1);
Encoder encoder_0(Encoder::D0);
Encoder encoder_1(Encoder::D1);
Linha seguidor_linha;
//Baterias baterias;

#define TEMPO_MOTOR (30)

/* ENUM MODOS DE OPERACAO */
enum ModoOperacao {
  MODO_PADRAO,
  MODO_DESVIO
};

/* Variável para armazenar o modo atual */
ModoOperacao modoAtual = MODO_PADRAO;

void setup() {
  Serial.begin(9600);
}

/* Variáveis de MEDICAO */
int tempo, distancia, pulsos_encoder0, pulsos_encoder1, dadoBluetooth = 0;
volatile int direita, esquerda;
float trajeto_encoder0 = 0, trajeto_encoder1 = 0;


/* Variáveis de CONTROLE */
bool seguidor = true;
bool desvia = false;
bool ultra = false;
int vlcdd = 200;

void loop() {
//    linha(vlcdd);

    vira180();

   delay(1000);

/*
  if (monitora_ultra(0)) {
    vira180();
  } else {
    linha(vlcdd);
  }
*/

/*
  controle(); 
  //encoder();
  // Verifica o modo atual e executa a operação correspondente
  switch (modoAtual) {
    case MODO_PADRAO:
      linha(vlcdd);
      break;
    case MODO_DESVIO:
      vira(vlcdd);
      break;
    default:
      modoAtual = MODO_PADRAO;
      break;
  }

  //volta_trajeto(vlcdd);
  //imprimir();
*/
}

void controle(){
  if (monitora_ultra(0)) {
    seguidor = false;
    desvia = true;
    modoAtual = MODO_DESVIO;
  } else if(desvia) {
    seguidor = false;
    desvia = true;
    modoAtual = MODO_DESVIO; 
  } else {
    seguidor = true;
    desvia = false;
    modoAtual = MODO_PADRAO;
  }
}

bool monitora_ultra(int debug){
  ultrassom_1.atualizar();
  tempo = ultrassom_1.obter_distancia();
  distancia = (tempo/2) * 0.0343;
  
  if (debug == 1){
    Serial.print("\t");
    Serial.println(distancia);
  }

  return (distancia < 30);
}

void encoder(){
  // A cada 20 pulsos, 1 volta, anda 22,5 cm.
  pulsos_encoder0 = encoder_0.obter_pulsos(); // lado direito
  trajeto_encoder0 += (pulsos_encoder0/20) * 0.225;
  
  pulsos_encoder1 = -encoder_1.obter_pulsos(); // lado esquerdo
  trajeto_encoder1 += (pulsos_encoder1/20) * 0.225;
  
   delay(1000);
}

void vira180()
{
  // vire até encontrar a linha novamente
  while (seguidor_linha.obter_direita())
  {
    motores.direita(vlcdd);
    delay(TEMPO_MOTOR * 3);
    motores.parar();
    delay(TEMPO_MOTOR);
  }

  // vire mais um pouco para centralizar na linha
  while (seguidor_linha.obter_direita() == false)
  {
    motores.direita(vlcdd);
    delay(TEMPO_MOTOR);
    motores.parar();
    delay(TEMPO_MOTOR);
  }
}

void linha(int vlcdd) {

  direita = seguidor_linha.obter_direita();
  esquerda = seguidor_linha.obter_esquerda();
  
  Serial.println("direita");
  Serial.println(direita);
  Serial.println("esquerda");
  Serial.println(esquerda);

  if (esquerda && direita){
    motores.tras(vlcdd);
  } else if (esquerda) {
    motores.direita(vlcdd);
  } else if (direita) {
    motores.esquerda(vlcdd);
  } else {
    motores.frente(vlcdd);
  }

  delay(TEMPO_MOTOR);
  motores.parar();
  delay(TEMPO_MOTOR);
}

void vira(int vlcdd){
  motores.parar();
  delay(40);

  motores.direita(vlcdd);
  delay(80);

  direita = seguidor_linha.obter_direita();
  esquerda = seguidor_linha.obter_esquerda();

  if (desvia) {
    if (esquerda && direita){
      motores.direita(vlcdd);
      delay(40);
    } else if (esquerda){
      motores.direita(vlcdd);
      delay(40);
      desvia = false;
      seguidor = true;
      modoAtual = MODO_PADRAO;
    } else if (direita){
      motores.direita(vlcdd);
      delay(40);
    } else{
      motores.parar();
      desvia = false;
      seguidor = true;
      modoAtual = MODO_PADRAO;
    }
  }else{
    modoAtual = MODO_PADRAO; 
  }
  
}

void volta_trajeto(int vlcdd){
  // Gira para a direita para desviar do obstáculo
  motores.direita(vlcdd);
  delay(500);
  
  /* Verifica se ainda há obstáculo à frente
  while (bool a = monitora_ultra(0)) {
    motores.direita(vlcdd);
  }*/
  
  motores.parar();
  delay(200);

  // Retorna ao trajeto
  motores.frente(vlcdd);
  delay(500);

  motores.parar();
  delay(200);

  motores.esquerda(vlcdd);
  delay(500);

  motores.parar();
  delay(200);

  motores.frente(vlcdd);
  delay(500);

  motores.parar();
  delay(200);

  motores.esquerda(vlcdd);
  delay(500);
    
  motores.parar();
  delay(200);

  motores.frente(vlcdd);
  delay(500);

  motores.parar();
  delay(200);

  motores.direita(vlcdd);
  delay(500);

  bool a = monitora_ultra(0);
  if(a){
    modoAtual = MODO_PADRAO;
  }
}

void imprimir(){
  Serial.print("Encoder0 = "); // lado direito
  Serial.println(encoder_0.obter_pulsos());
  Serial.print("Encoder1 = "); // lado esquerdo
  Serial.println(encoder_1.obter_pulsos());
  Serial.print("\n"); 

  Serial.print("Trajeto Encoder0 = ");
  Serial.println(trajeto_encoder0);
  Serial.print("Trajeto Encoder1 = ");
  Serial.println(trajeto_encoder1);
  Serial.print("\n"); 

  Serial.print("direita = "); // lado direito
  Serial.println(seguidor_linha.obter_direita());
  Serial.print("esquerda = "); // lado esquerdo
  Serial.println(seguidor_linha.obter_esquerda());
  Serial.print("\n"); 

  Serial.print("dist = ");
  Serial.println(ultrassom_1.obter_distancia());
  Serial.print("\n"); 
}