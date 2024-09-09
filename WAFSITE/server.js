const express = require('express');
const bodyParser = require('body-parser');
const bcrypt = require('bcrypt');
const db = require('./database');

const app = express();
const port = 3000;

app.use(bodyParser.json());
app.use(express.static('public'));

// Register endpoint
app.post('/register', async (req, res) => {
    const { username, password } = req.body;
    
    try {
        const hashedPassword = await bcrypt.hash(password, 10);
        db.run('INSERT INTO users (username, password) VALUES (?, ?)', [username, hashedPassword], (err) => {
            if (err) {
                res.status(500).json({ error: 'Error registering user' });
            } else {
                res.status(201).json({ message: 'User registered successfully' });
            }
        });
    } catch {
        res.status(500).json({ error: 'Error registering user' });
    }
});

// Login endpoint
app.post('/login', (req, res) => {
    const { username, password } = req.body;
    
    db.get('SELECT * FROM users WHERE username = ?', [username], async (err, user) => {
        if (err) {
            res.status(500).json({ error: 'Error logging in' });
        } else if (!user) {
            res.status(400).json({ error: 'User not found' });
        } else {
            try {
                if (await bcrypt.compare(password, user.password)) {
                    res.status(200).json({ message: 'Logged in successfully' });
                } else {
                    res.status(400).json({ error: 'Invalid password' });
                }
            } catch {
                res.status(500).json({ error: 'Error logging in' });
            }
        }
    });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});