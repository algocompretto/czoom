#include <stdio.h>
#include "lib_ppm.h"

enum COLOR_CHANNEL
{
    RED,
    GREEN,
    BLUE
};

/**

    @brief Esta função define o valor de um pixel em uma imagem ampliada.
    @param pixel_value O valor do pixel a ser definido.
    @param i A posição horizontal do pixel na imagem ampliada.
    @param j A posição vertical do pixel na imagem ampliada.
    @param zoomed_image Um ponteiro para a imagem ampliada.
    @param type O tipo de pixel a ser definido (RED, GREEN ou BLUE).
    @return void
*/
void set_pixel(int pixel_value, int i, int j, struct image_s *zoomed_image, enum COLOR_CHANNEL canal)
{
    int z = canal;

    // Define os ponteiros para os três pixels correspondentes ao canal de cor escolhido
    struct pixel_s *p1 = &zoomed_image->pix[(j * 3 + 0) * zoomed_image->width + (i * 3 + z)];
    struct pixel_s *p2 = &zoomed_image->pix[(j * 3 + 1) * zoomed_image->width + (i * 3 + z)];
    struct pixel_s *p3 = &zoomed_image->pix[(j * 3 + 2) * zoomed_image->width + (i * 3 + z)];

    // Define o valor do pixel com base no valor informado e no tipo de pixel escolhido
    if (z == 0)
    {
        set_pixel_helper(pixel_value, &p1->r, &p2->r, &p3->r);
    }
    else if (z == 1)
    {
        set_pixel_helper(pixel_value, &p1->g, &p2->g, &p3->g);
    }
    else
    {
        set_pixel_helper(pixel_value, &p1->b, &p2->b, &p3->b);
    }
}

/**
    @brief Esta função auxiliar define o valor de um pixel em um canal de cor específico,
    a partir das regras estabelecidas na descrição do problema.
    @param pixel_value O valor do pixel a ser definido.
    @param p1 Ponteiro para o primeiro pixel do canal de cor a ser modificado.
    @param p2 Ponteiro para o segundo pixel do canal de cor a ser modificado.
    @param p3 Ponteiro para o terceiro pixel do canal de cor a ser modificado.
    @return void
*/
void set_pixel_helper(int pixel_value, char *p1, char *p2, char *p3)
{
    if (pixel_value >= 180)
    {
        *p1 = 255;
        *p2 = 255;
        *p3 = 255;
    }
    else if (pixel_value >= 135)
    {
        *p1 = 255;
        *p2 = 0;
        *p3 = 255;
    }
    else if (pixel_value >= 75)
    {
        *p1 = 0;
        *p2 = 255;
        *p3 = 0;
    }
    else if (pixel_value >= 0)
    {
        *p1 = 0;
        *p2 = 0;
        *p3 = 0;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Manual de uso: %s <ppm-entrada> <ppm-saida>\n", argv[0]);
        return 1;
    }

    struct image_s data;
    struct image_s *image = &data;
    struct image_s zoomed_data;
    struct image_s *zoomed_image = &zoomed_data;
    int i, j, n;

    if (read_ppm(argv[1], image) != 0)
    {
        fprintf(stderr, "Falha ao ler o arquivo %s\n", argv[1]);
        return 1;
    }

    n = new_ppm(zoomed_image, image->width * 3, image->height * 3);

    for (j = 0; j < image->height; j++)
    {
        for (i = 0; i < image->width; i++)
        {
            struct pixel_s p = image->pix[j * image->width + i];

            set_pixel(p.r, i, j, zoomed_image, RED);
            set_pixel(p.g, i, j, zoomed_image, GREEN);
            set_pixel(p.b, i, j, zoomed_image, BLUE);
        }
    }

    if (write_ppm(argv[2], zoomed_image) != 0)
    {
        fprintf(stderr, "Falha ao escrever no arquivo de saida %s\n", argv[2]);
        free_ppm(zoomed_image);
        free_ppm(image);
        return 1;
    }

    free_ppm(zoomed_image);
    free_ppm(image);

    return 0;
}
