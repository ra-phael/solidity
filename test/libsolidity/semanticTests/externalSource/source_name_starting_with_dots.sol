==== ExternalSource: ./a.sol=_source_name_starting_with_dots/dot_a.sol ====
==== ExternalSource: ../b.sol=_source_name_starting_with_dots/dot_dot_b.sol ====
==== ExternalSource: _source_name_starting_with_dots/dir/a.sol ====
==== ExternalSource: _source_name_starting_with_dots/b.sol ====
==== ExternalSource: _source_name_starting_with_dots/dir/contract.sol ====
import "_source_name_starting_with_dots/dir/contract.sol";

contract Contract {
    A _a;
    B _b;
    constructor() {
        _a = new A();
        _b = new B();
    }
    function foo() public returns (uint) {
        return _a.foo() + _b.foo();
    }
}
// ====
// compileViaYul: also
// ----
// foo() -> 357
