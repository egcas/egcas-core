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

#include <QString>
#include <QtTest>
#include "tst_egcastest_structural.h"
#include "casKernel/parser/abstractkernelparser.h"
#include "casKernel/parser/restructparserprovider.h"

//implementation of some mock classes for restruct parser
class EgcTestKernelParser : public AbstractKernelParser
{
public:
        EgcTestKernelParser() {}
        virtual ~EgcTestKernelParser() {}
        virtual EgcNode* restructureFormula(const QString& strToParse, NodeIterReStructData& iterData, int* errCode) override {return nullptr;};
};

AbstractKernelParser* RestructParserProvider::s_parser = nullptr;
RestructParserProvider::RestructParserProvider()
{
        if (s_parser == nullptr)
                s_parser = new EgcTestKernelParser();
}
RestructParserProvider::~RestructParserProvider() {}
AbstractKernelParser* RestructParserProvider::getRestructParser(void) { return s_parser;}


class EgcasTest_Structural : public QObject
{
        Q_OBJECT

public:
        EgcasTest_Structural() {}

private Q_SLOTS:
        void testChildDeletion();
        void testCopyConstructors();
        void testIterator();
        void testTransferProperties();
        void testInsertDelete();
        void testVisitors();
        void testFlexNode();
        void testFlexNodeVisitors();
private:
        EgcNode* addChild(EgcNode&parent, EgcNodeType type, QString number = "0");
        EgcNode* addLeftChild(EgcNode&parent, EgcNodeType type, QString number = "0");
        EgcNode* addRightChild(EgcNode&parent, EgcNodeType type, QString number = "0");
};


bool EgcUnaryNodeTestChild::deleted = false;
bool EgcBinaryNodeTestChild::deleted = false;
bool EgcBinaryNodeTestChild2::deleted = false;

void EgcasTest_Structural::testChildDeletion()
{
        //check if childs getting deleted
        EgcUnaryNodeTest *parent = new EgcUnaryNodeTest();
        EgcUnaryNodeTestChild *child = new EgcUnaryNodeTestChild();

        parent->setChild(0, *child);

        QVERIFY(child->deleted == false);
        delete(parent);
        QVERIFY(child->deleted == true);

        //do the same with the binary class
        EgcBinaryNodeTest *parent_bin = new EgcBinaryNodeTest();
        EgcBinaryNodeTestChild *child_bin = new EgcBinaryNodeTestChild();
        EgcBinaryNodeTestChild2 *child2_bin = new EgcBinaryNodeTestChild2();

        parent_bin->setChild(0, *child_bin);
        parent_bin->setChild(1, *child2_bin);

        QVERIFY(child_bin->deleted == false);
        QVERIFY(child2_bin->deleted == false);
        delete(parent_bin);
        QVERIFY(child_bin->deleted == true);
        QVERIFY(child2_bin->deleted == true);
}

void EgcasTest_Structural::testCopyConstructors()
{
        //test copy constructor of binary expression
        EgcRootNode rootExpression;
        auto *rootChildExpression = new EgcRootNode();
        auto *numberExpression = new EgcNumberNode();
        auto *numberExpression2 = new EgcNumberNode();
        numberExpression->setValue(QString("200.1"));
        numberExpression2->setValue("90.365");
        rootChildExpression->setChild(0, *numberExpression);
        rootExpression.setChild(0, *rootChildExpression);
        rootExpression.setChild(1, *numberExpression2);

        EgcRootNode copyExpression(rootExpression);

        auto *copyChild = static_cast<EgcBinaryNode*>(copyExpression.getChild(0));
        auto *numberChild1 = static_cast<EgcNumberNode*>(copyChild->getChild(0));
        auto *numberChild2 = static_cast<EgcNumberNode*>(copyExpression.getChild(1));
        QVERIFY(numberChild1->getValue() == "200.1");
        QVERIFY(numberChild2->getValue() == "90.365");
        QVERIFY(copyChild->valid() == false);
        QVERIFY(numberChild2->valid() == true);

}

