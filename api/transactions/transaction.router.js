const { createTransaction, getAllTransactions, getByDate_of_Transfer } = require("./transaction.controller")
const router = require("express").Router()


router.post("/", createTransaction)
router.get("/:id", getByDate_of_Transfer)

module.exports = router