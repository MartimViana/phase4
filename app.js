require("dotenv").config()
const express = require("express")
const app = express()
const transactionRouter = require("./api/transactions/transaction.router")
app.use(express.json())
/*
app.get("/api", (req, res) => {
	res.json({
		message: "Success!"
	})
})*/
app.use("/api/transactions", transactionRouter)

app.listen(process.env.APP_PORT, () => {
	console.log("Server up and running on port : ", process.env.APP_PORT)
})