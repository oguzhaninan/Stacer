import '../../libs/amaran.min'

exports.prettyMemSize = (size) => {
  return (size / (Math.pow(1024, 3))).toFixed(2)
}

exports.prettyDiskSize = (size) => {
  return (size / Math.pow(1000, 3)).toFixed(1)
}

exports.showMessage = (message, status) => {
  let bgcolor
  switch (status) {
    case 'success':
      bgcolor = '#27ae60';
      break;
    case 'error':
      bgcolor = '#c0392b';
      break;
    default:
      bgcolor = '#f39c12';
      break;
  }
  $.amaran({
    'theme': 'colorful',
    'content': {
      bgcolor: bgcolor,
      message: message
    },
    'position': 'top right',
    'outEffect': 'slideTop',
    'delay': 5000
  })
}

exports.command = (command) => `bash -c "${command}"`

exports.formatBytes = (bytes, decimals = 0) => {
  if (bytes == 0) return '0 Bytes'
  let k = 1024,
    dm = decimals + 1 || 3,
    sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'],
    i = Math.floor(Math.log(bytes) / Math.log(k))
  return parseFloat((bytes / Math.pow(k, i)).toFixed(dm)) + ' ' + sizes[i]
}