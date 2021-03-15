==== ExternalSource: _relative_imports/dir/contract.sol ====
==== ExternalSource: _relative_imports/dir/a.sol ====
==== ExternalSource: _relative_imports/dir/B/b.sol ====
==== ExternalSource: _relative_imports/c.sol ====
==== ExternalSource: _relative_imports/D/d.sol ====
==== ExternalSource: _relative_imports/dir/G/g.sol ====
==== ExternalSource: _relative_imports/h.sol ====
import "_relative_imports/dir/contract.sol";

contract C {
    Contract _c;
    constructor() {
        _c = new Contract();
    }
    function foo() public returns (uint) {
        return _c.foo();
    }
}

// ====
// compileViaYul: also
// ----
// foo() -> 2292
