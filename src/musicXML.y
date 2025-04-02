%{
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
%}

%union {
    char *string;
    int valInt;
    struct TagOpen {
        char* tag;
        int line;
    } tagOpen;
}

%token <string> DECLARATION
%token <tagOpen> TAG_OPEN
%token <string> TAG_CLOSE
%token <string> SCHEMA_OPEN
%token <string> SCHEMA_CLOSE
%token <string> SCHEMA_DATA
%token <string> TEXT
%token ATTRIBUTE
%token OPEN_END
%token SINGLE
%type <string> content texts element_list element

%start start

%%

start:
    xml{
    inicializarArchivo("result.txt");
    };

schema:
    SCHEMA_OPEN schema_datas SCHEMA_CLOSE{
        schema_text = concatenar_cadenas("\n", schema_text);
        schema_text = concatenar_cadenas($1, schema_text);
        //schema_text = concatenar_cadenas(schema_text, $3);
        root = parse_schema(schema_text);
        schema=1;
    }
    //|SCHEMA_OPEN schema_datas
    //|SCHEMA_OPEN
    |
    ;

schema_datas:

    schema_datas schema_data_constructor {
        //schema_text = concatenar_cadenas(schema_text, "\n");
        //schema_text = concatenar_cadenas(schema_text, schema_text_aux);
        schema_text_aux = "";
    }
    | schema_data_constructor {
        //schema_text = concatenar_cadenas(schema_text, "\n");
        //schema_text = concatenar_cadenas(schema_text, schema_text_aux);
        schema_text_aux = "";
    }
    ;

schema_data_constructor:
    SCHEMA_DATA texts_schema OPEN_END{
        schema_text_aux = concatenar_cadenas($1, schema_text_aux);
        schema_text_aux = concatenar_cadenas(schema_text_aux, ">");
    }
    ;

texts_schema:
    TEXT{
        schema_text_aux = concatenar_cadenas(schema_text_aux, $1);
    }
    | texts TEXT{
        schema_text_aux = concatenar_cadenas(schema_text_aux, $2);
    }
    ;

xml:
    DECLARATION schema element YYEOF {

    }
    | DECLARATION schema element error {
//         printf(stderr, "Error: Después del nodo raíz no puede haber nada, en la línea %d.\n", line);
        YYABORT;
    }

    | DECLARATION schema texts error{
          printf("Error: Los elementos deben estar entre tags, en la línea %d.\n", line);
          YYABORT;
        }

    |
        {
          printf("Error: Debe haber una Declaración XML correcta, en la línea %d.\n", line);
          YYABORT;
        }
    ;

element_list:
      element_list element
    | element
    ;

element:

      TAG_OPEN attributes OPEN_END content TAG_CLOSE {
          if (strcmp($1.tag, $5) != 0) {
              printf("Error: Etiquetas desajustadas %s (línea %d) y %s (línea %d)\n", $1.tag, $1.line, $5, line);
              YYABORT;
          }

          if (!strcmp($1.tag, "note")){
              note_count++;
              instruments[current_part].notes++;
          }

          if (!strcmp($1.tag, "step")){
              // Cambiado para almacenar cadenas en lugar de caracteres
              measures[current_measure][measure_note] = strdup($4);
              measure_note++;
          }

          if (!strcmp($1.tag, "measure")){
              // Terminar el compás actual con NULL
              measures[current_measure][measure_note] = NULL;
              current_measure++;
              measure_note = 0;
          }

          if (!strcmp($1.tag, "instrument-name")){
              instruments[total_parts].notes = 0;
              instruments[total_parts].name = $4;
              total_parts++;
          }
          if (!strcmp($1.tag, "part")){
              current_part++;
          }
      }
    | TAG_OPEN attributes OPEN_END content YYEOF{
          printf("Error: Falta cerrar elemento %s en la línea %d.\n", $1.tag, $1.line);
          YYABORT;
        }
    | TAG_OPEN attributes SINGLE{
        // Manejo de tags auto-cerrados si es necesario
    }

    ;

attributes:
    | att_list
    ;

att_list:
    ATTRIBUTE
    | att_list ATTRIBUTE
    ;

content:
    { $$ = "";}
    | element_list
    | texts
    | error{
          printf("Error: Los elementos deben estar entre tags, en la línea %d.\n", line);
          YYABORT;
        }
    ;

texts:
    TEXT
    | texts TEXT
    ;

%%

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
