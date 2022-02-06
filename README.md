## Permisos
La carpeta (NEB-frontend-p1-1.0) necesita lectura y ejecución (755); y la carpeta de medios (/img) necesita permisos de lectura (711). Dentro de la carpeta, tras extraer la release, son necesarios los comandos:
chmod 755 .
chmod 711 ./img/

## Compilar spaceinv.c
Requisitos:
>clang compiler-rt lld wasi-compiler-rt wasi-libc wasi-libc++   

Hay un .wasm compilado en el repositorio.

```
clang --target=wasm32-wasi --sysroot=/usr/share/wasi-sysroot -nostartfiles -O3 -flto -Wl,--no-entry -Wl,--lto-O3 spaceinv.c -o spaceinv.wasm  
```

## Levantar el contenedor con docker-compose
El puerto 8081 de la host tiene que estar libre.
```
docker-compose up
```
http://localhost:8081/space_invaders.html
