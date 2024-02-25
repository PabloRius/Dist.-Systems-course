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

`````c
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
int set_value(int key, char *value1, int *N_value2, double *V_value2)
```
Devuelve la tupla asociada a la clave key en los punteros pasados como argumentos.
Los punteros deben tener reservado espacio suficiente para almacenar el tamaño máximo posible de los elementos.
Devuelve 0 en caso de éxito y -1 en caso de error.
Se considera error:
- La clave no existe.
- Error en la comunicación.

### MODIFY VALUE

````c
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

## Autor

- [@PabloRius](https://github.com/PabloRius)
`````
