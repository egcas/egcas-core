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

#ifndef EGCTEXTCREATOR_H
#define EGCTEXTCREATOR_H

#include "egcabstractentitycreator.h"

class EgcTextCreator : public EgcAbstractEntityCreator
{
public:
        EgcTextCreator();
        virtual ~EgcTextCreator();
        /**
         * @brief create create a entity including all necessary setup in the scene (creating scene items etc.)
         * @param list the list where to insert the entity
         * @param point the point where to show the entity on the scene
         * @return the created entity
         */
        virtual EgcEntity* create(EgcEntityList* list, QPointF point) override;
        /**
         * @brief copy copy a given entity including all of its subsequent elements
         * @param list the list to add the copied entity to
         * @param entity entity to copy
         * @return the created entity
         */
        virtual EgcEntity* clone(EgcEntityList& list, EgcEntity& entity2copy) override;
};

#endif // EGCTEXTCREATOR_H
