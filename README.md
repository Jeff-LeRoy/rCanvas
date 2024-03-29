# rCanvas
rCanvas is a minimilistic image viewer with the advantage that you can load multiple images onto a single canvas for reference. It was designed around the needs of digital artist where often times it is benifical to have a collage of reference images instead of flipping back and forth through images, tabs and folders.  

# Current Release
[v1.0.0](https://github.com/Jeff-LeRoy/rCanvas/releases) Initial release of rCanvas, saving / loading is working.

# Examples
A few examples with placeholder images, imagine it with real reference images.
![rCanvas](https://github.com/Jeff-LeRoy/rCanvas/assets/7757495/3d9a6990-3f3d-44e4-ad9f-4124386ed31a)
![Capture](https://user-images.githubusercontent.com/7757495/155215023-f28c532b-1cd4-4d01-85d3-92d0f16968ef.PNG)


# Features
* Load JPG, PNG, GIF and BMP images
* Scale and translate images on canvas
* Delete images from canvas
* Restore original dimensions of images after translating
* ImageWidgets scale from offset of mouse cursor
* Save/load canvas configurations


# Shortcut Keys
### Global Shortcut Keys
* **F1** - Help Menu
* **Right Mouse + Drag** - Pan canvas
* **CTRL + Right Mouse + Drag** - Accelerate the panning speed
* **C** - Center canvas 
* **A** - Add a new image
* **O** - Open Canvas file
* **CTRL + S** - Save Canvas File
* **R** - Resize Canvas

### ImageWidget Shortcut Keys ( mouse is hovering over a loaded image on the canvas )
* **Mouse Scrollwheel** - Scale image up / down
* **CTRL + Mouse Scrollwheel** - Accelerate Scaling speed
* **D** - Delete an ImageWidget
* **Left Mouse + Drag** - Move image widget
* **Alt + Left Mouse + Drag** - Move image widget without changing z-order
* **F** - Reset image to original dimensions

# Windows building
1. Requires [wxWidgets](https://www.wxwidgets.org/downloads/) GUI library. You will probably need to download and build the library from source. I used wxWidgets-3.1.5 for rCanvas.

2. Make sure the compiler knows where to look for the header file(s) for the library. Open the .sln and navigate to **Project > Properties > Configuration Properties > C/C++ > General**. Then under **Additional Include Directories** add the location for the header files:
    -  `...\wxWidgets-3.1.5\include`
    -  `...\wxWidgets-3.1.5\include\msvc`

3. Tell the linker where to look for the library file(s). Open the .sln and navigate to **Project > Properties > Configuration Properties > Linker > General**. Then under **Additional Library Directories** add path for library files. 
    - `...\wxWidgets-3.1.5\lib\vc_lib`
