---
title: "Codecs y Contenedores"
author: Juan Navarro
---



# Intro

Un **codec** es un formato de compresión de datos multimedia, tanto para audio como para video. Ejemplos de codecs de audio son `MP3`, `WAV`, `AAC`, `FLAC`, `OPUS`, etc. Ejemplos de codecs de video son `MPEG-2`, `Dirac`, `MPEG-4 Part 2` (*DivX*, *Xvid*), `H.264`, `VP8`, `H.265`, `VP9`, `AV1`.

Las fuentes de audio y video sin compresión (cámaras, renderizados) suelen ocupar en el orden de GB por segundo, por lo que estos flujos se comprimen en algún codec según las necesidades. El resultado ocupa menos espacio pero sigue siendo puramente información de audio o video, y carece de otros metadatos necesarios para su reproducción como duración total, frames por segundo, tipo de compresión, etc. Por ello tras la compresión, el resultado suele ser guardado en *contenedores multimedia*.

Un **contenedor** es un formato de archivo pensado para almacenar audio y video comprimidos, así como toda la información necesaria para reproducirlos, y en algunos casos muchos otros datos relevantes como información de capítulos, subtítulos, etc. Ejemplos de contenedores son `AVI`, `FLV`, `QT` (Apple QuickTime), `MP4`, `MKV` (*Matroska*).

El proceso de introducir uno o varios flujos comprimidos en un contenedor se llama *multiplexar* o *mux*. Más tarde un reproductor tendrá que extraer el audio o video, en el proceso contrario: *demultiplexar* o *demux*. Así, veremos en librerías como GStreamer que existen módulos llamados *matroskamux* y *matroskademux*, por ejemplo.



# Características del vídeo

Un vídeo es una secuencia de **frames**: imágenes cuya intención es ser enviadas a un visualizador, o almacenadas en un archivo. El vídeo en sí se compone de varios tipos de información, tanto por parte de su propia naturaleza (ej. duración, frames por segundo) como por parte del codec que ha sido usado para comprimirlo. En general los parámetros más importantes son:

* **Resolución**. Tamaño en píxeles, ancho x alto.

* **Frames por segundo** (*FPS*). Número de veces por segundo que la imagen se actualiza. Esto *no equivale* a que haya ese mismo número de imágenes completas por cada segundo, como veremos más tarde con los *tipos de frames (I, P, B)*.

Una vez el video se comprime con un codec, hay muchos parámetros que pueden cambiar el resultado obtenido. Cada codec tiene los suyos, pero hay algunos que son comunes y están presentes en todos:

* **Bitrate**. Cantidad de bits por segundo que el codec intenta obtener en el video resultante. Si el bitrate es muy bajo, obtendremos un vídeo de pésima calidad ya que el codec deberá eliminar todo tipo de detalles para lograr el objetivo; por otro lado si el bitrate es muy alto, todos los detalles del video original podrán ser mantenidos, a costa de que el archivo resultante ocupe más espacio.

* **Tipos de frames**.

* **Timestamps**.



## Tipos de frames

Un video sin compresión, obtenido de alguna fuente original (como una cámara), estará compuesto de varias imágenes completas que se van visualizando sucesivamente a una determinada velocidad (impuesta por el valor de *frames por segundo*). En el momento de comprimir un video así, el codec transformará estas imagenes en 3 tipos distintos:

* **Frames I** (*Intra*, *Keyframe*). Equivalentes a los frames originales, estos contienen una imagen completa, es decir un fotograma completo del vídeo. Por eso este tipo de frame es el que más tamaño ocupa.

    > Si hubiese que mostrar un *frame I* en pantalla, este podría ser descomprimido y mostrado de forma inmediata, ya que contiene toda la información necesaria para mostrar una imagen completa.

* **Frames P** (*Predicted*). Contienen solo las partes que hayan cambiado desde el frame anterior. Dicho de otra manera, en un *frame P* se eliminan todas las partes de la imagen que puedan ser reaprovechadas de frames anteriores. Por ejemplo, en una escena con fondo estático donde un personaje se mueve, el fondo se elimina de los frames *P* ya que esa parte de la imagen no cambia de un frame a otro.

    > Para mostrar un *frame P* en pantalla, habría que decodificar desde el último *frame I* hasta el *frame P* deseado, ya que los *frames P* solo contienen cambios acumulativos y por tanto no pueden ser decodificados de forma independiente.

* **Frames B** (*Bidirectional*). Basados en los *P*, llevan un paso más allá el concepto de descartar partes innecesarias de la imagen. Estos frames tienen en cuenta las diferencias tanto en frames *anteriores* como en los cambios que van a ocurrir en frames *futuros*. De esa manera consiguen eliminar la mayor cantidad de información posible, y así ser el tipo de frames que menos tamaño ocupan.

    > Mostrar un *frame B* en pantalla es el proceso más costoso de todos, ya que se deben examinar todos los frames acumulativos tanto antes como después del frame deseado.

Una proporción excesiva de *frames B* tiene dos consecuencias: tamaño mucho menor de archivo (dado que cuantos más *frames B*, mayor compresión), pero una búsqueda más costosa (porque para saltar a un punto arbitrario del vídeo se debe procesar mucha más información).



## Timestamps

Cada frame contiene información sobre el momento en que su decodificación y su presentación en pantalla debe ocurrir, para que la sincronización con el audio se mantenga perfecta. Para esto, hay dos tipos de marcas de tiempo:

* **Presentation Time Stamp** (*PTS*). Este dato marca el instante de tiempo en el que el frame debería ser mostrado en pantalla. Así, el reproductor de vídeo más básico podría escribirse con este código:

    ```c
    decode_frame(packet, &frame);
    sleep(frame.pts - now);
    display_frame(&frame);  // immediate
    now = frame.pts;
    ```

* **Decoding Time Stamp** (*DTS*). Para codecs que usan *frames B*, el valor de DTS indica el instante en el que un frame determinado debería ser decodificado. Este momento es diferente al PTS ya que los *frames B* deben reordenarse para ser decodificados después de los demás frames de los que dependen.

Por ejemplo, dado un stream de video con *frames I*, *P* y *B*:

```
Stream: I B B P
   PTS: 1 2 3 4
   DTS: 1 3 4 2
```

El tiempo de presentación es el dado por el orden de los frames, sin embargo para decodificar ambos *frames B* necesitamos haber procesado antes tanto el *frame I* como el *P*, por lo que el orden de decodificación no coindice con el de presentación.

Además, los frames pueden haber sido grabados en el archivo en cualquier otro orden por motivos de optimización, por lo que el PTS no necesariamente va a estar siempre en orden ascendente.



# Lecturas

* Tutoriales de Leandro Moreira:

    - [Digital Video Introduction / Frame types](https://github.com/leandromoreira/digital_video_introduction#frame-types)
    - [Digital Video Introduction / Video codecs](https://github.com/leandromoreira/digital_video_introduction#how-does-a-video-codec-work)
    - [FFmpeg libav tutorial](https://github.com/leandromoreira/ffmpeg-libav-tutorial)

* [FFmpeg Tutorial 05: Synching Video](http://dranger.com/ffmpeg/tutorial05.html)
