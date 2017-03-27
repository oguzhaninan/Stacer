import App from './components/App'

window.vm = new Vue({
    el: 'app',
    components: {
        'app': App
    },
    mounted() {
        document.getElementById('loading').remove()
        // setTimeout(() => document.getElementById('loading').remove(), 2000)
    }
})