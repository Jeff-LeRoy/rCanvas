# rCanvas
rCanvas is a minimilistic image viewer with the advantage that you can load multiple images onto a single canvas for reference. It was designed around the needs of digital artist where often times it is benifical to have a collage of reference images instead of flipping back and forth through images, tabs and folders.  

# Current Release
[v0.3.0-alpha](https://github.com/Jeff-LeRoy/rCanvas/releases) The main feature that still needs to be implimented is saving/loading a user canvas. Some performance improvements to scaling images and other misc tweaks are planned for the next and probably last alpha release. Other than that everything is working well.

# Examples
A few examples with placeholder images, imagine it with real reference images.
![Capture](https://user-images.githubusercontent.com/7757495/155215023-f28c532b-1cd4-4d01-85d3-92d0f16968ef.PNG)

# Features
* Load JPG, PNG, GIF and BMP images
* Scale and translate images on canvas
* Delete images from canvas
* Restore original dimensions of images after translating
* ImageWidgets scale from offset of mouse cursor
### Imminent Features
* Save/load canvas configurations
* Lock Images from transforms

# Shortcut Keys
### Global Shortcut Keys
* **F1** - Help Menu
* **Right Mouse + Drag** - Pan canvas
* **CTRL + Right Mouse + Drag** - Accelerate the panning speed
* **a** - Center canvas 
* **o** - Open a new image

### ImageWidget Shortcut Keys ( mouse is hovering over a loaded image on the canvas )
* **Mouse Scrollwheel** - Scale image up / down
* **CTRL + Mouse Scrollwheel** - Accelerate Scaling speed
* **d** - Delete an ImageWidget
* **Left Mouse + Drag** - Move image widget
* **Alt + Left Mouse + Drag** - Move image widget without changing z-order
* **f** - Reset image to original dimensions

# Windows building
1. Requires [wxWidgets](https://www.wxwidgets.org/downloads/) GUI library. You will probably need to download and build the library from source. I used wxWidgets-3.1.5 for rCanvas.

2. Make sure the compiler knows where to look for the header file(s) for the library. Open the .sln and navigate to **Project > Properties > Configuration Properties > C/C++ > General**. Then under **Additional Include Directories** add the location for the header files:
    -  `...\wxWidgets-3.1.5\include`
    -  `...\wxWidgets-3.1.5\include\msvc`

3. Tell the linker where to look for the library file(s). Open the .sln and navigate to **Project > Properties > Configuration Properties > Linker > General**. Then under **Additional Library Directories** add path for library files. 
    - `...\wxWidgets-3.1.5\lib\vc_lib`
