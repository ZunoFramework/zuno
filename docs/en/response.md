# Response API

## status(code)

Set HTTP status code:

```cpp
res.status(404);
```

## setHeader(key, value)

Set a custom HTTP header:

```cpp
res.setHeader("X-Powered-By", "Zuno");
```

## send(content)

Send raw string content:

```cpp
res.send("Hello world");
```

## json(object)

Send a JSON response:

```cpp
res.json({ {"ok", true} });
```
## statusText()

Get the standard message for the current status code:

```cpp
std::string text = res.statusText(); // "Not Found"
```