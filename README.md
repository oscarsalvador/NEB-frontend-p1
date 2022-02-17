## Permisos
La carpeta (NEB-frontend-p1-x.x) necesita lectura y ejecución (755); y la carpeta de medios (/img) necesita permisos de lectura (711). Dentro de la carpeta, tras extraer la release, son necesarios los comandos:
```
chmod 755 .
chmod 711 ./img/
```

## Compilar spaceinv.c
Requisitos:
>clang compiler-rt lld wasi-compiler-rt wasi-libc wasi-libc++   

Hay un .wasm compilado en el repositorio.

El servicio manjaro-wasm del docker-compose construye su imagen con el dockerfile adjuntado. Parte de la imagen mas reciente de la distribucion y le añade las herramientas necesarias para compilar. El siguiente comando hace la compilacion.
```
docker-compose run --rm manjaro-wasm clang --target=wasm32-wasi --sysroot=/usr/share/wasi-sysroot -nostartfiles -O3 -flto -Wl,--no-entry -Wl,--lto-O3 spaceinv2.c -o spaceinv2.wasm
```

## Levantar el contenedor con docker-compose
El puerto 8081 de la host tiene que estar libre. Con dos servicios, ahora es necesario especificar cual levantar para no levantar ambos.
```
docker-compose up nginx
```
http://localhost:8081/space_invaders.html
