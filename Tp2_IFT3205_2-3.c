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


#define NAME_VISUALISER "display "
#define NAME_IMG_IN1  "UdM_1"
#define NAME_IMG_IN2  "UdM_2"
#define NAME_IMG_OUT1 "image-TP2-4-1"
#define NAME_IMG_OUT2 "image-TP2-4-2"
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
      newX =  (x - width / 2) * cos(angle) - (y - length / 2) * sin(angle) + width / 2;
      newY =  (x - width / 2) * sin(angle) + (y - length / 2) * cos(angle) + length / 2;

      newYInt = round(newY);
      newXInt = round(newX);

      outOfBounds = newXInt < 0 || newXInt >= length || newYInt < 0 || newYInt > width;
      dest[x][y] = outOfBounds ? 0 : src[newXInt][newYInt];
    }
  }
}

void zeroMatrix(float **mat, int length, int width)
{
  for(int i = 0; i < width; i++)
    for(int j = 0; j < length; j++)
      mat[i][j] = 0.0f;
}
/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/
int main(int argc, char **argv)
{
  int i, j, k;
  int length, width;
  float Theta0;
  int x0, y0;
  char BufSystVisuImg[100];

  //Constante
  length = 512;
  width = 512;

  //Allocation Memoire
  float** outputMatrix = fmatrix_allocate_2d(length, width);

  //Lecture Image
  float** inputMatrix = LoadImagePgm(NAME_IMG_IN1, &length, &width);

  float angle = 0, bestAngle = 0, minErr = INFINITY, err = 0.0;

  for (angle = -PI / 16; angle < PI / 16; angle += 0.005)
  {
    err = 0.0;
    rotateMatrix(inputMatrix, outputMatrix, angle, length, width);
    for (int i = 0; i < length; i++)
    {
      for (int j = 0; j < width; j++)
      {
        err += fabs(inputMatrix[i][j] - outputMatrix[i][j]);
      }
    }
    if (err < minErr)
    {
      minErr = err;
      bestAngle = angle;
    }
  }
  printf("Le meilleur angle est %f avec une erreur de %f \n", bestAngle, minErr);


  //==End=========================================================

  //Liberation memoire
  free_fmatrix_2d(inputMatrix);
  free_fmatrix_2d(outputMatrix);


  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0;
}


