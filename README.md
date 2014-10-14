Nanogear
======

Description 
------

Nanogear is a C++ library for building client and server side RESTful applications

History
------

Nanogear was originally written by Lorenzo Villani but has languished unfinished and unappreciated since 2008. I have always felt it had unrealised potential but have never had the time to hack on it. Until now. 

The original repository can be found [here](https://code.google.com/p/nanogear/)

Quickstart
------

After cloning the repository make a parallel build directory 

### Macintosh 

```bash
mkdir nanogear_build
cd nanogear_build 
cmake -G Xcode -DWITH_EXAMPLES=ON ../nanogear
open nanogear.xcodeproj
```

Hack away

