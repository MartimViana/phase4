const { create, getAll, getByDate_of_Transfer } = require('./transaction.service')

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
	},
	getByDate_of_Transfer: (req, res) => {
		const id = req.params.id
		getByDate_of_Transfer(id, (err, results) => {
			if (err) {
				console.log(err);
				return;
			}
			if(!results) {
				return res.json({
					message: "Transaction not found."
				})
			}
			return res.json({
				data: results
			})
		})
	}
}