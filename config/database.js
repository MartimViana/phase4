const { createPool } = require('mysql')

const pool = createPool({
	port: 3306,
	host: '35.240.51.198',
	//user: process.env.DB_USER,
	user: 'sampleuser',
	//password: process.env.DB_PASSWORD,
	password: 'sampleuser',
	database: 'uk_house_pricing',
	connectionLimit: 10
})
module.exports = pool