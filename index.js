var matrixcombine = require('bindings')('sparsematrix'),
    layouts = require('./layouts.js'),
    sparsearray = require('./sparsearray.js'),
    Jimp = require("jimp");

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
    var instructions = "Usage: var hm = new NodeHeatmap(width, height, layout, arrayofsparsearrays);\n\n";
    if (!width || !height || width < 1 || height < 1) {
        throw new Error(instructions + "Please provide non-zero width and height.");
    }
    this.width = width;
    this.height = height;
    this.max = 0;

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
    if (typeof(blobtype) != 'string') {
        throw new Error(instructions + "Missing blobtype.");
    }
    this.blobtype = blobtype;
    this._blobImg = [];
    this._blobWidth = 0;
    this._blobHeight = 0;
    var ctx = this;
    Jimp.read("./assets/" + blobtype, function (err, blobimg) {
        if (err) {
            throw err;
        }
        ctx._blobWidth = blobimg.bitmap.width;
        ctx._blobHeight = blobimg.bitmap.height;
        blobimg.scan(0, 0, blobimg.bitmap.width, blobimg.bitmap.height, function (x, y, idx) {
            // x, y is the position of this pixel on the image
            // idx is the position start position of this rgba tuple in the bitmap Buffer
            // this is the image

            var red = this.bitmap.data[idx + 0];
            var green = this.bitmap.data[idx + 1];
            var blue = this.bitmap.data[idx + 2];
            var alpha = this.bitmap.data[idx + 3];
            ctx._blobImg.push(red);

            // rgba values run from 0 - 255
            // e.g. this.bitmap.data[idx] = 0; // removes red from this pixel
        });
        initcallback.apply(ctx);
    });
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
        this._compiledData = matrixcombine.compile(this.width, this.height, this.layout, this.data, this._blobWidth, this._blobHeight, this._blobImg);
    }
    return this._compiledData;
};

// Export it
module.exports = NodeHeatmap;