var ColorModel = require('../lib/colormodel.js');

/**
 * Handles the generation of colors and alphas on a 256 (8 bit) palette
 * @constructor
 */
var ColorEngine = function (colorArray, maxColors) {
  this.colors = colorArray || [];
  this.maxColors = maxColors || 256;
  this._finalColors = [];

  // If no color array was presented, then create one
  if (!colorArray) {

    // WHITE / TRANSPARENT
    this.colors.push(new ColorModel(255, 255, 255, 0));

    // BLUE
    this.colors.push(new ColorModel(0, 0, 255, 60));

    // CYAN
    this.colors.push(new ColorModel(0, 255, 255, 100));

    // GREEN
    this.colors.push(new ColorModel(0, 255, 0, 150));

    // YELLOW
    this.colors.push(new ColorModel(255, 255, 0, 190));

    // RED
    this.colors.push(new ColorModel(255, 0, 0, 220));

  }

  this._colorPositions = [];

  for (var i = 0; i < this.colors.length; i++) {
    this._colorPositions.push(i / ((this.colors.length - 1) + 0.000000001));
  }
  this._colorPositions[0] = 0;
  this._colorPositions[this._colorPositions.length - 1] = 1;

  for (var c = 0; c < this.maxColors; c++) {
    var prog = c / this.maxColors,
      baseIdx = 0;

    for (var d = 0; d < this._colorPositions.length; d++) {
      if (this._colorPositions[d] >= prog) {
        baseIdx = d - 1;
        if (d === 0) {
          baseIdx = 0;
        }
        break;
      }
    }

    prog = (prog - this._colorPositions[baseIdx]) / (this._colorPositions[baseIdx + 1] - this._colorPositions[baseIdx]);

    // Fix the extremes
    if (c === 0) {
      prog = 0;
    } else if (c == this.maxColors - 1) {
      prog = 1;
    }

    this._finalColors.push(ColorModel.calculateMidPoint(this.colors[baseIdx], this.colors[baseIdx + 1], prog));
  }
};

/**
 * Get a color for a level of intensity.
 * @param intensity - Number between 0 and 1
 */
ColorEngine.prototype.getColorForIntensity = function (intensity) {
  intensity = Math.min(1, Math.max(0, intensity));
  if (intensity === 0) {
    return this._finalColors[0];
  } else if (intensity == 1) {
    return this._finalColors[this._finalColors.length - 1];
  } else {
    return this._finalColors[Math.round((this._finalColors.length - 1) * intensity)];
  }
};

// Export it
module.exports = ColorEngine;