void EgcasTest_Structural::testTransferProperties()
{

        /*  This tree is tested within the transfer test below
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 5 |
                                       |---|             |---|
                                    /       \
                                   /         \
                                |---|       |---|
                                | 3 |       | 4 |
                                |---|       |---|
        */

        EgcFormulaEntity formula(EgcNodeType::RootNode);

        EgcRootNode* node1 = static_cast<EgcRootNode*>(formula.getRootElement());
        auto *node2 = new EgcRootNode();
        auto *node3 = new EgcNumberNode();
        auto *node4 = new EgcNumberNode();
        auto *node5 = new EgcNumberNode();
        node3->setValue("3");
        node4->setValue("4");
        node5->setValue("5");
        node1->setChild(0, *node2);
        node1->setChild(1, *node5);
        node2->setChild(0, *node3);
        node2->setChild(1, *node4);

        bool retval;
        EgcRootNode *transferNode1 = static_cast<EgcRootNode*>(
                                        EgcNodeCreator::create(EgcNodeType::RootNode));
        transferNode1->setChild(0, *(new EgcEmptyNode()));        
        retval = transferNode1->transferProperties(*node2);
        QVERIFY(retval == false);
        EgcRootNode *transferNode2 = static_cast<EgcRootNode*>(
                                        EgcNodeCreator::create(EgcNodeType::RootNode));
        transferNode2->setChild(1, *(new EgcEmptyNode()));
        retval = transferNode2->transferProperties(*node2);
        QVERIFY(retval == false);
        EgcParenthesisNode *transferNode3 = static_cast<EgcParenthesisNode*>(
                                        EgcNodeCreator::create(EgcNodeType::ParenthesisNode));
        transferNode3->setChild(0, *(new EgcEmptyNode()));
        retval = transferNode3->transferProperties(*node2);
        QVERIFY(retval == false);
        EgcRootNode *transferNode4 = static_cast<EgcRootNode*>(
                                        EgcNodeCreator::create(EgcNodeType::RootNode));
        retval = transferNode4->transferProperties(*node2);
        QVERIFY(retval == true);

        QVERIFY(node2->getChild(0) == nullptr);
        QVERIFY(node2->getChild(1) == nullptr);
        QVERIFY(node2->getParent() == nullptr);
        QVERIFY(node1->getChild(0) == transferNode4);

        QVERIFY((static_cast<EgcNumberNode*>(transferNode4->getChild(0)))->getValue() == "3");
        QVERIFY((static_cast<EgcNumberNode*>(transferNode4->getChild(1)))->getValue() == "4");
        QVERIFY(transferNode4->getParent() == node1);

        delete(node2);

        QVERIFY((static_cast<EgcNumberNode*>(transferNode4->getChild(0)))->getValue() == "3");
        QVERIFY((static_cast<EgcNumberNode*>(transferNode4->getChild(1)))->getValue() == "4");
        QVERIFY(transferNode4->getParent() == node1);

        delete(transferNode1);
        delete(transferNode2);
        delete(transferNode3);
}

