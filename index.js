var matrixcombine = require('bindings')('sparsematrix'),
    layouts = require('./layouts.js'),
    sparsearray = require('./sparsearray.js');

/**
 * Sets up a new heatmap
 * @param width (Number) The width of the final heatmap
 * @param height (Number) the height of the final heatmap
 * @param layout (NodeHeatmap.LAYOUTS) the layout of the page
 * @param arrayofsparsearrays (Array{NodeHeatmap.SparseArray}) array of sparse arrays
 * @constructor
 */
var NodeHeatmap = function (width, height, layout, arrayofsparsearrays) {
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
        this._compiledData = matrixcombine.compile(this.width, this.height);
    }
    return this._compiledData;
};

// Export it
module.exports = NodeHeatmap;