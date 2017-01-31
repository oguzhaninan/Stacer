var series = require('../')
var test = require('tape')

test('no callback', function (t) {
  t.plan(2)

  var tasks = [
    function (cb) {
      t.pass('cb 1')
      cb(null)
    },
    function (cb) {
      t.pass('cb 2')
      cb(null)
    }
  ]

  series(tasks)
})
