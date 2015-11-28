var heatmap = require('../index'),
  fs = require('fs');

var data = [],
  totalpoints = 0;
for (var i = 0; i < 250; i++) {
  var dta = [],
    wt = Math.round(Math.random() * 1000),
    ht = Math.round(Math.random() * 2000),
    points = Math.round(Math.random() * 3000) + 2000;
  totalpoints += points;
  for (var pt = 0; pt < points; pt++) {
    dta.push(0);
    dta.push(Math.round(Math.random() * wt));
    dta.push(Math.round(Math.random() * ht));
  }
  data.push(new heatmap.SparseArray(heatmap.LAYOUTS.CENTERFIXEDWIDTH, wt, ht, dta));
}

var time1 = new Date();
var hm = new heatmap(1000, 1500, heatmap.LAYOUTS.CENTERFIXEDWIDTH, data, heatmap.BLOBTYPE.LARGE, function () {
  var time2 = new Date();
  var res = hm.compile();
  var time3 = new Date();
  hm.getPNG(500, function (dta) {
    var time4 = new Date();
    fs.writeFile("test/test.png", dta, function (err) {
      if (err) {
        return console.log(err);
      }

      console.log("The click heatmap file was saved! Datapoints: ", totalpoints, "Times: ", (time2 - time1), (time3 - time2), (time4 - time3));
    });
  });
});


