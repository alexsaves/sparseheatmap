var matrixcombine = require('bindings')('sparsematrix');

/**
 * Sets up a new heatmap
 * @param width
 * @param height
 * @constructor
 */
var NodeHeatmap = function(width, height) {
    this.result = matrixcombine.add(width, height);
};

// Export it
module.exports = NodeHeatmap;