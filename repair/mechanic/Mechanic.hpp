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

#ifndef Mechanic_hpp
#define Mechanic_hpp

#include <WCDB/Assembler.hpp>
#include <WCDB/Crawlable.hpp>
#include <WCDB/Material.hpp>
#include <WCDB/Repairman.hpp>

namespace WCDB {

namespace Repair {

class Material;

class Mechanic : public Repairman, public Crawlable {
#pragma mark - Initialize
public:
    Mechanic(const std::string &path);

#pragma mark - Mechanic
public:
    void work();

#pragma mark - Material
public:
    void setMaterial(const Material &material);
    void setMaterial(Material &&material);

protected:
    Material m_material;

#pragma mark - Crawlable
protected:
    bool onCellCrawled(const Cell &cell) override;
    bool onPageCrawled(const Page &page, int unused) override;
};

} //namespace Repair

} //namespace WCDB

#endif /* Mechanic_hpp */
