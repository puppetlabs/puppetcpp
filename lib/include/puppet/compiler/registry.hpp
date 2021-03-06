/**
 * @file
 * Declares the type registry.
 */
#pragma once

#include "resource.hpp"
#include "ast/ast.hpp"
#include "evaluation/scope.hpp"
#include "../runtime/values/value.hpp"
#include <boost/optional.hpp>
#include <memory>
#include <vector>
#include <unordered_map>

namespace puppet { namespace compiler {

    namespace evaluation {

        // Forward declaration of context
        struct context;

    }  // namespace puppet::compiler::evaluation

    // Forward declaration of node.
    struct node;

    /**
     * Represents a defined class.
     */
    struct klass
    {
        /**
         * Constructs a class.
         * @param name The fully-qualified name of the class.
         * @param statement The class statement.
         */
        klass(std::string name, ast::class_statement const& statement);

        /**
         * Gets the fully-qualified name of the class.
         * @return Returns the fully-qualified name of the class.
         */
        std::string const& name() const;

        /**
         * Gets the statement that defines the class.
         * @return Returns the statement that defines the class.
         */
        ast::class_statement const& statement() const;

     private:
        std::string _name;
        std::shared_ptr<ast::syntax_tree> _tree;
        ast::class_statement const& _statement;
    };

    /**
     * Represents a defined type.
     */
    struct defined_type
    {
        /**
         * Constructs a defined type.
         * @param name The fully-qualified name of the defined type.
         * @param statement The defined type statement.
         */
        defined_type(std::string name, ast::defined_type_statement const& statement);

        /**
         * Gets the fully-qualified name of the defined type.
         * @return Returns the fully-qualified name of the defined type.
         */
        std::string const& name() const;

        /**
         * Gets the statement that defines the defined type.
         * @return Returns the statement that defines the defined type.
         */
        ast::defined_type_statement const& statement() const;

     private:
        std::string _name;
        std::shared_ptr<ast::syntax_tree> _tree;
        ast::defined_type_statement const& _statement;
    };

    /**
     * Represents a node definition.
     */
    struct node_definition
    {
        /**
         * Constructs a node definition.
         * @param statement The node statement.
         */
        explicit node_definition(ast::node_statement const& statement);

        /**
         * Gets the statement that defines the node.
         * @return Returns the statement that defines the node.
         */
        ast::node_statement const& statement() const;

     private:
        std::shared_ptr<ast::syntax_tree> _tree;
        ast::node_statement const& _statement;
    };

    /**
     * Represents a type alias.
     */
    struct type_alias
    {
        /**
         * Constructs a type alias.
         * @param statement The type alias statement.
         */
        explicit type_alias(ast::type_alias_statement const& statement);

        /**
         * Gets the statement for the type alias.
         * @return Returns the statement for the type alias.
         */
        ast::type_alias_statement const& statement() const;

     private:
        std::shared_ptr<ast::syntax_tree> _tree;
        ast::type_alias_statement const& _statement;
    };

    /**
     * Represents the compiler registry.
     */
    struct registry
    {
        /**
         * Default constructor for registry.
         */
        registry() = default;

        /**
         * Default move constructor for registry.
         */
        registry(registry&&) = default;

        /**
         * Default move assignment operator for registry.
         * @return Returns this registry.
         */
        registry& operator=(registry&&) = default;

        /**
         * Finds a class given the qualified name.
         * @param name The fully-qualified name of the class (e.g. foo::bar).
         * @return Returns a pointer to the class definition if found or nullptr if the class does not exist.
         */
        klass const* find_class(std::string const& name) const;

        /**
         * Registers a class.
         * @param klass The class to register.
         */
        void register_class(compiler::klass klass);

        /**
         * Finds a defined type given the qualified name.
         * @param name The fully-qualified name of the defined type (e.g. foo::bar).
         * @return Returns a pointer to the defined type or nullptr if the defined type does not exist.
         */
        defined_type const* find_defined_type(std::string const& name) const;

        /**
         * Registers a defined type.
         * @param type The defined type to register.
         */
        void register_defined_type(defined_type type);

        /**
         * Finds a matching node definition and scope name for the given node.
         * @param node The node to find the node definition for.
         * @return Returns the pair of node definition and matching name if found or a nullptt if one does not exist.
         */
        std::pair<node_definition const*, std::string> find_node(compiler::node const& node) const;

        /**
         * Finds a matching node definition for the given node statement.
         * @param statement The node statement to find a matching node definition for.
         * @return Returns a pointer to the node definition if found or nullptr if one does not exist.
         */
        node_definition const* find_node(ast::node_statement const& statement) const;

        /**
         * Registers a node definition.
         * @param node The node to register.
         * @return Returns nullptr if the node was successfully registered or the pointer to the previous definition.
         */
        node_definition const* register_node(node_definition node);

        /**
         * Determines if the registry has a node definition.
         * @return Returns true if the registry contains any node definitions or false if not.
         */
        bool has_nodes() const;

        /**
         * Registers a type alias.
         * @param alias The type alias to register.
         */
        void register_type_alias(type_alias alias);

        /**
         * Finds a type alias by name.
         * @param name The name of the type alias (e.g. Foo::Bar).
         * @return Returns the type alias or nullptr if the type alias does not exist.
         */
        type_alias* find_type_alias(std::string const& name);

        /**
         * Finds a type alias by name.
         * @param name The name of the type alias (e.g. Foo::Bar).
         * @return Returns the type alias or nullptr if hte type alias does not exist.
         */
        type_alias const* find_type_alias(std::string const& name) const;

     private:
        registry(registry&) = delete;
        registry& operator=(registry&) = delete;

        std::unordered_map<std::string, klass> _classes;
        std::unordered_map<std::string, defined_type> _defined_types;
        std::vector<node_definition> _nodes;
        std::unordered_map<std::string, size_t> _named_nodes;
        std::vector<std::pair<runtime::values::regex, size_t>> _regex_nodes;
        boost::optional<size_t> _default_node_index;
        std::unordered_map<std::string, type_alias> _aliases;
    };

}}  // puppet::compiler
