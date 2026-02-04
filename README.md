# image_ppm

Simple image resize tool for consistent pixels per meter. I use it to convert photographic textures tilesets for Crocotile3D while keeping visual consistency. 

# Requirements
- C++ 17 compiler (tested on MSVC/Clang/GCC)
- CMake 3.20

# Usage

```bash
image_ppm <input> --ppm <value> [--width <meters>] [--height <meters>]

# examples
image_ppm in.png --width 2.0 --ppm 128
image_ppm in.png --height 1.5 --ppm 256
```

Image input from `stb_image` supports formats `png`, `jpg/jpeg`, `bmp`, `tga`, `psd`, `gif` (first frame), `pic` and `pnm`. R, RG, RGB and RGBA are the supported LDR pixel layouts. HDR is not supported as its not necessary for albedo painting. Any color space can be used. Output image will be using the same format and pixel layout as the provided input.

Either `--width` or `--height` must be provided to represent the input real size along at least one dimension. If dimensions are provided the original aspect ratio will not be preserved.

`--ppm` represents the desired pixels per meter output. Typical usage is to set this to the same value as Crocotile3D's pixel per meter setting.