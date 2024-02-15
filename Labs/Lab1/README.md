# Laboratorio 1 - Introducción a C

El fichero Makefile será utilizado para la compilación de los programas de este laboratorio.
Para compilar el ejercicio basta con poner make {nombre del ejercicio que se quiere ejecutar(p1,p2,...)} en la linea de comandos, con make clean se eliminan los objetos y ejecutables generados.

## Ejercicio 1

Escribir un programa (p1.c) que acepte en la línea de mandatos un número indeterminado de argumentos. El programa debe imprimir cada argumento en una línea diferente como una línea de caracteres

```bash
./p1 uno 4 hola tres 1245
```

El programa debe imprimir

```bash
Argumento 1 = Uno
Argumento 2 = 4
Argumento 3 = hola
Argumento 4 = tres
Argumento 5 = 1245
```

## Ejercicio 2

Modificar el programa anterior (p2.c) (se asume que todos los inputs son números enteros). El programa debe convertir cada argumento de entrada (en formato cadena de caracteres) a su respectivo número entero.
Se puede usar la siguiente función de la biblioteca atoi:

```c
int atoi(const char *nptr);
```

Esta función convierte la cadena a int, pero no detecta errores (si el input no es entero) y en su lugar es mejor usar la función equivalente:

```c
char *end;
strtol(nptr, &end, 10);
```

En caso de error el puntero end no habrá llegado al final de la cadena, por tanto, con la siguiente comprobación se podría detectar el error:

```c
if(*end != '\0'){
  // error en la conversión
}
```

El programa debería funcionar de la misma forma que el anterior con los números enteros, y en caso de que se introdujese un input inválido como el siguiente:

```bash
./p2 8 10 tres 45
```

Debería mostrar el siguiente output:

```bash
Argumento 1 = 8
Argumento 2 = 10
Argumento 3 = Error de conversión
Argumento 4 = 45
```

## Ejercicio 3

Modificar el programa anterior (p3.c) de forma que los argumentos introducidos se transformen a enteros y se almacenen en un array creado de forma dinámica. Programar una función ObtenerMinMax(), que reciba el array y obtenga el valor mínimo y máximo del array.

## Ejercicio 4

Modificar el programa anterior (p4.c) de forma que los argumentos pasados se almacenen como cadenas de caracteres en un array de forma dinámica.

## Ejercicio 5

Modificar el programa anterior (p5.c) de forma que los argumentos pasados se almacenen en una lista enlazada. En cada nodo se almacenará el valor numérico y el mismo valor en forma de cadena de caracteres.

## Ejercicio 6

Modificar el programa del Ejercicio 3(p6.c) de forma que ordene de menor a mayor los elementos del array utilizando la función de biblioteca qsort().

## Autor

- [@PabloRius](https://github.com/PabloRius)
