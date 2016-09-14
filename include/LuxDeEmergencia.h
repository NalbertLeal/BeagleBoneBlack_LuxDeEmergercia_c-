#ifndef _LuxDeEmergencia_H
#define _LuxDeEmergencia_H

#include "BlackGPIO/BlackGPIO.h"

class LuxDeEmergencia {
  private:
    BlackLib::BlackGPIO  *botao;

    BlackLib::BlackGPIO  *ledVermalha;

    BlackLib::BlackGPIO  *ledR;
    BlackLib::BlackGPIO  *ledG;
    BlackLib::BlackGPIO  *ledB;

    bool listen;                    // recebe true se o loop interno de controle do programa deve continuar rodando, e false caso contrario.
    bool isLedVermelhaAcessa;       // recebe valor true se o led vermelho estiver ligado, e false caso contrario.
    bool buttonDown;                // recebe 1 se o butão foi precionado e 0 caso contrario.

    // void turnOnRedLed();
    // void ValueOfFotoresistor();
    // void ValueOfPotenciometer();

  public:
    LuxDeEmergencia();
    ~LuxDeEmergencia();

    int readAnalog(int number);
    void ligaRGB(char inicial);
    int runProgram();

    int valorDoFotoResistor;        // valorDoPotenciometroComLux recebe a função readAnalog criada pelo professor Ivanovich para o recebimento do valor do fotoresistor.
    int valorDoPotenciometro;       // valorDoPotenciometroComLux recebe a função readAnalog criada pelo professor Ivanovich para o recebimento do valor do fotoresistor e do potenciometro.

};

#endif
