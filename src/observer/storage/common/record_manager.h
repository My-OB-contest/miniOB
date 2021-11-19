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
// Created by Longda on 2021/4/13.
//
#ifndef __OBSERVER_STORAGE_COMMON_RECORD_MANAGER_H_
#define __OBSERVER_STORAGE_COMMON_RECORD_MANAGER_H_

#include <sstream>
#include "storage/default/disk_buffer_pool.h"

typedef int SlotNum;
struct PageHeader;
class ConditionFilter;

struct RID 
{
  PageNum page_num; // record's page number
  SlotNum slot_num; // record's slot number
  // bool    valid;    // true means a valid record

  bool operator== (const RID &other) const {
    return page_num == other.page_num && slot_num == other.slot_num;
  }
};

class RidDigest {
public:
  size_t operator() (const RID &rid) const {
    return ((size_t)(rid.page_num) << 32) | rid.slot_num;
  }
};

struct Record 
{
  // bool valid; // false means the record hasn't been load
  RID  rid;   // record's rid
  char *data; // record's data
};

class TextAddress
{
public:
    TextAddress(int sp, int ss, int ep, int es):
    start_pagenum(sp),
    start_slot_num(ss),
    end_pagenum(ep),
    end_slot_num(es)
    {}

    TextAddress():
            start_pagenum(-1),
            start_slot_num(-1),
            end_pagenum(-1),
            end_slot_num(-1)
    {}

   PageNum start_pagenum;
   SlotNum start_slot_num;
   PageNum end_pagenum;
   SlotNum end_slot_num;

   RC encode(char data[17]){
//     char data[17];
     char* start_page = transToString(start_pagenum);
     strncpy(data, start_page, 4);
     char* start_slot = transToString(start_slot_num);
     strncpy(data+4, start_slot, 4);
     char* end_page = transToString(end_pagenum);
     strncpy(data+8, end_page, 4);
     char* end_slot = transToString(end_slot_num);
     strncpy(data+12, end_slot, 4);
     data[16] = '\0';
//     decode(data);
     return RC::SUCCESS;
   }

   TextAddress(const char* data){
     char start_page[5];
     char start_slot[5];
     char end_page[5];
     char end_slot[5];
     strncpy(start_page, data, 4);
     strncpy(start_slot, data+4, 4);
     strncpy(end_page, data+8, 4);
     strncpy(end_slot, data+12, 4);
     start_page[4] = '\0';
     start_slot[4] = '\0';
     end_page[4] = '\0';
     end_slot[4] = '\0';
     start_pagenum = atoi(start_page);
     start_slot_num = atoi(start_slot);
     end_pagenum = atoi(end_page);
     end_slot_num = atoi(end_slot);
//     return TextRecord(atoi(start_page),atoi(start_slot),atoi(end_page),atoi(end_slot));
   }

private:
    char* transToString(int num){
      std::stringstream ss;
      ss << num;
      std::string str = ss.str();
      while (str.size() < 4) str = "0" + str;
      return (char*)(str.data());
   }
};

class RecordPageHandler {
public:
  RecordPageHandler();
  ~RecordPageHandler();
  RC init(DiskBufferPool &buffer_pool, int file_id, PageNum page_num);
  RC initfrom_pageone(DiskBufferPool &buffer_pool, int file_id, PageNum page_num);
  RC init_empty_page(DiskBufferPool &buffer_pool, int file_id, PageNum page_num, int record_size);
  RC deinit();

  RC insert_record(const char *data, RID *rid);
  RC update_record(const Record *rec);

  template <class RecordUpdater>
  RC update_record_in_place(const RID *rid, RecordUpdater updater) {
    Record record;
    RC rc = get_record(rid, &record);
    if (rc != RC::SUCCESS) {
      return rc;
    }
    rc = updater(record);
    disk_buffer_pool_->mark_dirty(&page_handle_);
    return rc;
  }

  RC delete_record(const RID *rid);

  RC get_record(const RID *rid, Record *rec);
  RC get_first_record(Record *rec);
  RC get_next_record(Record *rec);
  RC get_text_next_record(Record *rec);

  PageNum get_page_num() const;

