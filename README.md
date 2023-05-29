# GNU Debugger

El GNU Debugger (gdb) es una herramienta de depuración poderosa que puede ayudarte a encontrar la línea específica
que provoca el error de Segmentation Fault. Aquí tienes una guía básica sobre cómo utilizar gdb para depurar tu
programa y encontrar la causa del error:

* Compila tu programa con la opción de depuración: ``gcc -g programa.c -o programa``
* Inicia gdb con tu programa: ``gdb programa``
* Ejecuta tu programa en gdb: ``run``
* Encuentra la línea que causa el Segmentation Fault:

  - ``backtrace`` o ``bt`` para mostrar las llamadas de funciones y las líneas de código donde se origina el error    .
  - ``list`` para mostrar el código fuente alrededor de la línea problemática.

* Establecer un punto de interrupción en una línea de código:
  - por funcion: ``break nombre_funcion``
  - por linea de codigo: ``break archivo.c:10`` 10 numero de linea de codigo

* Examina los valores de las variables: ``print`` o ``p``
* Continuar la ejecución después de recibir una señal de error:  ``continue `` o   ``c ``
* Para terminar gdb ``quit`` o ``q`` y luego del mensaje ¿Salir de cualquier modo? (y or n) ``y``

# Libreria Interfaz GTK

### Comandos de instalacion:

* comando 1:  `sudo apt-get update`
* comando 2:  `sudo apt-get install libgtk-3-dev`
* comando 3:  `sudo apt-get install libgdk-pixbuf2.0-dev`

# Libreria sonido GStreamer

### Comandos de instalación Terminal Librerias

``` sudo apt-get update ```

```sudo apt-get install libgstreamer1.0-dev```

```sudo apt-get install gstreamer1.0-plugins-bad```


### Comandos de instalación Terminal Librerias opcionales

```sudo apt install snapd```

```sudo snap install gstreamer --edge```

### Generar un compilador con las librerias
* ```gcc main.c -o exe `pkg-config --cflags --libs gtk+-3.0 gstreamer-1.0` ```

