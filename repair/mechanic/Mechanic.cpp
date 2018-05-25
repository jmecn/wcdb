/*
 * Tencent is pleased to support the open source community by making
 * WCDB available.
 *
 * Copyright (C) 2017 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use
 * this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *       https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <WCDB/Assertion.hpp>
#include <WCDB/Mechanic.hpp>
#include <WCDB/Page.hpp>

namespace WCDB {

namespace Repair {

#pragma mark - Initialize
Mechanic::Mechanic(const std::string &path)
    : Repairman(), Crawlable(path, false)
{
}

#pragma mark - Material
void Mechanic::setMaterial(const Material &material)
{
    m_material = material;
}

void Mechanic::setMaterial(Material &&material)
{
    m_material = std::move(material);
}

#pragma mark - Mechanic
void Mechanic::work()
{
    WCTInnerAssert(canAssembled());
    if (!m_pager.initialize()) {
        markAsError();
        return;
    }

    int pageCount = 0;
    for (const auto &content : m_material.contents) {
        pageCount += content.pagenos.size();
    }
    setPageWeight((double) 1.0 / pageCount);

    for (const auto &content : m_material.contents) {
        if (!markAsAssembling(content.tableName) ||
            !assembleTable(content.sql)) {
            markAsError();
            continue;
        }
        for (const auto &pageno : content.pagenos) {
            if (!crawl(pageno)) {
                markAsError();
            }
        }
        if (!markAsAssembled()) {
            markAsError();
        }
    }
}

#pragma mark - Crawlable
bool Mechanic::onCellCrawled(const Cell &cell)
{
    Crawlable::onCellCrawled(cell);
    if (!assembleCell(cell)) {
        markAsError();
    }
    return true;
}

bool Mechanic::onPageCrawled(const Page &page, int unused)
{
    Crawlable::onPageCrawled(page, unused);
    if (page.getType() == Page::Type::LeafTable) {
        markCellCount(page.getCellCount());
        return true;
    }
    markAsCorrupted();
    return false;
}

} //namespace Repair

} //namespace WCDB
