contract ClientReceipt {
    constructor() payable {}
}
// ====
// compileViaYul: also
// ----
// constructor(), 1 ether ->
// contract_balance -> 1000000000000000000
