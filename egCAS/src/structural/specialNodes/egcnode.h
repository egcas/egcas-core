/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#ifndef EGCNODE_H
#define EGCNODE_H

#include <QString>
#include "egcnode_gen.h"

class EgcContainerNode;
class EgcNodeVisitor;

/**
 * @brief describes which side of the node is meant 
 */
enum class EgcNodeSide
{
        left = 0, right, middle
};


/** macro for setting the expression type of a class. Change this if you want to have the type
 * changed in a subclass */
#define EGC_SET_EXPRESSION_TYPE(classname, type)                                                                       \
public:                                                                                                                \
        virtual EgcNodeType getNodeType(void) const {return s_nodeType;}                                               \
        virtual QString getNodeName(void) const {return QString(#type);}                                               \
        virtual EgcNode* copy(void) {                                                                                  \
                return new (std::nothrow) classname(static_cast<const classname&>(*this));                             \
        }                                                                                                              \
        static EgcNode* create(void) {return new (std::nothrow) classname();}                                          \
protected:                                                                                                             \
        static const EgcNodeType s_nodeType = type

/**
 * @brief The EgcNode class defines the base class for all expressions
 * (multiplication, substraction, ...) in the expression tree.
 * The class is built up as composite pattern.
 */
class EgcNode
{
public:
        virtual EgcNodeType getNodeType(void) const {return s_nodeType;}
        virtual QString getNodeName(void) const {return QString("EgcNodeType::NodeUndefined");}
        virtual EgcNode* copy(void) {return nullptr;}
        static EgcNode* create() {return nullptr;}
        EgcNode();
        virtual ~EgcNode() = 0;
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * An expression is valid if all nodes are valid.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void);
        /**
         * @brief isContainer returns if the current element is a container or not
         * @return true if it is a container, false otherwise
         */
        virtual bool isContainer(void) const;
        /**
         * @brief isUnaryExpression returns if the current element is a unary expression (container) or not
         * @return true if it is a unary expression, false otherwise
         */
        virtual bool isUnaryNode(void) const;
        /**
         * @brief isBinaryExpression returns if the current element is a binary expression (container) or not
         * @return true if it is a binary expression, false otherwise
         */
        virtual bool isBinaryNode(void) const;
        /**
         * @brief isFlexNode returns if the current element is a flex node (container) or not
         * @return true if it is a flex node, false otherwise
         */
        virtual bool isFlexNode(void) const;
        /**
         * @brief getParent returns a pointer to the parent node
         * @return a pointer to the parent node or NULL if this is the root element
         */
        EgcContainerNode* getParent(void) const;
        /**
         * @brief isChild checks if the given node is the parent of this node
         * @param parent the node to check wether it is the parent of the current node or not.
         * @return true if the given node is the parent, false otherwise
         */
        bool isChild(EgcNode& parent);
        /**
         * @brief provideParent THIS IS NO USER INTERFACE! DO NOT USE THAT FROM USER POINT OF VIEW!
         * This is used to set the parent pointers in the implementation of the tree.
         * @param parent a pointer to the parent of this node. This must be a container pointer.
         */
        void provideParent(EgcContainerNode* parent);
        /**
         * @brief accept takes a visitor and calls the given visitor back to be able to extract data from this node
         * @param visitor the visitor to call back
         */
        virtual void accept(EgcNodeVisitor *visitor);
        /**
         * @brief operator== comparison operator overload
         * @param node the node to compare against
         * @return true if the trees are equal
         */
        virtual bool operator==(const EgcNode& node) const;
        /**
         * @brief nrSubindexes returns the number of subindexes of this node. This can be e.g. the number of characters
         * of a number or variable
         * @return 0 if not overwritten, since a std. node has no subindexes
         */
        virtual int nrSubindexes(void) const;
        /**
         * @brief isOperation checks if the node is a operation. There are also nodes that are containers, but no operations
         * @return true if the node is an operation, false otherwise
         */
        virtual bool isOperation(void) const;
        /**
         * @brief cursorSnaps find out where a cursor will snap in (e.g. a division node will snap at right and at the 
         * left side of the container)
         * @param side the side to test for cursor snap.
         * @return true if the cursor will snap in at the given side, false otherwise
         */
        virtual bool cursorSnaps(EgcNodeSide side) const;
        /**
         * @brief visibleSigns find out where the node has visible signs (e.g. a division node has visible signs in the 
         * middle of the container)
         * @param side the side to test for visible signs
         * @return true if the given side of the node has visible signs.
         */
        virtual bool visibleSigns(EgcNodeSide side) const;
        /**
         * @brief hasVisibleSigns checks if the node has any visible signs
         * @return returns true if the node has visible signs, false otherwise
         */
        bool hasVisibleSigns(void) const;
        /**
         * @brief modifyableElement find out where the node has modifyable elements (e.g. a exponent node has a
         * modifyable operator in the middle, but it is not visible)
         * @param side the side to test for visible signs
         * @return true if the given side of the node has modifyable elements.
         */
        virtual bool modifyableElement(EgcNodeSide side) const;
        /**
         * @brief isAtomicallyBoundChild if a node is a child of a atomic node it may not be deleted seperately. There may be a
         * parent that is marked as atomic (is deleteable), then the parent must be deleted in order to delete the not
         * deleteable sub-node. This functionality is in some ways comparable to a composition. The also must not be
         * inserted a node in between (this would change the structure).
         * @return true if node is a child of a atomic node, false if NOT a child of a atomic node (majority).
         */
        virtual bool isAtomicallyBoundChild(void) const;

protected:

        static const EgcNodeType s_nodeType = EgcNodeType::NodeUndefined;
        /**
         * @brief notifyContainerOnChildDeletion notifies a parent (container type) about deletion of (one) of its childs
         * @param child a pointer to the child that will be deleted soon
         */
        virtual void notifyContainerOnChildDeletion(EgcNode* child) { (void)child; }

        EgcContainerNode *m_parent;    ///< pointer to the parent (is needed for traversing the tree)
};

#endif // EGCNODE_H
