# Logging System

Zuno offers a simple log utility with colored output and timestamps.

## log::log

```cpp
log::log("This is an example");
```
🧰 Prints general information without any format.

## log::info

```cpp
log::info("This is an example");
```
✅ Prints general information in blue.

## log::warn

```cpp
log::warn("Token not provided");
```
⚠️ Highlights warnings in yellow.

## log::error

```cpp
log::error("Database connection failed");
```
❌ Indicates critical issues in red.

## Output format

- Each log includes a timestamp (HH:MM:SS):

```
[INFO] [14:03:22] Server started
[WARN] [14:03:35] Token not provided
[ERROR] [14:03:47] Database connection failed
```

## Integration

Available by including:

```cpp
#include "zuno/logger.hpp"
```