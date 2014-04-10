#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include <editline/history.h>

int main(int argc, char *agrv[])
{
  /* Imprime la versión e información de salida */
  puts("miniLisp Version 0.0.0.0.1");
  puts("Presiona Ctrl+C para Salir\n");

  /* Entramos al ciclo inifnito */
  while (1){

    /* Imprimimos el prompt y obtenemos la entrada*/
    char* input = readline("miniLisp> ");

    /* Agregamos la entrada al historial */
    add_history(input);

    /* Regresamos lo que el usuario ingresó */
    printf("%s\n", input);

    /* Liberamos el espacio utilizado como entrada */
    free(input);
    
  }
  
  return 0;
}
