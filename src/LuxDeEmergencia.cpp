#include <string>
#include<sstream>
#include<fstream>
#include <ctime>
#include <cerrno>
#include <unistd.h>
#include "../include/LuxDeEmergencia.h"

#define PATH_ADC "/sys/bus/iio/devices/iio:device0/in_voltage"

LuxDeEmergencia::LuxDeEmergencia() {
  listen = true;
  isLedVermelhaAcessa = false;
  buttonDown = 0;

  botao = new BlackLib::BlackGPIO(BlackLib::GPIO_115,BlackLib::input, BlackLib::SecureMode);
  ledVermalha = new BlackLib::BlackGPIO(BlackLib::GPIO_4,BlackLib::output, BlackLib::SecureMode);
  ledR = new BlackLib::BlackGPIO(BlackLib::GPIO_60,BlackLib::output, BlackLib::SecureMode);
  ledG = new BlackLib::BlackGPIO(BlackLib::GPIO_50,BlackLib::output, BlackLib::SecureMode);
  ledB = new BlackLib::BlackGPIO(BlackLib::GPIO_51,BlackLib::output, BlackLib::SecureMode);
}

LuxDeEmergencia::~LuxDeEmergencia() {
  delete this->botao;
  delete this->ledVermalha;
  delete this->ledR;
  delete this->ledG;
  delete this->ledB;
}

/*!
* The function readAnalog get the value of the fotoresitor or of the potenciometer (this function was created by Ivanovch, teacher of OS on UFRN).
* param   number    This parameter receive the number of the port where the potenciometer or the fotoresistor is alocated.
* return This function return a integer that is the value of the fotoresistor or of the potenciometer.
*/
int LuxDeEmergencia::readAnalog(int number){
   std::stringstream ss;
   ss << PATH_ADC << number << "_raw";
   std::fstream fs;
   fs.open(ss.str().c_str(), std::fstream::in);
   fs >> number;
   fs.close();
   return number;
}

/*!
* The function ligaLed turnOn the RGB led on the beagleBone Black.
* param   inicial   This parameter is a char that receive the letter r, g or b to indicate the color of the RGB led. (r = red ligth, g = green ligth and b = blue ligth).
* return This function doesn't have a return.
*/
void LuxDeEmergencia::ligaRGB(char inicial) {
  // BlackLib::BlackGPIO ledR(BlackLib::GPIO_60,BlackLib::output, BlackLib::SecureMode);
  // BlackLib::BlackGPIO ledG(BlackLib::GPIO_50,BlackLib::output, BlackLib::SecureMode);
  // BlackLib::BlackGPIO ledB(BlackLib::GPIO_51,BlackLib::output, BlackLib::SecureMode);
  switch(inicial) {
    case 'r': // if inicial store the value 'r' the RGB led is red.
      ledR->setValue(BlackLib::high);
      ledG->setValue(BlackLib::low);
      ledB->setValue(BlackLib::low);
      break;
    case 'g': // if inicial store the value 'g' the RGB led is green.
      ledR->setValue(BlackLib::low);
      ledG->setValue(BlackLib::high);
      ledB->setValue(BlackLib::low);
      break;
    default: // if inicial store the value 'b' the RGB led is blue.
      ledR->setValue(BlackLib::low);
      ledG->setValue(BlackLib::low);
      ledB->setValue(BlackLib::high);
  }
}

/*!
* The function runProgram run the function of the class LuxDeEmergencia in the order to the program have the comportament of a emergency ligth.
* return  This function doesn't return anything.
*/
int LuxDeEmergencia::runProgram() {

  int auxBotao;   // store the int that represent the value of the button(1 to pressed and 0 to not pressed).

  while(listen) {   // Loop to the program know when stop.
    auxBotao = std::stoi(botao->getValue()); // get the value of the button,1 if is pressed and 0 other case.

    if(auxBotao == 1) {   // If the button is pressed this->buttonDown receive true and receive false other case.
      buttonDown = true;
      sleep(2);
      auxBotao = 0;
      auxBotao = std::stoi(botao->getValue());
      if(auxBotao == 1) {
        return 0;
      }
    }
    else {
      buttonDown = false;
      ledVermalha->setValue(BlackLib::low);  // turn off the red led.
      ledR->setValue(BlackLib::low);
      ledG->setValue(BlackLib::low);
      ledB->setValue(BlackLib::low);
    }

    if(buttonDown == false && isLedVermelhaAcessa == true) {  // if the button isn't pressed and the red led is on.
      ledVermalha->setValue(BlackLib::low);  // turn off the red led.
    }

    if(buttonDown == true) { // If the buttom was pressed.
      while(buttonDown == true) { // while the buttom is not pressed again.
        valorDoFotoResistor = readAnalog(0);  // receive the value of the fotoresistor
        if(1100 < valorDoFotoResistor) {  // if the fotoresistor have a value bigger than 1100 so the enviroment is not dark.
          if(isLedVermelhaAcessa == false) { // if the red led isn't on.
            ledVermalha->setValue(BlackLib::high);   // turn on the red led to indicate that the system is on and is not necessary turn on the RGB led.
            ledR->setValue(BlackLib::low);
            ledG->setValue(BlackLib::low);
            ledB->setValue(BlackLib::low);
          }
        }
        else {
          ledVermalha->setValue(BlackLib::low);
          valorDoPotenciometro = readAnalog(1);
          if(valorDoPotenciometro < 1365) {
            ligaRGB('r');
          }
          else if(valorDoPotenciometro < 2730) {
            ligaRGB('g');
          }
          else  {
            ligaRGB('b');
          }
        }
        auxBotao = 0;
        auxBotao = std::stoi(botao->getValue()); // If the button is pressed turn of the system.
        if(auxBotao == 1) {
          buttonDown = false;
          auxBotao = 0;
          break;
        }
        else {
          buttonDown = true;
        }
      }

    }
  }

}
