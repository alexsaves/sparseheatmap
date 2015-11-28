var heatmap = require('../index'),
    fs = require('fs');

var data = [];
data.push(new heatmap.SparseArray(500, 502, new Array(1, 23, 94, 1, 83, 102, 1, 230, 339)));
data.push(new heatmap.SparseArray(403, 604, new Array(1, 13, 94, 1, 183, 202, 1, 331, 19)));
data.push(new heatmap.SparseArray(705, 306, new Array(1, 93, 94, 1, 203, 301, 1, 201, 210)));
data.push(new heatmap.SparseArray(407, 908, new Array(1, 63, 94, 1, 301, 9, 1, 109, 421)));
var time1 = new Date();
var hm = new heatmap(700, 1000, heatmap.LAYOUTS.VERTICALSCROLL, data, function () {
    var time2 = new Date();
    var res = hm.compile();
    var time3 = new Date();
    hm.getPNG(500, function (dta) {
        var time4 = new Date();
        fs.writeFile("test/test2.png", dta, function (err) {
            if (err) {
                return console.log(err);
            }

            console.log("The scroll heatmap file was saved! Times: ", (time2 - time1), (time3 - time2), (time4 - time3));
        });
    });
});


