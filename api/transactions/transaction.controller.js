const { create } = require('./transaction.service')

module.exports = {
	createTransaction: (req, res) => {
		const body = req.body
		create(body, (err, results) => {
			if (err) {
				console.log(err)
				return res.status(500).json({
					message: "Database connection error"
				})
			}
			return res.status(200).json({
				data: results
			})
		})
	}
}