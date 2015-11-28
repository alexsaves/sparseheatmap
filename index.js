var matrixcombine = require('bindings')('sparsematrix'),
  layouts = require('./lib/layouts.js'),
  sparsearray = require('./lib/sparsearray.js'),
  Jimp = require("jimp"),
  ColorEngine = require('./lib/colorengine.js');

/**
 * Sets up a new heatmap
 * @param width (Number) The width of the final heatmap
 * @param height (Number) the height of the final heatmap
 * @param layout (NodeHeatmap.LAYOUTS) the layout of the page
 * @param arrayofsparsearrays (Array{NodeHeatmap.SparseArray}) array of sparse arrays
 * @param blobtype {NodeHeatmap.BLOBTYPE} The type of blob to draw with
 * @param initcallback (Function) Callback to fire after initialization is complete
 * @constructor
 */
var NodeHeatmap = function (width, height, layout, arrayofsparsearrays, blobtype, initcallback) {
  var instructions = "Usage: var hm = new NodeHeatmap(width, height, layout, arrayofsparsearrays, blobtype, initcallback);\n\n";
  if (!width || !height || width < 1 || height < 1) {
    throw new Error(instructions + "Please provide non-zero width and height.");
  }
  this.width = width;
  this.height = height;
  this.max = 0;

  if (layout === layouts.VERTICALSCROLL) {
    initcallback = blobtype;
    blobtype = null;
  }

  var fountLayout = false,
    possibleLayouts = [];
  for (var l in NodeHeatmap.LAYOUTS) {
    if (NodeHeatmap.LAYOUTS[l] === layout) {
      fountLayout = true;
      break;
    } else {
      possibleLayouts.push("NodeHeatmap.LAYOUTS." + l);
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
  if (!(arrayofsparsearrays[0] instanceof NodeHeatmap.SparseArray)) {
    throw new Error(instructions + "The array of data must be instances of NodeHeatmap.SparseArray.");
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
        var green = this.bitmap.data[idx + 1];
        var blue = this.bitmap.data[idx + 2];
        var alpha = this.bitmap.data[idx + 3];
        max = Math.max(max, red);
        ctx._blobImg.push(red);

        // rgba values run from 0 - 255
        // e.g. this.bitmap.data[idx] = 0; // removes red from this pixel
      });
      // Normalize to 20 so the numbers are a bit smaller
      for (var k = 0; k < ctx._blobImg.length; k++) {
        ctx._blobImg[k] = Math.round((ctx._blobImg[k] / max) * 50);
      }
      initcallback.apply(ctx);
    });
  } else {
    setTimeout(function () {
      initcallback.apply(ctx);
    }, 20);
  }
};

/**
 * Set the enumerable LAYOUTS
 */
NodeHeatmap.LAYOUTS = layouts;

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
 * Get the area
 * @returns {number}
 */
NodeHeatmap.prototype.area = function () {
  return this.width * this.height;
};

/**
 * Compile the heatmap
 */
NodeHeatmap.prototype.compile = function () {
  if (!this._compiledData) {
    if (this.layout === layouts.VERTICALSCROLL) {
      this._compiledData = matrixcombine.compile_vertical_scroll(this.height, this.data);
    } else {
      this._compiledData = matrixcombine.compile_canvas(this.width, this.height, this.layout, this.data, this._blobWidth, this._blobHeight, this._blobImg);
    }
    var cd = this._compiledData,
      cdl = cd.length;
    this.max = 0.00000001;
    for (var i = 0; i < cdl; i++) {
      this.max = Math.max(this.max, cd[i]);
    }
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
 * Get a png of the heatmap assuming a page width and height
 * @param width - image width
 * @param callback
 */
NodeHeatmap.prototype.getPNG = function (imageWidth, callback) {
  // Compile things just in case
  this.compile();
  var tctx = this;
  var imageHeight = Math.round((this.height / this.width) * imageWidth),
    ctx = this,
    image = new Jimp(imageWidth, imageHeight, function (err, image) {
      image.rgba(true);

      var ce = new ColorEngine(),
        idx = 0,
        clr,
        ph = imageHeight,
        pw = imageWidth,
        scaleFactor = imageWidth / ctx.width,
        incVal = 1 / scaleFactor,
        data = image.bitmap.data,
        maxVal = ctx.max;

      if (tctx.layout === layouts.VERTICALSCROLL) {
        for (var y = 0; y < imageHeight; y += 1) {
          var intensity = ctx.getFloatScrollY(y * incVal) / maxVal,
            clr = ce.getColorForIntensity(intensity);
          if (clr) {
            for (var x = 0; x < imageWidth; x += 1) {
              idx = (imageWidth * y + x) << 2;
              data[idx] = clr.r_byte;
              data[idx + 1] = clr.g_byte;
              data[idx + 2] = clr.b_byte;
              data[idx + 3] = clr.a_byte;
            }
          }
        }
      } else {
        for (var y = 0; y < imageHeight; y += 1) {
          for (var x = 0; x < imageWidth; x += 1) {
            var intensity = ctx.getFloatXY(x * incVal, y * incVal) / maxVal;
            clr = ce.getColorForIntensity(intensity);
            if (clr) {
              idx = (imageWidth * y + x) << 2;
              data[idx] = clr.r_byte;
              data[idx + 1] = clr.g_byte;
              data[idx + 2] = clr.b_byte;
              data[idx + 3] = clr.a_byte;
            }
          }
        }
      }
      image.getBuffer(Jimp.MIME_PNG, function (err, buf) {
        callback(buf);
      });
    });
};


// Export it
module.exports = NodeHeatmap;