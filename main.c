#include <stdio.h>
#include <stdlib.h>
#include <string.h>		// Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "include/SOIL.h"

// Um pixel RGB
typedef struct {
    unsigned char r, g, b;
} Pixel;

// Uma imagem em RGB
typedef struct {
    int width, height;
    Pixel* img;
} Img;

// Protótipos
void load(char* name, Img* pic);

// Carrega uma imagem para a struct Img
void load(char* name, Img* pic)
{
    int chan;
    pic->img = (Pixel*) SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if(!pic->img)
    {
        printf( "SOIL loading error: %s (%s)\n", SOIL_last_result(), name);
        exit(1);
    }
    printf("Load: %s (%d x %d x %d)\n", name, pic->width, pic->height, chan);
}

int main(int argc, char** argv)
{
    // As 2 imagens
    Img pic[2];
    if(argc < 2) {
        printf("loader [img base]\n");
        exit(1);
    }
    load(argv[1], &pic[0]);

    int scale;
    printf("Digite o percentual que deseja redimensionar a imagem (0/100): ");
    scanf("%i", &scale);

    // A largura e altura da imagem de saída de acordo com a escala da imagem
    pic[1].width  = (pic[0].width) + (pic[0].width * scale/100) ;
    pic[1].height = (pic[0].height) + (pic[0].height * scale/100) ;
	pic[1].img = calloc(pic[1].width * pic[1].height, sizeof(int)); // W x H x 3 bytes (Pixel)


    int x_ratio = (int)((pic[0].width<<16) / pic[1].width) +1;
    int y_ratio = (int)((pic[0].height<<16) / pic[1].height) +1;
    int px,py;

    printf("O tamanho ficará : %d x %d\n", pic[1].width, pic[1].height );

    for (int i=0;i<pic[1].height;i++){
            for (int j=0;j<pic[1].width;j++) {
            px = ((j*x_ratio)>>16);
            py = ((i*y_ratio)>>16);
            pic[1].img[(i*pic[1].width)+j] = pic[0].img[(py*pic[0].width)+px];
        }
    }


    printf("Gravando imagem decodificada em saida2.bmp\n");
    SOIL_save_image("saida2.bmp", SOIL_SAVE_TYPE_BMP,
    pic[1].width, pic[1].height, 3, (const unsigned char*) pic[1].img);

    free(pic[1].img);
    free(pic[0].img);
}
