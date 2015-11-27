var heatmap = require('./index');

var data = [];
data.push(new heatmap.SparseArray(501, 502, new Array(1, 23, 94, 1, 83, 102, 1, 230, 339)));
data.push(new heatmap.SparseArray(403, 604, new Array(1, 13, 94, 1, 183, 202, 1, 331, 19)));
data.push(new heatmap.SparseArray(705, 306, new Array(1, 93, 94, 1, 203, 301, 1, 201, 210)));
data.push(new heatmap.SparseArray(407, 908, new Array(1, 63, 94, 1, 301, 9, 1, 109, 421)));

var hm = new heatmap(100, 250, heatmap.LAYOUTS.CENTERFIXEDWIDTH, data, heatmap.BLOBTYPE.LARGE, function () {
    var res = hm.compile();
    console.log("The result is ", res);
});


