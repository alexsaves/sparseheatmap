var matrixcombine = require('bindings')('sparsematrix'),
  layouts = require('./lib/layouts.js'),
  sparsearray = require('./lib/sparsearray.js'),
  Jimp = require("jimp"),
  ColorEngine = require('./lib/colorengine.js'),
  pjson = require('./package.json');

/**
 * Sets up a new heatmap
 * @param width (Number) The width of the canvas to apply the heatmap
 * @param height (Number) The height of the canvas to apply the heatmap
 * @param imageWidth (Number) The width of the final image to generate. Note: the height will be computed based on aspect ratio.
 * @param layout (NodeHeatmap.LAYOUTS) the layout of the page
 * @param arrayofsparsearrays (Array{NodeHeatmap.SparseArray}) array of sparse arrays
 * @param blobtype {NodeHeatmap.BLOBTYPE} The type of blob to draw with
 * @param initcallback (Function) Callback to fire after initialization is complete
 * @params trimPixelsLeft, trimPixelsTop, trimPixelsRight, trimPixelsBottom - Trim values (optional)
 * @constructor
 */
var NodeHeatmap = function (width, height, imageWidth, layout, arrayofsparsearrays, blobtype, initcallback, trimPixelsLeft, trimPixelsTop, trimPixelsRight, trimPixelsBottom) {
  if (NodeHeatmap._DEBUGMODE_) {
    console.log("SparseHeatmap DEBUG: " + pjson.name + " " + pjson.version + ".");
  }
  var instructions = "Usage: var hm = new NodeHeatmap(width, height, layout, arrayofsparsearrays, blobtype or yaxismultiplier, initcallback);\n\n";
  if (!width || !height || width < 1 || height < 1) {
    throw new Error(instructions + "Please provide non-zero width and height.");
  }
  this.width = width;
  this.height = height;
  this.imageWidth = imageWidth;
  this.max = 0;
  this._startTime = new Date();
  this.times = {};
  this._colorSets = NodeHeatmap.COLORMAP;
  this.trimPixelsLeft = Math.max(trimPixelsLeft || 0);
  this.trimPixelsTop = Math.max(trimPixelsTop || 0);
  this.trimPixelsRight = Math.max(trimPixelsRight || 0);
  this.trimPixelsBottom = Math.max(trimPixelsBottom || 0);

  if (layout === layouts.VERTICALSCROLL) {
    this.yaxismultiplier = blobtype;
  }

  var fountLayout = false,
    possibleLayouts = [];
  for (var l in NodeHeatmap.LAYOUTS) {
    if (NodeHeatmap.LAYOUTS[l] === layout) {
      fountLayout = true;
      break;
    } else {
      possibleLayouts.push("SparseHeatmap.LAYOUTS." + l);
    }
  }
  if (!fountLayout) {
    throw new Error(instructions + "Missing: layout. Please use one of: " + possibleLayouts.join(', '));
  }
  this.layout = layout;
  if (!arrayofsparsearrays || !(arrayofsparsearrays instanceof Array)) {
    throw new Error(instructions + "Missing: an array of sparse arrays.");
  }
  if (arrayofsparsearrays.length === 0) {
    throw new Error(instructions + "Empty array of sparse arrays provided.");
  }
  if (arrayofsparsearrays.length > 0 && !(arrayofsparsearrays[0] instanceof NodeHeatmap.SparseArray)) {
    throw new Error(instructions + "The array of data must be instances of SparseHeatmap.SparseArray.");
  }
  this.data = arrayofsparsearrays;
  this._compiledData = null;
  if (!initcallback || typeof(initcallback) != 'function') {
    throw new Error(instructions + "Missing initialization callback.");
  }
  this.initCallback = initcallback;
  if (typeof(blobtype) != 'string' && layout !== layouts.VERTICALSCROLL) {
    throw new Error(instructions + "Missing blobtype.");
  }
  this.blobtype = blobtype;
  this._blobImg = [];
  this._blobWidth = 0;
  this._blobHeight = 0;
  var ctx = this;
  if (layout !== layouts.VERTICALSCROLL) {
    Jimp.read(__dirname + "/assets/" + blobtype, function (err, blobimg) {
      if (err) {
        throw err;
      }
      ctx._blobWidth = blobimg.bitmap.width;
      ctx._blobHeight = blobimg.bitmap.height;
      var max = 0;
      blobimg.scan(0, 0, blobimg.bitmap.width, blobimg.bitmap.height, function (x, y, idx) {
        // x, y is the position of this pixel on the image
        // idx is the position start position of this rgba tuple in the bitmap Buffer
        // this is the image

        var red = this.bitmap.data[idx + 0];
        /*var green = this.bitmap.data[idx + 1];
         var blue = this.bitmap.data[idx + 2];
         var alpha = this.bitmap.data[idx + 3];*/
        max = Math.max(max, red);
        ctx._blobImg.push(red);

        // rgba values run from 0 - 255
        // e.g. this.bitmap.data[idx] = 0; // removes red from this pixel
      });
      // Normalize to 20 so the numbers are a bit smaller
      for (var k = 0; k < ctx._blobImg.length; k++) {
        ctx._blobImg[k] = Math.round((ctx._blobImg[k] / max) * 20);
      }
      ctx._initTime = new Date();
      ctx._getPNG(imageWidth, initcallback);
    });
  } else {
    setTimeout(function () {
      ctx._initTime = new Date();
      ctx._getPNG(imageWidth, initcallback);
    }, 20);
  }
};

