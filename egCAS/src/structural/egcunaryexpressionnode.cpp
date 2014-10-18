#include "egcunaryexpressionnode.h"
#include "egcexpressionnodecreator.h"

EgcUnaryExpressionNode::EgcUnaryExpressionNode() : m_child(nullptr)
{

}

EgcUnaryExpressionNode::EgcUnaryExpressionNode(const EgcUnaryExpressionNode& orig)
{
        m_child = nullptr;
        EgcExpressionNode *originalChild = const_cast<EgcUnaryExpressionNode&>(orig).getChild();
        if (originalChild)
                m_child = EgcExpressionNodeCreator::copy(*originalChild);

        //set the parent also
        if(originalChild)
                originalChild->provideParent(*this);
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

        //set the parent also
        if(m_child)
                m_child->provideParent(*this);
}

EgcExpressionNode* EgcUnaryExpressionNode::getChild(void) const
{
        return m_child;
}

EgcUnaryExpressionNode& EgcUnaryExpressionNode::operator=(const EgcUnaryExpressionNode &rhs)
{
        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (m_child) {
                delete m_child;
                m_child = nullptr;
        }
        //and create a new one
        EgcExpressionNode *originalChild = rhs.getChild();
        if (originalChild)
                m_child = EgcExpressionNodeCreator::copy(*originalChild);

        return *this;
}

bool EgcUnaryExpressionNode::valid(void)
{
        if (m_child)
                if (m_child->valid())
                        return true;

        return false;
}

bool EgcUnaryExpressionNode::isUnaryExpression(void)
{
        return true;
}

void EgcUnaryExpressionNode::notifyContainerOnChildDeletion(EgcExpressionNode* child)
{
        if (m_child == child)
                m_child = nullptr;
}

bool EgcUnaryExpressionNode::isLeaf(void) const
{
        if (m_child == nullptr)
                return true;
        else
                return false;
}
