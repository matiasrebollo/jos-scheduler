# sched

# Parte 1
Las imagenes se encuentran en la carpeta "seguimiento_parte1"
Primero ponemos un breakpoint en context_switch (imagen1)
Luego empezamos a ejecutar desde ese punto y vemos el estado actual de los registros (imagen2) 
Luego vemos el estado de stack (imagen3)
Despues vamos avanzando de a paso y viendo como cambia el stack (imagenes 4 a 8)
Luego vemos los registros antes de llamar a iret (imagen9)
Y finalmente vemos como se cambian los registros luego de ejecutar iret(imagen10)


# Parte 3

## Funcionamiento

Nuestro scheduler por prioridades es del tipo Multi-level Feedback Queue (MLFQ), basado en un sistema de cola de prioridades con 5 niveles, siendo q4 la cola de mayor prioridad y q0 la menor. En un principio, todos los procesos comienzan en la cola de mayor prioridad (q4), y se establecen sus contadores del proceso de ejecuciones en la cola actual en 0.

La selección del siguiente proceso a correr comienza en la cola de mayor prioridad, y sino hay procesos listos en esa cola, se continúa por la cola de prioridad inferior hasta acabar en q0. En una misma cola, se aplica la lógica del Round Robin para seleccionar un proceso disponible, y este se ejecuta. Sino se encuentra un proceso para correr, y el proceso actual está en ejecucion, el scheduloer lo vuelve a ejecutar. En caso de no haber ningún proceso disponible, el scheduler entra en un estado de espera (halt).

Una vez elegido un proceso a ejecutar, su contador de ejecuciones en la cola actual aumenta, y una vez que este supera las 10 máximas ejecuciones permitidas, el proceso es degradado a la siguiente cola de menor prioridad, y su contador es reseteado. De esta manera, el scheduler puede aprender sobre el comportamiento de los procesos, y así los procesos más exigentes bajarán su prioridad, lo que le permite al scheduler poder predecir y adaptar su planifiación.

Sin embargo, si hubiese varios procesos en las colas de  mayor prioridad y otros en las menores, podría pasar mucho tiempo hasta que uno de estos últimos sean ejecutados, generándose lo que se llama 'starvation'. Para evitar esto, el scheduler cuenta con un contador el cual aumenta en cada llamado, y así una vez superado el límite de ejecuciones antes de mejora, todas las prioridades de los procesos son reseteados en la cola de mayor prioridad. De esta manera, se garantiza que todos los procesos, independientemente de su demanda, tendrán oportunidad de ser ejecutados.

## Estadísticas

En la parte de kern/init.c, probamos con distintos entornos de usuario los cuales nos fueron provistos por el esqueleto, para ver con las estadísticas como trabaja el scheduler con prioridades y compararlo con el Round Robin.

Se pueden ver los resultados de esto si se ejecuta la linea `make qemu-nox` y si se quiere eliminar o añadir entornos de usuario, se puede hacer borrando o agregando líneas en el archivo de kern/init.c

Lo que pudimos ver al realizar estas ejecuciones es que, en el caso de round robin, como era de esperarse, los procesos son elegidos por el scheduler una cantidad casi igual de veces. Por ejemplo, en el caso en el que teníamos 4 procesos porque ejecutamos 'pingpong' y 'spin', el scheduler round robin ejecutó 8, 8, 8 y 9 veces cada proceso respectivamente. A su vez, estos procesos utilizaron respectivamente 58, 33, 18 y 44 time slices. En cambio, en las ejecuciones utilizando el scheduler con prioridades, pudimos ver que la cantidad de veces que el scheduler selecciona a cada proceso varía en función de la prioridad. En el ejemplo anterior, los mismos procesos fueron elegidos 7, 8, 9 y 12 veces, utilizando 57, 33, 19 y 47 time slices. Vemos que hay procesos que son seleccionados mas o menos veces por el scheduler que en la ejecución con round robin. Lo mismo sucederá si se corren mas entornos que generen más procesos. 