/**
 * Are we in debug mode?
 * @type {boolean}
 * @private
 */
NodeHeatmap._DEBUGMODE_ = false;

/**
 * A custom color map (optional)
 * @type {null}
 */
NodeHeatmap.COLORMAP = [];

/**
 * Set the enumerable LAYOUTS
 */
NodeHeatmap.LAYOUTS = layouts;

/**
 * Possible filters
 * @type {{NONE: number, LOWPASS: number}}
 */
NodeHeatmap.FILTERS = {
  NONE: 0,
  LOWPASS: 1
};

/**
 * The filter to use
 * @type {number}
 */
NodeHeatmap.FILTER = NodeHeatmap.FILTERS.NONE;

/**
 * Make available the sparse array class
 */
NodeHeatmap.SparseArray = sparsearray;

/**
 * Describes the type of blob
 * @type {{LARGE: string, SMALL: string}}
 */
NodeHeatmap.BLOBTYPE = {
  LARGE: "largeblob.png",
  SMALL: "smallblob.png"
};

/**
 * Add a color to the color set
 * @param r
 * @param g
 * @param b
 * @param a
 * @private
 */
NodeHeatmap.createColorMap = function (r, g, b, a) {
  var cSets = [];
  cSets.push(r);
  cSets.push(g);
  cSets.push(b);
  cSets.push(a);
  return cSets
};

/**
 * Get the area
 * @returns {number}
 */
NodeHeatmap.prototype.area = function () {
  return this.width * this.height;
};

/**
 * Compile the heatmap
 * @private
 */
NodeHeatmap.prototype._compile = function () {
  if (!this._compiledData) {
    this._compileStartTime = new Date();
    if (this.layout === layouts.VERTICALSCROLL) {
      this._compiledData = matrixcombine.compile_vertical_scroll(this.width, this.height, this.data, this.imageWidth, this.yaxismultiplier, this._colorSets, NodeHeatmap._DEBUGMODE_ ? 1 : 0, NodeHeatmap.FILTER);
    } else {
      this._compiledData = matrixcombine.compile_canvas(this.width, this.height, this.layout, this.data, this._blobWidth, this._blobHeight, this._blobImg, this.imageWidth, this._colorSets, NodeHeatmap._DEBUGMODE_ ? 1 : 0, NodeHeatmap.FILTER, this.trimPixelsLeft, this.trimPixelsTop, this.trimPixelsRight, this.trimPixelsBottom);
    }
    this._compileEndTime = new Date();
  }
  return this._compiledData;
};

