SparseHeatmap
===================
A high-speed heatmap generator using sparse matrices for NodeJS. Uses native extensions to maximize performance.
###Installation &nbsp;  [![npm version](https://badge.fury.io/js/sparseheatmap.svg)](http://badge.fury.io/js/sparseheatmap)
```sh
npm install sparseheatmap
```
###Heatmap Types
Two general types of heatmaps are supported currently: canvas and scrolling. Scrolling heatmaps show only a vertical heatmap of exposure time for a scrolling region of space.

Canvas heatmaps are traditional spacial maps. These can represent any type of 2D coordinate data, but the API is oriented around a UI heatmap and includes convenience features for combining responsive UI's.

![alt tag](https://raw.githubusercontent.com/alexsaves/sparseheatmap/master/docs/images/overall_example.png)

###Intensity Normalization
The way intensities are calculated is by additively combining all the individual sparse matrices into a single height-map, and then normalizing by the maximum values. In some cases, this can 'wash out' some of the low-intensity detail. In these cases, try playing with the low-pass filter feature.

By default, no filter is applied. To use a low-pass filter, set the ``FILTER`` attribute:

```javascript
var sparse = require('sparseheatmap');

// sparse.FILTER = sparse.FILTERS.NONE;
sparse.FILTER = sparse.FILTERS.LOWPASS;
```
![alt tag](https://raw.githubusercontent.com/alexsaves/sparseheatmap/master/docs/images/lowpass_example.png)

###Custom Color Maps
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

###Debug Mode
Additional console messages (including timings) can be made available by turning on the ``SparseHeatmap._DEBUGMODE_`` boolean.
```javascript
var sparse = require('sparseheatmap');
sparse._DEBUGMODE_ = true;
```
