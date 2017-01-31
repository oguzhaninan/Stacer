/*global module:false, require:true, console:true */

module.exports = function(grunt) {

  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-qunit');
  grunt.loadNpmTasks('grunt-contrib-concat');
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-bump');
  grunt.loadNpmTasks('grunt-contrib-yuidoc');
  grunt.loadNpmTasks('grunt-contrib-copy');

  var banner = [
        '/* <%= pkg.name %> - v<%= pkg.version %> - ',
        '<%= grunt.template.today("yyyy-mm-dd") %> - <%= pkg.homepage %> */\n'
      ].join('');

  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),
    qunit: {
      files: ['tests/*.html']
    },
    uglify: {
      standardTarget: {
        files: {
          'dist/shifty.min.js': [
            'dist/shifty.js'
          ]
        }
      },
      options: {
        banner: banner
      }
    },
    jshint: {
      all_files: [
        'src/shifty.!(intro|outro)*.js'
      ],
      options: {
        jshintrc: '.jshintrc'
      }
    },
    concat: {
      options: {
        banner: banner
      },
      minimal: {
        src: [
          'src/shifty.intro.js',
          'src/shifty.core.js',
          'src/shifty.outro.js'
        ],
        dest: 'dist/shifty.js'
      },
      minimalDebug: {
        src: [
          'src/shifty.intro.js',
          'src/shifty.core.js',
          'src/shifty.outro.js'
        ],
        dest: 'dist/shifty.js'
      },
      // Includes extensions needed by Rekapi (see: rekapi.com)
      forRekapi: {
        src: [
          'src/shifty.intro.js',
          'src/shifty.core.js',
          'src/shifty.formulas.js',
          'src/shifty.bezier.js',
          'src/shifty.interpolate.js',
          'src/shifty.token.js',
          'src/shifty.outro.js'
        ],
        dest: 'dist/shifty.js'
      },
      forRekapiDebug: {
        src: [
          'src/shifty.intro.js',
          'src/shifty.core.js',
          'src/shifty.formulas.js',
          'src/shifty.bezier.js',
          'src/shifty.interpolate.js',
          'src/shifty.token.js',
          'src/shifty.outro.js'
        ],
        dest: 'dist/shifty.js'
      }
    },
    bump: {
      options: {
        files: ['package.json', 'bower.json'],
        commit: false,
        createTag: false,
        tagName: '%VERSION%',
        tagMessage: 'Version %VERSION%',
        push: false
      }
    },
    yuidoc: {
      compile: {
        name: 'Shifty',
        description: '<%= pkg.description %>',
        version: '<%= pkg.version %>',
        url: '<%= pkg.homepage %>',
        logo: '../../asset/img/shifty-logo-200.png',
        options: {
          paths: ['dist'],
          recurse: false,
          outdir: 'dist/doc'
        }
      }
    },
    copy: {
      redirects: {
        files: [
          { src: ['redirects/src/shifty.bezier.js.html'], dest: 'dist/doc/src/shifty.bezier.js.html' },
          { src: ['redirects/src/shifty.core.js.html'], dest: 'dist/doc/src/shifty.core.js.html' },
          { src: ['redirects/src/shifty.interpolate.js.html'], dest: 'dist/doc/src/shifty.interpolate.js.html' },
          { src: ['redirects/src/shifty.token.js.html'], dest: 'dist/doc/src/shifty.token.js.html' },
        ]
      }
    }
  });

  grunt.registerTask('default', [
      'jshint',
      'qunit'
    ]);

  grunt.registerTask('build', [
      'concat:forRekapi',
      'uglify:standardTarget',
      'concat:forRekapiDebug',
      'yuidoc',
      'copy:redirects'
    ]);

  grunt.registerTask('build-minimal', [
      'concat:minimal',
      'uglify:standardTarget',
      'concat:minimalDebug',
      'yuidoc',
      'copy:redirects'
    ]);
};
