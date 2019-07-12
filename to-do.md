# TO-DO

Temas pendientes


cpp_intro.md
============

c++, una extension de c
codigo c puede compilar en cxx

librería stslib con todas las features típicas:
vector, string, map, thread,

OO: herencia, polimorfismo

constructor, destructor, copia

paso por valor, puntero, referencia



rtp_sdp.md
==========

Qué es RTP, transmisión paquetes UDP
RTP header, números de secuencia, retransmisión
Puertos de envío, de recepción
Negociación SDP, codecs, puertos, features
Modo seguro (SRTP)
Metadatos del stream (RTCP)



nat_stun_turn_ice.md
====================

Problemática
Tipos de NAT (https://doc-kurento.readthedocs.io/en/latest/knowledge/nat.html)
Hole-punching
ICE



webrtc.md
=========

La suma de (por orden) SDP + ICE + (S)RTP



gstreamer.md
============

Librería C que proporciona un framework completo para crear aplicaciones de multimedia.

Basado en plugins: librerías que no se enlazan a tu binario, sino que se buscan al ejecutar y se cargan solo cuando se necesita (mecanismo dlopen).

Dos modos de uso:

1. Preview. Diseño de una pipeline basado en prueba y error.
gst-launch parsea una cadena de texto que expresa la construcción de una pipeline.
Se pueden ver los resultados de forma inmediata.

2. Cuando tenemos la pipeline diseñada, implementación en C.

Ejemplo mínimo cargando fakesrc y videosink



kurento.md
==========

arquitectura Kurento

Servidor JSON-RPC en Cpp.
Construye poco a poco pipeline gstreamer segun llegan comandos.

Capa control Gstreamer, gestiona enlazado de obketos, etc.

Elementos gstreamer propios de Kurento
- webrtcendpoint
- agnosticbin (cómo funciona)

Librerías API cliente
- Archivos .KMD.JSON
- Auto-generación

Integración con Gstreamer, elementos y ejemplo crear elemento.

C con Pseudo-Object-Orientation de GObject basado en macros y boilerplate

Kurento modules, crear modulo.

Estructura de kurento:
codigo cpp, c
codigo autogenerado
definicion de modulos kmd.json

CI
Pipeline de jobs parametrizada
branches experimentales
deploy deb y docker
