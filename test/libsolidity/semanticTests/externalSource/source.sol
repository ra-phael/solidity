==== ExternalSource: _external/external.sol ====
import "_external/external.sol";
contract C {
    External _external;
    constructor() {
        _external = new External();
    }
    function foo() public returns (uint) {
        return _external.foo();
    }
}
// ====
// compileViaYul: also
// ----
// foo() -> 0x04d2
