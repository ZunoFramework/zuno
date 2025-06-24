## Sistema de Logging
Zuno ofrece un sistema de logs sencillo con colores y marcas de tiempo para mejorar la trazabilidad de lo que ocurre en el servidor.

## log::log
Se utiliza para información general sin formato.

```cpp
log::log("Esto es un ejemplo.");
```

## Salida esperada:

```
Esto es un ejemplo
```

## log::info
Se utiliza para información general y seguimiento del flujo normal.

```cpp
log::info("Servidor iniciado");
```

## Salida esperada:

```
[INFO] [14:03:22] Servidor iniciado
```

## log::warn

Se emplea para advertencias que no detienen el funcionamiento, pero podrían indicar un problema potencial.

```cpp
log::warn("Token no proporcionado");
```
## Salida esperada:

```
[WARN] [14:03:35] Token no proporcionado
```

## log::error

Se usa para errores críticos que requieren atención inmediata.

```cpp
log::error("Fallo al conectar con la base de datos");
```

## Salida esperada:

```
[ERROR] [14:03:47] Fallo al conectar con la base de datos
```

## Formato de salida

Todos los logs incluyen una marca de tiempo en formato HH:MM:SS, justo después del tipo de log:

```
[INFO] [13:42:01] Mensaje informativo
[WARN] [13:42:15] Algo inusual ocurrió
[ERROR] [13:42:44] Error inesperado
```

## Integración
Para usar las funciones de logging, simplemente incluye el header correspondiente:

```cpp
#include "zuno/logger.hpp"
```

## Colores en la consola

```
[INFO]: azul
[WARN]: amarillo
[ERROR]: rojo
[HH:MM:SS]: gris
```