### Bug

* Bug-1 -> corregido

    - Los procesos entran en lista de contenedor.
    - Nunca entran en lista listo.
    - Ocurre luego que sale un proceso.
* Bug-2
    - Ocurre en el metodo continuarProcesosEspera
    - Inserta todos los procesos en lista de espera, en listos
    - Incluso un proceso que tiene mucho tiempo en E/S
    - Incluye al proceso en ejecucion 
    - Es el metodo de eliminar? el nodo se bugea