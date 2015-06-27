/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2014 Krzysztof Narkiewicz <krzysztof.narkiewicz@ezaquarii.com>
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <QSet>
#include "scanner.h"

// autogenerated by Bison, don't panic
// if your IDE can't resolve it - call make first
#include "parser.hpp"

namespace CASParser {

/**
 * This class is the interface for our scanner/lexer. The end user
 * is expected to use this. It drives scanner/lexer, keeps
 * parsed AST and generally is a good place to store additional
 * context data. Both parser and lexer have access to it via internal 
 * references.
 * 
 * I know that the AST is a bit too strong word for a simple
 * vector with nodes, but this is only an example. Get off me.
 */
class Interpreter
{
public:
        Interpreter();
        ///destructor
        virtual ~Interpreter();

        /**
         * Run parser. Results are stored inside.
         * \returns 0 on success, 1 on failure
         */
        int parse();

        /**
         * Clear AST
         */
        void clear();

        /**
         * @brief setRootNode stores the root node inside the interpreter class.
         * @param node root node of the parsed formula to remember.
         */
        void setRootNode(EgcNode* node);

        /**
         * @brief getRootNode returns the root node of the lastly parsed formula. ATTENTION: the caller takes ownership
         * of the root node returned.
         * @return a pointer to the root node of the lastly parsed formula
         */
        EgcNode* getRootNode(void);

        /**
         * Switch scanner input stream. Default is standard input (std::cin).
         * It will also reset AST.
         */
        void switchInputStream(std::istream *is);

        /**
         * This is needed so that Scanner and Parser can call some
         * methods that we want to keep hidden from the end user.
         */
        friend class MaximaParser;
        friend class MaximaScanner;

private:
        Q_DISABLE_COPY(Interpreter)

        /**
         * @brief addBinaryExpression add binary Expression to the current AST
         * @param type the type of binary expression to create
         * @param node0 the first node to add to the binary expression
         * @param node1 the second (right) node to add to the binary expression
         * @return a pointer to the binary expression created
         */
        EgcNode* addBinaryExpression(EgcNodeType type, EgcNode* node0,
                                           EgcNode* node1);

        /**
         * @brief addUnaryExpression add unary Expression to the current AST
         * @param type the type of binary expression to create
         * @param node0 the node to add to the unary expression
         * @return a pointer to the unary expression created
         */
        EgcNode* addUnaryExpression(EgcNodeType type, EgcNode* node0);

        /**
         * @brief addStringNode add node Expression to the current AST
         * @param type the type of binary expression to create
         * @param value the value to use to build the node from
         * @return a pointer to the expression created
         */
        EgcNode* addStringNode(EgcNodeType type, const std::string& value);

#warning turn EgcExpressionNode into s.th. like EgcArgListExpressionNode to be more typesafe
        /**
         * @brief addFunction add a user defined function to the formula
         * @param fncName the function name of the function to create
         * @param argList the argument list with all expression to add to the function. The List can be integrated
         * directly into the function.
         * @return pointer to the function created
         */
        EgcNode* addFunction(const std::string& fncName, EgcNode* argList);

#warning turn EgcExpressionNode into s.th. like EgcArgListExpressionNode to be more typesafe
        /**
         * @brief addBuiltinFunction add a builtin function to the formula
         * @param fncName the function name of the function to create
         * @param argList the argument list with all expression to add to the function. The List can be integrated
         * directly into the function.
         * @return pointer to the function created
         */
        EgcNode* addBuiltinFunction(const std::string& fncName, EgcNode* argList);

#warning turn returnvalue into s.th. like EgcArgListExpressionNode to be more typesafe
        /**
         * @brief createArgList creates an argument list that is integrated later on in the function where it is used in
         * @param expression the expression to add to the argument list
         * @return a pointer to the created argument list
         */
        EgcNode* createArgList(EgcNode* expression);

#warning turn returnvalue and 2nd argument into s.th. like EgcArgListExpressionNode to be more typesafe
        /**
         * @brief addArgument adds an argument to the argument list given
         * @param expressionToAdd the expression (argument) to add
         * @param argumentList the argument list to use to add the argument to
         * @return a pointer to the changed argument list
         */
        EgcNode* addArgument(EgcNode* expressionToAdd, EgcNode* argumentList);

        /**
         * @brief addDanglingNode add a dangling node to the set of dangling nodes. So it is easy to delete them later
         * in case of an error during parsing.
         * @param node the node to add
         */
        void addDanglingNode(EgcNode* node);

        /**
         * * @brief removeDanglingNode remove a dangling node from the set of dangling nodes. So it is easy to delete
         * them later in case of an error during parsing.
         * @param node the node to remove
         */
        void setNotDangling(EgcNode* node);

        /**
         * @brief deleteDanglingNodes delete all dangling nodes in case of an error during build up of the AST
         */
        void deleteDanglingNodes(void);

        // Used internally by MaximaScanner YY_USER_ACTION to update location indicator
        void increaseLocation(unsigned int loc);
    
        // Used to get last MaximaScanner location. Used in error messages.
        unsigned int location() const;
    
private:
        MaximaScanner m_scanner;                        /// the scanner to use for parsing
        MaximaParser m_parser;                          /// the parser to use
        QScopedPointer<EgcNode> m_rootNode;             /// the base node of the formula
        unsigned int m_location;                        /// Used by scanner
        QSet<EgcNode*> m_danglingNodes;                 /// holds the dangling nodes during AST is built up
};

}

#endif // INTERPRETER_H
