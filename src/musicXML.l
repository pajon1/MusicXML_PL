%{
#include "practica2.tab.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int line=1;

// Estructura del elemento en el esquema
typedef struct Element {
    char *name;
    struct Element **children;
    int child_count;
} Element;

// Pila
#define STACK_MAX 300
Element* stack[STACK_MAX];
int stack_top = -1;

// Raíz del esquema (DTD parseado)
Element *root = NULL;

// Códigos de estado para funciones
typedef enum {
    SUCCESS,
    ERROR_STACK_FULL,
    ERROR_STACK_EMPTY,
    ERROR_CHILD_NOT_FOUND
} Status;

// Funciones de pila
Status push(Element *elem) {
    if (stack_top < STACK_MAX - 1) {
        stack[++stack_top] = elem;
        return SUCCESS;
    } else {
        fprintf(stderr, "Error: Pila llena,en linea %d\n",line);
        return ERROR_STACK_FULL;
    }
}

Status pop() {
    if (stack_top >= 0) {
        stack_top--;
        return SUCCESS;
    } else {
        fprintf(stderr, "Error: Pila vacía,en linea %d.\n",line);
        return ERROR_STACK_EMPTY;
    }
}

Element* peek() {
    if (stack_top >= 0) {
        return stack[stack_top];
    } else {
        return NULL;
    }
}

// Crear un nuevo elemento
Element* create_element(const char *name) {
    Element *elem = (Element*)malloc(sizeof(Element));
    if (!elem) {
        fprintf(stderr, "Error: No se pudo asignar memoria para '%s',en linea %d.\n", name,line);
        exit(EXIT_FAILURE);
    }
    elem->name = strdup(name);
    if (!elem->name) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nombre del elemento,en linea %d.\n",line);
        free(elem);
        exit(EXIT_FAILURE);
    }
    elem->children = NULL;
    elem->child_count = 0;
    return elem;
}

// Agregar un hijo
void add_child(Element *parent, Element *child) {
    parent->children = (Element**)realloc(parent->children,
                          sizeof(Element*) * (parent->child_count + 1));
    if (!parent->children) {
        fprintf(stderr, "Error: No se pudo asignar memoria para hijos de '%d',en linea %s.\n",line,
                parent->name);
        exit(EXIT_FAILURE);
    }
    parent->children[parent->child_count++] = child;
}

// Quitar espacios en blanco al inicio
char* ltrim(char *s) {
    while(isspace((unsigned char)*s)) s++;
    return s;
}

// Función para buscar un elemento por nombre en el árbol ya construido
Element* find_element(Element *current_root, const char *name) {
    if (!current_root) return NULL;
    if (strcmp(current_root->name, name) == 0) return current_root;
    for (int i = 0; i < current_root->child_count; i++) {
        Element *found = find_element(current_root->children[i], name);
        if (found) return found;
    }
    return NULL;
}

// ---------------------------------------------------
// PARSER del DTD (simplificado)
// ---------------------------------------------------
Element* parse_schema(char *schema_str) {
    stack_top = -1;
    root = NULL;

    char *ptr = schema_str;
    while (*ptr) {
        ptr = ltrim(ptr);
        if (strncmp(ptr, "<!ELEMENT", 9) == 0) {
            ptr += 9;
            ptr = ltrim(ptr);

            // Extraer nombre del elemento
            char elem_name[100];
            int i = 0;
            while (*ptr && !isspace((unsigned char)*ptr) && *ptr != '(' && *ptr != '>') {
                elem_name[i++] = *ptr++;
            }
            elem_name[i] = '\0';

            // Buscar '('
            char *start = strchr(ptr, '(');
            if (!start) {
                // Declaración vacía o inválida, saltar hasta '>'
                char *end_decl = strchr(ptr, '>');
                if (end_decl) ptr = end_decl + 1;
                continue;
            }
            ptr = start + 1;

            // Extraer contenido entre '(...)'
            char content[200];
            i = 0;
            int parentheses_count = 1; // Para manejar anidaciones futuras
            while (*ptr && parentheses_count > 0) {
                if (*ptr == '(') parentheses_count++;
                if (*ptr == ')') parentheses_count--;
                if (parentheses_count > 0) {
                    content[i++] = *ptr++;
                }
            }
            content[i] = '\0';

            // Saltar el ')'
            if (*ptr == ')') ptr++;

            // Saltar el '>'
            char *end_decl = strchr(ptr, '>');
            if (end_decl) ptr = end_decl + 1;

            // Ver si es (#PCDATA) o lista de hijos
            char *content_trim = ltrim(content);
            if (strstr(content_trim, "#PCDATA")) {
                // Solo es definición del tipo de contenido => no crear hijos
                continue;
            } else {
                // Ejemplo: (titulo, autor, año)
                // Primero, buscar si el elemento ya existe
                Element *elem = find_element(root, elem_name);
                if (!elem) {
                    elem = create_element(elem_name);
                    // Si no hay root, asumimos que este es el principal
                    if (!root) root = elem;
                }

                // Parsear lista de hijos separados por comas
                char content_copy[200];
                strncpy(content_copy, content_trim, sizeof(content_copy));
                content_copy[sizeof(content_copy)-1] = '\0'; // Asegurar terminación

                char *token = strtok(content_copy, ",");
                while (token) {
                    token = ltrim(token);
                    // Quitar espacios finales
                    int len = strlen(token);
                    while (len > 0 && isspace((unsigned char)token[len - 1])) {
                        token[len - 1] = '\0';
                        len--;
                    }
                    if (len > 0) {
                        // Buscamos si ya existe el elemento
                        Element *child = find_element(root, token);
                        if (!child) {
                            child = create_element(token);
                        }
                        add_child(elem, child);
                    }
                    token = strtok(NULL, ",");
                }

                // Si el elemento es "libro", se redefine root (por conveniencia)
                // (Este fragmento puede ser eliminado si no es necesario)
                /*
                if (strcmp(elem_name, "libro") == 0) {
                    root = elem;
                }
                */
            }
        }
        else if (*ptr == ']') {
            // Fin del DOCTYPE
            ptr++;
        }
        else {
            ptr++;
        }
    }

    return root;
}

