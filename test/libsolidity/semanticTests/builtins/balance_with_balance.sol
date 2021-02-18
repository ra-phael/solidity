contract ClientReceipt {
    constructor() payable {}
}
// ====
// compileViaYul: also
// ----
// constructor(), 1000 wei ->
// contract_balance -> 1000
