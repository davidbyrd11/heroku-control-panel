var config = require('../config.json')
  
  /**
   * Serial Port wraper for communicating with the Arduino.
   */
  , SerialPort = require('serialport').SerialPort
  , sp = new SerialPort(config.serialport, {
        baudrate: 9600
      , parser: require('serialport').parsers.readline("\n") 
    })

  /**
   * Application is a tiny Heroku API wrapper.
   */
  , Application = require('./heroku')
  , app = new Application({
        appname: config.appname
      , key: config.herokukey
    })

sp.on('open', function () {
  console.log('Connection Opened ...')
  sp.on('data', function (data) {
    var cmd = data.substring(0, 6)
      , val = data[6]
      , confirm = val+"\n"

    if (cmd == 'dynos:')
      app.scale(val, function (err, data) {
        !err ? sp.write(confirm) : console.log("Error: dyno scale failed.")
      })
        
    if (cmd == 'reset:' && val)
      app.restart(function (err, data) {
        !err ? sp.write("100\n") : console.log("Error: app reset failed.")
      })
    
  })
})



