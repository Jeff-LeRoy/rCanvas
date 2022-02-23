# rCanvas
rCanvas is a minimilistic image viewer with the advantage that you can load multiple images onto a single canvas for reference. It was designed around the needs of digital artist where often times it is benifical to have a collage of reference images instead of flipping back and forth through images, tabs and folders.  

# Examples
A few examples with placeholder images, imagine it with real reference images.
![Capture](https://user-images.githubusercontent.com/7757495/155215023-f28c532b-1cd4-4d01-85d3-92d0f16968ef.PNG)

# Features
* Load .jpg images
* Scale and translate images on canvas
* Delete images from canvas
* Restore original dimensions of images ( it is scaled to not be larger than viewable canvas on import )
### Imminent Features
* Support for other image formats
* Save/load canvas configurations
* Lock Images from transforms

# Shortcut Keys
### Global Shortcut Keys
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