void EgcasTest_Structural::testIterator()
{
        //-----------------------------------------------------------------------------
        /*This tree is tested within the iterator test below
        //-----------------------------------------------------------------------------

                                               |---|
                                               | 1 |  (Numbernode)
                                               |---|
        */

        //test iterator with big structure
        EgcFormulaEntity formula3(EgcNodeType::NumberNode);
        EgcIteratorState stepState;
        EgcNode* nodePointer;

        //test step iterator
        EgcNodeIterator iter7(formula3);

        iter7.toFront();

        QVERIFY(iter7.hasNext() == true);
        QVERIFY(iter7.hasPrevious() == false);
        nodePointer = &(iter7.next());
        QVERIFY(nodePointer == formula3.getRootElement());
        QVERIFY(iter7.hasNext() == false);

        nodePointer = &(iter7.next());
        QVERIFY(nodePointer == formula3.getRootElement());

        nodePointer = &(iter7.next());
        QVERIFY(nodePointer == formula3.getRootElement());

        iter7.toBack();

        QVERIFY(iter7.hasPrevious() == true);
        QVERIFY(iter7.hasNext() == false);
        nodePointer = &(iter7.previous());
        QVERIFY(nodePointer == formula3.getRootElement());
        QVERIFY(iter7.hasPrevious() == false);

        nodePointer = &(iter7.previous());
        QVERIFY(nodePointer == formula3.getRootElement());


        /*  This tree is tested within the iterator test below
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 4 |
                                       |---|             |---|
                                    /       \
                                   /         \
                                |---|       nullptr
                                | 3 |
                                |---|
        */

        EgcFormulaEntity formula(EgcNodeType::PlusNode);

        EgcNodeIterator iter(formula);
        QVERIFY(iter.hasNext() == true);
        QVERIFY(iter.hasPrevious() == false);

        EgcPlusNode* rootExpression = static_cast<EgcPlusNode*>(formula.getRootElement());
        auto *rootChildExpression = new EgcPlusNode();
        auto *numberExpression = new EgcNumberNode();
        auto *numberExpression2 = new EgcNumberNode();
        numberExpression->setValue("200.1");
        numberExpression2->setValue("90.365");
        rootChildExpression->setChild(0, *numberExpression);
        rootExpression->setChild(0, *rootChildExpression);
        rootExpression->setChild(1, *numberExpression2);

        //-------------------test this structure with iterator--------------------


        EgcNodeIterator iter6(formula);

        //test hasNext and hasPrevious
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(iter6.hasPrevious() == false);
        //test peek functions
        QVERIFY(&(iter6.peekNext()) == rootExpression);
        QVERIFY(&(iter6.peekPrevious()) == &formula.getBaseElement());

        //test next functions (node 1)
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 2
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 3
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 2
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 1
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 4
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 1
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter6.hasNext() == false);

        iter6.toBack();

        //node 1
        nodePointer = &(iter6.previous());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 4
        nodePointer = &(iter6.previous());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 1
        nodePointer = &(iter6.previous());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 2
        nodePointer = &(iter6.previous());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 3
        nodePointer = &(iter6.previous());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 2
        nodePointer = &(iter6.previous());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 1
        nodePointer = &(iter6.previous());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter6.hasPrevious() == false);

        //test overflow
        nodePointer = &(iter6.previous());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter6.hasPrevious() == true);
        QVERIFY(iter6.hasNext() == true);

        iter6.toFront();

        //node 1
        QVERIFY(iter6.hasPrevious() == false);        
        nodePointer = &(iter6.next());
        stepState = iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //node 2
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 3
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == numberExpression);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 2
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootChildExpression);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 1
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 4
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == numberExpression2);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter6.hasNext() == true);

        //node 1
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter6.hasNext() == false);

        //check overflow
        nodePointer = &(iter6.next());
        stepState =iter6.getLastState();
        QVERIFY(nodePointer == rootExpression);
        QVERIFY(iter6.hasNext() == true);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter6.hasPrevious() == true);

        //test find routines
        iter6.toFront();

        QVERIFY(iter6.findNext(EgcNodeType::NumberNode) == true);
        QVERIFY(&(iter6.previous()) == numberExpression);

        iter6.toBack();

        QVERIFY(iter6.findPrevious(EgcNodeType::NumberNode) == true);
        QVERIFY(&(iter6.next()) == numberExpression2);


        //-----------------------------------------------------------------------------
        /*This tree is tested within the iterator test below
        //-----------------------------------------------------------------------------
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 7 |
                                       |---|             |---|
                                   /        \               /       \
                                  /          \           /            \
                              |---|         |---|      |---|         |---|
                              | 3 |         | 6 |      | 8 |         | 9 |
                              |---|         |---|      |---|         |---|
                             /  \                                  /        \
                            /    \                               /             \
                         |---|  |---|                      |---|             |---|
                         | 4 |  | 5 |                      | 10|             | 12|
                         |---|  |---|                      |---|             |---|
                                                             |
                                                           |---|
                                                           | 11|
                                                           |---|
        */

        //test iterator with big structure        
        EgcFormulaEntity formula2(EgcNodeType::PlusNode);

        EgcNode* rootExpression2;
        EgcNode* node2;
        EgcNode* node3;
        EgcNode* node7;
        EgcNode* node9;
        EgcNode* node10;
        EgcNode* node4;
        EgcNode* node5;
        EgcNode* node6;
        EgcNode* node8;
        EgcNode* node11;
        EgcNode* node12;

        rootExpression2 = static_cast<EgcPlusNode*>(formula2.getRootElement());
        node2 = addLeftChild(*rootExpression2, EgcNodeType::PlusNode);
        QVERIFY(node2 != nullptr);

        QVERIFY(addRightChild(*node2, EgcNodeType::NumberNode, "6.0") != nullptr);
        node6 = static_cast<EgcBinaryNode*>(node2)->getChild(1);

        node3 = addLeftChild(*node2, EgcNodeType::PlusNode);

        QVERIFY(addLeftChild(*node3, EgcNodeType::NumberNode, "4.0") != nullptr);
        QVERIFY(addRightChild(*node3, EgcNodeType::NumberNode, "5.0") != nullptr);
        node4 = static_cast<EgcBinaryNode*>(node3)->getChild(0);
        node5 = static_cast<EgcBinaryNode*>(node3)->getChild(1);

        node7 = addRightChild(*rootExpression2, EgcNodeType::PlusNode);
        QVERIFY(node7 != nullptr);        

        QVERIFY(addLeftChild(*node7, EgcNodeType::NumberNode, "8.0") != nullptr);
        node8 = static_cast<EgcBinaryNode*>(node7)->getChild(0);

        node9 = addRightChild(*node7, EgcNodeType::PlusNode);
        QVERIFY(node9 != nullptr);

        node10 = addLeftChild(*node9, EgcNodeType::ParenthesisNode);
        QVERIFY(node10 != nullptr);

        QVERIFY(addChild(*node10, EgcNodeType::NumberNode, "11.0") != nullptr);
        node11 = static_cast<EgcUnaryNode*>(node10)->getChild(0);

        QVERIFY(addRightChild(*node9, EgcNodeType::NumberNode, "12.0") != nullptr);
        node12 = static_cast<EgcBinaryNode*>(node9)->getChild(1);
        
        EgcBinaryNode* binaryNode;

        //-----------------------------------------------------------------------------
        //test step iterator
        //-----------------------------------------------------------------------------
        EgcNodeIterator iter5(formula2);

        //test hasNext and hasPrevious
        QVERIFY(iter5.hasNext() == true);
        QVERIFY(iter5.hasPrevious() == false);
        //test peek functions
        QVERIFY(&(iter5.peekNext()) == rootExpression2);

        //test next functions (node 1)
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter5.hasNext() == true);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 3
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node4
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 3
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 5
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 3
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 6
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 1
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 7
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 8
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 7
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 9
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 10
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 11
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        EgcUnaryNode* unaryNode = static_cast<EgcUnaryNode*>(node10);
        QVERIFY(nodePointer == unaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 10
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 9
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 12
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 9
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 7
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 1
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == false);

        //jump over to the start if one doesn't look at hasNext
        //node 1
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //test the previous function
        iter5.toBack();

        //-----------------------------------------------------------------------------
        //test step iterator backwards
        //-----------------------------------------------------------------------------

        //node 1
        QVERIFY(iter5.hasPrevious() == true);
        QVERIFY(iter5.hasNext() == false);
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 9
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 12
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 9
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 10
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 11
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        unaryNode = static_cast<EgcUnaryNode*>(node10);
        QVERIFY(nodePointer == unaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 10
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node9);
        QVERIFY(nodePointer == binaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 9
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 8
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node7);
        QVERIFY(nodePointer == binaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 1
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 2
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 6
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 2
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 3
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 5
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 3
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node4
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(node3);
        QVERIFY(nodePointer == binaryNode->getChild(0));
        QVERIFY(stepState == EgcIteratorState::MiddleIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 3
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node3);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 2
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 1
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == false);

        //jump over to the start if one doesn't look at hasPrevious
        //node 1
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //test turnarounds (from previous to next)
        iter5.toBack();

        //node 1
        QVERIFY(iter5.hasPrevious() == true);
        QVERIFY(iter5.hasNext() == false);
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 7
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        binaryNode = static_cast<EgcBinaryNode*>(rootExpression2);
        QVERIFY(nodePointer == binaryNode->getChild(1));
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 1
        QVERIFY(iter5.hasPrevious() == true);
        QVERIFY(iter5.hasNext() == true);
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::RightIteration);
        QVERIFY(iter5.hasNext() == false);

        //test turnarounds (from next to previous)
        iter5.toFront();

        //test next functions (node 1)
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(iter5.hasNext() == true);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.next());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasNext() == true);

        //node 2
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == true);

        //node 1
        nodePointer = &(iter5.previous());
        stepState = iter5.getLastState();
        QVERIFY(nodePointer == rootExpression2);
        QVERIFY(stepState == EgcIteratorState::LeftIteration);
        QVERIFY(iter5.hasPrevious() == false);
}

