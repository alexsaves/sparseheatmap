var heatmap = require('../index'),
    fs = require('fs');

heatmap._DEBUGMODE_ = true;

var data = [],
    finalHeatmapWidth = 1500;
for (var i = 0; i < 250; i++) {
    var dta = [];
    var ht = 600 + Math.round(Math.random() * 4000);
    var ht = 1000;
    for (var d = 0; d < ht; d++) {
        //dta.push(Math.round(Math.random() * 400) + 25);
        dta.push(50);
    }
    data.push(new heatmap.SparseArray(heatmap.LAYOUTS.VERTICALSCROLL, 500, ht, dta));
}
console.log("Data is ready. Parsing..");
var time1 = new Date();
var hm = new heatmap(700, 1000, finalHeatmapWidth, heatmap.LAYOUTS.VERTICALSCROLL, data, function (dta) {
    var time2 = new Date();
    fs.writeFile("test/test2.png", dta, function (err) {
        if (err) {
            return console.log(err);
        }

        console.log("The scroll heatmap file was saved! Times: ", (time2 - time1));
    });

});


