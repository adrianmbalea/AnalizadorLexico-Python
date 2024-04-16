#ifndef DEFINICIONES_H
#define DEFINICIONES_H

#define ID 300

#define FALSE 301
#define AWAIT 302
#define ELSE 303
#define IMPORT 304
#define PASS 305
#define NONE 306
#define BREAK 307
#define EXCEPT 308
#define IN 309
#define RAISE 310
#define TRUE 311
#define CLASS 312
#define FINALLY 313
#define IS 314
#define RETURN 315
#define AND 316
#define CONTINUE 317
#define FOR 318
#define LAMBDA 319
#define TRY 320
#define AS 321
#define DEF 322
#define FROM 323
#define NONLOCAL 324
#define WHILE 325
#define ASSERT 326
#define DEL 327
#define GLOBAL 328
#define NOT 329
#define WITH 330
#define ASYNC 331
#define ELIF 332
#define IF 333
#define OR 334
#define YIELD 335

#define ASTERISCO_ASTERISCO 336
#define SLASH_SLASH 337
#define MENOR_MENOR 338
#define MAYOR_MAYOR 339
#define DOS_PUNTOS_IGUAL 340
#define MENOR_IGUAL 341
#define MAYOR_IGUAL 342
#define IGUAL_IGUAL 343
#define DISTINTO 344
#define FLECHA 345
#define MAS_IGUAL 346
#define MENOS_IGUAL 347
#define POR_IGUAL 348
#define SLASH_IGUAL 349
#define SLASH_SLASH_IGUAL 350
#define PORCENTAJE_IGUAL 351
#define ARROBA_IGUAL 352
#define AMPERSAND_IGUAL 353
#define BARRA_VERTICAL_IGUAL 354
#define CIRCUNFLEJO_IGUAL 355
#define MAYOR_MAYOR_IGUAL 356
#define MENOR_MENOR_IGUAL 357
#define ASTERISCO_ASTERISCO_IGUAL 358

#define STRING_LITERAL 359
#define BYTE_LITERAL 360

#define INTEGER_LITERAL 361
#define FLOATING_POINT_LITERAL 362

#define COMENTARIO 363
#define ENCODING_DECLARATION 364

typedef struct {
    char *lexema;
    int compLex;
} componente;

static const componente palabrasReservadas[] = {
    {"False", FALSE},
    {"await", AWAIT},
    {"else", ELSE},
    {"import", IMPORT},
    {"pass", PASS},
    {"None", NONE},
    {"break", BREAK},
    {"except", EXCEPT},
    {"in", IN},
    {"raise", RAISE},
    {"True", TRUE},
    {"class", CLASS},
    {"finally", FINALLY},
    {"is", IS},
    {"return", RETURN},
    {"and", AND},
    {"continue", CONTINUE},
    {"for", FOR},
    {"lambda", LAMBDA},
    {"try", TRY},
    {"as", AS},
    {"def", DEF},
    {"from", FROM},
    {"nonlocal", NONLOCAL},
    {"while", WHILE},
    {"assert", ASSERT},
    {"del", DEL},
    {"global", GLOBAL},
    {"not", NOT},
    {"with", WITH},
    {"async", ASYNC},
    {"elif", ELIF},
    {"if", IF},
    {"or", OR},
    {"yield", YIELD}
};

#endif	/* DEFINICIONES_H */