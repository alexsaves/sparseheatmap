var heatmap = require('../index'),
    fs = require('fs');

var data = [];
for (var i = 0; i < 250; i++) {
    var dta = [],
        ht = 100 + Math.round(Math.random() * 1000);
    for (var d = 0; d < ht; d++) {
        dta.push(Math.round(Math.random() * 400));
    }
    data.push(new heatmap.SparseArray(heatmap.LAYOUTS.VERTICALSCROLL, 500, ht, dta));
}

var time4 = new Date();
var hm = new heatmap(700, 1000, heatmap.LAYOUTS.VERTICALSCROLL, data, function () {
    var time5 = new Date();
    var res = hm.compile();
    var time6 = new Date();
    hm.getPNG(500, function (dta) {
        var time7 = new Date();
        fs.writeFile("test/test2.png", dta, function (err) {
            if (err) {
                return console.log(err);
            }

            console.log("The scroll heatmap file was saved! Times: ", (time5 - time4), (time6 - time5), (time7 - time6));
        });
    });
});