void EgcasTest_Structural::testInsertDelete()
{
        /*  This tree is tested within the iterator test below
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 4 |
                                       |---|             |---|
                                    /       \
                                   /         \
                                |---|       |---|
                                | 3 |       | 5 |
                                |---|       |---|
        */

        EgcFormulaEntity formula4(EgcNodeType::PlusNode);
        EgcIteratorState state;
        EgcNode *nodePointer;
        EgcNodeIterator iter8(formula4);
        EgcNode *node1;
        EgcNode *node3;
        EgcNode *node2;
        EgcNode *node4;
        EgcNode *node5;

        nodePointer = &(iter8.next());
        node1 = nodePointer;
        iter8.insert(EgcNodeType::PlusNode, false);
        nodePointer = &(iter8.previous());
        nodePointer = &(iter8.next());
        node2 = nodePointer;
        nodePointer = &(iter8.next());
        node3 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter8.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberNode*>(nodePointer)->setValue("3.452");
        node3 = nodePointer;
        nodePointer = &(iter8.next());
        nodePointer = &(iter8.next());
        node5 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter8.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberNode*>(nodePointer)->setValue("5.647");
        node5 = nodePointer;
        node4 = static_cast<EgcBinaryNode*>(node1)->getChild(1);

        //test if the nodes are in the order intended
        iter8.toFront();
        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcIteratorState::LeftIteration);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(state == EgcIteratorState::LeftIteration);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node3);
        QVERIFY(state == EgcIteratorState::MiddleIteration);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(state == EgcIteratorState::MiddleIteration);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node5);
        QVERIFY(state == EgcIteratorState::MiddleIteration);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node2);
        QVERIFY(state == EgcIteratorState::RightIteration);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcIteratorState::MiddleIteration);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node4);
        QVERIFY(state == EgcIteratorState::MiddleIteration);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcIteratorState::RightIteration);


        //now we remove node 2
        iter8.toFront();
        (void) iter8.next();
        (void) iter8.next();
        iter8.remove(false);

        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == formula4.getRootElement());
        QVERIFY(state == EgcIteratorState::MiddleIteration);

        //test if the nodes are in the order intended
        iter8.toFront();
        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcIteratorState::LeftIteration);
        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        QVERIFY(state == EgcIteratorState::MiddleIteration);
        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcIteratorState::MiddleIteration);
        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node4);
        QVERIFY(state == EgcIteratorState::MiddleIteration);
        nodePointer = &(iter8.next());
        state = iter8.getLastState();
        QVERIFY(nodePointer == node1);
        QVERIFY(state == EgcIteratorState::RightIteration);

}

