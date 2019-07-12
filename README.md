# Curso Desarrollador Kurento

Este repo contiene todo el material usado para el curso.

Los archivos originales están escritos en formato Markdown, y antes de ser convertidos a HTML deben ser pre-procesados para inyectar el contenido correspondiente de cada ejemplo:

1. Pre-procesado.

    Usando [GPP](https://github.com/logological/gpp), se sustituyen todas las macros `<#include>` por los archivos correspondientes. Además, los ejemplos de código son compilados y ejecutados, para sustituir también las macros `<#exec>` por el resultado correspondiente a ejecutar cada ejemplo.

2. Conversión.

    Usando [Pandoc](https://github.com/jgm/pandoc), se convierten los archivos Markdown a formato HTML.
    
Todo este proceso está automatizado en el script [bin/generate.sh](https://github.com/j1elo/curso-desarrollador-kurento/blob/master/bin/generate.sh). Antes de usarlo necesitas instalar sus dependencias:

```
sudo apt-get update && sudo apt-get install --yes \
    gpp \
    pandoc \
    libglib2.0-dev
```
