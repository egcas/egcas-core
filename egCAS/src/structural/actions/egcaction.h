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

#ifndef EGCACTION_H
#define EGCACTION_H

#include <QChar>
#include <QVariant>
#include <QMetaType>
#include "egcoperations.h"

/**
 * @brief The EgcAction struct describes an operation to do
 */
class EgcAction
{
public:
        EgcAction() : m_op{EgcOperations::formulaActivated}, m_elementId{0}, m_subId{0},
                      m_intType{InternalFunctionType::undefined},
                      m_OpModificators{OpModificators::standard}, m_lookModificatiors{LookModificators::standard} {}
        EgcAction(EgcOperations op, QChar ch, quint32 elId = 0, quint32 subId = 0,
                  OpModificators mod = OpModificators::standard, LookModificators look = LookModificators::standard,
                  QVariant addDat = QVariant(), InternalFunctionType intType = InternalFunctionType::undefined) :
                                                                            m_op{op},
                                                                            m_character{ch},
                                                                            m_elementId{elId},
                                                                            m_subId{subId},
                                                                            m_intType{intType},
                                                                            m_OpModificators{mod},
                                                                            m_lookModificatiors{look},
                                                                            m_additionalData{addDat}

        {}

        EgcAction(EgcOperations op,
                  InternalFunctionType intType,
                  OpModificators mod = OpModificators::standard,
                  LookModificators look = LookModificators::standard,
                  QVariant addDat = QVariant()) :
                                      m_op{op}, m_character{QChar()},
                                      m_elementId{0}, m_subId{0},
                                      m_intType{intType},
                                      m_OpModificators{mod},
                                      m_lookModificatiors{look},
                                      m_additionalData{addDat}
        {}

        EgcAction(EgcOperations op,
                  OpModificators mod = OpModificators::standard,
                  LookModificators look = LookModificators::standard,
                  QVariant addDat = QVariant()) :
                                      m_op{op}, m_character{QChar()},
                                      m_elementId{0}, m_subId{0},
                                      m_intType{InternalFunctionType::undefined},
                                      m_OpModificators{mod},
                                      m_lookModificatiors{look},
                                      m_additionalData{addDat}
        {}

        EgcAction(const EgcAction &rhs) {m_op = rhs.m_op; m_character = rhs.m_character; m_elementId = rhs.m_elementId;
                                         m_subId = rhs.m_subId; m_intType = rhs.m_intType ;
                                         m_OpModificators = rhs.m_OpModificators;
                                         m_lookModificatiors = rhs.m_lookModificatiors;
                                         m_additionalData = rhs.m_additionalData;}
        ~EgcAction() {}

        EgcOperations m_op;               ///< the operation to do
        QChar m_character;                ///< any character that comes along with this action (e.g. user pressed any key)
        quint32 m_elementId;              ///< any element id. Must be interpreted by receiver class
        quint32 m_subId;                  ///< additional id data. Must be interpreted by the receiver class
        InternalFunctionType m_intType;   ///< type of the internal function
        OpModificators m_OpModificators;  ///< any modificators of the action/operation
        LookModificators m_lookModificatiors; ///< modificators for the look of an operator
        QVariant m_additionalData;        ///< an action can have some additional data
};

Q_DECLARE_METATYPE(EgcAction);

#endif // EGCACTION_H
