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

#ifndef EGCFNCCONTAINERNODE_H
#define EGCFNCCONTAINERNODE_H

#include <QString>
#include "../specialNodes/egcargumentsnode.h"

/**
 * @brief The EgcFuntionNode class is a class to model function calls. This manages e.g. something like calculation1(10, x + 3, 9.8234).
 */
class EgcFncContainerNode : public EgcArgumentsNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcFncContainerNode, EgcNodeType::FncContainerNode);
public:
        EgcFncContainerNode();
        /**
         * @brief transferArgs transfers all arguments to the new function container
         * @param args the argument list to transfer
         * @return true if everything went well, false otherwise
         */
        virtual bool transferArgs(EgcArgumentsNode& args);
        /**
         * @brief cursorSnaps find out where a cursor will snap in (e.g. a division node will snap at right and at the
         * left side of the container)
         * @param side the side to test for cursor snap.
         * @return true if the cursor will snap in at the given side, false otherwise
         */
        virtual bool cursorSnaps(EgcNodeSide side) const override;
        /**
         * @brief visibleSigns find out where the node has visible signs (e.g. a division node has visible signs in the
         * middle of the container)
         * @param side the side to test for visible signs
         * @return true if the given side of the node has visible signs.
         */
        virtual bool visibleSigns(EgcNodeSide side) const override;
};

#endif // EGCFNCCONTAINERNODE_H