void EgcasTest_Structural::testVisitors()
{
        /*  This tree is tested within the iterator test below
                                               |---|
                                               | 1 |
                                               |---|
                                             /         \
                                          /               \
                                       |---|             |---|
                                       | 2 |             | 4 |
                                       |---|             |---|
                                    /       \
                                   /         \
                                |---|       |---|
                                | 3 |       | 5 |
                                |---|       |---|
        */

        EgcFormulaEntity formula4(EgcNodeType::PlusNode);
        EgcNode *nodePointer;
        EgcNodeIterator iter8(formula4);
        EgcNode *node1;
        EgcNode *node3;
        EgcNode *node2;
        EgcNode *node4;
        EgcNode *node5;

        nodePointer = &(iter8.next());
        node1 = nodePointer;
        iter8.insert(EgcNodeType::PlusNode, false);
        nodePointer = &(iter8.previous());
        nodePointer = &(iter8.next());
        node2 = nodePointer;
        nodePointer = &(iter8.next());
        node3 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter8.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberNode*>(nodePointer)->setValue("30.452");
        node3 = nodePointer;
        nodePointer = &(iter8.next());
        nodePointer = &(iter8.next());
        node5 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter8.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberNode*>(nodePointer)->setValue("3");
        node5 = nodePointer;
        nodePointer = &(iter8.next());
        nodePointer = &(iter8.next());
        nodePointer = &(iter8.next());
        nodePointer = iter8.replace(*nodePointer, EgcNodeType::NumberNode);
        static_cast<EgcNumberNode*>(nodePointer)->setValue("2");
        node4 = nodePointer;

        //test copy constructor of the formula expression
        EgcFormulaEntity formula5 = EgcFormulaEntity(formula4);
        EgcNode *node1_5 = formula5.getRootElement();
        QVERIFY(node1 != node1_5);
        nodePointer = static_cast<EgcRootNode*>(node1_5)->getChild(1);
        QVERIFY(static_cast<EgcNumberNode*>(nodePointer)->getValue() == "2");

        //test maxima visitor
        EgcMaximaVisitor maximaVisitor(formula4);
        QString result(maximaVisitor.getResult());
        QVERIFY(result == QString("fpprintprec:0$((30.452)+(3))+(2);"));
        QVERIFY(formula4.getCASKernelCommand() == QString("fpprintprec:0$((30.452)+(3))+(2);"));

        //test maxima visitor with copied formula
        EgcMaximaVisitor maximaVisitor2(formula5);
        QString result2(maximaVisitor2.getResult());
        QVERIFY(result2 == QString("fpprintprec:0$((30.452)+(3))+(2);"));
        QVERIFY(formula5.getCASKernelCommand() == QString("fpprintprec:0$((30.452)+(3))+(2);"));

        //test math ml visitor
        EgcMathMlVisitor mathMlVisitor(formula4);
        result = mathMlVisitor.getResult();        
        QVERIFY(result == QString("<math><mrow  id=\"6\" ><mrow  id=\"3\" ><mn id=\"1\" >30.452</mn><mo id=\"4\" >+</mo><mn id=\"2\" >3</mn></mrow><mo id=\"7\" >+</mo><mn id=\"5\" >2</mn></mrow></math>"));
        QVERIFY(formula4.getMathMlCode() == QString("<math><mrow  id=\"6\" ><mrow  id=\"3\" ><mn id=\"1\" >30.452</mn><mo id=\"4\" >+</mo><mn id=\"2\" >3</mn></mrow><mo id=\"7\" >+</mo><mn id=\"5\" >2</mn></mrow></math>"));
}

