/**
 * Module dependencies.
 */

var DOMParser = require('xmldom').DOMParser;

/**
 * Module exports.
 */

exports.parse = parse;

/**
 * We ignore raw text (usually whitespace), <!-- xml comments -->,
 * and raw CDATA nodes.
 *
 * @param {Element} node
 * @returns {Boolean}
 * @api private
 */

function shouldIgnoreNode (node) {
  return node.nodeType === 3 // text
    || node.nodeType === 8   // comment
    || node.nodeType === 4;  // cdata
}

/**
 * Check if the node is empty. Some plist file has such node:
 * <key />
 * this node shoud be ignored.
 *
 * @see https://github.com/TooTallNate/plist.js/issues/66
 * @param {Element} node
 * @returns {Boolean}
 * @api private
 */
function isEmptyNode(node){
  if(!node.childNodes || node.childNodes.length === 0) {
    return true;
  } else {
    return false;
  }
}

/**
 * Parses a Plist XML string. Returns an Object.
 *
 * @param {String} xml - the XML String to decode
 * @returns {Mixed} the decoded value from the Plist XML
 * @api public
 */

function parse (xml) {
  var doc = new DOMParser().parseFromString(xml);
  if (doc.documentElement.nodeName !== 'plist') {
    throw new Error('malformed document. First element should be <plist>');
  }
  var plist = parsePlistXML(doc.documentElement);

  // the root <plist> node gets interpreted as an Array,
  // so pull out the inner data first
  if (plist.length == 1) plist = plist[0];

  return plist;
}

/**
 * Convert an XML based plist document into a JSON representation.
 *
 * @param {Object} xml_node - current XML node in the plist
 * @returns {Mixed} built up JSON object
 * @api private
 */

function parsePlistXML (node) {
  var i, new_obj, key, val, new_arr, res, d;

  if (!node)
    return null;

  if (node.nodeName === 'plist') {
    new_arr = [];
    for (i=0; i < node.childNodes.length; i++) {
      // ignore comment nodes (text)
      if (!shouldIgnoreNode(node.childNodes[i])) {
        new_arr.push( parsePlistXML(node.childNodes[i]));
      }
    }
    return new_arr;

  } else if (node.nodeName === 'dict') {
    new_obj = {};
    key = null;
    for (i=0; i < node.childNodes.length; i++) {
      // ignore comment nodes (text)
      if (!shouldIgnoreNode(node.childNodes[i])) {
        if (key === null) {
          key = parsePlistXML(node.childNodes[i]);
        } else {
          new_obj[key] = parsePlistXML(node.childNodes[i]);
          key = null;
        }
      }
    }
    return new_obj;

  } else if (node.nodeName === 'array') {
    new_arr = [];
    for (i=0; i < node.childNodes.length; i++) {
      // ignore comment nodes (text)
      if (!shouldIgnoreNode(node.childNodes[i])) {
        res = parsePlistXML(node.childNodes[i]);
        if (null != res) new_arr.push(res);
      }
    }
    return new_arr;

  } else if (node.nodeName === '#text') {
    // TODO: what should we do with text types? (CDATA sections)

  } else if (node.nodeName === 'key') {
    if(isEmptyNode(node)) return null;
    res = '';
    if(node.childNodes[0]) {
      res = node.childNodes[0].nodeValue;
    }
    return res;
  } else if (node.nodeName === 'string') {
    res = '';
    if(isEmptyNode(node)) return null;
    for (d=0; d < node.childNodes.length; d++) {
      res += node.childNodes[d].nodeValue;
    }
    return res;

  } else if (node.nodeName === 'integer') {
    // parse as base 10 integer
    return parseInt(node.childNodes[0].nodeValue, 10);

  } else if (node.nodeName === 'real') {
    res = '';
    for (d=0; d < node.childNodes.length; d++) {
      if (node.childNodes[d].nodeType === 3) {
        res += node.childNodes[d].nodeValue;
      }
    }
    return parseFloat(res);

  } else if (node.nodeName === 'data') {
    res = '';
    for (d=0; d < node.childNodes.length; d++) {
      if (node.childNodes[d].nodeType === 3) {
        res += node.childNodes[d].nodeValue.replace(/\s+/g, '');
      }
    }

    // decode base64 data to a Buffer instance
    return new Buffer(res, 'base64');

  } else if (node.nodeName === 'date') {
    return new Date(node.childNodes[0].nodeValue);

  } else if (node.nodeName === 'true') {
    return true;

  } else if (node.nodeName === 'false') {
    return false;
  }
}
