# Scheduler en JOS – Sistemas Operativos (FIUBA)

Este proyecto extiende el kernel educativo **JOS** para implementar cambios de contexto y distintas políticas de planificación de procesos: *Round Robin* y *Prioridades*.

Trabajo práctico de la materia **Sistemas Operativos** (FIUBA) – Grupo 25 – Cátedra Méndez-Fresia.

Integrantes:

- Agustín García Dresch
- Matías Gabriel Rebollo
- Marcos García Neira
- Sabrina García Lucentini

#### Mi contribución

Implementación del scheduler Round Robin (Parte 2).

---

## Compilar

Por _default_ se compilará el _scheduler_ en versión **round-robin**.

```bash
make
```

## Compilación condicional de _schedulers_

Para compilar y probar el kernel y poder probar ambos planificadores, se puede:

- **round-robin**:

```bash
make <target> USE_RR=1
```

- **priorities**:

```bash
make <target> USE_PR=1
```

## Pruebas

```bash
make grade
```

## Docker

Se provee un _script_ `dock` que permite ejecutar los siguientes comandos:

- **build**: genera la imagen del proyecto usando el `Dockerfile` provisto
- **run**: genera un _container_ a partir de la imagen anterior y lo corre
- **exec**: permite abrir una nueva _shell_ en el _container_ anterior

Dentro del _container_ se pueden ejecutar todos los comandos provistos por el `GNUmakefile` como `make grade` o `make qemu-nox`.

El _container_ utiliza [mount volumes](https://docs.docker.com/storage/volumes/) con lo cual los cambios que se realicen por fuera del mismo, serán visibles de forma automática.

## Linter

```bash
$ make format
```

Para efectivamente subir los cambios producidos por el `format`, hay que `git add .` y `git commit`.

## Funcionalidades implementadas

### Parte 1: Cambio de Contexto
- Implementación de `context_switch` en `switch.S` usando `iret`
- Compleción de `env_run` en `env.c`
- Implementación de `_alltraps` en `trapentry.S`
- Manejo de interrupciones y syscalls básicas
- Validación con el proceso `user_hello`

### Parte 2: Scheduler Round Robin
- Implementación de `sched_yield` con política Round Robin
- Selección cíclica entre procesos activos
- Manejo correcto de finalización de procesos

### Parte 3: Scheduler con Prioridades
- Implementación de `sched_yield` con política de Prioridades
- Procesos con prioridad asignada al crearse
- Syscalls para obtener y modificar prioridades (sin escalada)
- Estadísticas de planificación:
  - Historial de procesos ejecutados
  - Cantidad de ejecuciones por proceso
  - Tiempos de inicio y fin
- Validación con procesos de usuario que muestran el cambio de prioridades

---

## 🛠️ Tecnologías y herramientas

- Lenguaje: C y x86 Assembly
- Kernel base: [JOS (MIT)](https://pdos.csail.mit.edu/6.828/2021/jos/)
- Arquitectura: Intel x86
- Emulador: QEMU
- Compilación: `make`

Este proyecto fue desarrollado como parte del trabajo práctico de Sistemas Operativos (FIUBA) – Grupo 25 – Cátedra Méndez-Fresia.
