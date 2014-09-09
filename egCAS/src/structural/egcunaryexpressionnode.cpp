#include "egcunaryexpressionnode.h"
#include "egcexpressionnodecreator.h"

EgcUnaryExpressionNode::EgcUnaryExpressionNode() : m_child(nullptr)
{

}

EgcUnaryExpressionNode::EgcUnaryExpressionNode(const EgcUnaryExpressionNode& orig)
{
        EgcExpressionNode *originalChild = const_cast<EgcUnaryExpressionNode&>(orig).getChild();
        m_child = EgcExpressionNodeCreator::copy(*originalChild);
}

EgcUnaryExpressionNode::~EgcUnaryExpressionNode()
{
        if (m_child)
                delete m_child;
}

void EgcUnaryExpressionNode::setChild(const EgcExpressionNode& expression)
{
        if (m_child)
                delete m_child;
        m_child = const_cast<EgcExpressionNode*>(&expression);
}

EgcExpressionNode* EgcUnaryExpressionNode::getChild(void)
{
        return m_child;
}

EgcUnaryExpressionNode& EgcUnaryExpressionNode::operator=(const EgcUnaryExpressionNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        delete m_child;
        //and create a new one
        EgcExpressionNode *originalChild = const_cast<EgcUnaryExpressionNode&>(rhs).getChild();
        m_child = EgcExpressionNodeCreator::copy(*originalChild);

        return *this;
}
