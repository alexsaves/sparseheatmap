/**
 * Created by whitehawk on 2015-11-26.
 */
var layouts = require('../lib/layouts.js')

var SparseArray = function (layout, width, height, dataarray) {
  var instructions = "Constructor for SparseArray. Eg: var a = new SparseArray(layout, width, height, data);\n\n";
  this.width = width;
  this.height = height;
  this.data = dataarray;
  if (typeof(layout) != 'number' || typeof(width) != 'number' || typeof(height) != 'number' || !(dataarray instanceof Array)) {
    throw new Error(instructions + "Required arguments: layout (sparseheatmap.LAYOUT), width (Number), height (Number), and dataarray (Array).");
  }
  if (width < 1 || height < 1) {
    throw new Error(instructions + "Width and height must be non-zero. width: " + width + ", height: " + height + ".");
  }
  if (dataarray.length % 3 !== 0 && layout != layouts.VERTICALSCROLL) {
    throw new Error(instructions + "The data array must be an ordered list of UInt32's in the format of: n, x, y.");
  }
  if (dataarray.length > 0 && typeof(dataarray[0]) != 'number') {
    throw new Error("The data array must contain Numbers. Seems to contain '" + typeof(dataarray[0]) + "'.");
  }
};

// Export it
module.exports = SparseArray;