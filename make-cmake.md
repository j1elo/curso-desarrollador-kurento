---
title: "Construcción de proyectos: Make y CMake"
author: Juan Navarro
---



# Intro

En esta sección usaremos el siguiente programa de pruebas:

```c
<#include src/make-intro/make-intro-calc.h>
```

```c
<#include src/make-intro/make-intro-calc.c>
```

```c
<#include src/make-intro/make-intro-main.c>
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
<#include src/make-intro/make-intro-Makefile>
```

Cada línea de un *Makefile* expresa un objetivo ("*target*") y los otros objetivos de los que este depende. Para cumplir los objetivos dependientes se ejecutarán las líneas que se listan debajo, que **deben ir TABULADAS**.

Cuando el objetivo coincide con el nombre de un archivo existente, este se considera cumplido si no ha habido cambios en el archivo desde la última ejecución.

Por ejemplo:

```sh
$ make -f make-intro-Makefile run
<#exec make --always-make --directory src/make-intro --no-print-directory -f make-intro-Makefile run>
```

al ejecutar esto, `make` realiza los siguientes pasos:

1. Busca un archivo `make-intro`. Como no existe, busca el objetivo `make-intro` en el `Makefile` y lo intenta cumplir.
2. Para `make-intro` se comprueba si existen `make-intro-main.c` y `libmake-calc.so`. El segundo no existe así que el proceso se repite recursivamente con el objetivo del mismo nombre.
3. `libmake-calc.so` solo depende de `make-intro-calc.c`, que ya existe, por lo que se ejecuta el comando GCC correspondiente.
4. La recursión de objetivos se va resolviendo de nuevo hacia arriba, hasta que la dependencia de `run` ha sido satisfecha y por fin se pueden ejecutar las reglas de este, que es básicamente ejecutar la aplicación.

El target `all` es el que se ejecuta por defecto cuando se llama a `make` sin especificar ningún parámetro, simplemente porque es el primero que es definido en el archivo. Por convención, todo Makefile suele tener un target `all` que sirve para compilar el programa cuando `make` se ejecuta sin argumentos. Además, si nuestro archivo se llamase simplemente "*Makefile*", nos podríamos ahorrar el argumento `-f`.

*Make* y su *Makefile* no solo sirven para compilar proyectos de C o C++; también pueden ser una herramienta muy potente para facilitar muchas otras tareas de administración o desarrollo, ya que su sistema basado en un árbol de objetivos permite expresar fácilmente muchos tipos de trabajos, desde preparar un entorno con máquinas Docker a asegurarse de que todos los pre-requisitos para algún proceso han sido descargados con `scp` de algún servidor remoto. Las reglas de un *Makefile* pueden contener cualquier comando de consola!



# CMake

