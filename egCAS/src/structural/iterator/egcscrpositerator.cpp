/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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


#include "egcscrpositerator.h"
#include "entities/egcformulaentity.h"
#include "specialNodes/egcnode.h"
#include "egcscreenpos.h"
#include "iterator/screenHelpers/egcidnodeiter.h"
#include "iterator/screenHelpers/egcsubidnodeiter.h"

EgcScrPosIterator::EgcScrPosIterator(const EgcFormulaEntity& formula) : m_lookup{formula.getMathmlMappingCRef()},
                                                                        m_nodeIter{new EgcIdNodeIter(formula)},
                                                                        m_subind{-1}
{

        m_node = &m_nodeIter->previous();
        m_subIdIter.reset(new EgcSubindNodeIter(*m_node));
        // the subid iterator is at the beginning, so position it at the end.
        m_subIdIter->toBack();
}

EgcScrPosIterator::~EgcScrPosIterator()
{
}

bool EgcScrPosIterator::hasNext(void) const
{
        if (m_nodeIter->hasNext() || m_subIdIter->hasNext())
                return true;
        else
                return false;
}

bool EgcScrPosIterator::hasPrevious(void) const
{
        if (m_nodeIter->hasPrevious() || m_subIdIter->hasPrevious())
                return true;
        else
                return false;
}

const quint32 EgcScrPosIterator::next(void)
{
        if (m_subIdIter->hasNext()) {
                m_subind = m_subIdIter->next();
        } else if (m_nodeIter->hasNext()) {
                m_node = &m_nodeIter->next();
                m_subIdIter->setNode(*m_node);
        }

        return m_nodeIter->id();
}

const quint32 EgcScrPosIterator::previous(void)
{
        if (m_subIdIter->hasPrevious()) {
                m_subind = m_subIdIter->previous();
        } else if (m_nodeIter->hasPrevious()) {
                m_node = &m_nodeIter->previous();
                m_subIdIter->setNode(*m_node);
                m_subIdIter->toBack();
        }

        return m_nodeIter->id();
}

const quint32 EgcScrPosIterator::peekNext(void) const
{
        m_nodeIter->peekNextId();
}

const quint32 EgcScrPosIterator::peekPrevious(void) const
{
        m_nodeIter->peekPreviousId();
}

void EgcScrPosIterator::toBack(void)
{
        m_subind = -1;
        m_nodeIter->toBack();
        m_node = &m_nodeIter->previous();
        m_subIdIter->setNode(*m_node);
        m_subIdIter->toBack();
}

void EgcScrPosIterator::toFront(void)
{
        m_subind = -1;
        m_nodeIter->toFront();
        m_node = &m_nodeIter->next();
        m_subIdIter->setNode(*m_node);
        m_subIdIter->toFront();
}

const EgcNode* EgcScrPosIterator::node(void)
{
        return m_node;
}

bool EgcScrPosIterator::rightSide(void)
{

}

int EgcScrPosIterator::subIndex(void)
{
        return m_subind;
}

quint32 EgcScrPosIterator::id(void)
{
        return m_nodeIter->id();
}
