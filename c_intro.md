---
title: "Lidiando con C"
author: Juan Navarro
---



# Intro

C es un lenguaje de programación que sienta las bases de la mayoría de los lenguajes imperativos conocidos a día de hoy. Se escribe en archivos `.c` y `.h`, y se compila a código máquina para obtener archivos binarios que sean ejecutables.

El compilador de C funciona en dos pasos:

1. Convierte el código a instrucciones equivalentes en lenguaje Ensamblador ("*ASM*").

2. Convierte ese código ensamblador en objetos binarios; estos últimos pueden ser programas ejecutables (ej. archivos `.exe` en Windows) o librerías (ej. archivos `.dll` en Windows, `.so` en Linux).

Los dos principales compiladores son [GCC](https://gcc.gnu.org/) y [LLVM/Clang](https://clang.llvm.org/). En esta introducción usaremos GCC:

```sh
$ sudo apt-get install --yes gcc
```

El programa mínimo es una función `main()` que no recibe argumentos y devuelve un valor entero:

```c
<#include c_intro.c>
```

lo compilamos:

```sh
$ gcc -o c_intro c_intro.c
<#exec gcc -o c_intro src/c_intro.c>
```

y ejecutamos:

```sh
$ ./c_intro
<#exec ./c_intro>
```



# Tipo de datos

C soporta los tipos de datos y construcciones sintácticas más habituales que conocemos de otros lenguajes imperativos: `int`, `float`, `double`, `struct`, arrays, funciones, y uso de punteros.

Por otro lado, construcciones más avanzadas que han ido apareciendo en otros lenguajes, como tuplas, funciones lambda, o estructuras de datos avanzadas, no existen en C.

```c
<#include c_types.c>
```

```sh
$ gcc -o c_types c_types.c
<#exec gcc -o c_types src/c_types.c>
```

```sh
$ ./c_types
<#exec ./c_types>
```

En las llamadas a `printf()`, `%d` es un conversor que procesa los argumentos de la función y los convierte a texto; este formato es bien conocido y existe en muchos otros lenguajes. La lista completa de conversiones se puede consultar en cualquier referencia de `printf()`, por ejemplo [en cplusplus.com](http://www.cplusplus.com/reference/cstdio/printf/).



## Arrays y punteros

Los arrays y los punteros son en el fondo lo mismo, y al no existir una diferencia clara entre ambos es fácil caer en la confusión a la hora de trabajar con ellos en C.

La clave es pensar en que realmente C solo sabe manejar áreas de memoria, y los conceptos de "puntero" o "array" no son más que diferentes maneras de ver (y dar nombre) a lo mismo.

En el ejemplo anterior vimos `char* letters = "abcdefg"`. El significado de esto es que un puntero a tipo de datos `char` está apuntando a un área de memoria que contiene las letras desde la *a* hasta la *g*. En este ejemplo vemos como esto es lo mismo que un array:

```c
<#include c_types_array.c>
```

```sh
$ gcc -o c_types_array c_types_array.c
<#exec gcc -o c_types_array src/c_types_array.c>
```

```sh
$ ./c_types_array
<#exec ./c_types_array>
```

Las cadenas de texto se manejan de forma dinámica en forma de punteros a arrays de caracteres. El manejo de strings en C es bastante engorroso ya que se debe hacer todo de forma manual con funciones como [strlen()](http://www.cplusplus.com/reference/cstring/strlen/) y [strcpy()](http://www.cplusplus.com/reference/cstring/strcpy/), o usar una librería de terceros que provea de operaciones más avanzadas.

El acceso directo a memoria con punteros es una herramienta muy potente pero también peligrosa, ya que cualquier uso incorrecto que hagamos de un puntero desembocará en corrupción de memoria y el Kernel abortando nuestro programa.



# Paso de argumentos

En la llamada a una función, se puede pasar cada argumento por valor (copia) o puntero (acceso directo a memoria). En el primer caso, una copia completa de la variable es creada en el punto de llamada, por lo que la función no podrá afectar al valor original:

```c
<#include c_args_value.c>
```

```sh
$ gcc -o c_args_value c_args_value.c
<#exec gcc -o c_args_value src/c_args_value.c>
```

```sh
$ ./c_args_value
<#exec ./c_args_value>
```

En el caso de paso por puntero, la función tiene acceso al área de memoria donde está almacenado el valor de la variable, por lo que es posible cambiarlo:

```c
<#include c_args_pointer.c>
```

```sh
$ gcc -o c_args_pointer c_args_pointer.c
<#exec gcc -o c_args_pointer src/c_args_pointer.c>
```

```sh
$ ./c_args_pointer
<#exec ./c_args_pointer>
```



## Argumentos de programa

Nuestro programa recibe parámetros de ejecución a través de dos argumentos especiales que pueden ser declarados en la función `main()`:

- `int argc`: Número de argumentos presentes en la siguiente variable.
- `char* argv[]`: Array de strings con el valor de cada argumento. El primero siempre es el comando con el que se ejecutó el programa.

Así podemos pasar parámetros a la ejecución de nuestro programa:

```c
<#include c_args_main.c>
```

```sh
$ gcc -o c_args_main c_args_main.c
<#exec gcc -o c_args_main src/c_args_main.c>
```

```sh
$ ./c_args_main Param1 Param2 --Option1 --Option2
<#exec ./c_args_main Param1 Param2 --Option1 --Option2>
```

Para parsear estos argumentos y así poder procesarlos de forma estructurada, existen funciones como [getopt()](https://manpages.ubuntu.com/manpages/bionic/en/man3/getopt.3.html).



# Modularización del código

La forma de no crear un único archivo `.c` enorme con todo el código de nuestra aplicación es separarlo en distintos archivos. Sin embargo, cada archivo `.c` forma lo que se conoce como *unidad de compilación*; estas unidades son independientes y no pueden acceder a las funciones o variables que existan en otros archivos:

```c
<#include c_include1_calc.c>
```

```c
<#include c_include1_main.c>
```

En este ejemplo, los contenidos de `calc.c` y `main.c` son independientes, aun si se compilan en el mismo binario:

```sh
$ gcc -o c_include1 c_include1_main.c c_include1_calc.c
<#exec gcc -o c_include1 src/c_include1_main.c src/c_include1_calc.c 2\>&1>
```

```sh
$ ./c_include1
<#exec ./c_include1>
```

Cuando GCC compila el archivo `main.c`, la función `main()` accede a una función `calc()` que no existe en la unidad de compilación (porque esa función está definida en una unidad distinta), y por tanto avisa de que se está usando una función desconocida (en ese instante).

Más tarde, al crear el binario final, resulta que esa necesidad de una función `calc()` acaba siendo cumplida, pero en la fase de compilación no era posible saber si eso iba a ocurrir o no, y de ahí el aviso.

Para permitir compartir código entre unidades, y que el compilador sepa en todo momento de dónde vienen las funciones a usar, debemos escribir la *declaración* (tipo de datos y nombre de variables y funciones, sin el código que las implementa) de este código en un archivo de cabecera, `.h`, mientras que la *definición* (código que implementa las funciones) seguirá en un archivo `.c`:

```c
<#include c_include2_calc.h>
```

```c
<#include c_include2_calc.c>
```

```c
<#include c_include2_main.c>
```

Cuando ponemos la declaración de una variable o función en un archivo de cabecera `.h`, le estamos diciendo al compilador algo así como "*fíate de mí cuando encuentres esto, la definición vendrá en otro archivo*", y por eso el aviso no ocurre en este caso:

```sh
$ gcc -o c_include2 c_include2_main.c c_include2_calc.c
<#exec gcc -Isrc -o c_include2 src/c_include2_main.c src/c_include2_calc.c>
```

```sh
$ ./c_include2
<#exec ./c_include2>
```



## Librerías externas: `#include <header.h>`

La librería estándar de C (*stdlib*) provee de funciones para manipulación de memoria a bajo nivel (buffers de datos y cadenas de texto), matemáticas, hilos, etc.

"Importamos" las funciones de cualquier librería externa en dos pasos:

1. En el código, usamos `#include` para indicar que usaremos la cabeceras correspondiente, y haremos uso de las funciones externas que son importadas por esa cabecera:

```c
<#include c_libm.c>
```

2. Al compilar, indicamos la librería en la que las funciones externas están compiladas. La función `sqrt()` se encuentra en la librería estándar *math*, que en Ubuntu se instala como parte del paquete *libc6-dev*:

```sh
$ sudo apt-get install --yes libc6-dev
```

```sh
$ dpkg -L libc6-dev | grep -e /math.h -e /libm.so
/usr/lib/x86_64-linux-gnu/libm.so
/usr/include/math.h
```

   Cuando se indica una librería al compilar, esta debe estar disponible tanto en el momento de la compilación y enlazado (*link*) como en el momento de la ejecución. Por tanto, la librería debe estar instalada tanto en la máquina de desarrollo como en la que se ejecute el programa.

   Si intentamos compilar sin la librería *math*, nos encontramos con que la función `sqrt()` no ha sido definida en ninguna parte:

```sh
$ gcc -o c_libm c_libm.c
<#exec gcc -o c_libm src/c_libm.c 2\>&1>
```

   Indicando al compilador el uso de esta librería, todo compila bien:

```sh
$ gcc -o c_libm c_libm.c -lm
<#exec gcc -o c_libm src/c_libm.c -lm>
```

   Para obtener el nombre `lm` seguimos este proceso:

   - Del nombre del archivo, sustituimos "*lib*" por "*l*".
   - Quitamos el sufijo "*.so*".

   Dado que el archivo de la librería es "`libm.so`", el resultado es "*lm*" y se lo pasamos así al compilador como argumento.

   Por último, la función `printf()` también requiere el mismo proceso, pero al formar parte de la colección estándar de GCC, esta función no requiere enlazar expresamente con ninguna librería.

   Ejecutamos para comprobar el resultado:

```sh
$ ./c_libm
<#exec ./c_libm>
```



## Librerías locales: `#include "header.h"`

Podemos crear nuestras propias librerías, lo cual es buena idea para no crear un bloque monolítico y poder separar funciones en grupos que tengan sentido y ayuden a administrar el código de una aplicación.

Por ejemplo, podemos crear una librería con los contenidos de la función `calc()`:

```c
<#include c_lib_calc.h>
```

```c
<#include c_lib_calc.c>
```

```c
<#include c_lib_main.c>
```

Para compilar debemos tener en cuenta que, por defecto, **las rutas de búsqueda de librerías se limitan a las rutas del sistema**. Nuestra librería es local y por tanto reside en el directorio actual; deberemos añadir la ruta actual a la búsqueda de librerías, tanto en el momento de compilar:

```sh
$ gcc -shared -fPIC -o libcalc.so c_lib_calc.c -lm
<#exec gcc -shared -fPIC -o libcalc.so src/c_lib_calc.c -lm>
$ gcc -o c_lib_calc c_lib_main.c -L. -lcalc
<#exec gcc -o c_lib_calc src/c_lib_main.c -L. -lcalc>
```

como en el de ejecutar:

```sh
$ LD_LIBRARY_PATH=. ./c_lib_calc
<#exec LD_LIBRARY_PATH=. ./c_lib_calc>
```

* `-shared` le indica a GCC que estamos compilando una librería y no una aplicación ejecutable final.
* `-fPIC` es una opción de seguridad que en principio no necesitamos pero en Ubuntu es necesaria para poder usar otras librerías del sistema ("*libm.so*" en nuestro caso).
* `-L.` añade el directorio actual (`.`) a la ruta de búsqueda para que GCC encuentre la librería "*libdemo_calc.so*" al compilar nuestro binario.
* `LD_LIBRARY_PATH=.` añade el directorio actual (`.`) a la ruta de búsqueda para que Linux encuentre la librería "*libdemo_calc.so*" al ejecutar nuestro binario.

En este ejemplo, un usuario de la nueva librería "*demo_calc*" tendría a su disposición dos archivos: la cabecera (`.h`) indicando qué funciones hay disponibles en la librería, y el binario (`.so`), conteniendo la implementación de la misma.



## Librerías en runtime: `dlopen`

Existe un tercer método de cargar código externo en nuestro programa, sin necesidad de incluirlo explícitamente durante las fases de compilado y en el momento de ejecución; la llamada de sistema `dlopen` permite abrir una librería `.so` "al vuelo", y cargar las funciones que esta contenga, para su uso.

Este método es mucho más engorroso de usar que los anteriores, pero tiene la gran ventaja de no requerir información sobre la librería en el momento en que se compila nuestro programa. Piensa, por ejemplo, en sistemas de plugins dinámicos: una aplicación de dibujo podría cargar un directorio de plugins en el que cada librería contiene un tipo diferente de pincel; puede haber más o menos pinceles a cargar, según los que el usuario haya instalado, pero en cualquier caso esto no es una información que el creador del programa tenga en el momento en que compila el código antes de distribuír sus binarios.

Este método es justamente el que usa GStreamer para cargar opcionalmente los plugins que añaden soporte para todo tipo de codecs, contenedores, formatos, etc.

```c
<#include c_dlopen_calc.c>
```

```c
<#include c_dlopen_main.c>
```

```sh
$ gcc -shared -o libdlopen_calc.so c_dlopen_calc.c
<#exec gcc -shared -o libdlopen_calc.so src/c_dlopen_calc.c>
$ gcc -o c_dlopen c_dlopen_main.c -ldl
<#exec gcc -o c_dlopen src/c_dlopen_main.c -ldl>
```

```sh
$ ./c_dlopen
<#exec ./c_dlopen>
```

```sh
$ rm libdlopen_calc.so
<#exec rm libdlopen_calc.so>
$ ./c_dlopen
<#exec ./c_dlopen>
```

En este programa de ejemplo, el nombre de la función "calc" es conocido; sin embargo podríamos usar mecanismos más avanzados que nos permitiesen cargar funciones sin saber siquiera su nombre (haciendo un análisis inicial de todos los símbolos y cargando los que cumplan con determinadas condiciones).

En este momento, nuestro programa "demo" intentará cargar opcionalmente una librería externa para usar la función `calc()`, y hará otra cosa en caso de error. Esto significa que la librería podría estar instalada o no en el sistema, y nuestro programa funcionará correctamente haciendo algo distinto en cada caso.



# Opciones del compilador

GCC acepta muchas opciones que pueden alterar el resultado de la compilación; las más utilizadas son:

* `-Wall`: Activa avisos para todas las construcciones sintácticas que podrían inducir a error, aun no siendo errores. Cosas como usar una variable sin inicializar, usar funciones que no han sido definidas, etc. En general, no es necesario pero sí recomendable compilar con `-Wall` y resolver todos los avisos, ya que suelen ser indicativos de que se está haciendo algo raro en el código y por norma general no somos más inteligentes que el propio compilador.

* `-Werror`: Esta opción convierte todos los avisos en errores de compilación. Es por tanto una manera muy eficaz de asegurar que ningún aviso se queda sin solucionar durante el desarrollo.

* `-g`: Incluye símbolos de depuración en el binario resultante. Estos símbolos ocupan muchísimo espacio (algo así como 10 o 15 veces, si no más, el tamaño del binario propiamente dicho), pero permiten depurar el programa paso por paso (p.ej. con **GDB**) y obtener trazas con los nombres de archivo y números de línea.

* `-O<n>`: Activa las optimizaciones de rendimiento de las que el compilador es capaz. <n> es un número de 0 a 3; el nivel más habitual es el **2**, para compilar versiones de producción, ya que el nivel *3* suele activar algunas optimizaciones tan agresivas que pueden ser contraproducentes. Idealmente usaremos `-O0` para compilaciones de debug, de forma que las optimizaciones no afecten al depurador paso a paso.



# Herramientas

En C no hay magia: un binario no es más que una colección de instrucciones en las que se contiene el código que hemos programado, separado por funciones. Por otro lado, las dependencias de librerías son almacenadas en los propios binarios y pueden ser consultadas a posteriori (salvo en el caso del método `dlopen`).

Mostraremos un par en base a este programa:

```c
<#include c_tools.c>
```

```sh
$ gcc -g -o c_tools c_tools.c -lm
<#exec gcc -g -o c_tools src/c_tools.c -lm>
```



## ldd

Con `ldd` podemos consultar las librerías de las que un binario depende. Por ejemplo cuando compilamos nuestro programa con `-lm`, podemos comprobar que el archivo resultante muestra una dependencia explícita de la librería `libm.so`:

```sh
$ sudo apt-get install --yes libc-bin
```

```sh
$ ldd ./c_tools | grep libm.so
<#exec ldd ./c_tools \| grep libm.so>
```



## nm

Con `nm` podemos inspeccionar el contenido de cualquier archivo binario. Veremos un listado de todas las direcciones de memoria que existen dentro del binario, y entre otras cosas podremos ver cómo nuestras variables y las funciones están presentes, dejando así claro que el proceso de ejecución del programa no consiste más que en saltar de una dirección de memoria a otra:

```sh
$ sudo apt-get install --yes binutils
```

```sh
$ nm -n ./c_tools | grep -E 'main|calc|sqrt|start'
<#exec nm -n ./c_tools \| grep -E 'main\|calc\|sqrt\|start'>
```

```sh
$ ./c_tools
<#exec ./c_tools>
```



## GDB

Este es el depurador por excelencia en el mundo de C y C++. Está integrado en la mayoría de IDEs, y también tiene una interfaz de línea de comandos.

En este ejemplo, situamos un breakpoint justo a la entrada de la función `calc()`, de forma que podemos analizar el valor de sus argumentos, incluso modificarlos, y posteriormente continuar con la ejecución.

```sh
$ gdb ./c_tools

(gdb) break calc

    Breakpoint 1 at 0x400634: file src/c_tools.c, line 8.

(gdb) run

    Starting program: c_tools
    printf 1, address of calc(): 0x400626               <-- printf()

    Breakpoint 1, calc (a=3, b=25) at src/c_tools.c:8
    8        return a + sqrt(b);

(gdb) backtrace

    #0  calc (a=3, b=25) at src/c_tools.c:8
    #1  0x0000000000400686 in main () at src/c_tools.c:14

(gdb) info args

    a = 3
    b = 25

(gdb) set variable a = 100

(gdb) continue

    Continuing.
    printf 2, value from calc(): 105                    <-- printf()
    [Inferior 1 (process 19097) exited normally]

(gdb) quit
```

**NOTA**: Observar la diferencia de información incluida en el `backtrace` cuando compilamos el programa con símbolos de debug (`gcc -g`) y sin ellos.
