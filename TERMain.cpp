#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
 

#include <assert.h>
#include <stdlib.h>
#include "jbus.h"

#include <csignal>
#include <iostream>

using namespace std;

//pointeur d'instance jbus
jbus *j1;
 

int choix_carte()
{
  int choix;
  cout << "Veuillez choisir sur quelle carte écrire :\n1)DAC\n2)CNT\n3)VDOUT\n4)APAL\n5)VDAD\n6)VDIN\n" << endl;

  cin >> choix;

  if(choix < 1)
  {
    cout << "Choix de carte erroné, valeur choisie mise à 1" << endl;
    choix = 1;
  } 
  if(choix > 6)
  {
    cout << "Choix de carte erroné, valeur choisie mise à 6" << endl;
    choix = 6;
  } 
  return choix;
}

int choix_valeur()
{
  int choix;
  
  cout << "Veuillez choisir une valeur a insérer dans la carte (De 0 à 131071)\n" << endl;

  cin >> choix;

  if(choix < 0)
  {
    cout << "Choix de valeur erroné, valeur choisie mise à 0" << endl;
    choix = 0;
  }

  if(choix > 131071)
  {
    cout << "Choix de carte erroné, valeur choisie mise à 131071" << endl;
    choix = 131071;
  }

  return choix;
}

void arret(int signal)
{
  
  std::cout << "arret jbus par signal CTRL-C : nO " << signal << endl;
     
  j1->~jbus();//laisser cet appel...
  std::raise(SIGKILL);
}
 

int main(int argc, char **argv)
{
   int nb = 0; 
   unsigned char buf[10]; // trame de test lecture jbus
	// TO DO creation d'une instance de jbus avec les réglages 2 bit de STOP sans parite  
   j1 =  new jbus("/dev/ttyUSB0",9600,8,1,0,0);
       

// Install a signal handler : ne pas retirer
  std::signal(SIGINT, arret);

  




  cout << "Veuillez choisir une opération à effectuée :\n\n1) Mode Lecture\n2) Mode Ecriture\n3) Mode Défilement\n"
  << endl;

  int operation;

  cin >> operation;

  switch (operation)
  {
    case 1:

      j1->API_Module_Read_Nwords(choix_carte(),0,1);
      j1->read_value_R(buf,1);
      for(int i = 0 ; i < 10 ; i ++)
      cout << j1->HexToCar(buf[i]) << endl;
      break;

    case 2:

      j1->API_Module_Write_1word(choix_carte(),0,choix_valeur());
      /*j1->read_value_R(buf,1);
      for(int i = 0 ; i < 10 ; i ++)
      cout << j1->HexToCar(buf[i]) << endl;*/
      break;

    case 3:
    
      for(int i = 0b0000000000001111 ;  ; i = i << 1)  /* boucle IHM requetes MODBUS*/
      {
        j1->API_Module_Write_1word(3,0,i);
        if(i > 131071) i = 0b0000000000001111;
      }
      break;
      
    default:

      exit(0);
      break;
  }
}
