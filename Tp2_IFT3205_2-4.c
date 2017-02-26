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
#define NAME_IMG_IN1  "UdM_1"
#define NAME_IMG_IN2  "UdM_2"
#define NAME_IMG_OUT1 "image-TP2-4-1"
#define NAME_IMG_OUT2 "image-TP2-4-2"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
void FFTTranslation(float** Matrix, int length, int width) {
    int x,y;
    for(x=0; x<length; x++)
        for(y=0; y<width; y++)
            if((x + y) % 2 == 1)
                Matrix[x][y] = -Matrix[x][y];
}

void zeroMatrix(float **mat, int length, int width)
{
  for(int i = 0; i < width; i++)
    for(int j = 0; j < length; j++)
      mat[i][j] = 0.0f;
}

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
  float** MatriceImgI1=fmatrix_allocate_2d(length,width);
  float** MatriceImgM1=fmatrix_allocate_2d(length,width);
  
  float** MatriceImgI2=fmatrix_allocate_2d(length,width);
  float** MatriceImgM2=fmatrix_allocate_2d(length,width);

  float** MatriceImgI3=fmatrix_allocate_2d(length,width);
  float** MatriceImgM3=fmatrix_allocate_2d(length,width);

  float** MatriceImg3=fmatrix_allocate_2d(length,width);

  //Lecture Image 
  float** MatriceImg1=LoadImagePgm(NAME_IMG_IN1,&length,&width);
  float** MatriceImg2=LoadImagePgm(NAME_IMG_IN2,&length,&width);

  FFTTranslation(MatriceImg1,length,width);
  FFTTranslation(MatriceImg2,length,width);
 
  FFTDD(MatriceImg1, MatriceImgI1, length, width);
  FFTDD(MatriceImg2, MatriceImgI2, length, width);

  Mod(MatriceImgM1, MatriceImg1, MatriceImgI1, length, width);
  Mod(MatriceImgM2, MatriceImg2, MatriceImgI2, length, width);

  float angle = 0, bestAngle = 0, minErr = INFINITY, err = 0.0;

  for (angle = -PI / 16; angle < PI / 16; angle += 0.005)
  {
    rotateMatrix(MatriceImgM2, MatriceImgM3, angle, length, width);
    err = 0.0;
    for (int i = 0; i < length; i++)
    {
      for (int j = 0; j < width; j++)
      {
        err += fabs(MatriceImgM3[i][j] - MatriceImgM2[i][j]);
      }
    }
    if (err < minErr)
    {
      minErr = err;
      bestAngle = angle;
    }
  }
  printf("Le meilleur angle est %f avec une erreur de %f \n", bestAngle, minErr);

  IFFTDD(MatriceImg2, MatriceImgI2, length, width);
  FFTTranslation(MatriceImg2, length, width);
  rotateMatrix(MatriceImg2, MatriceImg3, bestAngle, length, width);

    //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT1,MatriceImg3,length,width);
  SaveImagePgm(NAME_IMG_OUT2,MatriceImg2,length,width);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT2);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);


  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(MatriceImgI1);
  free_fmatrix_2d(MatriceImgM1);
  free_fmatrix_2d(MatriceImgI2);
  free_fmatrix_2d(MatriceImgM2);
  free_fmatrix_2d(MatriceImgI3);
  free_fmatrix_2d(MatriceImgM3);
  free_fmatrix_2d(MatriceImg1);
  free_fmatrix_2d(MatriceImg2);  
  free_fmatrix_2d(MatriceImg3);

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0;    
}


