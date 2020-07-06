# PXL8R

by Hapaxia
July 2020
based on PXL8 (July 2014)
> github.com/Hapaxia
> twitter.com/Hapaxiation

Pixelation Class

Creates a pixelated render target. It is drawn to using views in a similar fashion to SFML.
The size of the pixels can be specified (both dimensions can be specified separately) which changes the amount of pixelation.
The class is an sf::Drawable so can be drawn directly to your window as normal. It is also transformable so can be transformed as normal.
PXL8R is contained within a single header file.
Now features "pixel lock" that internally resizes pixels so that the division is a whole number.

See example.cpp to see it in action.
It requires a font for the overlay text and an image as an example pixelation test. Some are provided for use with this example only but you can easily use your own instead.
Anti-alias is automatically set to display the rotation more nicely. It doesn't affect the pixelation. Anti-alias is set to 16; if 16 is not available, it should pick the highest available (below 16). You can change this to 0 (or comment out the line) to remove anti-aliasing.

Controls for example.cpp:

## SIZING
    1 and 2 - reduce and expand respectively the pixel size.
    3 and 4 - reduce and expand respectively the pixel width.
    5 and 6 - reduce and expand respectively the pixel height.
    - and = - reduce and expand respectively its size.
      All of the above SIZING controls can be modified:
      NONE - increases or decreases size by 10%
      ALT - increases or decreases size by 1 unit.
      SHIFT - doubles or halves the size.
      ALT+SHIFT - reset size (pixel size/width/height to 1, size to fit window)
      CONTROL - (size only) does not automatically reform circles to fit the new size
    Q - quantize pixel size (round to nearest integer).

## COLOURS
    7 - set colour to red (with ALT = cyan)
    8 - set colour to green (with ALT = magenta)
    9 - set colour to blue (with ALT = yellow)
    0 - set colour to white (with ALT = white with half transparency)

## ROTATION
    , - increase anti-clockwise rotation speed/decrease clockwise rotation speed (with SHIFT = 10 times)
    . - increase clockwise rotation speed/decrease anti-clockwise rotation speed (with SHIFT = 10 times)
    / - reset rotation (with ALT = also stop rotation)
    ALT+, - stop anti-clockwise rotation/flip clockwise rotation to anti-clockwise
    ALT+. - stop clockwise rotation/flip anti-clockwise rotation to clockwise

## MISC
    TAB - toggles pixel lock (defaults to on)
    ENTER - randomise the circles' colours.
    ALT+ENTER - toggle circles' outlines (also automatically reforms circles to fit the current size).
    CONTROL+ENTER - reform circles to fit the current size.
    SPACE - switch between the circles and the image.
    ESCAPE - close application.

Both PXL8R and its example require SFML 2 (www.sfml-dev.org)
