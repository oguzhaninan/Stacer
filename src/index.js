import App from './components/App'

new Vue({
    el: 'app',
    data: {
        activeNav: 1,
    },
    components: {
        'app': App
    }
})

setTimeout(() => {
    $('#loading').remove()
}, 3000);
