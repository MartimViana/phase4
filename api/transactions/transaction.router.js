const { createTransaction, getAllTransactions, getByDate_of_Transfer, changeTransactionByTransaction_unique_identifier } = require("./transaction.controller")
const router = require("express").Router()


router.post("/", createTransaction)
router.get("/:id", getByDate_of_Transfer)
router.post("/change", changeTransactionByTransaction_unique_identifier)
router.get("/", getAllTransactions)
module.exports = router