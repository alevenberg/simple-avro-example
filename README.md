# simple-avro-example

Docs: https://avro.apache.org/docs/current/api/cpp/html/index.html

```sh
cd simple-avro-example
cmake -S . -B .build -DCMAKE_TOOLCHAIN_FILE=$HOME/vcpkg/scripts/buildsystems/vcpkg.cmake -G Ninja
cmake --build .build
.build/quickstart schema.avro
.build/generic schema.avro
```
