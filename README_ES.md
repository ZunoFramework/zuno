# ⚡ Zuno Framework

Zuno Framework es un potente y moderno framework web escrito en C++, diseñado para ofrecer un rendimiento excepcional sin sacrificar la simplicidad. Inspirado en la filosofía de Express.js y potenciado por Asio standalone, Zuno permite a los desarrolladores construir aplicaciones web seguras, modulares y escalables con una API clara y elegante.

## 🚀 Características principales

- 🔧 **Modularidad total**: Diseña tu aplicación como un conjunto de módulos independientes y reutilizables.  
- 🔒 **Soporte TLS con `useTLS`**: Seguridad lista para producción con una función clara y directa.  
- ⚙️ **Basado en Asio standalone**: Aprovecha la eficiencia de I/O asincrónico sin dependencias externas.  
- 🧩 **Routing intuitivo**: Define rutas con expresividad y control total.  
- 🛡️ **Middlewares globales y por ruta**: Encadena lógica personalizada para autenticación, logging, validación y más.  
- 📦 **Ligero y sin dependencias pesadas**: Ideal para sistemas embebidos o entornos de alto rendimiento.  

## 🛠️ Ejemplo rápido

```cpp
#include <zuno/zuno.hpp>
#include <zuno/logger.hpp>

int main() {
    zuno::App app;

    app.use([](const zuno::Request& req, zuno::Response& res, zuno::Next next) {
        zuno::log::info("Middleware global");
        next();
    });

    app.get("/hello", [](const zuno::Request& req, zuno::Response& res) {
        res.send("¡Hola desde Zuno!");
    });

    zuno::useTLS(app, {
        .certFile = "cert.pem",
        .keyFile = "key.pem"
    });

    app.listen(3456);
}
```

## 📚 Documentación

La documentación completa estará disponible pronto. Mientras tanto, puedes explorar los siguientes componentes clave:

- `zuno::App` – Núcleo del framework  
- `zuno::Request` y `zuno::Response` – Manejo de solicitudes y respuestas  
- `zuno::Next` – Control de flujo entre middlewares  
- `useTLS(app, config)` – Activación de HTTPS con certificados  

## 🧪 Estado del proyecto

Zuno Framework está en desarrollo activo. Las siguientes funcionalidades están disponibles o en progreso:

| Funcionalidad         | Estado           |
|-----------------------|------------------|
| Routing básico        | ✅ Listo          |
| TLS con `useTLS`      | ✅ Listo          |
| Middleware global     | ✅ Listo          |
| Middleware por ruta   | ✅ Listo          |
| Plantillas HTML       | 🔜 Planeado       |
| WebSockets            | 🔜 Planeado       |

## 🤝 Contribuciones

¡Las contribuciones son bienvenidas! Si deseas colaborar, revisa los [issues](https://github.com/ZunoFramework/zuno/issues) o abre un pull request.

## 📄 Licencia

Zuno Framework está licenciado bajo la [GNU Affero General Public License v3.0](LICENSE).

---

Desarrollado con pasión por **Seobryn** ⚙️