/**
 * Get Y
 */
NodeHeatmap.prototype.getYScroll = function (y) {
  if (y < 0 || y >= this.height) {
    return 0;
  }
  return this._compiledData[y];
};

/**
 * Get Y float
 */
NodeHeatmap.prototype.getFloatScrollY = function (y) {
  var y1 = Math.floor(y),
    y2 = Math.ceil(y),
    yprog = y - y1,
    cy1 = this.getYScroll(y1),
    cy2 = this.getYScroll(y2),
    totalsum = (cy1 * (1 - yprog)) + (cy2 * yprog);

  return totalsum;
};

/**
 * Get XY
 */
NodeHeatmap.prototype.getXY = function (x, y) {
  if (x < 0 || y < 0 || x >= this.width || y >= this.height) {
    return 0;
  }
  return this._compiledData[(y * this.width) + x];
};

/**
 * Get XY float
 */
NodeHeatmap.prototype.getFloatXY = function (x, y) {
  var x1 = Math.floor(x),
    x2 = Math.ceil(x),
    y1 = Math.floor(y),
    y2 = Math.ceil(y),
    xprog = x - x1,
    yprog = y - y1,
    x1y1 = this.getXY(x1, y1),
    x1y2 = this.getXY(x1, y2),
    x2y1 = this.getXY(x2, y1),
    x2y2 = this.getXY(x2, y2),
    y1sum = (x1y1 * (1 - xprog)) + (x2y1 * xprog),
    y2sum = (x1y2 * (1 - xprog)) + (x2y2 * xprog),
    totalsum = (y1sum * (1 - yprog)) + (y2sum * yprog);

  return totalsum;

};

/**
 * Convert buffer to an array (slow)
 * @param buf
 */
NodeHeatmap.prototype._bufferToArray = function (buf) {
  var res = [],
    blen = buf.length;
  for (var i = 0; i < blen; i++) {
    res[i] = buf[i];
  }
  return res;
};

/**
 * Get a png of the heatmap assuming a page width and height
 * @param width - image width
 * @param callback
 * @private
 */
NodeHeatmap.prototype._getPNG = function (imageWidth, callback) {
  // Compile things just in case
  var resData = this._compile(),
    imageHeight = (this.width / imageWidth) * this.height;

  // Compile some times
  this.times.initTime = this._initTime - this._startTime;
  this.times.compileTime = this._compileEndTime - this._compileStartTime;

  if (NodeHeatmap._DEBUGMODE_) {
    console.log("SparseHeatmap DEBUG: Result data length: ", resData.length, ". Dimensions: ", imageWidth + ", " + imageHeight, " (" + imageHeight + ").");
  }

  this.times.total = this.times.initTime + this.times.compileTime;
  if (NodeHeatmap._DEBUGMODE_) {
    console.log("SparseHeatmap DEBUG:", this.times);
  }
  callback(resData);
};

// **************** Set up the color maps

// WHITE / TRANSPARENT
NodeHeatmap.COLORMAP = NodeHeatmap.COLORMAP.concat(NodeHeatmap.createColorMap(255, 255, 255, 0));

// BLUE
NodeHeatmap.COLORMAP = NodeHeatmap.COLORMAP.concat(NodeHeatmap.createColorMap(0, 0, 255, 80));

// CYAN
NodeHeatmap.COLORMAP = NodeHeatmap.COLORMAP.concat(NodeHeatmap.createColorMap(0, 255, 255, 120));

// GREEN
NodeHeatmap.COLORMAP = NodeHeatmap.COLORMAP.concat(NodeHeatmap.createColorMap(0, 255, 0, 160));

// YELLOW
NodeHeatmap.COLORMAP = NodeHeatmap.COLORMAP.concat(NodeHeatmap.createColorMap(255, 255, 0, 200));

// RED
NodeHeatmap.COLORMAP = NodeHeatmap.COLORMAP.concat(NodeHeatmap.createColorMap(255, 0, 0, 230));

// Export it
module.exports = NodeHeatmap;