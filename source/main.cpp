#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include <stb_image.h>
#include <stb_image_resize2.h>
#include <stb_image_write.h>

struct cli_args {
    const char* input = nullptr;
    float width_meters = -1.0f;
    float height_meters = -1.0f;
    int ppm = -1;
};

[[nodiscard]] bool parse_args(int argc, char** argv, cli_args& args)
{
    if (argc < 2) {
        return false;
    }
    args.input = argv[1];
    for (int i = 2; i < argc; ++i) {
        if (!strcmp(argv[i], "--width") && i + 1 < argc) {
            args.width_meters = std::stof(argv[++i]);
        } else if (!strcmp(argv[i], "--height") && i + 1 < argc) {
            args.height_meters = std::stof(argv[++i]);
        } else if (!strcmp(argv[i], "--ppm") && i + 1 < argc) {
            args.ppm = std::stoi(argv[++i]);
        } else {
            return false;
        }
    }
    if (args.ppm <= 0) {
        return false;
    }
    if (args.width_meters <= 0.f && args.height_meters <= 0.f) {
        return false;
    }
    return true;
}

int main(int argc, char** argv)
{
    cli_args args;
    if (!parse_args(argc, argv, args)) {
        std::cerr << "Usage:" << std::endl
                  << "  img2tileset <input> --ppm <value> [--width <meters>] [--height <meters>]\n" << std::endl
                  << "Examples:" << std::endl
                  << "  img2tileset myimage.png --width 2.0 --ppm 128" << std::endl
                  << "  img2tileset myimage.png --height 1.5 --ppm 256" << std::endl;
        return 1;
    }

    // load pixels
    int _input_width, _input_height, _input_channels;
    unsigned char* _input_pixels = stbi_load(args.input, &_input_width, &_input_height, &_input_channels, 0);
    if (!_input_pixels) {
        std::cerr << "Failed to load image" << std::endl;
        return 1;
    }
    stbir_pixel_layout _pixel_layout;
    switch (_input_channels) {
    case 1:
        _pixel_layout = STBIR_1CHANNEL;
        break;
    case 2:
        _pixel_layout = STBIR_2CHANNEL;
        break;
    case 3:
        _pixel_layout = STBIR_RGB;
        break;
    case 4:
        _pixel_layout = STBIR_RGBA;
        break;
    default:
        std::cerr << "Unsupported channel count" << std::endl;
        stbi_image_free(_input_pixels);
        return 1;
    }

    // resize pixels
    int _output_width = 0, _output_height = 0;
    std::vector<unsigned char> _output_pixels(_output_width * _output_height * _input_channels);
    if (args.width_meters > 0 && args.height_meters > 0) {
        _output_width = static_cast<int>(std::round(args.width_meters * args.ppm));
        _output_height = static_cast<int>(std::round(args.height_meters * args.ppm));
    } else if (args.width_meters > 0) {
        _output_width = static_cast<int>(std::round(args.width_meters * args.ppm));
        _output_height = static_cast<int>(std::round(_output_width * (float)_input_height / (float)_input_width));
    } else {
        _output_height = static_cast<int>(std::round(args.height_meters * args.ppm));
        _output_width = static_cast<int>(std::round(_output_height * (float)_input_width / (float)_input_height));
    }
    if (_output_width <= 0 || _output_height <= 0) {
        std::cerr << "Invalid output size" << std::endl;
        stbi_image_free(_input_pixels);
        return 1;
    }
    stbir_resize(
        _input_pixels, _input_width, _input_height, 0,
        _output_pixels.data(), _output_width, _output_height, 0,
        _pixel_layout,
        STBIR_TYPE_UINT8,
        STBIR_EDGE_CLAMP,
        STBIR_FILTER_BOX);

    // write pixels
    const std::filesystem::path _input_path(args.input);
    const std::filesystem::path _input_filename = _input_path.filename().replace_extension("");
    const std::filesystem::path _input_extension = _input_path.extension();
    const std::string _output_filename = _input_filename.string() + " [" + std::to_string(args.ppm) + "]" + _input_extension.string();
    const std::filesystem::path _output_path(_input_path.parent_path() / _output_filename);
    std::cout << _output_path << std::endl;
    if (!stbi_write_png(_output_path.string().c_str(), _output_width, _output_height, _input_channels,
            _output_pixels.data(), _output_width * _input_channels)) {
        std::cerr << "Failed to save image" << std::endl;
        stbi_image_free(_input_pixels);
        return 1;
    }
    stbi_image_free(_input_pixels);
    std::cout << "Output: " << _output_width << " x " << _output_height << " px\n";
    return 0;
}