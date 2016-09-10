# ngx_http_avatars_gen_module

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

        location /avatar {
            avatars_gen;
            avatars_gen_bg_color 4285F4;
            avatars_gen_contour_color 333333;
            avatars_gen_font_color FFFFFF;
            avatars_gen_font_face sans;
        }
    }
}
```

Directives:

- `avatars_gen` - enables avatars generation on this location
- `avatars_gen_bg_color` - background color of avatar
- `avatars_gen_contour_color` - circle contour color
- `avatars_gen_font_color` - font color
- `avatars_gen_font_face` - css-like simplified font-face

## Usage

You can make request to `http://localhost/avatar/GH` and it will generate 100x100 png image on-the-go:

![ava.png](ava.png)

__Module currently supports only 1 and 2 symbols initials.__

## TODO

- [x]  fix HEAD method
- [x]  default settings
- [x]  unicode initials
- [ ]  avatars size configuration
- [ ]  enable/disable border
- [ ]  rich font settings
- [ ]  square avatars
- [ ]  colors configuration through GET parameters
- [ ]  random colors mode
- [ ]  different formats (jpg/svg)
- [ ]  configurations merge


## License

The MIT License (MIT)

Copyright (c) 2016 Yuri Shikanov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
