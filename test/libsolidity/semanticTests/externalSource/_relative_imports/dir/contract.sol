import "./a.sol" as a;
import "./B/b.sol" as b;
import "../c.sol" as c;
import "../D/d.sol" as d;
import "./E/../F/../G/./g.sol" as g;
import "../../../../_relative_imports/h.sol" as h;

contract Contract {
    a.A _a;
    b.B _b;
    c.C _c;
    d.D _d;
    g.G _g;
    h.H _h;
    constructor() {
        _a = new a.A();
        _b = new b.B();
        _c = new c.C();
        _d = new d.D();
        _g = new g.G();
        _h = new h.H();
    }
    function foo() public returns (uint) {
        return _a.foo() + _b.foo() + _c.foo() + _d.foo() + _g.foo() + _h.foo();
    }
}
