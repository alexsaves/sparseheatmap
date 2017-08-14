SparseHeatmap
===================
A high-speed heatmap generator using sparse matrices for NodeJS. Uses native extensions to maximize performance.
### Installation &nbsp;  [![npm version](https://badge.fury.io/js/sparseheatmap.svg)](http://badge.fury.io/js/sparseheatmap)
```sh
npm install sparseheatmap
```
### Heatmap Types
Two general types of heatmaps are supported currently: canvas and scrolling. Scrolling heatmaps show only a vertical heatmap of exposure time for a scrolling region of space.

Canvas heatmaps are traditional spacial maps. These can represent any type of 2D coordinate data, but the API is oriented around a UI heatmap and includes convenience features for combining responsive UI's.

![Heatmap types](https://raw.githubusercontent.com/alexsaves/sparseheatmap/master/docs/images/overall_example.png)

### Intensity Normalization
The way intensities are calculated is by additively combining all the individual sparse matrices into a single height-map, and then normalizing by the maximum values. In some cases, this can 'wash out' some of the low-intensity detail. In these cases, try playing with the low-pass filter feature.

By default, no filter is applied. To use a low-pass filter, set the ``FILTER`` attribute:

```javascript
var sparse = require('sparseheatmap');

// sparse.FILTER = sparse.FILTERS.NONE;
sparse.FILTER = sparse.FILTERS.LOWPASS;
```
![Low-pass filters](https://raw.githubusercontent.com/alexsaves/sparseheatmap/master/docs/images/lp_example.png)

### Usage
SparseHeatmap can efficiently combine many individual datasets together into a single heatmap. Think of a dataset (represented by a ``SparseArray``) as a single session. For example, in the case of websites, a single dataset could represent the mouse movements of a particular user. 

Usage varies depending on what type of map you are producing. For regular 2D spacial maps, begin by converting your coordinate data to ``SparseArray`` format like so:

```javascript
var heatmap = require('sparseheatmap');

var data = [];
// Add two sets of data:
data.push(new heatmap.SparseArray(heatmap.LAYOUTS.CENTERFIXEDWIDTH, 600, 800, [1, 23, 94, 1, 103, 582]));
data.push(new heatmap.SparseArray(heatmap.LAYOUTS.CENTERFIXEDWIDTH, 780, 900, [2, 193, 52, 1, 503, 882]));
```

Ultimately, the data you pass to the ``SparseHeatmap`` constructor should be an array of ``SparseArray`` instances. The constructor for ``SparseArray`` is:

```javascript
SparseArray(layout, width, height, coords)
```

The ``layout`` attribute is one of the available ``SparseHeatmap.LAYOUTS`` and the options are:

 * ``CENTERFIXEDWIDTH`` - This is the most common layout for web pages. Use this when you have a fixed-width column in the middle of the page that centers itself as the user resizes the window. Coordinate data will be lined up along a common axis in the middle of the final heatmap.
 * ``STRETCH`` - Use this if the layout is mostly responsive and sizes proportionally with the width of the window. Coordinates will be scaled along the x-axis.
 * ``LEFTFIXEDWIDTH`` - This is for layouts that have a mostly fixed-width but flush with the left-hand side of the UI.
 * ``VERTICALSCROLL`` - This is for scroll heatmaps. This is covered below.
 
The ``width`` and ``height`` attributes are the pixel sizes of the canvas used to collect these particular coordinates. 

The ``coords`` paramters is an array of integers in a ``n``, ``x``, ``y`` sequence. The ``n`` value is the 'amplitude' of this coordinate (usually 1), and ``x``, and ``y`` are the coordinates of the data point. This sequence can repeat in a contiguous stream of coordinates. Amplitudes are for situations where (for example) there are two clicks in exactly the same place for the same dataset.

Once you have an array of ``SparseArray`` instances, pass it to the heatmap constructor:

```javascript
var myHmPNG = new heatmap(canvasWidth, canvasHeight, finalPNGWidth, layout, data, blobtype, callback);
```

The first two attributes: ``canvasWidth`` and ``canvasHeight`` are similar to the width and heights you passed to each ``SparseArray``. They represent the size of the canvas you are projecting all your datapoints onto. The next attribute: ``finalPNGWidth`` is the width of the PNG you want generated. We use a smooth up-sampling technique to ensure that images look smooth at any size. The height of the resulting PNG will be determined by the aspect ratio of the canvas.

The ``layout`` attribute also uses the ``SparseHeatmap.LAYOUTS`` enumerable and should be the same as the value you used for each of the sparse arrays.

Your ``data`` comes from the array of ``SparseArray`` instances. The ``blobtype`` attribute is an enumerable and comes from ``SparseHeatmap.BLOBTYPES`` object. Possible values are:

 * ``SparseHeatmap.BLOBTYPE.LARGE`` - A large blob. Heatmaps will look busier and have bigger markings for coordinate data.
 * ``SparseHeatmap.BLOBTYPE.SMALL`` - A smaller, more precise blob.
 
The ``callback`` attribute, a function, will contain one argument which will be a ``Buffer`` containing the encoded PNG data.

Here is an example:

```javascript
var hm = new heatmap(canvasWidth, canvasHeight, finalPNGWidth, heatmap.LAYOUTS.CENTERFIXEDWIDTH, data, heatmap.BLOBTYPE.LARGE, function (dta) {
  fs.writeFile("canvas.png", dta, function (err) {
    if (err) {
      return console.log(err);
    }    
  });
});
```

Scroll heatmaps are a little different. We currently handle scroll heatmaps in 1 dimension only. Use a SparseArray, but think of the coordinate data as a string of intensities from a particular session. For example, if you wanted to represent the fact that most users scrolled down the page 5 pixels and stayed there (and had a window size of 10 pixels), you might have a lot of datasets that look like:

```javascript
[3, 3, 3, 3, 15, 40, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50]
```

Also, we have the ability to specify a multiplier so that each coordinate actually represents an area of x pixels of screen space. This is for more efficient storage and processing of this data.

```javascript
var heatmap = require('sparseheatmap');

var data = [];
// Add two sets of data:
data.push(new heatmap.SparseArray(heatmap.LAYOUTS.VERTICALSCROLL, 600, 800, [20, 30, 30, 40, 50, ...]));
data.push(new heatmap.SparseArray(heatmap.LAYOUTS.VERTICALSCROLL, 780, 900, [90, 85, 76, 60, 56, ...]));
```

Then, when it comes to building the heatmap itself, you can omit the ``BLOBTYPE`` parameter:

```javascript
var hm = new heatmap(canvasWidth, canvasHeight, finalPNGWidth, heatmap.LAYOUTS.VERTICALSCROLL, data, pixelsPerCoord, function (dta) {
    fs.writeFile("scroll.png", dta, function (err) {
        if (err) {
            return console.log(err);
        }
    });
});
```

In this example, the ``pixelsPerCoord`` attribute represents the compression - the number of y-axis pixels represented by each step in the data from top to bottom.

### Custom Color Maps
A default color map is provided, but you can override it. To do this, override the ``SparseHeatmap.COLORMAP`` array. Populate it with new colors by calling the ``SparseHeatmap.createColorMap(r, g, b, a)`` function which accepts values from between ``0`` and ``255``. Eg:

```javascript
var sparse = require('sparseheatmap');

sparse.COLORMAP = []; // Empty it out

// WHITE / TRANSPARENT
sparse.COLORMAP = sparse.COLORMAP.concat(sparse.createColorMap(255, 255, 255, 0));

// BLUE
sparse.COLORMAP = sparse.COLORMAP.concat(sparse.createColorMap(0, 0, 255, 80));

// CYAN
sparse.COLORMAP = sparse.COLORMAP.concat(sparse.createColorMap(0, 255, 255, 120));

// GREEN
sparse.COLORMAP = sparse.COLORMAP.concat(sparse.createColorMap(0, 255, 0, 160));

// YELLOW
sparse.COLORMAP = sparse.COLORMAP.concat(sparse.createColorMap(255, 255, 0, 200));

// RED
sparse.COLORMAP = sparse.COLORMAP.concat(sparse.createColorMap(255, 0, 0, 230));
```
### Trimming Edge Values
It's possible to pass trimming parameters into your heatmap to omit coordinates on the edges of your datasets. You might to this if there is edge-noise in your data that you want efficiently eliminated. You can specify trimming values on canvas heatmaps only:

```javascript
var hm = new heatmap(canvasWidth, canvasHeight, finalPNGWidth, heatmap.LAYOUTS.CENTERFIXEDWIDTH, data, heatmap.BLOBTYPE.LARGE, function (dta) {
  fs.writeFile("canvas.png", dta, function (err) {
    if (err) {
      return console.log(err);
    }    
  });
}, trimPixelsLeft, trimPixelsTop, trimPixelsRight, trimPixelsBottom);
```

The trim values are in pixels and specified after your callback.

### Debug Mode
Additional console messages (including timings) can be made available by turning on the ``SparseHeatmap._DEBUGMODE_`` boolean.
```javascript
var sparse = require('sparseheatmap');
sparse._DEBUGMODE_ = true;
```
