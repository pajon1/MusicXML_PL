/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "practica.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaración de funciones y variables externas
void yyerror(const char *s);
extern int yylex(void);

// Declaración de la variable global para el archivo
FILE *archivoGlobal = NULL;

/**
 * Función para inicializar el archivo.
 * Si el archivo ya existe, se borra su contenido.
 * @param nombreArchivo: Nombre del archivo a crear o limpiar.
 * @return 0 si todo salió bien, -1 si hubo un error.
 */
int inicializarArchivo(const char *nombreArchivo) {
    // Abre el archivo en modo escritura ("w"), lo que crea el archivo si no existe
    // o borra el contenido si ya existe.
    archivoGlobal = fopen(nombreArchivo, "w");
    if (archivoGlobal == NULL) {
        perror("Error al abrir el archivo");
        return -1;
    }
    return 0;
}

/**
 * Función para escribir una línea en el archivo.
 * @param linea: Cadena de caracteres a escribir en el archivo.
 * @return 0 si todo salió bien, -1 si hubo un error.
 */
int escribirLinea(const char *linea) {
    if (archivoGlobal == NULL) {
        printf("El archivo no está inicializado. Llama a inicializarArchivo primero.\n");
        return -1;
    }

    // Escribe la línea en el archivo
    if (fprintf(archivoGlobal, "%s\n", linea) < 0) {
        perror("Error al escribir en el archivo");
        return -1;
    }

    return 0;
}

/**
 * Función para cerrar el archivo.
 * @return 0 si todo salió bien, -1 si hubo un error.
 */
int cerrarArchivo() {
    if (archivoGlobal == NULL) {
        printf("El archivo ya está cerrado o no fue inicializado.\n");
        return -1;
    }

    if (fclose(archivoGlobal) != 0) {
        perror("Error al cerrar el archivo");
        return -1;
    }

    archivoGlobal = NULL;
    return 0;
}


// Función para concatenar cadenas
char* concatenar_cadenas(const char* cadena1, const char* cadena2) {
    // Verificar que las cadenas no sean NULL
    if (cadena1 == NULL || cadena2 == NULL) {
        return NULL;
    }

    // Calcular la longitud de ambas cadenas
    size_t longitud1 = strlen(cadena1);
    size_t longitud2 = strlen(cadena2);

    // Asignar memoria para la nueva cadena (+1 para el carácter nulo)
    char* resultado = (char*)malloc((longitud1 + longitud2 + 1) * sizeof(char));
    if (resultado == NULL) {
        // Fallo en la asignación de memoria
        return NULL;
    }

    // Copiar la primera cadena en resultado
    strcpy(resultado, cadena1);

    // Concatenar la segunda cadena
    strcat(resultado, cadena2);

    return resultado;
}

typedef struct Element {
    char *name;
    struct Element **children;
    int child_count;
} Element;

typedef enum {
    SUCCESS,
    ERROR_STACK_FULL,
    ERROR_STACK_EMPTY,
    ERROR_CHILD_NOT_FOUND
} Status;

char *schema_text = "";
char *schema_text_aux = "";

// Declaración de una variable personalizada para el conteo de líneas
extern int line;
extern Element* parse_schema(char *schema_str);
extern Element *root;
extern Status go_back();

int note_count = 0;



// Inicialización de variables para medir las notas en cada compás
int current_measure = -1;
int measure_note = 0;

struct Instrument {
    char* name;
    int notes;
};
int current_part = 0;
int total_parts = 0;
struct Instrument instruments[100];

#define MAX_NOTAS_COMPAS 300
#define MAX_COMPASES 1000
#define MAX_NOTAS_ACORDE 15 // Ahora permitimos hasta 6 notas por acorde
#define MAX_NOTAS_UNICAS 100

const char *measures[5000][MAX_NOTAS_COMPAS];

typedef struct {
    const char *nombre;
    const char *notas[MAX_NOTAS_ACORDE];
    int numNotas; // número real de notas en este acorde
} Acorde;

