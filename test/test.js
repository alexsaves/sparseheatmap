var heatmap = require('../index'),
    fs = require('fs');

heatmap._DEBUGMODE_ = true;
heatmap.FILTER = heatmap.FILTERS.LOWPASS;

var data = [],
    totalpoints = 0,
    finalHeatmapWidth = 1500;
for (var i = 0; i < 250; i++) {
    var dta = [],
        wt = (Math.round(Math.random() * 800) + 400),
        ht = (Math.round(Math.random() * 2000) + 600),
        points = 3000;
    totalpoints += points;
    for (var pt = 0; pt < points; pt++) {
        dta.push(0);
        dta.push(Math.round(Math.random() * wt));
        dta.push(Math.round(Math.random() * ht));
    }
    data.push(new heatmap.SparseArray(heatmap.LAYOUTS.CENTERFIXEDWIDTH, wt, ht, dta));
}
console.log("Data is ready. Parsing..");
var time1 = new Date();
var hm = new heatmap(1000, 1500, finalHeatmapWidth, heatmap.LAYOUTS.CENTERFIXEDWIDTH, data, heatmap.BLOBTYPE.LARGE, function (dta) {
  var time2 = new Date();
  fs.writeFile("test/test.png", dta, function (err) {
    if (err) {
      return console.log(err);
    }
    var time3 = new Date();
    console.log("The click heatmap file was saved! Datapoints: ", totalpoints, "Times: ", (time2 - time1), (time3 - time2));
  });
});


