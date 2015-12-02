var heatmap = require('../index'),
    fs = require('fs');

heatmap._DEBUGMODE_ = true;
heatmap.FILTER = heatmap.FILTERS.LOWPASS;

var data = [],
    totalpoints = 0,
    finalHeatmapWidth = 1000,
    coord1 = {
        x: 250,
        y: 200
    }, coord2 = {
        x: -175,
        y: 300
    };

for (var i = 0; i < 250; i++) {
    var dta = [],
        wt = (Math.round(Math.random() * 800) + 400),
        ht = (Math.round(Math.random() * 2000) + 600),
        points = 3000;
    totalpoints += points;
    for (var pt = 0; pt < points; pt++) {
        dta.push(0);
        dta.push((wt / 2) + coord1.x);
        dta.push(coord1.y);
        dta.push(0);
        dta.push((wt / 2) + coord2.x);
        dta.push(coord2.y);
    }
    data.push(new heatmap.SparseArray(heatmap.LAYOUTS.CENTERFIXEDWIDTH, wt, ht, dta));
}
console.log("Data is ready. Parsing..");
var time1 = new Date();
var hm = new heatmap(1000, 1500, finalHeatmapWidth, heatmap.LAYOUTS.CENTERFIXEDWIDTH, data, heatmap.BLOBTYPE.SMALL, function (dta) {
    var time2 = new Date();
    fs.writeFile("test/alignment.png", dta, function (err) {
        if (err) {
            return console.log(err);
        }
        var time3 = new Date();
        console.log("The click heatmap file was saved! Datapoints: ", totalpoints, "Times: ", (time2 - time1), (time3 - time2));
    });
});