// Base extensa de acordes
Acorde base_acordes[] = {
    // -------------------------------------
    // Acordes en C
    // -------------------------------------
    {"C",      {"C", "E", "G"}, 3},
    {"Cm",     {"C", "Eb", "G"}, 3},
    {"Cdim",   {"C", "Eb", "Gb"}, 3},
    {"Caug",   {"C", "E", "G#"}, 3},

    // Triadas con sexta
    {"C6",     {"C", "E", "G", "A"}, 4},
    {"Cm6",    {"C", "Eb", "G", "A"}, 4},

    // Con séptima (dominante, mayor, menor)
    {"C7",     {"C", "E", "G", "Bb"}, 4},
    {"Cmaj7",  {"C", "E", "G", "B"}, 4},
    {"Cm7",    {"C", "Eb", "G", "Bb"}, 4},
    {"Cdim7",  {"C", "Eb", "Gb", "Bbb"}, 4}, // Bbb = A

    // Semidisminuido
    {"Cm7b5",  {"C", "Eb", "Gb", "Bb"}, 4}, // También conocido como Cø7 (C half-diminished)

    // Séptima aumentada/disminuida
    {"C7#5",   {"C", "E", "G#", "Bb"}, 4},
    {"C7b5",   {"C", "E", "Gb", "Bb"}, 4},
    {"CmMaj7", {"C", "Eb", "G", "B"}, 4},

    // Acordes con tensiones (9, 11, 13)
    {"C9",      {"C", "E", "G", "Bb", "D"}, 5},
    {"Cmaj9",   {"C", "E", "G", "B", "D"}, 5},
    {"Cm9",     {"C", "Eb", "G", "Bb", "D"}, 5},
    {"C11",     {"C", "E", "G", "Bb", "D", "F"}, 6},
    {"Cm11",    {"C", "Eb", "G", "Bb", "D", "F"}, 6},
    {"Cmaj11",  {"C", "E", "G", "B", "D", "F"}, 6},
    {"C13",     {"C", "E", "G", "Bb", "D", "F", "A"}, 7},
    {"Cm13",    {"C", "Eb", "G", "Bb", "D", "F", "A"}, 7},
    {"Cmaj13",  {"C", "E", "G", "B", "D", "F", "A"}, 7},

    // Alteraciones a las extensiones
    {"C7b9",    {"C", "E", "G", "Bb", "Db"}, 5},
    {"C7#9",    {"C", "E", "G", "Bb", "D#"}, 5},
    {"C7#11",   {"C", "E", "G", "Bb", "D", "F#"}, 6},
    {"C7b13",   {"C", "E", "G", "Bb", "D", "F", "Ab"}, 7},

    // Más variaciones interesantes
    {"Csus2",   {"C", "D", "G"}, 3},
    {"Csus4",   {"C", "F", "G"}, 3},
    {"Cadd9",   {"C", "E", "G", "D"}, 4},
    {"Cadd11",  {"C", "E", "G", "F"}, 4},
    {"Cadd13",  {"C", "E", "G", "A"}, 4},

    // Ejemplos en alteraciones de la fundamental (Ej. C#):
    {"C#maj",   {"C#", "F", "G#"}, 3},
    {"C#m",     {"C#", "E", "G#"}, 3},
    {"C#7",     {"C#", "F", "G#", "B"}, 4},
    {"C#maj7",  {"C#", "F", "G#", "C"}, 4},  // C = B#, enharmónico

    // -------------------------------------
    // Acordes en D
    // -------------------------------------
    // Triadas básicas
    {"D",      {"D", "F#", "A"}, 3},
    {"Dm",     {"D", "F", "A"}, 3},
    {"Ddim",   {"D", "F", "Ab"}, 3},
    {"Daug",   {"D", "F#", "A#"}, 3},

    // Triadas con sexta
    {"D6",     {"D", "F#", "A", "B"}, 4},
    {"Dm6",    {"D", "F", "A", "B"}, 4},

    // Con séptima (dominante, mayor, menor)
    {"D7",     {"D", "F#", "A", "C"}, 4},
    {"Dmaj7",  {"D", "F#", "A", "C#"}, 4},
    {"Dm7",    {"D", "F", "A", "C"}, 4},
    {"Ddim7",  {"D", "F", "Ab", "Cb"}, 4}, // Cb = B

    // Semidisminuido
    {"Dm7b5",  {"D", "F", "Ab", "C"}, 4}, // También conocido como Dø7 (D half-diminished)

    // Séptima aumentada/disminuida
    {"D7#5",   {"D", "F#", "A#", "C"}, 4},
    {"D7b5",   {"D", "F#", "Ab", "C"}, 4},
    {"DmMaj7", {"D", "F", "A", "C#"}, 4},

    // Acordes con tensiones (9, 11, 13)
    {"D9",      {"D", "F#", "A", "C", "E"}, 5},
    {"Dmaj9",   {"D", "F#", "A", "C#", "E"}, 5},
    {"Dm9",     {"D", "F", "A", "C", "E"}, 5},
    {"D11",     {"D", "F#", "A", "C", "E", "G"}, 6},
    {"Dm11",    {"D", "F", "A", "C", "E", "G"}, 6},
    {"Dmaj11",  {"D", "F#", "A", "C#", "E", "G"}, 6},
    {"D13",     {"D", "F#", "A", "C", "E", "G", "B"}, 7},
    {"Dm13",    {"D", "F", "A", "C", "E", "G", "B"}, 7},
    {"Dmaj13",  {"D", "F#", "A", "C#", "E", "G", "B"}, 7},

    // Alteraciones a las extensiones
    {"D7b9",    {"D", "F#", "A", "C", "Eb"}, 5},
    {"D7#9",    {"D", "F#", "A", "C", "F"}, 5},
    {"D7#11",   {"D", "F#", "A", "C", "E", "G#"}, 6},
    {"D7b13",   {"D", "F#", "A", "C", "E", "G", "Bb"}, 7},

    // Más variaciones interesantes
    {"Dsus2",   {"D", "E", "A"}, 3},
    {"Dsus4",   {"D", "G", "A"}, 3},
    {"Dadd9",   {"D", "F#", "A", "E"}, 4},
    {"Dadd11",  {"D", "F#", "A", "G"}, 4},
    {"Dadd13",  {"D", "F#", "A", "B"}, 4},

    // -------------------------------------
    // Acordes en E
    // -------------------------------------
    // Triadas básicas
    {"E",      {"E", "G#", "B"}, 3},
    {"Em",     {"E", "G", "B"}, 3},
    {"Edim",   {"E", "G", "Bb"}, 3},
    {"Eaug",   {"E", "G#", "B#"}, 3}, // B# = C

    // Triadas con sexta
    {"E6",     {"E", "G#", "B", "C#"}, 4},
    {"Em6",    {"E", "G", "B", "C#"}, 4},

    // Con séptima (dominante, mayor, menor)
    {"E7",     {"E", "G#", "B", "D"}, 4},
    {"Emaj7",  {"E", "G#", "B", "D#"}, 4},
    {"Em7",    {"E", "G", "B", "D"}, 4},
    {"Edim7",  {"E", "G", "Bb", "Db"}, 4}, // Db = C#

    // Semidisminuido
    {"Em7b5",  {"E", "G", "Bb", "D"}, 4}, // También conocido como Eø7 (E half-diminished)

    // Séptima aumentada/disminuida
    {"E7#5",   {"E", "G#", "B#", "D"}, 4}, // B# = C
    {"E7b5",   {"E", "G#", "Bb", "D"}, 4},
    {"EmMaj7", {"E", "G", "B", "D#"}, 4},

    // Acordes con tensiones (9, 11, 13)
    {"E9",      {"E", "G#", "B", "D", "F#"}, 5},
    {"Emaj9",   {"E", "G#", "B", "D#", "F#"}, 5},
    {"Em9",     {"E", "G", "B", "D", "F#"}, 5},
    {"E11",     {"E", "G#", "B", "D", "F#", "A"}, 6},
    {"Em11",    {"E", "G", "B", "D", "F#", "A"}, 6},
    {"Emaj11",  {"E", "G#", "B", "D#", "F#", "A"}, 6},
    {"E13",     {"E", "G#", "B", "D", "F#", "A", "C#"}, 7},
    {"Em13",    {"E", "G", "B", "D", "F#", "A", "C#"}, 7},
    {"Emaj13",  {"E", "G#", "B", "D#", "F#", "A", "C#"}, 7},

    // Alteraciones a las extensiones
    {"E7b9",    {"E", "G#", "B", "D", "F"}, 5},
    {"E7#9",    {"E", "G#", "B", "D", "G"}, 5}, // G = Fb#
    {"E7#11",   {"E", "G#", "B", "D", "F#", "A#"}, 6},
    {"E7b13",   {"E", "G#", "B", "D", "F#", "A", "Cb"}, 7}, // Cb = B

    // Más variaciones interesantes
    {"Esus2",   {"E", "F#", "B"}, 3},
    {"Esus4",   {"E", "A", "B"}, 3},
    {"Eadd9",   {"E", "G#", "B", "F#"}, 4},
    {"Eadd11",  {"E", "G#", "B", "A"}, 4},
    {"Eadd13",  {"E", "G#", "B", "C#"}, 4},

    // -------------------------------------
    // Acordes en F
    // -------------------------------------
    // Triadas básicas
    {"F",      {"F", "A", "C"}, 3},
    {"Fm",     {"F", "Ab", "C"}, 3},
    {"Fdim",   {"F", "Ab", "Cb"}, 3}, // Cb = B
    {"Faug",   {"F", "A", "C#"}, 3},

    // Triadas con sexta
    {"F6",     {"F", "A", "C", "D"}, 4},
    {"Fm6",    {"F", "Ab", "C", "D"}, 4},

    // Con séptima (dominante, mayor, menor)
    {"F7",     {"F", "A", "C", "Eb"}, 4},
    {"Fmaj7",  {"F", "A", "C", "E"}, 4},
    {"Fm7",    {"F", "Ab", "C", "Eb"}, 4},
    {"Fdim7",  {"F", "Ab", "Cb", "Ebb"}, 4}, // Ebb = D

    // Semidisminuido
    {"Fm7b5",  {"F", "Ab", "Cb", "Eb"}, 4}, // También conocido como Fø7 (F half-diminished)

    // Séptima aumentada/disminuida
    {"F7#5",   {"F", "A", "C#", "Eb"}, 4},
    {"F7b5",   {"F", "A", "Cb", "Eb"}, 4},
    {"FmMaj7", {"F", "Ab", "C", "E"}, 4},

    // Acordes con tensiones (9, 11, 13)
    {"F9",      {"F", "A", "C", "Eb", "G"}, 5},
    {"Fmaj9",   {"F", "A", "C", "E", "G"}, 5},
    {"Fm9",     {"F", "Ab", "C", "Eb", "G"}, 5},
    {"F11",     {"F", "A", "C", "Eb", "G", "Bb"}, 6},
    {"Fm11",    {"F", "Ab", "C", "Eb", "G", "Bb"}, 6},
    {"Fmaj11",  {"F", "A", "C", "E", "G", "Bb"}, 6},
    {"F13",     {"F", "A", "C", "Eb", "G", "Bb", "D"}, 7},
    {"Fm13",    {"F", "Ab", "C", "Eb", "G", "Bb", "D"}, 7},
    {"Fmaj13",  {"F", "A", "C", "E", "G", "Bb", "D"}, 7},

    // Alteraciones a las extensiones
    {"F7b9",    {"F", "A", "C", "Eb", "Gb"}, 5},
    {"F7#9",    {"F", "A", "C", "Eb", "G#"}, 5}, // G# = Ab
    {"F7#11",   {"F", "A", "C", "Eb", "G", "B"}, 6},
    {"F7b13",   {"F", "A", "C", "Eb", "G", "Bb", "Db"}, 7},

    // Más variaciones interesantes
    {"Fsus2",   {"F", "G", "C"}, 3},
    {"Fsus4",   {"F", "Bb", "C"}, 3},
    {"Fadd9",   {"F", "A", "C", "G"}, 4},
    {"Fadd11",  {"F", "A", "C", "Bb"}, 4},
    {"Fadd13",  {"F", "A", "C", "D"}, 4},

    // -------------------------------------
    // Acordes en G
    // -------------------------------------
    // Triadas básicas
    {"G",      {"G", "B", "D"}, 3},
    {"Gm",     {"G", "Bb", "D"}, 3},
    {"Gdim",   {"G", "Bb", "Db"}, 3}, // Db = C#
    {"Gaug",   {"G", "B", "D#"}, 3}, // D# = Eb

    // Triadas con sexta
    {"G6",     {"G", "B", "D", "E"}, 4},
    {"Gm6",    {"G", "Bb", "D", "E"}, 4},

    // Con séptima (dominante, mayor, menor)
    {"G7",     {"G", "B", "D", "F"}, 4},
    {"Gmaj7",  {"G", "B", "D", "F#"}, 4},
    {"Gm7",    {"G", "Bb", "D", "F"}, 4},
    {"Gdim7",  {"G", "Bb", "Db", "Fb"}, 4}, // Fb = E

    // Semidisminuido
    {"Gm7b5",  {"G", "Bb", "Db", "F"}, 4}, // También conocido como Gø7 (G half-diminished)

    // Séptima aumentada/disminuida
    {"G7#5",   {"G", "B", "D#", "F"}, 4}, // D# = Eb
    {"G7b5",   {"G", "B", "Db", "F"}, 4},
    {"GmMaj7", {"G", "Bb", "D", "F#"}, 4},

    // Acordes con tensiones (9, 11, 13)
    {"G9",      {"G", "B", "D", "F", "A"}, 5},
    {"Gmaj9",   {"G", "B", "D", "F#", "A"}, 5},
    {"Gm9",     {"G", "Bb", "D", "F", "A"}, 5},
    {"G11",     {"G", "B", "D", "F", "A", "C"}, 6},
    {"Gm11",    {"G", "Bb", "D", "F", "A", "C"}, 6},
    {"Gmaj11",  {"G", "B", "D", "F#", "A", "C"}, 6},
    {"G13",     {"G", "B", "D", "F", "A", "C", "E"}, 7},
    {"Gm13",    {"G", "Bb", "D", "F", "A", "C", "E"}, 7},
    {"Gmaj13",  {"G", "B", "D", "F#", "A", "C", "E"}, 7},

    // Alteraciones a las extensiones
    {"G7b9",    {"G", "B", "D", "F", "Ab"}, 5},
    {"G7#9",    {"G", "B", "D", "F", "A#"}, 5}, // A# = Bb
    {"G7#11",   {"G", "B", "D", "F", "A", "C#"}, 6}, // C# = Db
    {"G7b13",   {"G", "B", "D", "F", "A", "C", "Eb"}, 7}, // Eb = D#

    // Más variaciones interesantes
    {"Gsus2",   {"G", "A", "D"}, 3},
    {"Gsus4",   {"G", "C", "D"}, 3},
    {"Gadd9",   {"G", "B", "D", "A"}, 4},
    {"Gadd11",  {"G", "B", "D", "C"}, 4},
    {"Gadd13",  {"G", "B", "D", "E"}, 4},

    // -------------------------------------
    // Acordes en A
    // -------------------------------------
    // Triadas básicas
    {"A",      {"A", "C#", "E"}, 3},
    {"Am",     {"A", "C", "E"}, 3},
    {"Adim",   {"A", "C", "Eb"}, 3},
    {"Aaug",   {"A", "C#", "E#"}, 3}, // E# = F

    // Triadas con sexta
    {"A6",     {"A", "C#", "E", "F#"}, 4},
    {"Am6",    {"A", "C", "E", "F#"}, 4},

    // Con séptima (dominante, mayor, menor)
    {"A7",     {"A", "C#", "E", "G"}, 4},
    {"Amaj7",  {"A", "C#", "E", "G#"}, 4},
    {"Am7",    {"A", "C", "E", "G"}, 4},
    {"Adim7",  {"A", "C", "Eb", "Gb"}, 4}, // Gb = F#

    // Semidisminuido
    {"Am7b5",  {"A", "C", "Eb", "G"}, 4}, // También conocido como Aø7 (A half-diminished)

    // Séptima aumentada/disminuida
    {"A7#5",   {"A", "C#", "E#", "G"}, 4}, // E# = F
    {"A7b5",   {"A", "C#", "Eb", "G"}, 4},
    {"AmMaj7", {"A", "C", "E", "G#"}, 4},

    // Acordes con tensiones (9, 11, 13)
    {"A9",      {"A", "C#", "E", "G", "B"}, 5},
    {"Amaj9",   {"A", "C#", "E", "G#", "B"}, 5},
    {"Am9",     {"A", "C", "E", "G", "B"}, 5},
    {"A11",     {"A", "C#", "E", "G", "B", "D"}, 6},
    {"Am11",    {"A", "C", "E", "G", "B", "D"}, 6},
    {"Amaj11",  {"A", "C#", "E", "G#", "B", "D"}, 6},
    {"A13",     {"A", "C#", "E", "G", "B", "D", "F#"}, 7},
    {"Am13",    {"A", "C", "E", "G", "B", "D", "F#"}, 7},
    {"Amaj13",  {"A", "C#", "E", "G#", "B", "D", "F#"}, 7},

    // Alteraciones a las extensiones
    {"A7b9",    {"A", "C#", "E", "G", "Bb"}, 5}, // Bb = A#
    {"A7#9",    {"A", "C#", "E", "G", "B#"}, 5}, // B# = C
    {"A7#11",   {"A", "C#", "E", "G", "B", "D#"}, 6}, // D# = Eb
    {"A7b13",   {"A", "C#", "E", "G", "B", "D", "F"}, 7}, // F = E#

    // Más variaciones interesantes
    {"Asus2",   {"A", "B", "E"}, 3},
    {"Asus4",   {"A", "D", "E"}, 3},
    {"Aadd9",   {"A", "C#", "E", "B"}, 4},
    {"Aadd11",  {"A", "C#", "E", "D"}, 4},
    {"Aadd13",  {"A", "C#", "E", "F#"}, 4},

    // -------------------------------------
    // Acordes en B
    // -------------------------------------
    // Triadas básicas
    {"B",      {"B", "D#", "F#"}, 3},
    {"Bm",     {"B", "D", "F#"}, 3},
    {"Bdim",   {"B", "D", "F"}, 3},
    {"Baug",   {"B", "D#", "F##"}, 3}, // F## = G

    // Triadas con sexta
    {"B6",     {"B", "D#", "F#", "G#"}, 4},
    {"Bm6",    {"B", "D", "F#", "G#"}, 4},

    // Con séptima (dominante, mayor, menor)
    {"B7",     {"B", "D#", "F#", "A"}, 4},
    {"Bmaj7",  {"B", "D#", "F#", "A#"}, 4},
    {"Bm7",    {"B", "D", "F#", "A"}, 4},
    {"Bdim7",  {"B", "D", "F", "Ab"}, 4}, // Ab = G#

    // Semidisminuido
    {"Bm7b5",  {"B", "D", "F", "A"}, 4}, // También conocido como Bø7 (B half-diminished)

    // Séptima aumentada/disminuida
    {"B7#5",   {"B", "D#", "F##", "A"}, 4}, // F## = G
    {"B7b5",   {"B", "D#", "F", "A"}, 4},
    {"BmMaj7", {"B", "D", "F#", "A#"}, 4},

    // Acordes con tensiones (9, 11, 13)
    {"B9",      {"B", "D#", "F#", "A", "C#"}, 5},
    {"Bmaj9",   {"B", "D#", "F#", "A#", "C#"}, 5},
    {"Bm9",     {"B", "D", "F#", "A", "C#"}, 5},
    {"B11",     {"B", "D#", "F#", "A", "C#", "E"}, 6},
    {"Bm11",    {"B", "D", "F#", "A", "C#", "E"}, 6},
    {"Bmaj11",  {"B", "D#", "F#", "A#", "C#", "E"}, 6},
    {"B13",     {"B", "D#", "F#", "A", "C#", "E", "G#"}, 7},
    {"Bm13",    {"B", "D", "F#", "A", "C#", "E", "G#"}, 7},
    {"Bmaj13",  {"B", "D#", "F#", "A#", "C#", "E", "G#"}, 7},

    // Alteraciones a las extensiones
    {"B7b9",    {"B", "D#", "F#", "A", "C"}, 5}, // C = B#
    {"B7#9",    {"B", "D#", "F#", "A", "C##"}, 5}, // C## = D
    {"B7#11",   {"B", "D#", "F#", "A", "C#", "E#"}, 6}, // E# = F
    {"B7b13",   {"B", "D#", "F#", "A", "C#", "E", "G"}, 7}, // G = F##

    // Más variaciones interesantes
    {"Bsus2",   {"B", "C#", "F#"}, 3},
    {"Bsus4",   {"B", "E", "F#"}, 3},
    {"Badd9",   {"B", "D#", "F#", "C#"}, 4},
    {"Badd11",  {"B", "D#", "F#", "E"}, 4},
    {"Badd13",  {"B", "D#", "F#", "G#"}, 4},

    // ... Puedes seguir agregando acordes para otras tonalidades aquí
};

