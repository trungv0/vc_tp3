#include <stdlib.h>
#include <stdio.h>
#include "Util.h"
#include "pixel.h"



int main(int argc, char* argv[]) {
  FILE* ifp, *ofp_gray;
  pixel_t *pixels;
  int ich1, ich2, rows, cols, bitcols, maxval=255, is_raw;
  int i, j;

  /* Arguments */
  if ( argc != 2 ){
    printf("\nUsage: %s file \n\n", argv[0]);
    exit(0);
  }

  /* Opening */
  ifp = fopen(argv[1],"r");
  if (ifp == NULL) {
    printf("error in opening file %s\n", argv[1]);
    exit(1);
  }

  /*  Magic number reading */
  ich1 = getc( ifp );
  if ( ich1 == EOF )
      pm_erreur( "EOF / read error / magic number" );
  ich2 = getc( ifp );
  if ( ich2 == EOF )
      pm_erreur( "EOF /read error / magic number" );
  if(ich2 != '3' && ich2 != '6')
    pm_erreur(" wrong file type ");
  else
    if(ich2 == '3')
      is_raw = 0;
    else is_raw = 1;

  /* Reading image dimensions */
  cols = pm_getint( ifp );
  rows = pm_getint( ifp );
  bitcols = cols * 3;
  maxval = pm_getint( ifp );

  /* Memory allocation  */
  pixels = malloc(cols * rows * sizeof(pixel_t));

  /* Reading */
  for(i=0; i < rows; i++) {
    for(j=0; j < cols; j++) {
      pixel_t pixel;
      if(is_raw) {
        pixel.red = pm_getrawbyte(ifp);
        pixel.green = pm_getrawbyte(ifp);
        pixel.blue = pm_getrawbyte(ifp);
      } else {
        pixel.red = pm_getint(ifp);
        pixel.green = pm_getint(ifp);
      }
      pixel.x = i;
      pixel.y = j;
      pixels[i * cols + j] = pixel;
    }
  }

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      printf("r:%d g:%d b:%d --- x:%d y:%d\n",
        pixels[i * cols + j].red,
        pixels[i * cols + j].green,
        pixels[i * cols + j].blue,
        pixels[i * cols + j].x,
        pixels[i * cols + j].y);
    }
  }

  /* Writing */
  // fprintf(ofp_gray, "P5\n");
  //
  // fprintf(ofp_gray, "%d %d \n", cols, rows);
  // fprintf(ofp_gray, "%d\n",maxval);
  //
  // for (i = 0; i < rows; i++) {
  //   for (j = 0; j < bitcols; j++) {
  //     if (j % 3 == 0) {
  //       int coordinate = i * bitcols + j;
  //       char intensity = (char) (((int) colormap[coordinate] + (int) colormap[coordinate + 1] + (int) colormap[coordinate + 2]) / 3);
  //       fprintf(ofp_gray, "%c", intensity);
  //     }
  //   }
  // }


  /* Closing */
  fclose(ifp);
  return 0;
}
