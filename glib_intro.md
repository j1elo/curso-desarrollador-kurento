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
<#include glib_intro/main.c>
```

```sh
$ cd glib_intro
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=Debug ..
$ make
$ ./glib_intro
<#exec cd src/glib_intro && (cmake -H. -B. -DCMAKE_BUILD_TYPE=Debug && make)\>/dev/null && ./glib_intro>
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
valgrind --leak-check=full ./glib_intro
```

Comparar esto con el resultado de usar la función adecuada para liberar la memoria de cada elemento:

```c
g_list_free_full(list, g_free);
```



## Ejercicio: GHashTable

Escribir una función que, dado un string arbitrario, sea capaz de contar cuántas veces aparece cara carácter alfanumérico (es decir, solo letras y números).

Solución en `glib_ex_1/main.c`


# GObject

**GObject** es la parte de GLib que añade a C la posibilidad de programar con orientación a objetos. Además, incluye el tipo fundamental `GType`, en base al cual todo el sistema de tipos de GLib es construido. Las clases basadas en `GObject` tienen capacidad de introspección, y son capaces de generar señales asíncronas entre objetos.

Dado que C es un lenguaje puramente imperativo y no dispone de soporte para orientación a objetos, todo el sistema de clases de GObject requiere el uso intensivo de macros y conlleva un uso considerable de *boilerplate*.



# Lecturas

- [IBM | Manage C data using the GLib collections](https://developer.ibm.com/tutorials/l-glib/)

    ¡CUIDADO! Tiene muchos ejemplos y es una buena guía, pero el texto está codificado como UTF8 y muchos símbolos (como `->`, `*`, etc.) no son ASCII: no se pueden copiar y pegar porque provocarán errores al compilar.

- [GLib Reference Manual](https://developer.gnome.org/glib/stable/)
- [GObject Reference Manual](https://developer.gnome.org/gobject/stable/)
- [Subclassing GObject](https://developer.gnome.org/SubclassGObject/)
- [GNOME Platform Demos](https://developer.gnome.org/gnome-devel-demos/stable/c.html.en)


GString
https://developer.gnome.org/glib/stable/glib-Strings.html

GObject Doc | Part 1. Concepts
https://developer.gnome.org/gobject/stable/pt01.html

GObject Doc | Part 4. Tutorial
https://developer.gnome.org/gobject/stable/pt02.html

GNOME Platform Demos in C
https://developer.gnome.org/gnome-devel-demos/stable/c.html.en
