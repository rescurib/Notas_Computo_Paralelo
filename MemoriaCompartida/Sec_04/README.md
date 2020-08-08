# Sincronización

En esta seccción trataremos los constructos de OpenMP que permiten administrar los accesos a memoria desde los hilos activos en un programa. En muchas ocaciones un algoritmo requiere realizar una coordinación entre las lecturas y escrituras a las variables compartidas para garantizar la correctitud del programa.

## Constructo *Barrier*

Todas la directivas que hemos visto hasta ahora tienen una barrera de sincronización implicita. Esto significa que si un hilo terminó primero la sección o la parte de un ciclo for que le tocaba ejecutar, no avanzara hacía una nueva directiva hasta que el resto termine. Sin embargo no es raro que se requiera una barrera de sincrnización explisita dentro de una sección paralela. Veamos wl siguiente ejemplo.

## Ejemplo 4.1: Barrera de sicronización explicita