int num_acordes = sizeof(base_acordes) / sizeof(base_acordes[0]);

// Función para verificar si todas las notas de un acorde están presentes en el conjunto de notas del compás
int es_acorde(const char* notas_compas[], int count_notas_compas, const Acorde *acorde) {
    int count = 0;
    // Verificamos que cada nota del acorde esté en el compás
    for (int i = 0; i < acorde->numNotas; i++) {
        const char *nota_acorde = acorde->notas[i];
        int found = 0;
        for (int j = 0; j < count_notas_compas; j++) {
            if (strcmp(notas_compas[j], nota_acorde) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            // Falta una nota del acorde en el compás, por lo tanto no es ese acorde
            return 0;
        }
        count++;
    }

    // Si llegamos aquí, todas las notas del acorde están en el compás
    return (count == acorde->numNotas);
}

// Función para identificar el acorde dado el conjunto de notas únicas del compás
const char* identificar_acorde(const char *notas_compas[], int count_notas) {
    // Intentamos casar el conjunto de notas del compás con algún acorde de la base
    // La lógica aquí es simple: si el compás contiene al menos todas las notas del acorde,
    // decimos que es ese acorde.
    for (int i = 0; i < num_acordes; i++) {
        if (es_acorde(notas_compas, count_notas, &base_acordes[i])) {
            return base_acordes[i].nombre;
        }
    }
    return "No se encontró acorde en la base";
}

// Función para obtener las notas únicas de un compás
// Entradas:
//   compas: arreglo de notas (cadenas), terminado con NULL
// Salidas:
//   notas_unicas: arreglo de punteros a cadenas con las notas únicas
//   retorna el número de notas únicas
int obtener_notas_unicas(const char *compas[], const char *notas_unicas[]) {
    int count_unicas = 0;
    for (int i = 0; compas[i] != NULL && i < MAX_NOTAS_COMPAS; i++) {
        const char *nota = compas[i];
        int found = 0;
        for (int j = 0; j < count_unicas; j++) {
            if (strcmp(nota, notas_unicas[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found && count_unicas < MAX_NOTAS_UNICAS) {
            notas_unicas[count_unicas] = nota;
            count_unicas++;
        }
    }
    return count_unicas;
}

// Función principal para identificar los acordes de una partitura
// Entradas:
//   partitura: arreglo bidimensional, cada fila es un compás, cada uno con notas terminadas con NULL
//   compases: número de compases
// Salida: imprime por pantalla el acorde identificado por cada compás.
void identificar_acordes(const char *partitura[][MAX_NOTAS_COMPAS], int compases) {
    for (int i = 0; i < compases; i++) {
        const char *notas_unicas[MAX_NOTAS_UNICAS] = {0};
        int count_unicas = obtener_notas_unicas(partitura[i], notas_unicas);

        const char *acorde = identificar_acorde(notas_unicas, count_unicas);
        fprintf(archivoGlobal, "Compás %d: %s\n", i+1, acorde);
    }
}

extern int schema;

#line 667 "practica.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "practica.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DECLARATION = 3,                /* DECLARATION  */
  YYSYMBOL_TAG_OPEN = 4,                   /* TAG_OPEN  */
  YYSYMBOL_TAG_CLOSE = 5,                  /* TAG_CLOSE  */
  YYSYMBOL_SCHEMA_OPEN = 6,                /* SCHEMA_OPEN  */
  YYSYMBOL_SCHEMA_CLOSE = 7,               /* SCHEMA_CLOSE  */
  YYSYMBOL_SCHEMA_DATA = 8,                /* SCHEMA_DATA  */
  YYSYMBOL_TEXT = 9,                       /* TEXT  */
  YYSYMBOL_ATTRIBUTE = 10,                 /* ATTRIBUTE  */
  YYSYMBOL_OPEN_END = 11,                  /* OPEN_END  */
  YYSYMBOL_SINGLE = 12,                    /* SINGLE  */
  YYSYMBOL_YYACCEPT = 13,                  /* $accept  */
  YYSYMBOL_start = 14,                     /* start  */
  YYSYMBOL_schema = 15,                    /* schema  */
  YYSYMBOL_schema_datas = 16,              /* schema_datas  */
  YYSYMBOL_schema_data_constructor = 17,   /* schema_data_constructor  */
  YYSYMBOL_texts_schema = 18,              /* texts_schema  */
  YYSYMBOL_xml = 19,                       /* xml  */
  YYSYMBOL_element_list = 20,              /* element_list  */
  YYSYMBOL_element = 21,                   /* element  */
  YYSYMBOL_attributes = 22,                /* attributes  */
  YYSYMBOL_att_list = 23,                  /* att_list  */
  YYSYMBOL_content = 24,                   /* content  */
  YYSYMBOL_texts = 25                      /* texts  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   35

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  13
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  28
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  39

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   267


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   623,   623,   628,   637,   642,   647,   655,   662,   665,
     671,   674,   679,   685,   692,   693,   698,   731,   735,   741,
     742,   746,   747,   751,   752,   753,   754,   761,   762
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DECLARATION",
  "TAG_OPEN", "TAG_CLOSE", "SCHEMA_OPEN", "SCHEMA_CLOSE", "SCHEMA_DATA",
  "TEXT", "ATTRIBUTE", "OPEN_END", "SINGLE", "$accept", "start", "schema",
  "schema_datas", "schema_data_constructor", "texts_schema", "xml",
  "element_list", "element", "attributes", "att_list", "content", "texts", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-16)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-29)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       5,    14,    24,   -16,     6,     0,   -16,    16,     8,   -16,
      17,   -16,    18,     2,    19,    15,    20,   -16,   -16,   -16,
      11,    21,   -16,   -16,   -16,   -16,   -16,    23,     1,   -16,
     -16,   -16,    26,   -16,     7,    25,   -16,   -16,   -16
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      13,     4,     0,     2,     0,     0,     1,     0,     0,     6,
      19,    27,     0,     0,     8,     0,     0,     3,     5,    21,
       0,    20,    10,    11,    12,    28,     7,     9,     0,    18,
      22,    26,    24,    15,     0,    25,    14,    17,    16
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -16,   -16,   -16,   -16,    27,   -16,   -16,   -16,   -15,   -16,
     -16,   -16,    -7
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     5,     8,     9,    15,     3,    32,    12,    20,
      21,    34,    13
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      16,   -23,    31,    24,    10,    10,   -23,    37,     1,    11,
      11,    25,    38,    33,     7,    17,     7,    36,    22,    23,
       4,    35,    28,    29,     6,    14,    26,    19,   -27,    27,
      10,    30,   -28,     0,    25,    18
};

static const yytype_int8 yycheck[] =
{
       7,     0,     1,     1,     4,     4,     5,     0,     3,     9,
       9,     9,     5,    28,     8,     7,     8,    32,     0,     1,
       6,    28,    11,    12,     0,     9,    11,    10,     9,     9,
       4,    10,     9,    -1,     9,     8
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    14,    19,     6,    15,     0,     8,    16,    17,
       4,     9,    21,    25,     9,    18,    25,     7,    17,    10,
      22,    23,     0,     1,     1,     9,    11,     9,    11,    12,
      10,     1,    20,    21,    24,    25,    21,     0,     5
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    13,    14,    15,    15,    16,    16,    17,    18,    18,
      19,    19,    19,    19,    20,    20,    21,    21,    21,    22,
      22,    23,    23,    24,    24,    24,    24,    25,    25
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     0,     2,     1,     3,     1,     2,
       4,     4,     4,     0,     2,     1,     5,     5,     3,     0,
       1,     1,     2,     0,     1,     1,     1,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: xml  */
#line 623 "practica.y"
       {
    inicializarArchivo("result.txt");
    }
#line 1694 "practica.tab.c"
    break;

  case 3: /* schema: SCHEMA_OPEN schema_datas SCHEMA_CLOSE  */
#line 628 "practica.y"
                                         {
        schema_text = concatenar_cadenas("\n", schema_text);
        schema_text = concatenar_cadenas((yyvsp[-2].string), schema_text);
        //schema_text = concatenar_cadenas(schema_text, $3);
        root = parse_schema(schema_text);
        schema=1;
    }
#line 1706 "practica.tab.c"
    break;

  case 5: /* schema_datas: schema_datas schema_data_constructor  */
#line 642 "practica.y"
                                         {
        //schema_text = concatenar_cadenas(schema_text, "\n");
        //schema_text = concatenar_cadenas(schema_text, schema_text_aux);
        schema_text_aux = "";
    }
#line 1716 "practica.tab.c"
    break;

  case 6: /* schema_datas: schema_data_constructor  */
#line 647 "practica.y"
                              {
        //schema_text = concatenar_cadenas(schema_text, "\n");
        //schema_text = concatenar_cadenas(schema_text, schema_text_aux);
        schema_text_aux = "";
    }
#line 1726 "practica.tab.c"
    break;

  case 7: /* schema_data_constructor: SCHEMA_DATA texts_schema OPEN_END  */
#line 655 "practica.y"
                                     {
        schema_text_aux = concatenar_cadenas((yyvsp[-2].string), schema_text_aux);
        schema_text_aux = concatenar_cadenas(schema_text_aux, ">");
    }
#line 1735 "practica.tab.c"
    break;

  case 8: /* texts_schema: TEXT  */
#line 662 "practica.y"
        {
        schema_text_aux = concatenar_cadenas(schema_text_aux, (yyvsp[0].string));
    }
#line 1743 "practica.tab.c"
    break;

  case 9: /* texts_schema: texts TEXT  */
#line 665 "practica.y"
                {
        schema_text_aux = concatenar_cadenas(schema_text_aux, (yyvsp[0].string));
    }
#line 1751 "practica.tab.c"
    break;

  case 10: /* xml: DECLARATION schema element $end  */
#line 671 "practica.y"
                                     {

    }
#line 1759 "practica.tab.c"
    break;

  case 11: /* xml: DECLARATION schema element error  */
#line 674 "practica.y"
                                       {
//         printf(stderr, "Error: Después del nodo raíz no puede haber nada, en la línea %d.\n", line);
        YYABORT;
    }
#line 1768 "practica.tab.c"
    break;

  case 12: /* xml: DECLARATION schema texts error  */
#line 679 "practica.y"
                                    {
          printf("Error: Los elementos deben estar entre tags, en la línea %d.\n", line);
          YYABORT;
        }
#line 1777 "practica.tab.c"
    break;

  case 13: /* xml: %empty  */
#line 685 "practica.y"
        {
          printf("Error: Debe haber una Declaración XML correcta, en la línea %d.\n", line);
          YYABORT;
        }
#line 1786 "practica.tab.c"
    break;

  case 16: /* element: TAG_OPEN attributes OPEN_END content TAG_CLOSE  */
#line 698 "practica.y"
                                                     {
          if (strcmp((yyvsp[-4].tagOpen).tag, (yyvsp[0].string)) != 0) {
              printf("Error: Etiquetas desajustadas %s (línea %d) y %s (línea %d)\n", (yyvsp[-4].tagOpen).tag, (yyvsp[-4].tagOpen).line, (yyvsp[0].string), line);
              YYABORT;
          }

          if (!strcmp((yyvsp[-4].tagOpen).tag, "note")){
              note_count++;
              instruments[current_part].notes++;
          }

          if (!strcmp((yyvsp[-4].tagOpen).tag, "step")){
              // Cambiado para almacenar cadenas en lugar de caracteres
              measures[current_measure][measure_note] = strdup((yyvsp[-1].string));
              measure_note++;
          }

          if (!strcmp((yyvsp[-4].tagOpen).tag, "measure")){
              // Terminar el compás actual con NULL
              measures[current_measure][measure_note] = NULL;
              current_measure++;
              measure_note = 0;
          }

          if (!strcmp((yyvsp[-4].tagOpen).tag, "instrument-name")){
              instruments[total_parts].notes = 0;
              instruments[total_parts].name = (yyvsp[-1].string);
              total_parts++;
          }
          if (!strcmp((yyvsp[-4].tagOpen).tag, "part")){
              current_part++;
          }
      }
#line 1824 "practica.tab.c"
    break;

  case 17: /* element: TAG_OPEN attributes OPEN_END content $end  */
#line 731 "practica.y"
                                                {
          printf("Error: Falta cerrar elemento %s en la línea %d.\n", (yyvsp[-4].tagOpen).tag, (yyvsp[-4].tagOpen).line);
          YYABORT;
        }
#line 1833 "practica.tab.c"
    break;

  case 18: /* element: TAG_OPEN attributes SINGLE  */
#line 735 "practica.y"
                                {
        // Manejo de tags auto-cerrados si es necesario
    }
#line 1841 "practica.tab.c"
    break;

  case 23: /* content: %empty  */
#line 751 "practica.y"
    { (yyval.string) = "";}
#line 1847 "practica.tab.c"
    break;

  case 26: /* content: error  */
#line 754 "practica.y"
           {
          printf("Error: Los elementos deben estar entre tags, en la línea %d.\n", line);
          YYABORT;
        }
#line 1856 "practica.tab.c"
    break;


#line 1860 "practica.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 765 "practica.y"


void yyerror(const char *s) {
    printf( "Error: %s en la línea %d\n", s, line);
}

int main() {

    if (yyparse() == 0) {
        printf("La sintaxis XML es correcta.\n");
        fprintf(archivoGlobal,"Esta canción contiene %d notas\n", note_count);
        int i;
        for (i = 0; i < total_parts; i++){
            fprintf(archivoGlobal,"%d notas para %s.\n", instruments[i].notes, instruments[i].name);
        }
    }

    // Calcular el número total de compases de manera correcta
    // Cambiado de 'sizeof(measures) / sizeof(measures[0])' a 'current_measure + 1'
    int total_compases = current_measure;

    // Declarar partitura como arreglo de punteros a const char *
    // Aquí se utiliza 'measures' que ya está poblado por el parser
    // y que ahora es un arreglo de punteros a const char * terminados con NULL
    identificar_acordes(measures, total_compases);

    // Opcional: Liberar la memoria asignada con strdup
    /*
    for (int i = 0; i < total_compases; i++) {
        for (int j = 0; j < MAX_NOTAS_COMPAS && measures[i][j] != NULL; j++) {
            free((void*)measures[i][j]);
        }
    }
    */
    cerrarArchivo();

    return 0;
}
