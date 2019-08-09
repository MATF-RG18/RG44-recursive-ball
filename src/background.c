#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "background.h"

Background *background_init(int width, int height)
{
    Background *background;

    /*U slucaju greske prekida se program*/
    assert(width >= 0 && height >= 0);

    /*prostor za smetanje clanova strukture*/
    background = (Background *)malloc(sizeof(Background));
    assert(background != NULL);

    /*inicijalizacija clanova strukture*/
    background->width = width;
    background->height = height;
    if (width == 0 || height == 0)
    {
        background->pixels = NULL;
    }
    else
    {
        background->pixels = (char *)malloc(3 * width * height * sizeof(char));
        assert(background->pixels != NULL);
    }
    /*Vraca se pokazivac na inicijalizovanu strukturu*/
    return background;
}

void background_done(Background *background)
{
    /*Oslobadja se prostor za cuvanje podataka o pikselima 
    i prostor strukture*/
    free(background->pixels);
    free(background);
}
void background_read(Background *background, char *filename)
{
    FILE *file;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;

    unsigned int i;
    unsigned char r, g, b, a;

    free(background->pixels);
    background->pixels = NULL;

    /*fajl slike u binranom rezimu*/
    assert((file = fopen(filename, "rb")) != NULL);

    /*Ucitavaju se podaci provog zaglavlja*/
    fread(&bfh.type, 2, 1, file);
    fread(&bfh.size, 4, 1, file);
    fread(&bfh.reserved1, 2, 1, file);
    fread(&bfh.reserved2, 2, 1, file);
    fread(&bfh.offsetbits, 4, 1, file);

    /*Ucitavaju se podaci drugog zaglavlja*/
    fread(&bih.size, 4, 1, file);
    fread(&bih.width, 4, 1, file);
    fread(&bih.height, 4, 1, file);
    fread(&bih.planes, 2, 1, file);
    fread(&bih.bitcount, 2, 1, file);
    fread(&bih.compression, 4, 1, file);
    fread(&bih.sizeimage, 4, 1, file);
    fread(&bih.xpelspermeter, 4, 1, file);
    fread(&bih.ypelspermeter, 4, 1, file);
    fread(&bih.colorsused, 4, 1, file);
    fread(&bih.colorsimportant, 4, 1, file);

    /*Informacije o dimenzijama slike iz drugog zaglavlja*/
    background->width = bih.width;
    background->height = bih.height;

    /*U zavisnosti koliko bitova informacija se cita po pikselu
    mogu R,G i B komponente ili R,G,B i A komponente
    aloziramo niz odgovarajucih duzina*/
    if (bih.bitcount == 24)
        background->pixels = (char *)malloc(3 * bih.width * bih.height * sizeof(char));
    else if (bih.bitcount == 32)
        background->pixels = (char *)malloc(4 * bih.width * bih.height * sizeof(char));
    else
    {
        fprintf(stderr, "background_read(): Podrzane su samo slike koje po pikselu cuvaju 24 ili 32 bita podataka.\n");
        exit(1);
    }
    assert(background->pixels != NULL);

    if (bih.bitcount == 24)
    {
        /*po pikselu se cita 24 bita = 3 bajta informacija, pretposavljamo
        da oni (ta 3 bajta) predstavljaju R,G i B komponentu boje (1 bajt po komponenti)*/
        for (i = 0; i < bih.width * bih.height; i++)
        {
            /*Komponente boje se citaju u obrnutom redosledu, B,G,R*/
            fread(&b, sizeof(char), 1, file);
            fread(&g, sizeof(char), 1, file);
            fread(&r, sizeof(char), 1, file);

            background->pixels[3 * i] = r;
            background->pixels[3 * i + 1] = g;
            background->pixels[3 * i + 2] = b;
        }
    }
    else if (bih.bitcount == 32)
    {
        /*Po pikselu cita 3 bita = 4 bajta 
        R G B i A (1 bajt po komponenti)*/
        for (i = 0; i < bih.width * bih.height; i++)
        {
            fread(&b, sizeof(char), 1, file);
            fread(&g, sizeof(char), 1, file);
            fread(&r, sizeof(char), 1, file);
            fread(&a, sizeof(char), 1, file);

            background->pixels[4 * i] = r;
            background->pixels[4 * i + 1] = g;
            background->pixels[4 * i + 2] = b;
            background->pixels[4 * i + 3] = a;
        }
    }

    fclose(file);
}