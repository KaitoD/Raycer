# Utilities

Document for /util files

## ray_def.h

```c++
#define RAYCER_STACKTRACE
```

Define this macro to enable full stacktrace feature for functions that use `TRACE()` or `TRACEFUNC`, when encounter panic, stack would unwind and display the chain of call stack.

```c++
#define TRACEFUNC TRACE(__PRETTY_FUNCTION__)
```

Easy way to use `TRACE()` macro

## ray_image.h

```c++
class BMPImage;
```

This class is for generating a `.bmp` image, use `flush(const char* filename)` to flush to a certain file, **filename does not contain `.bmp`**. When you are trying to set a pixel outside the image, it would panic and terminate the program.

## ray_misc.h

```c++
#define TRACE(_Name)
```

Define this macro at the beginning of the function to determine the display information when doing stack rewind.

```c++
void panic(const char* fmt, ...);
```

Panic, terminate the program and do some stuff.