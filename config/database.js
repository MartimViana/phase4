const { createPool } = require('mysql')

const pool = createPool({
	port: 3306,
	host: 'localhost',
	//user: process.env.DB_USER,
	user: 'root',
	//password: process.env.DB_PASSWORD,
	password: 'root',
	database: 'uk_house_pricing',
	connectionLimit: 10
})

module.exports = pool