void EgcasTest_Structural::testFlexNode()
{
        /*  This tree is tested within the iterator test below
                                               |---|
                                               | 1 |
                                               |---|  \
                                              /           \
                                             /                \
                                       |----------|             |---|
                                       |    2     |             | 8 |
                                       |----------|             |---|
                                    /    |      |    \
                                   /     |      |     \
                                |---|  |---|  |---|  |---|
                                | 3 |  | 4 |  | 6 |  | 7 |
                                |---|  |---|  |---|  |---|
                                         |
                                         |
                                       |---|
                                       | 5 |
                                       |---|
        */

        EgcFormulaEntity formula5(EgcNodeType::PlusNode);
        EgcNode *nodePointer;
        EgcNodeIterator iter9(formula5);
        EgcNode *node1;
        EgcNode *node3;
        EgcNode *node2;
        EgcNode *node4;
        EgcNode *node5;
        EgcNode *node6;
        EgcNode *node7;
        EgcNode *node8;

        nodePointer = &(iter9.next());
        node1 = nodePointer;
        iter9.insert(EgcNodeType::FunctionNode, false);
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.next());
        node2 = nodePointer;
        nodePointer = &(iter9.next());
        node3 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberNode*>(nodePointer)->setValue("3");
        node3 = nodePointer;
        QVERIFY(iter9.insertChildSpace(EgcNodeType::FunctionNode) == true);
        QVERIFY(iter9.insertChildSpace(EgcNodeType::FunctionNode) == true);
        QVERIFY(iter9.insertChildSpace(EgcNodeType::FunctionNode) == true);
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->isFlexNode() == true);
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::ParenthesisNode);
        QVERIFY(nodePointer != nullptr);
        node4 = nodePointer;
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.previous());
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        node5 = nodePointer;
        static_cast<EgcNumberNode*>(nodePointer)->setValue("5");
        nodePointer = &(iter9.next());
        nodePointer = &(iter9.next());
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::FunctionNode);
        QVERIFY(nodePointer->isFlexNode() == true);
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        node6 = nodePointer;
        static_cast<EgcNumberNode*>(nodePointer)->setValue("6");
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->isFlexNode() == true);
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        node7 = nodePointer;
        static_cast<EgcNumberNode*>(nodePointer)->setValue("7");
        nodePointer = &(iter9.next());
        nodePointer = &(iter9.next());
        nodePointer = &(iter9.next());
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        node8 = nodePointer;
        static_cast<EgcNumberNode*>(nodePointer)->setValue("8");
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.previous());
        QVERIFY(static_cast<EgcNumberNode*>(nodePointer)->getValue() == "6");
        iter9.remove(true);
        QVERIFY(static_cast<EgcFlexNode*>(node2)->getNumberChildNodes() == 3);
}

