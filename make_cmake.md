# Herramientas de construcción: Make y CMake

## Intro

En esta sección usaremos el siguiente programa de pruebas:

```
cat <<EOF >demo_calc.h
int calc(int a, int b);
EOF

cat <<EOF >demo_calc.c
#include <math.h>

int calc(int a, int b)
{
    return a + sqrt(b);
}
EOF

cat <<EOF >demo_main.c
#include "demo_calc.h"
#include <stdio.h>

int main(void)
{
    int result = calc(3, 25);
    printf("Result: %d\n", result);

    return 0;
}
EOF
```

Y usaremos las siguientes herramientas:

```
sudo apt-get install --yes make cmake
```



## Make

Tener que ejecutar GCC archivo por archivo es pasable en proyectos de juguete, pero cuando la cantidad de archivos crece empieza a ser difícil de manejar. Para ayudar con esto existe [Make](https://www.gnu.org/software/make/), un intérprete que lee archivos *Makefile* con un lenguaje específico para expresar objetivos, dependencias, y acciones a tomar para cada tarea.

En el fondo lo que expresamos con la sintaxis de los archivos *Makefile* es un grafo acíclico de dependencias (*DAG*), el cual se resuelve dinámicamente en cada ejecución, para resolver aquellas tareas que falten, saltando las que ya estén resueltas de ejecuciones anteriores.

Con un ejemplo lo veremos más claro. Ejecuta:

```
cat >demo_Makefile
```

y pega esto:

```
all: demo

run: demo
	LD_LIBRARY_PATH=. ./demo

demo: demo_main.c libdemo_calc.so
	gcc -o demo demo_main.c -L. -ldemo_calc

libdemo_calc.so: demo_calc.c
	gcc -shared -fPIC -o libdemo_calc.so demo_calc.c -lm

clean:
	rm -f demo libdemo_calc.so
```

Después, presiona `Ctrl+D`. Es necesario pegar el texto así porque de otra manera la terminal eliminaría los tabuladores.

Cada línea de un *Makefile* expresa un objetivo ("*target*") y los otros objetivos de los que este depende. Para cumplir los objetivos dependientes se ejecutarán las líneas que se listan debajo, que **deben ir TABULADAS**.

Cuando el objetivo coincide con el nombre de un archivo existente, este se considera cumplido si no ha habido cambios en el archivo desde la última ejecución.

Por ejemplo:

```
make -f demo_Makefile run
```

al ejecutar esto, `make` realiza los siguientes pasos:

1. Busca un archivo `demo`. Como no existe, busca el objetivo `demo` en el `Makefile` y lo intenta cumplir.
2. Para `demo` se comprueba si existen `demo_main.c` y `libdemo_calc.so`. El segundo no existe así que el proceso se repite recursivamente con el objetivo del mismo nombre.
3. `libdemo_calc.so` solo depende de `demo_calc.c`, que ya existe, por lo que se ejecuta el comando GCC correspondiente.
4. La recursión de objetivos se va resolviendo de nuevo hacia arriba, hasta que la dependencia de `run` ha sido satisfecha y por fin se pueden ejecutar las reglas de este, que es básicamente ejecutar la aplicación.

El target `all` es el que se ejecuta por defecto cuando se llama a `make` sin especificar ningún parámetro, simplemente porque es el primero que es definido en el archivo. Además, si nuestro archivo se llamase simplemente "*Makefile*", nos podríamos ahorrar el argumento `-f`.

*Make* y su *Makefile* no solo sirven para compilar proyectos de C o C++; también pueden ser una herramienta muy potente para facilitar muchas otras tareas de administración o desarrollo, ya que su sistema basado en un árbol de objetivos permite expresar fácilmente muchos tipos de trabajos, desde preparar un entorno con máquinas Docker a asegurarse de que todos los pre-requisitos para algún proceso han sido descargados con `scp` de algún servidor remoto. Las reglas de un *Makefile* pueden contener cualquier comando de consola!

Lecturas:

- [How To Use Makefiles to Automate Repetitive Tasks on an Ubuntu VPS](https://www.digitalocean.com/community/tutorials/how-to-use-makefiles-to-automate-repetitive-tasks-on-an-ubuntu-vps)
- [Makefile Tutorial by Example](http://makefiletutorial.com/)



## CMake

En algún momento alguien decidió que la sintaxis de Make es demasiado complicada, y que demasiados proyectos acababan teniendo unos *Makefile* monstruosos y difíciles de comprender. Por ello la [solución obvia](https://xkcd.com/927/) era crear una alternativa que fuese más legible y permitiese configurar proyectos a mayor escala. Ahí es cuando nace CMake, posiblemente el gestor de compilación con peor sintaxis de la historia (con permiso de *autotools*).

CMake es un *generador de Makefiles*; su entrada son archivos `CMakeLists.txt` y su salida son archivos `Makefile`. Por ello la manera más común de compilar un proyecto basado en CMake es primero ejecutar `cmake` seguido inmediatamente por `make`.

Es cierto que para proyectos pequeños ofrece una sintaxis bastante más sencilla, y además su configuración por defecto ya se encarga de muchos detalles a la hora de lidiar con el compilador, como por ejemplo añadir `-shared -fPIC` al compilar librerías.

Este sería el archivo *CMakeLists* que proporciona un resultado similar al *Makefile* del ejemplo anterior:

```
cat <<'EOF' >CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(demo VERSION 1.0.0 LANGUAGES C)

add_library(demo_calc SHARED demo_calc.c)
target_link_libraries(demo_calc m)

add_executable(demo demo_main.c)
target_link_libraries(demo demo_calc)

add_custom_target(run COMMAND demo)

install(TARGETS demo_calc demo DESTINATION ${CMAKE_SOURCE_DIR})
EOF
```

CMake tiene opiniones fuertes sobre cómo deberías hacer ciertas cosas, y por ejemplo no permite que el archivo de proyecto tenga otro nombre que no sea `CMakeLists.txt`.

Los pasos de auto-generación que son ejecutados por CMake terminan creando una gran cantidad de archivos y directorios temporales, por lo que el *modus operandi* típico es crear un directorio `build`, y ejecutar CMake desde ahí:

```
mkdir build && pushd build

cmake ..
# -- The C compiler identification is GNU 5.4.0
# -- Check for working C compiler: /usr/bin/gcc
# -- Check for working C compiler: /usr/bin/gcc -- works
# -- Detecting C compiler ABI info
# -- Detecting C compiler ABI info - done
# -- Detecting C compile features
# -- Detecting C compile features - done
# -- Configuring done
# -- Generating done
# -- Build files have been written to: ~/tutorial/c/build

make
# Scanning dependencies of target demo_calc
# [ 25%] Building C object CMakeFiles/demo_calc.dir/demo_calc.c.o
# [ 50%] Linking C shared library libdemo_calc.so
# [ 50%] Built target demo_calc
# Scanning dependencies of target demo
# [ 75%] Building C object CMakeFiles/demo.dir/demo_main.c.o
# [100%] Linking C executable demo
# [100%] Built target demo

make install
# Install the project...
# -- Install configuration: ""
# -- Installing: /home/juan/work/kurento/3_FORMACION/c/libdemo_calc.so
# -- Installing: /home/juan/work/kurento/3_FORMACION/c/demo
# -- Set runtime path of "/home/juan/work/kurento/3_FORMACION/c/demo" to ""

popd

LD_LIBRARY_PATH=. ./demo
# Result: 8
```
