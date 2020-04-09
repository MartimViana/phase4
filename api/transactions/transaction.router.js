const { createTransaction } = require("./transaction.controller")
const router = require("express").Router()

router.post("/", createTransaction)

module.exports = router