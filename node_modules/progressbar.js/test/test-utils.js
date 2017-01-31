function getComputedStyle(element, property) {
    var computedStyle = window.getComputedStyle(element, null);
    return computedStyle.getPropertyValue(property);
}

module.exports = {
    getComputedStyle: getComputedStyle
};