// ---------------------------------------------------
// NAVEGACIÓN
// ---------------------------------------------------
Status advance_to(const char *child_name) {
    // Si la pila está vacía, interpretamos que el usuario quiere empezar en 'root'
    if (peek() == NULL) {
        // Verificar si el child_name es el root->name
        if (root && strcmp(root->name, child_name) == 0) {
            Status st = push(root);
            if (st == SUCCESS) {
                //printf("Avanzado a: %s\n", child_name);
            }
            return st;
        } else {
            fprintf(stderr, "Error: No hay elemento en la pila y '%s' no coincide con la raíz,en linea %d.\n",
                    child_name,line);
            return ERROR_CHILD_NOT_FOUND;
        }
    }

    // Si la pila NO está vacía, navegamos normalmente a un hijo
    Element *current = peek();
    if (!current) {
        fprintf(stderr, "Error: La pila está vacía,en linea %d.\n",line);
        return ERROR_STACK_EMPTY;
    }
    for (int i = 0; i < current->child_count; i++) {
        if (strcmp(current->children[i]->name, child_name) == 0) {
            Status st = push(current->children[i]);
            if (st == SUCCESS) {
                //printf("Avanzado a: %s\n", child_name);
            }
            return st;
        }
    }
    fprintf(stderr, "Error: Hijo '%s' no encontrado en '%s',en linea %d.\n",
            child_name, current->name,line);
    return ERROR_CHILD_NOT_FOUND;
}

Status go_back() {
    Status st = pop();
    if (st == SUCCESS) {
        Element *current = peek();
        if (current) {
            //printf("Retrocedido a: %s\n", current->name);
        } else {
            //printf("Pila vacía después de retroceder.\n");
        }
    }
    else if(st == ERROR_STACK_EMPTY)
        fprintf(stderr, "Error: La pila está vacía,en linea %d.\n",line);
    return st;
}

// Imprimir la estructura
void print_structure(Element *elem, int level) {
    if (!elem) return;
    for (int i = 0; i < level; i++) printf("  ");
    printf("- %s\n", elem->name);
    for (int i = 0; i < elem->child_count; i++) {
        print_structure(elem->children[i], level + 1);
    }
}

// Liberar memoria
void free_structure(Element *elem) {
    if (!elem) return;
    for (int i = 0; i < elem->child_count; i++) {
        free_structure(elem->children[i]);
    }
    free(elem->children);
    free(elem->name);
    free(elem);
}
int schema=0;
%}
version [0-9]+"."[0-9]+
encoding UTF-8
declar "<"\?xml[ \t]+version=\"{version}\"([ \t]+encoding=\"{encoding}\")?([ \t]+standalone=\"(yes|no)\")?[ \t]*\?">"
tag_name [a-zA-ZñÑáéíóúÁÉÍÓÚ_.-]+
attribute {tag_name}=\"[a-zA-Z0-9" "_.-]+\"
open    "<"{tag_name}
open_end ">"
schema_data "<"!ELEMENT[ \s]+{tag_name}[ \s]+\(
schema_open "<"!DOCTYPE[ \s]+{tag_name}[ \s]+\[
schema_close \]">"
single_end "/>"
close   "</"{tag_name}">"
texto [^<>\n\t\s" "]+
texto_com [^<>\n]+
coment <!--{texto_com}-->

%%

{declar} { return DECLARATION; }
{open}   {
            yylval.tagOpen.tag = strdup(yytext + 1); // Omite '<'
            yylval.tagOpen.line=line;
            if(schema){
            Status result=advance_to(yylval.tagOpen.tag);
            if(result!=SUCCESS)
                exit(1);

          }return TAG_OPEN;}
{schema_open} {yylval.string=yytext;return SCHEMA_OPEN;}
{schema_close} {yylval.string=yytext;return SCHEMA_CLOSE;}
{schema_data} {yylval.string=yytext;return SCHEMA_DATA;}
{open_end} { return OPEN_END;}
{attribute}	{ return ATTRIBUTE;}
{close}  {
            yylval.string = strdup(yytext + 2); // Omite '</'
            yylval.string[strlen(yylval.string) - 1] = '\0'; // Remueve '>'
            if(schema){Status result=go_back();
            if(result!=SUCCESS)
                exit(1);}
            return TAG_CLOSE;
          }
{single_end} {
            if(schema){Status result=go_back();
            if(result!=SUCCESS)
                exit(1);
                }return SINGLE;
            }
\t	 { /* Ignora tabular */ }
" "	 { /* Ignora espacios */ }
\s	 { /* Ignora espacio */ }
\n       { line++;}
{coment} { /* Ignora comentarios */ }
{texto}  {yylval.string = strdup(yytext); return TEXT; }
.        { /* Ignora otros caracteres */ }

%%

