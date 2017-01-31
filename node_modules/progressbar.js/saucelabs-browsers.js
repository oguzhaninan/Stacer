// Browsers on Sauce Labs
// Check out https://saucelabs.com/platforms for all browser/platform combos
// and https://wiki.saucelabs.com/display/DOCS/Platform+Configurator#/
module.exports = {

    // Chrome
    sauce_linux_chrome_47: {
        base: 'SauceLabs',
        browserName: 'chrome',
        platform: 'linux',
        version: '47'
    },
    sauce_windows_chrome_47: {
        base: 'SauceLabs',
        browserName: 'chrome',
        platform: 'Windows 8.1',
        version: '47'
    },
    sauce_osx_chrome_47: {
        base: 'SauceLabs',
        browserName: 'chrome',
        platform: 'OS X 10.9',
        version: '47'
    },

    // Safari
    sauce_osx_safari9: {
        base: 'SauceLabs',
        browserName: 'safari',
        platform: 'OS X 10.11',
        version: '9'
    },
    sauce_osx_safari8: {
        base: 'SauceLabs',
        browserName: 'safari',
        platform: 'OS X 10.10',
        version: '8'
    },

    // Firefox

    sauce_osx_firefox_beta: {
        base: 'SauceLabs',
        browserName: 'firefox',
        platform: 'OS X 10.11',
        version: 'beta'
    },
    sauce_osx_firefox_43: {
        base: 'SauceLabs',
        browserName: 'firefox',
        platform: 'OS X 10.9',
        version: '43'
    },

    sauce_linux_firefox_43: {
        base: 'SauceLabs',
        browserName: 'firefox',
        platform: 'linux',
        version: '43'
    },

    sauce_windows_firefox_43: {
        base: 'SauceLabs',
        browserName: 'firefox',
        platform: 'Windows 8.1',
        version: '43'
    },

    // Opera
    sauce_windows_opera_12: {
        base: 'SauceLabs',
        browserName: 'opera',
        platform: 'Windows 7',
        version: '12.12'
    },

    // Internet explorer
    sauce_windows_ie_11: {
        base: 'SauceLabs',
        browserName: 'internet explorer',
        platform: 'Windows 8.1',
        version: '11'
    },
    sauce_windows_ie_10: {
        base: 'SauceLabs',
        browserName: 'internet explorer',
        platform: 'Windows 7',
        version: '10'
    },
    sauce_windows_ie_9: {
        base: 'SauceLabs',
        browserName: 'internet explorer',
        platform: 'Windows 7',
        version: '9'
    },

    // MS Edge

    sauce_windows_edge: {
        base: 'SauceLabs',
        browserName: 'MicrosoftEdge',
        platform: 'Windows 10',
        version: '20.10240'
    },

    // Mobile

    // Android
    sauce_android: {
        base: 'SauceLabs',
        browserName: 'android',
        platform: 'Linux',
        version: '4.4'
    },

    // iOS iPhone
    sauce_ios9_iphone: {
        base: 'SauceLabs',
        browserName: 'iphone',
        platform: 'OS X 10.10',
        version: '9.2'
    },
    sauce_ios8_iphone: {
        base: 'SauceLabs',
        browserName: 'iphone',
        platform: 'OS X 10.10',
        version: '8.4'
    }
};
