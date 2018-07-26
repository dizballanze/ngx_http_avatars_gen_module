# ngx_http_avatars_gen_module

[![Say Thanks!](https://img.shields.io/badge/Say%20Thanks-!-1EAEDB.svg)](https://saythanks.io/to/dizballanze)

Nginx module to generate gmail-like avatars based on user initials.

## Dependencies

Module requires libcairo:

```
sudo apt-get install libcairo2 libcairo2-dev
```


## Installation

To install module you should compile nginx with it:

```
cd nginx-1.11.3/
./configure --add-module=/path/to/module/avatars_gen
make && sudo make install
```

## Configuration

Module could be enabled on location context:

```nginx
http {
    server {
        listen       80;
        server_name  localhost;

        # Default params
        location /avatar {
            avatars_gen;
        }

        # Black & white avatar
        location /bw-avatar {
            avatars_gen;
            avatars_gen_bg_color FFFFFF;
            avatars_gen_contour_color 000000;
            avatars_gen_font_color 000000;
            avatars_gen_font_face serif;
        }

        # Custom size and without contour
        location /big-contourless {
            avatars_gen;
            avatars_gen_size 400;
            avatars_gen_show_contour off;
        }

        # Customized font
        location /font {
            avatars_gen;
            avatars_gen_size 300;
            avatars_gen_font_size 100;
            avatars_gen_font_face "Liberation Sans Narrow";
            avatars_gen_font_bold on;
        }

        # Square form
        location /square {
            avatars_gen;
            avatars_gen_square on;
            avatars_gen_bg_color 72A3F1;
            avatars_gen_contour_color E9AD00;
            avatars_gen_font_color 9BFAB5;
            avatars_gen_size 100;
            avatars_gen_font_face "Liberation Sans";
            avatars_gen_show_contour off;
        }
    }
}
```

Directives:

- `avatars_gen` - enables avatars generation on this location
- `avatars_gen_bg_color` (default: `4285f5`) - background color of avatar
- `avatars_gen_contour_color` (default: `333333`) - circle contour color
- `avatars_gen_font_color` (default: `FFFFFF`) - font color
- `avatars_gen_font_face` (default: `sans`) - font-face name
- `avatars_gen_font_size` (default: `avatars_gen_size / 2`)  - font size in pixels
- `avatars_gen_size` (default: `100`) - avatar size in pixels
- `avatars_gen_show_contour` (default: `on`) - show/hide contour
- `avatars_gen_font_italic` (default: `off`) - italic font on/off
- `avatars_gen_font_bold` (default: `off`) - bold font on/off
- `avatars_gen_square` (default: `off`) - square avatars on/off

You could change avatar colors with GET args (i.e. `?bg_color=FFFFFF&font_color=000000`):

- `bg_color`
- `contour_color`
- `font_color`


## Usage

You can make request to `http://localhost/avatar/GH` and it will generate 100x100 png image on-the-go:

Default:

![default](samples/default.png)

Black & white:

![black and white](samples/black-and-white.png)

Custom size and without contour:

![big-contourless](samples/big-contourless.png)

Customized font:

![customized font](samples/font.png)

Square form:

![square](samples/square.png)

__Module currently supports only 1 and 2 symbols initials.__

## TODO

- [x]  fix HEAD method
- [x]  default settings
- [x]  unicode initials
- [x]  avatars size configuration
- [x]  enable/disable border
- [x]  rich font settings
- [x]  square avatars
- [x]  colors configuration through GET parameters
- [ ]  random colors mode
- [ ]  different formats (jpg/svg)
- [ ]  configurations merge


## License

The MIT License (MIT)

Copyright (c) 2016 Yuri Shikanov
