# Edición de código desde consola

Se que existe el entorno [Eclipse IDE for Scientific Computing](https://www.eclipse.org/downloads/packages/release/2020-06/r/eclipse-ide-scientific-computing) pero no tuve suerte a la hora de ejecutar los programas desde ahí por lo que no puedo escribir una guía. Pueden probarlo si quieren, es sólo descomprimir y abrir el ejecutable y seguir la instrucciones en la ayuda. Por otro lado, a veces es más adecuado (y rápido después de ganar práctica) trabajar desde consola, en especial cuando se tiene acceso a un cluster remoto. Una manera cómoda que he encontrado para trabajar en la terminal es usar Tmux y Vim. En esta carpeta se encuentran mis archivos de configuración para cada uno para verse así:

<p align="center">
<img src="https://4.bp.blogspot.com/-0awjnbltYlc/XwKC5hZxDfI/AAAAAAAACW4/7UmWVscpbfMfJ_neJuoxm3CjCtAObs6PwCLcBGAsYHQ/s1600/Captura%2Bde%2Bpantalla%2Bde%2B2020-07-05%2B20-47-58.png" alt="alt text" width="850">
</p>

La configuración de Vim es una modificación menor de [The Ultimate vimrc](https://github.com/amix/vimrc). Los contenidos de los archivos _tmux.conf y _vimrc deben sustituir los contenidos de ~/.tmux.conf y ~/.vimrc (o crearlos si no existen).

## Tmux
Tmux es un multiplexor de terminal que no solo te permite segmentar en ambas direcciones sino que tambien puede gestionar sesiones, lo cual resulta muy muy útil en loggins remotos porque puedes continuar con lo que estabas haciendo antes de salir. Para instalar en Debian y sus distros variantes:
```
sudo apt install tmux
```
Pueden aprender a usarlo con un solo [vídeo](https://www.youtube.com/watch?v=vwRxelWEuFE). Incluso con mi nota de ayuda basta.
## Vim
Advierto que lo van odiar al principio pero quizá terminen amandolo cuando memoricen los atajos. No es obligatorio utilizarlo, nano está bien si ya están acostumbrados a él. Si no está instalado por defecto:
```
sudo apt install vim
```
[Aquí](https://www.youtube.com/watch?v=iWo1cDGlNDI) hay un video-tutorial en español para usar Vim. Vale mucho la pena aprenderlo.

## Alias con notas de ayuda
Yo no tengo tan buena memoria así que un truco que uso es crear alias que me impriman notas en la terminal. En este caso una que contiene los atajos que más uso en Tmux, Vim y en la terminal en general. Para esto escribimos en la temrinal:
```
nano ~/.bash_aliases
```
Cuando abra el editor (use nano porque si es la primera vez abren Vim batallarán en salir) añadiremos esta línea:

```
alias ayuda='cat /home/tuUsuario/notasVimTmux.txt'
alias rm='rm -i' # De paso este, les puede salvar la vida (pregunta antes de borrar algo)
```
Pueden poner el archivo en el directorio que quieran, ese sólo de ejemplo. Para actualizar el bash:
```
exec bash
```
Ahora cada que escriban 'ayuda' la nota de texto aparecerá en la consola.

## Mover y renombrar carpetas y archivos

Para mover un directorio y meterlo en otro (FolderA -> FolderB) usamos mv <fuente> <destino>:
```
mv FolderA /path/de/FolderB
```
Lo mismo sirve para archivos:
```
mv miArchivo.c /path/nuevo/miArchivo_puedes_renombrarlo_si_quieres.c
```

El comando mv sirve tambien para renombrar directorios
```
mv nombreViejo nombreNuevo
```

Tambien funciona para archivos pero es mejor utilizar *rename* porque te permite usar patrones complejos de renombramiento. [Aquí](http://stg-pepper.blogspot.com/2020/06/renombrar-archivos-automaticamente-en.html) escribir una entrada en mi blog sobre como utilizarlo.

## Transferencia remota de archivos

Si les dan acceso a un cluster seguramente van a necesitar estar transifiendo archivos entre hosts. Si conocen de antemano las ubicaciones y nombres de los archivos pueden utilizar el comando [scp](http://www.hypexr.org/linux_scp_help.php).

De **local** --> **remoto**
```
scp /un/dir/local/archivo usuario@ipremota:/un/dir/remoto/archivo
```
De **remoto** --> **local**
```
scp usuario@ipremota:/un/dir/remoto/archivo /un/dir/local/archivo
```
Si no recuerdan el directorio o el nombre de archivo pueden usar [sftp](https://www.blog.binaria.uno/2019/05/20/como-usar-sftp-para-transferir-archivos-forma-segura/). Es un poco más complicado porque hay que recordar cuando estás ejecutando comandos en el el host remoto y cuando en el local. Se usa get y put para bajar (hacía el local) y subir archivos (hacía el remoto). También se puede hacer de [forma gráfica](http://www.ubuntufacil.com/2013/12/sftp-transferencia-de-archivos-segura-mediante-ssh/) con Nautilus o Filezila.

## Otros comandos útiles
Para mostrar el número de procesadores en su CPU:
```
nproc
```
Resumen de las caracteristicas de su CPU:
```
lscpu
```

Información detallada del CPU:
```
less /proc/cpuinfo
```

Historial de comandos:
```
history
```
Les mostrará una lista enumerada con los los ultimos comandos ejecutados. Si están en Tmux deberán presionar Ctrl+A+\[ para hacer scroll (q para salir). Pueden repertir la ejecución de un comando pasado útilizando su número. Por ejemplo:
```
!871
```
