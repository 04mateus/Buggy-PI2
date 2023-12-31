/*
  Encoder.h - Biblioteca para mediação de velocidade usando encoder para buggy 4x4.
  Created by Renan Augusto Starke, July 26, 2021.
  Released into the public domain.
*/
#ifndef ENCODER_H
#define ENCODER_H

#include "Arduino.h"

class Encoder
{
  public:
    static volatile unsigned long pulsos_d0;
    static volatile unsigned long pulsos_d1;
    static unsigned long pulsos_d0__;
    static unsigned long pulsos_d1__;

    
    Encoder(unsigned char pino);
    unsigned long obter_pulsos();
    void atualizar();    

    enum SENSORES { D0 = 13, D1 = 12};
    
  private:    
    unsigned char _pino;       
    unsigned long tempo_d0;
    unsigned long tempo_d1;
 
};

#endif
