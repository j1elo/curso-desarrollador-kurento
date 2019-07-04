---
title: "Construcción de proyectos: Make y CMake"
author: Juan Navarro
---



# Intro

En esta sección usaremos el siguiente programa de pruebas:

```c
<#include make_demo_calc.h>
```

```c
<#include make_demo_calc.c>
```

```c
<#include make_demo_main.c>
```

Y usaremos las siguientes herramientas:

```sh
$ sudo apt-get install --yes make cmake
```



# Make

Tener que ejecutar GCC a mano para compilar archivo por archivo es viable en proyectos de juguete, pero cuando la cantidad de archivos crece empieza a ser difícil de manejar. Para ayudar con esto existe [Make](https://www.gnu.org/software/make/), un intérprete que lee archivos *Makefile* con un lenguaje específico para expresar objetivos, dependencias, y acciones a tomar para cada tarea.

En el fondo lo que expresamos con la sintaxis de los archivos *Makefile* es un grafo acíclico dirigido ([Directed Acyclic Graph](https://en.wikipedia.org/wiki/Directed_acyclic_graph), *DAG*), el cual se resuelve dinámicamente en cada ejecución, para resolver aquellas tareas que falten, saltando las que ya estén resueltas de ejecuciones anteriores.

Con un ejemplo lo veremos más claro. Usamos el siguiente *Makefile*:

```makefile
<#include make_demo_Makefile>
```

Cada línea de un *Makefile* expresa un objetivo ("*target*") y los otros objetivos de los que este depende. Para cumplir los objetivos dependientes se ejecutarán las líneas que se listan debajo, que **deben ir TABULADAS**.

Cuando el objetivo coincide con el nombre de un archivo existente, este se considera cumplido si no ha habido cambios en el archivo desde la última ejecución.

Por ejemplo:

```
make -f make_demo_Makefile run
<#exec make --always-make --directory ./src --no-print-directory -f make_demo_Makefile run>
```

al ejecutar esto, `make` realiza los siguientes pasos:

1. Busca un archivo `make_demo`. Como no existe, busca el objetivo `make_demo` en el `Makefile` y lo intenta cumplir.
2. Para `make_demo` se comprueba si existen `make_demo_main.c` y `libmake_calc.so`. El segundo no existe así que el proceso se repite recursivamente con el objetivo del mismo nombre.
3. `libmake_calc.so` solo depende de `make_demo_calc.c`, que ya existe, por lo que se ejecuta el comando GCC correspondiente.
4. La recursión de objetivos se va resolviendo de nuevo hacia arriba, hasta que la dependencia de `run` ha sido satisfecha y por fin se pueden ejecutar las reglas de este, que es básicamente ejecutar la aplicación.

El target `all` es el que se ejecuta por defecto cuando se llama a `make` sin especificar ningún parámetro, simplemente porque es el primero que es definido en el archivo. Por convención, todo Makefile suele tener un target `all` que sirve para compilar el programa cuando `make` se ejecuta sin argumentos. Además, si nuestro archivo se llamase simplemente "*Makefile*", nos podríamos ahorrar el argumento `-f`.

*Make* y su *Makefile* no solo sirven para compilar proyectos de C o C++; también pueden ser una herramienta muy potente para facilitar muchas otras tareas de administración o desarrollo, ya que su sistema basado en un árbol de objetivos permite expresar fácilmente muchos tipos de trabajos, desde preparar un entorno con máquinas Docker a asegurarse de que todos los pre-requisitos para algún proceso han sido descargados con `scp` de algún servidor remoto. Las reglas de un *Makefile* pueden contener cualquier comando de consola!

Lecturas:

- [How To Use Makefiles to Automate Repetitive Tasks on an Ubuntu VPS](https://www.digitalocean.com/community/tutorials/how-to-use-makefiles-to-automate-repetitive-tasks-on-an-ubuntu-vps)
- [Makefile Tutorial by Example](http://makefiletutorial.com/)



# CMake

En algún momento alguien decidió que la sintaxis de Make es demasiado complicada, y que demasiados proyectos acababan teniendo unos *Makefile* monstruosos y difíciles de comprender. Por ello la [solución obvia](https://xkcd.com/927/) era crear una alternativa que fuese más legible y permitiese configurar proyectos a mayor escala. Ahí es cuando nace CMake, posiblemente el gestor de compilación con peor sintaxis de la historia (con permiso de *autotools*).

CMake es un *generador de Makefiles*; su entrada son archivos `CMakeLists.txt` y su salida son archivos `Makefile`. Por ello la manera más común de compilar un proyecto basado en CMake es primero ejecutar `cmake` seguido inmediatamente por `make`.

Es cierto que para proyectos pequeños ofrece una sintaxis bastante más sencilla, y además su configuración por defecto ya se encarga de muchos detalles a la hora de lidiar con el compilador, como por ejemplo añadir `-shared -fPIC` al compilar librerías.

Este sería el archivo *CMakeLists* que proporciona un resultado similar al *Makefile* del ejemplo anterior:

```cmake
<#include make_demo_CMakeLists.txt>
```

CMake tiene opiniones fuertes sobre cómo se deben hacer ciertas cosas, y por ejemplo no permite que el archivo de proyecto tenga otro nombre que no sea `CMakeLists.txt`.

Los pasos de auto-generación que son ejecutados por CMake terminan creando una gran cantidad de archivos y directorios temporales, por lo que el *modus operandi* típico es crear un directorio `build`, y ejecutar CMake desde ahí:

```sh
$ mkdir build && pushd build

$ cmake ..
-- The C compiler identification is GNU 5.4.0
-- Check for working C compiler: /usr/bin/gcc
-- Check for working C compiler: /usr/bin/gcc -- works
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: build
```

```sh
$ make
Scanning dependencies of target make_calc
[ 25%] Building C object CMakeFiles/make_calc.dir/make_demo_calc.c.o
[ 50%] Linking C shared library libmake_calc.so
[ 50%] Built target make_calc
Scanning dependencies of target make_demo
[ 75%] Building C object CMakeFiles/make_demo.dir/make_demo_main.c.o
[100%] Linking C executable make_demo
[100%] Built target make_demo
```

```sh
$ make install
Install the project...
-- Install configuration: ""
-- Installing: make_demo
-- Set runtime path of "make_demo" to ""
-- Installing: libmake_calc.so
```

```sh
$ popd

$ LD_LIBRARY_PATH=. ./make_demo
Result: 8
```