void EgcasTest_Structural::testFlexNodeVisitors()
{
        /*  This tree is tested within the iterator test below
                                               |---|
                                               | 1 |
                                               |---|  \
                                              /           \
                                             /                \
                                       |----------|             |---|
                                       |    2     |             | 8 |
                                       |----------|             |---|
                                    /    |      |    \
                                   /     |      |     \
                                |---|  |---|  |---|  |---|
                                | 3 |  | 4 |  | 6 |  | 7 |
                                |---|  |---|  |---|  |---|
                                         |
                                         |
                                       |---|
                                       | 5 |
                                       |---|
        */

        EgcFormulaEntity formula5(EgcNodeType::PlusNode);
        EgcNode *nodePointer;
        EgcNodeIterator iter9(formula5);
        EgcNode *node1;
        EgcNode *node3;
        EgcNode *node2;
        EgcNode *node4;
        EgcNode *node5;
        EgcNode *node6;
        EgcNode *node7;
        EgcNode *node8;

        nodePointer = &(iter9.next());
        node1 = nodePointer;
        iter9.insert(EgcNodeType::FunctionNode, false);
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.next());
        node2 = nodePointer;
        nodePointer = &(iter9.next());
        node3 = nodePointer;
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        static_cast<EgcNumberNode*>(nodePointer)->setValue("3");
        node3 = nodePointer;
        QVERIFY(iter9.insertChildSpace(EgcNodeType::FunctionNode) == true);
        QVERIFY(iter9.insertChildSpace(EgcNodeType::FunctionNode) == true);
        QVERIFY(iter9.insertChildSpace(EgcNodeType::FunctionNode) == true);
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->isFlexNode() == true);
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::ParenthesisNode);
        QVERIFY(nodePointer != nullptr);
        node4 = nodePointer;
        nodePointer = &(iter9.previous());
        nodePointer = &(iter9.previous());
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        node5 = nodePointer;
        static_cast<EgcNumberNode*>(nodePointer)->setValue("5");
        nodePointer = &(iter9.next());
        nodePointer = &(iter9.next());
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::FunctionNode);
        QVERIFY(nodePointer->isFlexNode() == true);
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        node6 = nodePointer;
        static_cast<EgcNumberNode*>(nodePointer)->setValue("6");
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->isFlexNode() == true);
        nodePointer = &(iter9.next());
        QVERIFY(nodePointer->getNodeType() == EgcNodeType::EmptyNode);
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        node7 = nodePointer;
        static_cast<EgcNumberNode*>(nodePointer)->setValue("7");
        nodePointer = &(iter9.next());
        nodePointer = &(iter9.next());
        nodePointer = &(iter9.next());
        nodePointer = iter9.replace(*nodePointer, EgcNodeType::NumberNode);
        QVERIFY(nodePointer != nullptr);
        node8 = nodePointer;
        static_cast<EgcNumberNode*>(nodePointer)->setValue("8");
        static_cast<EgcFunctionNode*>(node2)->setName("testFunction");

        //test maxima visitor
        QVERIFY(formula5.getCASKernelCommand() == QString("fpprintprec:0$(testFunction(3,(5),6,7))+(8);"));

        //test math ml visitor
        QVERIFY(formula5.getMathMlCode() == QString("<math><mrow  id=\"8\" ><mrow  id=\"6\" ><mi  mathvariant=\"italic\" "
                                                    "id=\"6\" >testFunction</mi><mo>&ApplyFunction;</mo><mrow><mo "
                                                    "id=\"6\" >(</mo><mrow><mn id=\"1\" >3</mn><mo id=\"6\" >,</mo>"
                                                    "<mfenced  id=\"3\"  open=\"(\" close=\")\" separators=\",\"><mrow>"
                                                    "<mn id=\"2\" >5</mn></mrow></mfenced><mo id=\"6\" >,</mo><mn "
                                                    "id=\"4\" >6</mn><mo id=\"6\" >,</mo><mn id=\"5\" >7</mn></mrow>"
                                                    "<mo id=\"6\" >)</mo></mrow></mrow><mo id=\"9\" >+</mo><mn "
                                                    "id=\"7\" >8</mn></mrow></math>"));
}

