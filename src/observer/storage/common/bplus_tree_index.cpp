/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by wangyunlai.wyl on 2021/5/19.
//

#include "storage/common/bplus_tree_index.h"
#include "common/log/log.h"

BplusTreeIndex::~BplusTreeIndex() noexcept {
  close();
}

RC BplusTreeIndex::create(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta>  &fields_meta) {
  if (inited_) {
    return RC::RECORD_OPENNED;
  }

  RC rc = Index::init(index_meta, fields_meta);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  rc = index_handler_.create(file_name, fields_meta);
  if (RC::SUCCESS == rc) {
    inited_ = true;
  }
  return rc;
}

RC BplusTreeIndex::open(const char *file_name, const IndexMeta &index_meta, const std::vector<FieldMeta>  &fields_meta) {
  if (inited_) {
    return RC::RECORD_OPENNED;
  }
  RC rc = Index::init(index_meta, fields_meta);
  if (rc != RC::SUCCESS) {
    return rc;
  }

  rc = index_handler_.open(file_name);
  if (RC::SUCCESS == rc) {
    inited_ = true;
  }
  return rc;
}

RC BplusTreeIndex::close() {
  if (inited_) {
    index_handler_.close();
    inited_ = false;
  }
  return RC::SUCCESS;
}
/*
 * fzh
 * 多列索引适配将record的属性装配起来
 */
RC BplusTreeIndex::insert_entry(const char *record, const RID *rid) {
  int sumattr_length=0;
  for(auto it : fields_meta_){
      sumattr_length+=it.len();
  }
  char *pkey = (char *)malloc(sumattr_length);
  sumattr_length=0;
  for(auto it : fields_meta_){
      memcpy(pkey+sumattr_length,record+it.offset(),it.len());
      sumattr_length=+it.len();
  }

  return index_handler_.insert_entry(pkey, rid);
}

RC BplusTreeIndex::insert_unique_entry(const char *record, const RID *rid ) {

    RID ridtmp={-1,-1};
    int sumattr_length=0;
    for(auto it : fields_meta_){
        sumattr_length+=it.len();
    }
    char *pkey = (char *)malloc(sumattr_length);
    for(auto it : fields_meta_){
        memcpy(pkey,record+it.offset(),it.len());
    }
    index_handler_.get_entry(pkey, &ridtmp);
    RID ridtmp2={-1,-1};
    if(ridtmp == ridtmp2){
        return index_handler_.insert_entry(pkey, rid);
    } else{
        free(pkey);
        return RC::RECORD_DUPLICATE_KEY;
    }

}

RC BplusTreeIndex::delete_entry(const char *record, const RID *rid) {
    int sumattr_length=0;
    for(auto it : fields_meta_){
        sumattr_length+=it.len();
    }
    char *pkey = (char *)malloc(sumattr_length);
    for(auto it : fields_meta_){
        memcpy(pkey,record+it.offset(),it.len());
    }
    return index_handler_.delete_entry(pkey, rid);
}
/*-------------------------------------------------------------------------------*/

/*
 * 原来create_scanner保留给单条件的使用并重写多条件的create_scanner
 */
IndexScanner *BplusTreeIndex::create_scanner(CompOp comp_op, const char *value) {
  /* @author: huahui  @what for: null字段 -------------------------------------------------*/
  // 如果是表达式中带有null，则返回nullptr
  if(value == nullptr) {
    return nullptr;
  }
  /* --------------------------------------------------------------------------------------*/
  BplusTreeScanner *bplus_tree_scanner = new BplusTreeScanner(index_handler_);
  RC rc = bplus_tree_scanner->open(comp_op, value);
  if (rc != RC::SUCCESS) {
    LOG_ERROR("Failed to open index scanner. rc=%d:%s", rc, strrc(rc));
    delete bplus_tree_scanner;
    return nullptr;
  }

  BplusTreeIndexScanner *index_scanner = new BplusTreeIndexScanner(bplus_tree_scanner);
  return index_scanner;
}

IndexScanner *BplusTreeIndex::create_scanner(std::vector<CompOp> compop_list , std::vector<const char *> value_list){
    BplusTreeScanner *bplus_tree_scanner = new BplusTreeScanner(index_handler_);
    RC rc = bplus_tree_scanner->open(compop_list, value_list);
    if (rc != RC::SUCCESS) {
        LOG_ERROR("Failed to open index scanner. rc=%d:%s", rc, strrc(rc));
        delete bplus_tree_scanner;
        return nullptr;
    }

    BplusTreeIndexScanner *index_scanner = new BplusTreeIndexScanner(bplus_tree_scanner);
    return index_scanner;
}



RC BplusTreeIndex::sync() {
  return index_handler_.sync();
}


BplusTreeIndexScanner::BplusTreeIndexScanner(BplusTreeScanner *tree_scanner) :
    tree_scanner_(tree_scanner) {
}

BplusTreeIndexScanner::~BplusTreeIndexScanner() noexcept {
  tree_scanner_->close();
  delete tree_scanner_;
}

RC BplusTreeIndexScanner::next_entry(RID *rid) {
  return tree_scanner_->next_entry(rid);
}

RC BplusTreeIndexScanner::destroy() {
  delete this;
  return RC::SUCCESS;
}