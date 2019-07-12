---
title: "GLib y GObject"
author: Juan Navarro
---



# Intro

En este curso no estudiamos uno a uno todos los aspectos de GLib y GObject; la mejor manera de familiarizarse con estos componentes es echar un vistazo a la documentación de referencia y buscar algún tutorial, como el de IBM (ver sección de lecturas).



# GLib

**GLib** es la librería base del proyecto GNOME. Provee al lenguaje C de muchas herramientas como estructuras de datos (listas enlazadas, árboles binarios, tablas hash) y utilidades como manipulación de strings.

En el siguiente ejemplo mostramos el uso de dos estructuras de datos implementadas en GLib: [GList](https://developer.gnome.org/glib/stable/glib-Doubly-Linked-Lists.html) (una lista doblemente enlazada), y [GHashTable](https://developer.gnome.org/glib/stable/glib-Hash-Tables.html) (una tabla asociativa).

```c
<#include src/glib-intro/main.c>
```

```sh
$ cd glib-intro
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
$ ./glib-intro
<#exec cd src/glib-intro && (cmake -H. -B. -DCMAKE_BUILD_TYPE=Debug && make)\>/dev/null && ./glib-intro>
```

Dado que estamos trabajando en C, donde no hay gestión automática de la memoria, todas las estructuras deben ser explícitamente creadas y destruidas. La convención general es usar funciones `*_new()` para crear nuevas instancias, y `*_destroy()`, `*_free()`, o `*_unref()` para liberar su memoria.

Un detalle muy importante es que, por lo general, las estructuras de datos **NO** son dueñas de los datos que contienen, es decir que al destruir el contenedor no se libera al memoria de cada elemento contenido.



## Ejercicio: g_list_free_full()

Mostrar lo que ocurre si en `test_list_malloc()` liberamos la lista de la forma simple:

```c
g_list_free(list);
```

y luego ejecutamos el programa con Valgrind para descubrir memoria perdida:

```sh
valgrind --leak-check=full ./glib-intro
```

Comparar esto con el resultado de usar la función adecuada para liberar la memoria de cada elemento:

```c
g_list_free_full(list, g_free);
```



## Ejercicio: GHashTable

Escribir una función que, dado un string arbitrario, sea capaz de contar cuántas veces aparece cada carácter alfanumérico (es decir, solo letras y números).

Solución en `glib-ex-1/main.c`


# GObject

**GObject** es la parte de GLib que añade a C la posibilidad de programar con orientación a objetos. Además, incluye el tipo fundamental [GType](https://developer.gnome.org/gobject/stable/chapter-gtype.html), en base al cual todo el sistema de tipos de GLib es construido. Las clases basadas en `GObject` tienen estas capacidades:

* Gestión de memoria basada en *reference counting*: [Object memory management](https://developer.gnome.org/gobject/stable/gobject-memory.html).
* Construcción y destrucción de instancias.
* Propiedades genéricas por instancia, con funciones get/set: [Object properties](https://developer.gnome.org/gobject/stable/gobject-properties.html).
* Señales asíncronas emitidas entre instancias: [Signals](https://developer.gnome.org/gobject/stable/signal.html).

Dado que C es un lenguaje puramente imperativo y no dispone de soporte para orientación a objetos, todo el sistema de clases de GObject requiere el uso intensivo de macros y conlleva un uso considerable de *boilerplate*.



# Lecturas

- GLib:

    - [IBM | Manage C data using the GLib collections](https://developer.ibm.com/tutorials/l-glib/)

        ¡CUIDADO! Tiene muchos ejemplos y es una buena guía, pero el texto está codificado como UTF8 y muchos símbolos (como `->`, `*`, etc.) no son ASCII: no se pueden copiar y pegar directamente, porque provocarán errores al compilar.

    - [GLib Reference Manual](https://developer.gnome.org/glib/stable/)

- GObject:
    - [GObject Doc | Part 1. Concepts](https://developer.gnome.org/gobject/stable/pt01.html)
    - [GObject Doc | Part 4. Tutorial](https://developer.gnome.org/gobject/stable/pt02.html)
    - [Subclassing GObject](https://developer.gnome.org/SubclassGObject/)
    - [GObject Reference Manual](https://developer.gnome.org/gobject/stable/)
    - [GNOME Platform Demos in C](https://developer.gnome.org/gnome-devel-demos/stable/c.html.en)