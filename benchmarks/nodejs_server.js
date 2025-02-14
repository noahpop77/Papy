const express = require('express');
const app = express();

app.get('/', (req, res) => {
    res.set('Content-Type', 'text/plain');
    res.send('hello node');
});

app.get('/benchmark', (req, res) => {
    res.set('Content-Type', 'text/plain');
    res.send('benching');
});

app.listen(5000, () => console.log('Server running on port 5000'));