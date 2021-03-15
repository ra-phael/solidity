==== ExternalSource: _external/external.sol ====
==== ExternalSource: _external/other_external.sol ====
import "_external/external.sol";
import "_external/other_external.sol";
contract C {
    External _external;
    OtherExternal _otherExternal;
    constructor() {
        _external = new External();
        _otherExternal = new OtherExternal();
    }
    function foo() public returns (uint) {
        return _external.foo();
    }
    function foofoo() public returns (uint) {
        return _otherExternal.foo();
    }
}
// ====
// compileViaYul: also
// ----
// foo() -> 1234
// foofoo() -> 2345
