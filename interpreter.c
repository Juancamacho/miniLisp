#include <stdio.h>

/* Declara un buffer estático para la entrada del usuario */
/* la entrada no debe ser mayor a 2048 caracteres */
static char input[2048];

int main(int argc, char *agrv[])
{
  /* Imprime la versión e información de salida */
  puts("miniLisp Version 0.0.0.0.1");
  puts("Presiona Ctrl+C para Salir\n");

  /* Entramos al ciclo inifnito */
  while (1){

    /* Imprimimos el prompt */
    fputs("miniLisp> ", stdout);

    /* Leemos la línea que el usuario ingresó */
    /*   la longitud máxima es 2048 */
    fgets(input, 2048, stdin);

    /* Regresamos lo que el usuario ingresó */
    printf("%s", input);
    
  }
  
  return 0;
}
