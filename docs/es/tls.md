# Configuración de la Conexión TLS en Zuno

Zuno proporciona soporte para configurar un servidor HTTP seguro usando TLS (Transport Layer Security). Este guía te guiará a través del proceso de configurar tu aplicación Zuno para usar TLS.

## Requisitos Previos

Antes de continuar, asegúrate de tener instalados lo siguiente:
- OpenSSL

## Paso 1: Generar Certificados SSL

Necesitas generar un archivo de clave privada y un certificado. Puedes usar OpenSSL para esto. Ejecuta los siguientes comandos en tu terminal:

```bash
# Genera una clave privada
openssl genpkey -algorithm RSA -out server.key -aes256

# Genera un certificado autofirmado
openssl req -new -x509 -key server.key -out server.crt -days 365
```

## Paso 2: Configurar Zuno para Usar TLS

Para configurar tu aplicación Zuno para usar TLS, necesitas modificar el archivo `main.cpp`. Aquí tienes un ejemplo de cómo hacerlo:

```cpp
#include "zuno/zuno.hpp"

int main() {
    zuno::App app;

    app.useTLS({"ruta/a/certificado/server.crt", "ruta/a/certificado/server.key"});

    // Registra una ruta
    app.get("/hola", [](const zuno::Request& req, zuno::Response& res) {
        res.send("¡Hola desde Zuno!");
    });

    // Inicia el servidor con TLS
    app.listen(443);

    return 0;
}
```

## Paso 3: Compilar y Ejecutar

Compila tu aplicación usando CMake:

```bash
mkdir build && cd build
cmake ..
make
```

Ejecuta el servidor compilado:

```bash
./server
```

Ahora, puedes acceder a tu servidor HTTP seguro en `https://localhost/hola`.

## Solución de Problemas

- **Archivos de Certificado y Clave**: Asegúrate de que las rutas a los archivos de certificado y clave sean correctas.
- 
- **Puerto 443**: El puerto 443 se usa comúnmente para HTTPS. Si encuentras problemas, intenta usar un puerto diferente.

Siguiendo estos pasos, deberías poder configurar un servidor HTTP seguro en tu aplicación Zuno usando TLS.