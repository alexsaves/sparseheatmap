/**
 * Created by whitehawk on 2015-11-26.
 */

var SparseArray = function (width, height, dataarray) {
    this.width = width;
    this.height = height;
    this.max = 0;
    this.data = dataarray;
    if (!width || !height || !(dataarray.length)) {
        throw new Error("Required arguments: width (int), height (int), and dataArray (Array)");
    }
    if (width < 1 || height < 1) {
        throw new Error("Width and height must be non-zero.");
    }
    if (dataarray.length % 3 === 0) {
        throw new Error("The data array must be an ordered list of UInt32's in the format of: n, x, y.");
    }
};

// Export it
module.exports = SparseArray;