EgcNode*EgcasTest_Structural::addChild(EgcNode& parent, EgcNodeType type, QString number)
{
        if (parent.isUnaryNode()) {
                EgcUnaryNode& node = static_cast<EgcUnaryNode&>(parent);
                if (   type == EgcNodeType::RootNode
                    || type == EgcNodeType::NumberNode
                    || type == EgcNodeType::ParenthesisNode) {
                        node.setChild(0, *EgcNodeCreator::create(type));
                        if (node.getChild(0) != nullptr) {
                                if (    type == EgcNodeType::RootNode
                                     || type == EgcNodeType::ParenthesisNode) {
                                        return node.getChild(0);
                                } else {
                                        EgcNumberNode* num = static_cast<EgcNumberNode*>(node.getChild(0));
                                        num->setValue(number);
                                        return node.getChild(0);
                                }

                        }
                }
        }

        return nullptr;
}

EgcNode*EgcasTest_Structural::addLeftChild(EgcNode& parent, EgcNodeType type, QString number)
{
        if (parent.isBinaryNode()) {
                EgcBinaryNode& node = static_cast<EgcBinaryNode&>(parent);
                if (   type == EgcNodeType::RootNode
                                || type == EgcNodeType::NumberNode
                                || type == EgcNodeType::ParenthesisNode
                                || type == EgcNodeType::PlusNode) {
                        node.setChild(0, *EgcNodeCreator::create(type));
                        if (node.getChild(0) != nullptr) {
                                if (    type == EgcNodeType::RootNode
                                                || type == EgcNodeType::ParenthesisNode
                                                || type == EgcNodeType::PlusNode) {
                                        return node.getChild(0);
                                } else {
                                        EgcNumberNode* num = static_cast<EgcNumberNode*>(node.getChild(0));
                                        num->setValue(number);
                                        return node.getChild(0);
                                }

                        }
                }
        }

        return nullptr;
}

EgcNode* EgcasTest_Structural::addRightChild(EgcNode& parent, EgcNodeType type, QString number)
{
        if (parent.isBinaryNode()) {
                EgcBinaryNode& node = static_cast<EgcBinaryNode&>(parent);
                if (   type == EgcNodeType::RootNode
                    || type == EgcNodeType::NumberNode
                    || type == EgcNodeType::ParenthesisNode
                    || type == EgcNodeType::PlusNode) {
                        node.setChild(1, *EgcNodeCreator::create(type));
                        if (node.getChild(1) != nullptr) {
                                if (    type == EgcNodeType::RootNode
                                     || type == EgcNodeType::ParenthesisNode
                                     || type == EgcNodeType::PlusNode) {
                                        return node.getChild(1);
                                } else {
                                        EgcNumberNode* num = static_cast<EgcNumberNode*>(node.getChild(1));
                                        num->setValue(number);
                                        return node.getChild(1);
                                }

                        }
                }
        }

        return nullptr;
}


QTEST_MAIN(EgcasTest_Structural)

#include "tst_egcastest_structural.moc"
