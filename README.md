
# BehaviorTree

## Compiling

This project requires a compiler that supports C++11.

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./install -DBUILD_SHARED_LIBS=True ..
make
make install
```
## Documentation

See the header files in src/BT for documentation (start with BT::BehaviorNode).

See luaScriptsForUnitTests/TestLuaFactoryScript.lua as an example for
generating a BehaviorTree with BT::BehaviorLuaFactory.

If you have doxygen installed, you can generate html documentation.

```
doxygen
firefox doxygen_html/html/index.html
```

