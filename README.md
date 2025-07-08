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

## Ejecución

Para ejecutar el kernel se utiliza QEMU:

```bash
make qemu
```
Requisitos: GCC, GNU Make, QEMU

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


