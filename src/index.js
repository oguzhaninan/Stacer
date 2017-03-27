import App from './components/App'

window.vm = new Vue({
    el: 'app',
    components: {
        'app': App
    },
    mounted() {
        setTimeout(() => document.getElementById('loading').remove(), 2000)
    }
})