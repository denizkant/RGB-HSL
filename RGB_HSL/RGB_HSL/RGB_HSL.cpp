#include <iostream>
#include <algorithm>

int RGBtoHSL(int rgb) {
    uint8_t r = ((rgb >> 16) & 0xFF);
    uint8_t g = ((rgb >> 8) & 0xFF);
    uint8_t b = (rgb & 0xFF);

    double rNorm = r / 255.0;
    double gNorm = g / 255.0;
    double bNorm = b / 255.0;

    double cmax = std::max({ rNorm, gNorm, bNorm });
    double cmin = std::min({ rNorm, gNorm, bNorm });
    double delta = cmax - cmin;

    double h = 0.0;
    if (delta != 0.0) {
        if (cmax == rNorm) {
            h = 60 * ((gNorm - bNorm) / delta);
        }
        else if (cmax == gNorm) {
            h = 60 * ((bNorm - rNorm) / delta + 2);
        }
        else if (cmax == bNorm) {
            h = 60 * ((rNorm - gNorm) / delta + 4);
        }
        if (h < 0) {
            h += 360;
        }
    }

    double l = (cmax + cmin) / 2;
    double s = delta / (1 - std::abs(2 * l - 1));

    // Pack HSL values into a single int (24-bit)
    int hsl = (int(h) << 16) | (int(s*100) << 8) | int(l*100);
    return hsl;

}

int HSLtoRGB(int hsl) {
    uint16_t h = (hsl >> 16) & 0xFF;
    uint8_t s = (hsl >> 8) & 0xFF;
    uint8_t l = hsl & 0xFF;

    double hNorm = h / 60.0;
    double sNorm = s / 100.0;
    double lNorm = l / 100.0;

    double c = (1 - std::abs(2 * lNorm - 1)) * sNorm;
    double x = c * (1 - std::abs(std::fmod(hNorm, 2) - 1));
    double m = lNorm - c / 2;

    double r, g, b;

    if (hNorm >= 0 && hNorm < 1.0) {
        r = c;
        g = x;
        b = 0;
    }
    else if (hNorm >= 1.0 && hNorm < 2.0) {
        r = x;
        g = c;
        b = 0;
    }
    else if (hNorm >= 2.0 && hNorm < 3.0) {
        r = 0;
        g = c;
        b = x;
    }
    else if (hNorm >= 3.0 && hNorm < 4.0) {
        r = 0;
        g = x;
        b = c;
    }
    else if (hNorm >= 4.0 && hNorm < 5.0) {
        r = x;
        g = 0;
        b = c;
    }
    else {
        r = c;
        g = 0;
        b = x;
    }

    int red = int((r + m) * 255);
    int green = int((g + m) * 255);
    int blue = int((b + m) * 255);

    // Pack RGB values into a single int (24-bit)
    int rgb = (red << 16) | (green << 8) | blue;
    return rgb;
}


int main() {
    int rgbInput = 0x112233;  // RGB color value

    int hslOutput = RGBtoHSL(rgbInput);
    int h = (hslOutput >> 16) & 0xFF;
    int s = (hslOutput >> 8) & 0xFF;
    int l = hslOutput & 0xFF;

    std::cout << "HSL: " << h << ", " << s << ", " << l << std::endl;
    
    int hslInput = 0x00d2320d;  // HSL color value

    int rgbOutput = HSLtoRGB(hslInput);
    int r = (rgbOutput >> 16) & 0xFF;
    int g = (rgbOutput >> 8) & 0xFF;
    int b = rgbOutput & 0xFF;

    std::cout << "RGB: " << r << ", " << g << ", " << b << std::endl;


    return 0;

}
