#include <stdio.h>
#include <stdlib.h>

/* Si el código se va a compilar en una máquina con Windows */
/* utilizamos estas funciones falsas */
#ifdef _WIN32

#include <string.h>

static char buffer[2048];

/* Función readline falsa */
char* readline(char *prompt)
{
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char *cpy = malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy) - 1] = '\0';
  return cpy;
}

/* Función add_history falsa */
void add_history(char *unused) {}

/* Si no usamos Windows cargamos las otras cabeceras */
#else

#include <editline/readline.h>
#include <editline/history.h>

#endif

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

    /* Liberamos el espacio utilizado como entrada */
    free(input);
    
  }
  
  return 0;
}
