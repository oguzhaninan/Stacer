var fs = require('fs');
var _ = require('lodash');


// Split array to smaller arrays containing n elements at max
function groupToElements(array, n) {
  var lists = _.groupBy(array, function(element, index){
    return Math.floor(index / n);
  });

  return _.toArray(lists);
}

function endsWith(str, suffix) {
  return str.indexOf(suffix, str.length - suffix.length) !== -1;
}

// Setup karma configuration dynamically to run browsers sequentially
// https://github.com/karma-runner/karma-sauce-launcher/issues/8
var MAXIMUM_CONCURRENT_SAUCE = 3;
var sauceBrowsers = require('./saucelabs-browsers');
var browserGroups = groupToElements(Object.keys(sauceBrowsers), MAXIMUM_CONCURRENT_SAUCE);

var karmaConfig = {
  options: {
    configFile: 'karma.conf.js'
  }
};

_.each(browserGroups, function(group, index) {
  karmaConfig['sauce' + index] = {
    browsers: group
  }
});


module.exports = function(grunt) {
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    jshint: {
      files: ['src/*.js', 'test/**/*.js'],
        options: {
          globals: {
            jQuery: true,
            module: true
          }
        }
    },
    shell: {
      stageMinified: {
        options: {
            stdout: true,
            // If minified files do not change on release,
            // git command fails
            failOnError: false
        },
        command: 'git add dist/progressbar.js dist/progressbar.min.js dist/progressbar.min.js.map'
      },
      browserifyDevelopment: {
        options: {
          stdout: true
        },
        command: 'browserify src/main.js -o dist/progressbar.js --standalone ProgressBar'
      },
      browserifyMinified: {
        options: {
          stdout: true
        },
        // The output file is minified by uglifyjs later.
        command: 'browserify src/main.js -o dist/progressbar.min.js --standalone ProgressBar'
      },
      watchifyLocalDev: {
        options: {
          stdout: true
        },
        command: 'watchify local-dev/main.js -o local-dev/bundle.js --debug --verbose'
      },
      release: {
        options: {
          stdout: true
        },
        command: function(bump) {
          bump = bump || 'patch';
          return './tools/release.js ' + bump;
        }
      },
      karma: {
        options: {
          stdout: true
        },
        // This will run tests in Sauce Lab
        command: './node_modules/karma/bin/karma start'
      },
      testem: {
        options: {
          stdout: true
        },
        // This will run tests in all local browsers available/detected
        command: 'testem ci -R dot -l chrome'
      }
    },
    // Uglify must be run after browserify
    uglify: {
      progressbar: {
        options: {
          sourceMap: true
        },
        files: {
          'dist/progressbar.min.js': ['dist/progressbar.min.js']
        }
      }
    },
    // Calculated dynamically above
    karma: karmaConfig
  });

  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-shell');
  grunt.loadNpmTasks('grunt-karma');

  grunt.registerTask('watch', function() {
    grunt.task.run(['shell:watchifyLocalDev']);
  });

  grunt.registerTask('stageMinified', function() {
    grunt.task.run(['shell:stageMinified']);
  });

  // Build distributables to dist folder
  grunt.registerTask('build', [
    'shell:browserifyDevelopment',
    'shell:browserifyMinified',
    'uglify:progressbar'
  ]);

  grunt.registerTask('test', ['shell:testem']);

  // Run multiple tests serially, but continue if one of them fails.
  // Adapted from http://stackoverflow.com/questions/16487681/gruntfile-getting-error-codes-from-programs-serially
  grunt.registerTask('sauce', function() {
      var done = this.async();
      var tasks = {};
      _.each(browserGroups, function(group, index) {
        tasks['karma:sauce' + index] = 0;
      });

      var success = true;
      grunt.util.async.forEachSeries(Object.keys(tasks), function(task, next) {
          grunt.util.spawn({
              // Use grunt to spawn
              grunt: true,
              args: [task],
              // Print to the same stdout
              opts: { stdio: 'inherit' }
          }, function(err, result, code) {
              tasks[task] = code;
              if (code !== 0) {
                  success = false;
              }
              next();
          });
      }, function() {
          done(success);
      });
  });

  // Test, build, and release library to public
  grunt.registerTask('release', function(bump) {
    bump = bump || 'patch';

    grunt.task.run([
      'jshint',
      'test',
      'build',
      'stageMinified',
      'shell:release:' + bump
    ]);
  });
};
