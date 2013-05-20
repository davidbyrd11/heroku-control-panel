/**
 * @fileoverview Wrapper for Subset of Heroku API.
 */

var exec = require('child_process').exec

/**
 * Wrapper for part of the Heroku API.
 * @constructor {Heroku}
 */
function Application (options) {
  this.appname = options.appname
  this.key = options.key
}

/**
 * Scales the number of dynos for this application.
 * @param {Number} number of web dynos you want to scale to.
 * @param {Function} callback to handle Heroku Response.
 */
Application.prototype.scale = function (qty, callback) {
  var curl = 'curl -H "Accept: application/json" -u :' + this.key + ' -d "type=web" -d "qty=' + qty + '" -X POST https://api.heroku.com/apps/' + this.appname + '/ps/scale'
  exec(curl, callback)
}

/**
 * Restarts the application.
 * @param {Function} callback to handle Heroku Response.
 */
Application.prototype.restart = function (callback) {
  var curl = 'curl -H "Accept: application/json" -u :' + this.key + ' -X POST https://api.heroku.com/apps/' + this.appname + '/ps/restart'
  exec(curl, callback)
}

module.exports = Application
