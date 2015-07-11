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

#ifndef EGCENTITYLIST_H
#define EGCENTITYLIST_H

#include <QList>
#include <memory>
#include "egcentity.h"

/**
 * @brief The EgcEntityList class is a list that holds formulas, text and picture items
 */
class EgcEntityList
{
public:
        /// std constructor
        EgcEntityList();
        /// sort the list
        void sort(void);
        /**
         * @brief addEntity add a entity (formula, text or picture) to the list. The list takes ownership of the entity
         * @param entity a pointer to the entity to add
         * @return true if everything worked well, false otherwise
         */
        bool addEntity(EgcEntity* entity);
        /**
         * @brief deleteEntity removes the given entity from the list and deletes it (since the list has the ownership).
         * @param entity the entity to delete
         * @return true if all was fine, false otherwise
         */
        bool deleteEntity(EgcEntity* entity);
        /**
         * @brief takeEntity removes the given entity from the list and returns it (the entity is not deleted). The
         * the caller takes the ownership of the entity!!!
         * @param entity the entiy to remove from list.
         * @return the entity removed from the list.
         */
        EgcEntity* takeEntity(EgcEntity* entity);
private:
        QList<std::unique_ptr<EgcEntity>> m_list;                ///< holds a bunch of entities of a document
};

#endif // EGCENTITYLIST_H