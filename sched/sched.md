# sched

# Parte 3

En la parte de kern/init.c, probamos con distintos entornos de usuario los cuales nos fueron provistos por el esqueleto, para ver con las estadísticas como trabaja el scheduler con prioridades y compararlo con el Round Robin.

Se pueden ver los resultados de esto si se ejecuta la linea `make qemu-nox` y si se quiere eliminar o añadir entornos de usuario, se puede hacer borrando o agregando líneas en el archivo de kern/init.c

Lo que pudimos ver al realizar estas ejecuciones es que, en el caso de round robin, como era de esperarse, los procesos son elegidos por el scheduler una cantidad casi igual de veces. Por ejemplo, en el caso en el que teníamos 4 procesos porque ejecutamos 'pingpong' y 'spin', el scheduler round robin ejecutó 8, 8, 8 y 9 veces cada proceso respectivamente. A su vez, estos procesos utilizaron respectivamente 58, 33, 18 y 44 time slices. En cambio, en las ejecuciones utilizando el scheduler con prioridades, pudimos ver que la cantidad de veces que el scheduler selecciona a cada proceso varía en función de la prioridad. En el ejemplo anterior, los mismos procesos fueron elegidos 7, 8, 9 y 12 veces, utilizando 57, 33, 19 y 47 time slices. Vemos que hay procesos que son seleccionados mas o menos veces por el scheduler que en la ejecución con round robin. Lo mismo sucederá si se corren mas entornos que generen más procesos. 
