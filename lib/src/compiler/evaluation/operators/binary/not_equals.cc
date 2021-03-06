#include <puppet/compiler/evaluation/operators/binary/not_equals.hpp>
#include <puppet/compiler/evaluation/operators/binary/call_context.hpp>
#include <puppet/unicode/string.hpp>

using namespace std;
using namespace puppet::runtime;

namespace puppet { namespace compiler { namespace evaluation { namespace operators { namespace binary {

    descriptor not_equals::create_descriptor()
    {
        binary::descriptor descriptor{ ast::binary_operator::not_equals };

        descriptor.add("String", "String", [](call_context& context) {
            auto& left = context.left().require<std::string>();
            auto& right = context.right().require<std::string>();

            if (left.size() < right.size()) {
                return unicode::string{ left }.compare(right, true) != 0;
            }
            return unicode::string{ right }.compare(left, true) != 0;
        });
        descriptor.add("Type", "Type", [](call_context& context) {
            auto& left = context.left().require<values::type>();
            auto& right = context.right().require<values::type>();
            types::recursion_guard guard;
            return left != right && (!left.is_assignable(right, guard) || !right.is_assignable(left, guard));
        });
        descriptor.add("Any", "Any", [](call_context& context) {
            return context.left() != context.right();
        });
        return descriptor;
    }

}}}}}  // namespace puppet::compiler::evaluation::operators::binary
