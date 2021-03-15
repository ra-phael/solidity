import "../../c.sol";

contract B {
    C _c;
    constructor() {
        _c = new C();
    }
    function foo() public returns (uint) {
        return _c.foo();
    }
}
