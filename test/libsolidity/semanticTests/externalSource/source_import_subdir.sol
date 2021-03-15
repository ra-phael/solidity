==== ExternalSource: _external/import_with_subdir.sol ====
==== ExternalSource: subdir/import.sol=_external/subdir/import.sol ====
==== ExternalSource: sub_external.sol=_external/subdir/sub_external.sol ====
import "sub_external.sol";
contract C {
    SubExternal _external;
    constructor() {
        _external = new SubExternal();
    }
    function foo() public returns (uint) {
        return _external.foo();
    }
}
// ====
// compileViaYul: also
// ----
// foo() -> 0x0929
