import "../B/b.sol";

contract G {
    B _b;
    constructor() {
        _b = new B();
    }
    function foo() public returns (uint) {
        return _b.foo();
    }
}
