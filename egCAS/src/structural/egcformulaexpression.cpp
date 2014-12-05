#include <new>
#include "egcformulaexpression.h"
#include "egcexpressionnode.h"
#include "egcexpressionnodecreator.h"
#include "egcbaseexpressionnode.h"

EgcFormulaExpression::EgcFormulaExpression(EgcExpressionNodeType type)
{
        m_data = new (std::nothrow) EgcBaseExpressionNode();
        if (m_data) {
                EgcExpressionNode* tmp = EgcExpressionNodeCreator::create(type);
                if (tmp)
                        m_data->setChild(*tmp);
        }
}

EgcFormulaExpression::~EgcFormulaExpression()
{
        // delete the formula with the complete tree
        delete m_data;
}

EgcBaseExpressionNode& EgcFormulaExpression::getBaseElement(void) const
{
        return *m_data;
}

EgcExpressionNode* EgcFormulaExpression::getRootElement(void) const
{
        EgcExpressionNode* retval = nullptr;

        if (m_data)
                retval = m_data->getChild();

        return retval;
}