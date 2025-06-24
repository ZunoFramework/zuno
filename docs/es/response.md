# API de Response

## status(code)

Configura el código de estado HTTP:

```cpp
    res.status(404);
```

## setHeader(clave, valor)

Establece headers personalizados:
```cpp
    res.setHeader("X-Powered-By", "Zuno");
```

## send(data)

Envía una respuesta con el dato proporcionado:

```cpp
  res.send("Hola mundo");  
```

## json(data)

Envía una respuesta en formato JSON:

- **Importante:** se utilza la biblioteca nlohmann/json para el manejo de los objetos JSON.

```cpp
    res.json({ {"ok", true} });
```

## statusText()

Devuelve el texto estándar del status actual:

```cpp
    std::string texto = res.statusText(); // "Not Found"
```

