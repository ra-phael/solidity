==== ExternalSource: a=_external/external.sol=sol ====
import "a";
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
