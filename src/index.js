import App from './components/App'

new Vue({
    el: 'app',
    data: {
        activeNav: 1,
    },
    components: {
        'app': App
    },
    mounted() {
        document.getElementById('loading').remove()
        // setTimeout(() => document.getElementById('loading').remove(), 2000)
    }
})