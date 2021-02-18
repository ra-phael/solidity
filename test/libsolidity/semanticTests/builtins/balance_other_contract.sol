contract Other {
    constructor() payable {
    }
    function getAddress() public returns (address) {
        return address(this);
    }
}
contract ClientReceipt {
    Other other;
    constructor() payable {
        other = new Other{value:500}();
    }
    function getAddress() public returns (address) {
        return other.getAddress();
    }
}
// ====
// compileViaYul: also
// ----
// constructor(), 2000 wei ->
// gas legacy: 235167
// contract_balance -> 1500
// gas legacy: 235167
// getAddress() -> 0xf01f7809444bd9a93a854361c6fae3f23d9e23db
// account_balance: 0xf01f7809444bd9a93a854361c6fae3f23d9e23db -> 500
