var series = require('../')
var test = require('tape')

test('empty tasks array', function (t) {
  t.plan(1)

  series([], function (err) {
    t.error(err)
  })
})

test('empty tasks array and no callback', function (t) {
  series([])
  t.pass('did not throw')
  t.end()
})
