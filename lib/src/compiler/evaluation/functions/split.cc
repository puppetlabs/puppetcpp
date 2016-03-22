#include <puppet/compiler/evaluation/functions/split.hpp>
#include <puppet/compiler/evaluation/functions/call_context.hpp>
#include <puppet/compiler/exceptions.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace puppet::runtime;

namespace puppet { namespace compiler { namespace evaluation { namespace functions {

    static values::value split_characters(string const& str)
    {
        values::array result;
        values::each_code_point(str, [&](string codepoint) {
            result.emplace_back(rvalue_cast(codepoint));
            return true;
        });
        return result;
    }

    descriptor split::create_descriptor()
    {
        functions::descriptor descriptor{ "split" };

        descriptor.add("Callable[String, String]", [](call_context& context) -> values::value {
            auto& first = context.argument(0).require<string>();
            auto& second = context.argument(1).require<string>();
            if (second.empty()) {
                return split_characters(first);
            }
            values::array result;
            boost::split_iterator<string::const_iterator> end;
            for (auto it = boost::make_split_iterator(first, boost::first_finder(second, boost::is_equal())); it != end; ++it) {
                if (!*it) {
                    continue;
                }
                result.emplace_back(boost::copy_range<string>(*it));
            }
            return result;
        });
        descriptor.add("Callable[String, Regexp]", [](call_context& context) -> values::value {
            auto& first = context.argument(0).require<string>();
            auto& second = context.argument(1).require<values::regex>();

            if (second.pattern().empty()) {
                return split_characters(first);
            }
            values::array result;
            for (sregex_token_iterator begin{ first.begin(), first.end(), second.value(), -1}, end; begin != end; ++begin) {
                result.emplace_back(string(*begin));
            }
            return result;
        });
        descriptor.add("Callable[String, Type[Regexp]]", [](call_context& context) -> values::value {
            auto& first = context.argument(0).require<string>();
            auto& second = boost::get<types::regexp>(context.argument(1).require<values::type>());

            if (second.pattern().empty()) {
                return split_characters(first);
            }
            values::array result;
            std::regex pattern(second.pattern());
            for (sregex_token_iterator begin{ first.begin(), first.end(), pattern, -1}, end; begin != end; ++begin) {
                result.emplace_back(string(*begin));
            }
            return result;
        });
        return descriptor;
    }

}}}}  // namespace puppet::compiler::evaluation::functions
