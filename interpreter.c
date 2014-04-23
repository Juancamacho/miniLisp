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

/* Función que calcula el número de nodos del AST */
int numero_de_nodos(mpc_ast_t *t)
{
  if (t->children_num == 0)
    return 1;
  if (t-> children_num >= 1){
    int total = 1;
    for (int i = 0; i < t-> children_num; i++)
      total = total + numero_de_nodos(t->children[i]);
    return total;
  }
  return 0;
}

/* Función que calcula la potencia */
long potencia(long x, long y)
{
  int i = 0;
  while (i++ < y)
    x *= y;
  return x;
}

/* Función que calcula el mínimo de una lista */


/* Usa la cadena encontrada como operador y se la aplica a los operandos */
long eval_op(long x, char *op, long y)
{
  if (strcmp(op, "+") == 0 || strcmp(op, "add") == 0)
    return x + y;
  
  if (strcmp(op, "-") == 0 || strcmp(op, "sub") == 0)
    return x - y;

  if (strcmp(op, "*") == 0 || strcmp(op, "mul") == 0)
    return x * y;

  if (strcmp(op, "/") == 0 || strcmp(op, "div") == 0)
    return x / y;

  if (strcmp(op, "%") == 0 || strcmp(op, "mod") == 0)
    return x % y;

  if (strcmp(op, "^") == 0)
    return potencia(x, y);
  return 0;
}

/* Función que evalua los nodos del AST */
long eval(mpc_ast_t *t)
{
  /* Si el tag es un número lo devolvemos */
  if (strstr(t->tag, "numero"))
    return atoi(t->contents);
  
  /* Un operador siempre es el segundo hijo */
  char * op = t->children[1]->contents;

  /* Guardamos el tercer hijo en 'x' */
  long x = eval(t->children[2]);
  
  /* Iteramos sobre los demas hijos */
  int i = 3;
  while (strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }
  return x;
}



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
    numero   : /-?[0-9]+/ ;                                       \
    operador : '+' | '-' | '*' | '/' | '%' | \"add\" | \"sub\" |  \
               \"mul\" | \"div\" | \"mod\" | '^';                 \
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

      /* Evaluamos la expresion */
      long resultado = eval(r.output);
      printf("%li\n", resultado);
      
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