El problema que suelen tener los archivos *Makefile* es que terminan siendo complejos y muy poco mantenibles cuando se usan para proyectos grandes. En algún momento alguien decidió que la [solución obvia](https://xkcd.com/927/) era crear una alternativa que fuese más legible y permitiese configurar proyectos a mayor escala. Ahí es cuando nace CMake, un sistema de compilación para proyectos C y C++, que podría equipararse a lo que es Maven para Java.

CMake es un **generador de Makefiles**; su entrada son archivos `CMakeLists.txt` y su salida son archivos `Makefile`. Por ello la manera más común de compilar un proyecto basado en CMake es primero ejecutar `cmake` seguido inmediatamente por `make`. En otras plataformas, CMake es capaz de generar archivos de proyecto `.vcxproj` para **Visual Studio** en Windows, o `.xcodeproj` para **Xcode** en Mac.

Es cierto que para proyectos pequeños, CMake ofrece una sintaxis bastante sencilla, y además su configuración por defecto ya se encarga de muchos detalles a la hora de lidiar con el compilador (como por ejemplo añadir `-shared -fPIC` al compilar librerías). Pero la sintaxis de CMake se basa principalmente en llamar a funciones que contaminen el entorno con variables creadas implícitamente, lo cual es un modo de operar que resulta bastante extraño para el programador, habituado a que los resultados de operar con funciones se reciban explícitamente en forma de variable o similar.

Este sería el archivo *CMakeLists* que proporciona un resultado similar al *Makefile* del ejemplo anterior:

```cmake
<#include src/make-intro/CMakeLists.txt>
```

CMake tiene opiniones fuertes sobre cómo se deben hacer ciertas cosas, y por ejemplo no permite que el archivo de proyecto tenga otro nombre que no sea `CMakeLists.txt`.

Los pasos de auto-generación que son ejecutados por CMake terminan creando una gran cantidad de archivos y directorios temporales, por lo que el *modus operandi* típico es crear un directorio `build`, y ejecutar CMake desde ahí. Esta forma de compilar se suele llamar **out-of-source build**:

```sh
$ cd make-intro
$ mkdir build && cd build
```

```sh
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
Scanning dependencies of target make-calc
[ 25%] Building C object CMakeFiles/make-calc.dir/make-intro-calc.c.o
[ 50%] Linking C shared library libmake-calc.so
[ 50%] Built target make-calc
Scanning dependencies of target make-intro
[ 75%] Building C object CMakeFiles/make-intro.dir/make-intro-main.c.o
[100%] Linking C executable make-intro
[100%] Built target make-intro
```

```sh
$ make install
Install the project...
-- Install configuration: ""
-- Installing: make-intro
-- Set runtime path of "make-intro" to ""
-- Installing: libmake-calc.so
```

```sh
$ make run
[ 50%] Built target make-calc
[100%] Built target make-intro
Scanning dependencies of target run
Result: 8
[100%] Built target run
```



## Ejemplo de proyecto con CMake

Escribiremos una aplicación que lea un objeto JSON, lo parsee e imprima el primer campo del mismo. Para manejar JSON desde C haremos uso de la la librería [cJSON](https://github.com/DaveGamble/cJSON), que se compila e instala usando CMake y Make:

```sh
$ git clone https://github.com/DaveGamble/cJSON.git && cd cJSON
$ mkdir build && cd build
$ cmake .. -DCMAKE_INSTALL_PREFIX=/opt/cJSON \
    -DENABLE_CJSON_TEST=OFF -DENABLE_CJSON_UTILS=OFF
$ make
$ sudo make install
```

Para indicar en nuestro proyecto que dependemos de esta librería, y en general para establecer cualquier dependencia, CMake tiene el comando [find_package()](https://cmake.org/cmake/help/latest/command/find_package.html). Este comando funciona de una forma similar a como lo hace el programa [pkg-config](https://manpages.ubuntu.com/manpages/bionic/en/man1/pkg-config.1.html).

Por ejemplo, dado el nombre `Foo` para la librería `libfoo.so`:

* `pkg-config` busca un archivo `Foo.pc`, y si lo encuentra imprime las opciones de compilador que son necesarias para usar la librería: `-lfoo`.

* `find_package()` puede funcionar de dos maneras:

    - En el modo "*Config*", CMake busca **en el sistema** un archivo `FooConfig.cmake`, y si lo encuentra carga su contenido en el proyecto actual. Esto suele traer consigo un *target* que puede ser usado en otros comandos de CMake para establecer dependencias. Este modo es usado normalmente cuando la dependencia provee en su instalación de los archivo *.cmake* necesarios para su uso.

    - En modo "*Module*", CMake busca **en el proyecto actual** un archivo `FindFoo.cmake`, que contenga la información de qué archivos y librerías componen la dependencia `Foo`. Este modo suele usarse cuando la dependencia no provee por sí misma de soporte de CMake, por lo que es el usuario de la dependencia (nosotros) quien tiene que ocuparse de escribir un archivo `Find*.cmake` que permita importarla.

En el caso de cJSON estamos en el primer caso: la configuración es instalada en `<prefix>/lib/cmake/cJSON/cJSONConfig.cmake`, por lo que podemos usar el modo *Config*. En nuestro ejemplo, `<prefix>` es `/opt/cJSON`, una ruta no estándar, por lo que debemos indicar a CMake en qué rutas buscar:

```cmake
<#include src/cmake-json/CMakeLists.txt>
```

```c
<#include src/cmake-json/main.c>
```

```sh
$ cd cmake-json
$ mkdir build && cd build
$ cmake ..
$ make
$ ./cmake-json
JSON object: '{ "name": "My Name", "Number": 112233 }'
JSON name: 'My Name'
```



# Lecturas

- Make:

    - [An Introduction to Makefiles](ftp://ftp.gnu.org/old-gnu/Manuals/make/html_chapter/make_2.html#SEC5)
    - [Makefile Tutorial by Example](http://makefiletutorial.com/)
    - [JavaScript bundling with Makefile](https://blog.jayway.com/2017/03/12/short-introduction-makefiles/)
    - [How To Use Makefiles to Automate Repetitive Tasks on an Ubuntu VPS](https://www.digitalocean.com/community/tutorials/how-to-use-makefiles-to-automate-repetitive-tasks-on-an-ubuntu-vps)

- CMake:

    - [An Introduction to Modern CMake](https://cliutils.gitlab.io/modern-cmake/)
    - [The Hitchhiker's Guide to CMake](https://cgold.readthedocs.io/en/latest/)
    - [StackOverflow | CMake Module mode vs. Config mode](https://stackoverflow.com/questions/20746936/what-use-is-find-package-if-you-need-to-specify-cmake-module-path-anyway/20857070#20857070)
    - [It's Time To Do CMake Right](https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/)

    Referencia oficial:

    - [CMake Build System](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html)
    - [CMake Commands](https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html)
    - [CMake Packages](https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html)
