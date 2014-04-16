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

/* Esta es la biblioteca que usaremos para la gramática */
#include "mpc.h"

#endif

int main(int argc, char *agrv[])
{
  /* Creamos los parsers para la notacion polaca */
  mpc_parser_t *Numero   = mpc_new("numero");
  mpc_parser_t *Operador = mpc_new("operador");
  mpc_parser_t *Expr     = mpc_new("expr");
  mpc_parser_t *miniLisp = mpc_new("miniLisp");

  /* Definimos la gramatica */
  mpca_lang(MPC_LANG_DEFAULT,
	    "                                                     \
    numero   : /-?[0-9]+(\\.[0-9]+)?/ ;                           \
    operador : '+' | '-' | '*' | '/' | '%';                       \
    expr     : <numero> | '(' <operador> <expr>+ ')' ;            \
    miniLisp : /^/ <operador> <expr>+ /$/ ;                       \
            ",
	    Numero, Operador, Expr, miniLisp);
  
  /* Imprime la versión e información de salida */
  puts("miniLisp Version 0.0.0.0.1");
  puts("Hecho en Mexico");
  puts("Presiona Ctrl+C para Salir\n");

  /* Entramos al ciclo inifnito */
  while (1){

    /* Imprimimos el prompt y obtenemos la entrada*/
    char* input = readline("miniLisp> ");

    /* Agregamos la entrada al historial */
    add_history(input);

    /* Intentamos parsear la entrada del usuario */
    mpc_result_t r;
    if(mpc_parse("<stdin>", input, miniLisp, &r)){
      /* Si la entrada es correcta imprimimos el arbol */
      /* 	de sintaxis abstracta */
      mpc_ast_print(r.output);
      mpc_ast_delete(r.output);
    } else {
      /* Si no es correcta imprimimos el error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }
    
    /* Liberamos el espacio utilizado como entrada */
    free(input);
    
  }

  /* Eliminamos los parses de la memoria */
  mpc_cleanup(4, Numero, Operador, Expr, miniLisp);
  
  return 0;
}
