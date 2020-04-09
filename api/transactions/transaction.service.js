const pool = require("../../config/database")

module.exports = {
	create: (data, callback) => {
		pool.query(
			`INSERT INTO transaction (Transaction_unique_identifier, Price, Date_of_Transfer, Property_type, Old_New, Duration, Town, District, County, PPDCategory_type, Record_status) 
			VALUES (?,?,?,?,?,?,?,?,?,?, 'A');`,
			[
				data.Transaction_unique_identifier,
				data.Price,
				data.Date_of_Transfer,
				data.Property_type,
				data.Old_New,
				data.Duration,
				data.Town,
				data.District,
				data.County,
				data.PPDCategory_type
			],
			(error, results, fields) => {
				if (error) {
					return callback(error)
				}
				return callback(null, results)
			}

		)
	},
	getAll: callback => {
		pool.query(`select * from transaction;`,
		[],
		(error, results, fields) => {
			if (error) {
				callback(error)
			}
			return callback(null, results)
		}
		)
	},
	getByDate_of_Transfer: (date_of_transfer, callback) => {
		pool.query(`select * from transaction where Date_of_Transfer = ?;`,
		[
			date_of_transfer
		],
		(error, results, fields) => {
			if (error) {
				callback(error)
			}
			return callback(null, results)
		}
		)
	}
}