  bool is_full() const;

private:
  DiskBufferPool * disk_buffer_pool_;
  int              file_id_;
  BPPageHandle     page_handle_;
  PageHeader    *  page_header_;
  char *           bitmap_;
};

class RecordFileHandler {
public:
  RecordFileHandler();
  RC init(DiskBufferPool &buffer_pool, int file_id);
  void close();

  /**
   * 更新指定文件中的记录，rec指向的记录结构中的rid字段为要更新的记录的标识符，
   * pData字段指向新的记录内容
   * @param rec
   * @return
   */
  RC update_record(const Record *rec);

  /**
   * 从指定文件中删除标识符为rid的记录
   * @param rid
   * @return
   */
  RC delete_record(const RID *rid);

  RC insert_text_record(const char *data, int record_size, RID *rid);

  /**
   * 插入一个新的记录到指定文件中，pData为指向新纪录内容的指针，返回该记录的标识符rid
   * @param data
   * @param rid
   * @return
   */
  RC insert_record(const char *data, int record_size, RID *rid);

  /**
   * 获取指定文件中标识符为rid的记录内容到rec指向的记录结构中
   * @param rid
   * @param rec
   * @return
   */
  RC get_record(const RID *rid, Record *rec);

  template<class RecordUpdater> // 改成普通模式, 不使用模板
  RC update_record_in_place(const RID *rid, RecordUpdater updater) {

    RC rc = RC::SUCCESS;
    RecordPageHandler page_handler;
    if ((rc != page_handler.init(*disk_buffer_pool_, file_id_, rid->page_num)) != RC::SUCCESS) {
      return rc;
    }

    return page_handler.update_record_in_place(rid, updater);
  }

private:
  DiskBufferPool  *   disk_buffer_pool_;
  int                 file_id_;                    // 参考DiskBufferPool中的fileId

  RecordPageHandler   record_page_handler_;        // 目前只有insert record使用
};

class RecordFileScanner 
{
public:
  RecordFileScanner();

  /**
   * 打开一个文件扫描。
   * 本函数利用从第二个参数开始的所有输入参数初始化一个由参数rmFileScan指向的文件扫描结构，
   * 在使用中，用户应先调用此函数初始化文件扫描结构，
   * 然后再调用GetNextRec函数来逐个返回文件中满足条件的记录。
   * 如果条件数量conNum为0，则意味着检索文件中的所有记录。
   * 如果条件不为空，则要对每条记录进行条件比较，只有满足所有条件的记录才被返回
   * @param buffer_pool 
   * @param file_id 
   * @param condition_num 
   * @param conditions
   * @return
   */
  RC open_scan(DiskBufferPool & buffer_pool, int file_id, ConditionFilter *condition_filter);

   /**
   * 打开一个隐藏表文件扫描。
   * 本函数利用从第二个参数开始的所有输入参数初始化一个由参数rmFileScan指向的文件扫描结构，
   * 在使用中，用户应先调用此函数初始化文件扫描结构，
   * 然后再调用GetNextRec函数来逐个返回文件中满足条件的记录。
   * @param buffer_pool
   * @param hide_file_id
   * @return
   */
  RC open_text_scan(DiskBufferPool & buffer_pool, int hide_file_id);
  /**
   * 关闭一个文件扫描，释放相应的资源
   * @return
   */
  RC close_scan();

  RC get_first_record(Record *rec);

  /**
   * 获取下一个符合扫描条件的记录。
   * 如果该方法成功，返回值rec应包含记录副本及记录标识符。
   * 如果没有发现满足扫描条件的记录，则返回RM_EOF
   * @param rec 上一条记录。如果为NULL，就返回第一条记录
   * @return
   */
  RC get_next_record(Record *rec);
  RC get_text_record(TextAddress* text_address, std::vector<Record>* text_record);

private:
  DiskBufferPool  *   disk_buffer_pool_;
  int                 file_id_;                    // 参考DiskBufferPool中的fileId

  ConditionFilter *   condition_filter_;
  RecordPageHandler   record_page_handler_;
};



#endif //__OBSERVER_STORAGE_COMMON_RECORD_MANAGER_H_