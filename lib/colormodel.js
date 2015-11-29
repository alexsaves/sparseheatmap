/**
 * Create a new color
 * @param r
 * @param g
 * @param b
 * @param a
 * @constructor
 */
var ColorModel = function (r, g, b, a) {
  this.r = typeof(r) == 'undefined' ? 0 : Math.min(255, Math.max(r, 0));
  this.g = typeof(g) == 'undefined' ? 0 : Math.min(255, Math.max(g, 0));
  this.b = typeof(b) == 'undefined' ? 0 : Math.min(255, Math.max(b, 0));
  this.a = typeof(a) == 'undefined' ? 255 : Math.min(255, Math.max(a, 0));
  this.r_byte = Math.min(255, Math.max(Math.round(this.r), 0));
  this.g_byte = Math.min(255, Math.max(Math.round(this.g), 0));
  this.b_byte = Math.min(255, Math.max(Math.round(this.b), 0));
  this.a_byte = Math.min(255, Math.max(Math.round(this.a), 0));
};

/**
 * Calculate a color between two other colors
 * @param colorA - On the left
 * @param colorB - On the right
 * @param percentage - Number between 0 and 1.. Fraction between the two
 */
ColorModel.calculateMidPoint = function (colorA, colorB, percentage) {
  percentage = Math.max(0, Math.min(1, percentage));
  return new ColorModel(((colorB.r - colorA.r) * percentage) + colorA.r, ((colorB.g - colorA.g) * percentage) + colorA.g, ((colorB.b - colorA.b) * percentage) + colorA.b, ((colorB.a - colorA.a) * percentage) + colorA.a);
};

// Export it
module.exports = ColorModel;