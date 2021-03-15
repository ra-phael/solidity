==== ExternalSource: _external/external.sol ====
==== Source: s1.sol ====
import "_external/external.sol";
contract S1 {
    External _external;
    constructor() {
        _external = new External();
    }
    function foo() public returns (uint) {
        return _external.foo();
    }
}
==== Source: s2.sol ====
import "s1.sol";
contract C {
    S1 _other;
    constructor() {
        _other = new S1();
    }
    function foo() public returns (uint) {
        return _other.foo();
    }
}
// ====
// compileViaYul: also
// ----
// foo() -> 1234
