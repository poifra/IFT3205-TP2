/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-4.c                          */
/* Auteur  : François Poitras et Charles Langlois       */
/* Date    : 24/02/2017                                 */
/* version :                                            */ 
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo2.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN1  "lena"
#define NAME_IMG_OUT1 "image-Out1-TP2-2"

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)


/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void rotateMatrix(float **src, float **dest, float angle, int length, int width)
{
  //methode du plus proche voisin
    int x, y;
    float newX, newY;
    int newXInt, newYInt;
    char outOfBounds = 0;
    for (x = 0; x < length; x++)
    {
      for (y = 0; y < width; y++)
      {
        newX =  (x-width/2)*cos(angle) - (y-length/2)*sin(angle) + width/2;
        newY =  (x-width/2)*sin(angle) + (y-length/2)*cos(angle) + length/2;
        
        newYInt = round(newY);
        newXInt = round(newX);

        outOfBounds = newXInt < 0 || newXInt >= length || newYInt < 0 || newYInt > width;
        dest[x][y] = outOfBounds ? 0 : src[newXInt][newYInt];
      }
    }
}



/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,k;
  int length,width;
  float Theta0;
  int x0,y0;
  char BufSystVisuImg[100];

  //Constante
  length=512;
  width=512;
  
  //Allocation Memoire 
  float** outputMatrix=fmatrix_allocate_2d(length,width);

  //Lecture Image 
  float** inputMatrix=LoadImagePgm(NAME_IMG_IN1,&length,&width);

  float angle = degreesToRadians(22.5f);
  rotateMatrix(inputMatrix, outputMatrix, angle, length, width);


    //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT1,outputMatrix,length,width);


  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);



  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(inputMatrix);
  free_fmatrix_2d(outputMatrix);


  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


