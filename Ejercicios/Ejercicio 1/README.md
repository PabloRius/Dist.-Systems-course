# Ejercicio Evaluable 1 - Colas de mensajes

Se desea diseñar e implementar un servicio distribuido (de colas de mensajes POSIX). que permita almacenar un número de

```
tupla <clave-valor1-valor2>
```

La clave será un _int_ y los valores:

- Una cadena de, como máximo, 255 caracteres (excluyendo el \0 que indica el final de cadena)
- Un valor de N entre 1 y 32 y un vector de N elementos _double_.

La aplicación contará con:

- Un servidor que almacenará y gestionará las claves
- Un API que permitirá a los procesos cliente hacer uso de los servicios implementados

## Servicios a implementar

### INIT

```c
int init()
```

Permite inicializar el servicio de tuplas. Con la llamada a este servicio, se eliminarán todas las tuplas que estuviesen almacenadas anteriormente.
Devuelve 0 en caso de éxito y -1 en caso de error.

### SET VALUE

```c
int set_value(int key, char *value1, int N_value2, double *V_value2)
```

Inserta la tupla definida por los argumentos.
Devuelve 0 si se insertó y -1 en caso de error.
Se considera error:

- Intentar insertar una clave ya existente.
- N está fuera de rango.
- Error en la comunicación.

### GET VALUE

```c
int get_value(int key, char *value1, int *N_value2, double *V_value2)
```

Devuelve la tupla asociada a la clave key en los punteros pasados como argumentos.
Los punteros deben tener reservado espacio suficiente para almacenar el tamaño máximo posible de los elementos.
Devuelve 0 en caso de éxito y -1 en caso de error.
Se considera error:

- La clave no existe.
- Error en la comunicación.

### MODIFY VALUE

```c
int modify_value(int key, char *value1, int N_value2, double *V_value2)
```

Modifica los valores asociados a una clave key.
Devuelve 0 en caso de éxito y -1 en caso de error.
Se considera error:

- La clave no existe.
- N está fuera de rango.
- Error en la comunicación.

### DELETE KEY

```c
int delete_key(int key)
```

Borra la tupla con clave key.
Devuelve 0 en caso de éxito y -1 en caso de error.
Se considera error:

- La clave no existe.
- Error en la comunicación.

### EXIST

```c
int exist(int key)
```

Permite determinar si existe un elemento con clave key.
Devuelve 1 en caso de que exista, 0 en caso de que no exista y -1 en caso de error.
Se considera error:

- Error en la comunicación.

##

Todas las operaciones se deben realizar de forma atómica en el servidor.

Utilizando únicamente colas de mensajes POSIX y el lenguaje de programación C, diseñe e implemente un sistema para este servicio de tuplas.

1. Desarrollar el código de _servidor.c_, que se encargará de gestionar las estructuras de datos que almacenarán las tuplas, este servidor debe ser concurrente. (La estructura de datos ha de ser elegida teniendo en cuenta que esta no debe suponer una limitación en el número de elementos que se pueden almacenar).
2. Desarrollar el código que implementa los servicios anteriores _claves.c_. Este servicio ofrece la interfaz a los clientes contactando con el servidor. A partir de dicha implementación se creará una biblioteca dinámica libclaves.so, que será la que utilicen las aplicaciones de usuario para usar el servicio. (Trabajo de investigación por parte del alumno la creación de dicha biblioteca).
3. Desarrollar el código de un cliente ejemplo _cliente.c_ que utilice la biblioteca anterior, el ejecutable de este programa debe generarse utilizando la biblioteca dinámica anterior.
4. Elaborar un plan de pruebas del servicio desarrollado. Este plan se probará con el código del cliente anteriormente desarrollado.

##

Al desarrollar el ejercicio, es posible que las colas de mensajes se queden en un estado inconsistente y, tanto el cliente como el servidor, fallen en tiempo de ejecución.
Lo mejor para arreglar este error es borrar las colas de mensajes que se hayan quedado en el sistema, en _/dev/mqueue_.

## Material a entregar

Fichero _ejercicio_evaluable1.tar_:

- cliente.c
- servidor.c
- Makefile que permita compilar todos los archivos anteriores y generar la biblioteca dinámica libclaves.so. Debe generar un ejecutable para el servidor y otro para el cliente.
- Pequeña memoria en pdf (máx 5 páginas, incluyendo portada), se indicarán las decisiones del diseño realizado,la estructura de datos elegida y la forma de compilar y generar los ejecutables.
- Ficheros adicionales necesarios para el desarrollo del servicio.

## Fecha límite

17/03/2024 (23:55 horas)

## Autor

- [@PabloRius](https://github.com/PabloRius)
