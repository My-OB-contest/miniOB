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
#ifndef __OBSERVER_STORAGE_COMMON_PAGE_MANAGER_H_
#define __OBSERVER_STORAGE_COMMON_PAGE_MANAGER_H_

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

#include <string.h>
#include <sys/stat.h>
#include <time.h>

#include <vector>
#include <map>
#include <list>
#include <stack>

#include "rc.h"

typedef int PageNum;

//
#define BP_INVALID_PAGE_NUM (-1)
#define BP_PAGE_SIZE (1 << 12)
#define TEXT_RECORD_SIZE 15
#define BP_PAGE_DATA_SIZE (BP_PAGE_SIZE - sizeof(PageNum))
#define BP_FILE_SUB_HDR_SIZE (sizeof(BPFileSubHeader))
#define BP_BUFFER_SIZE 50
#define MAX_OPEN_FILE 1024

typedef struct {
  PageNum page_num;
  char data[BP_PAGE_DATA_SIZE];
} Page;
// sizeof(Page) should be equal to BP_PAGE_SIZE

typedef struct {
  PageNum page_count;
  int allocated_pages;
} BPFileSubHeader;

typedef struct {
  bool dirty;
  unsigned int pin_count;
  unsigned long acc_time;
  int file_desc;
  int frame_id;
  Page page;
} Frame;

typedef struct {
  bool open;
  Frame *frame;
} BPPageHandle;

class BPFileHandle{
public:
  BPFileHandle() {
    memset(this, 0, sizeof(*this));
  }

public:
  bool bopen;
  const char *file_name;
  int file_desc;
  Frame *hdr_frame;
  Page *hdr_page;
  char *bitmap;
  BPFileSubHeader *file_sub_header;
} ;

class BPManager {
public:
//    class Node{
//    public:
//        Frame *frame;
//        int frame_id;
//        Node* pre;
//        Node* next;
//        Node(Frame *f,int fid,Node *pr,Node *ne):frame(f),frame_id(fid),pre(pr),next(ne){}
//        ~Node(){
//          delete frame;
//        }
//    };

    BPManager(int size = BP_BUFFER_SIZE) {
      frames = new Frame[size];
      this->size = size;
      allocated_size = 0;
      for(int i = size - 1; i >= 0; i--){
        free_frame.push(i);
      }
    }

    ~BPManager() {
//      Node *cur = head;
//      while(head != nullptr){
//        cur = head;
//        head = head->next;
//        delete(cur);
//      }
      size = 0;
      allocated_size = 0;
//      head = nullptr;
//      tail = nullptr;
      delete[] frames;
    }

    Frame *alloc() {
      // TODO for test
//      Node *node = nullptr;
      //如果还有空闲页面，直接分配
      if(allocated_size < size){
        int frame_id = free_frame.top();
        frames[frame_id].frame_id = frame_id;
//        node = new Node(&frames[frame_id], frame_id, nullptr, nullptr);
        allocated_frames.push_front(&frames[frame_id]);
//        setHead(node);
        allocated_size++;
        free_frame.pop();
        return &frames[frame_id];
//        return node->frame;
      }
      //如果没有空闲页面，要寻找到最久未使用的页面进行替换
//      node = tail;
//    检查pincount
//      while(node->frame->pin_count != 0){
//        node = node->pre;
//      }
//      if(node->frame->pin_count == 0){
//        moveToHead(node);
//        return node->frame;
//      }
      if(!allocated_frames.empty()){
        Frame *frame = allocated_frames.back();
        allocated_frames.push_front(frame);
        allocated_frames.pop_back();
        return frame;
      }
      return nullptr;
    }

    Frame *get(int file_desc, PageNum page_num) {
      std::map<std::pair<int, PageNum>, Frame*>::iterator it = page_table.find(std::pair<int, PageNum>(file_desc, page_num));
      if(it != page_table.end()){
        //先将该bpframe删除，再插入头部。
        allocated_frames.remove(it->second);
        allocated_frames.push_front(it->second);
//        moveToHead(it->second);
        return it->second;
      }
//      if(head == nullptr ){
//        return nullptr;
//      }
//      Node* cur = head;
//      while(cur!= nullptr){
//        if(cur->frame->file_desc == file_desc && cur->frame->page.page_num == page_num){
//          moveToHead(cur);
//          return cur->frame;
//        }
//        cur = cur->next;
//      }
      return nullptr;
    }

    std::vector<Frame*> getFlushFrames(int file_desc) {
      std::vector<Frame*> vf;
//      Node* cur = head;
      for (auto it = allocated_frames.begin(); it != allocated_frames.end() ; ++it) {
        if(it.operator*()->file_desc == file_desc){
          vf.push_back(it.operator*());
        }
      }
//      while(cur!= nullptr){
//        if(cur->frame->file_desc == file_desc){
//          vf.push_back(cur->frame);
//          Node *node = cur;
//          cur = cur->next;
//          delNode(node);
//          allocated_size--;
//        }
//        cur = cur->next;
//      }
      return vf;
    }

    RC freeFrame(Frame* frame){
//      std::map<std::pair<int, PageNum>, Frame*>::iterator it = page_table.find(std::pair<int, PageNum>(frame->file_desc, frame->page.page_num));
      if(page_table.erase(std::pair<int, PageNum>(frame->file_desc, frame->page.page_num))==0){
        return RC::BUFFERPOOL_DELETE_ERROR;
      }
      allocated_frames.remove(frame);
      free_frame.push(frame->frame_id);
      allocated_size--;
      return RC::SUCCESS;
    }

private:
//    RC moveToHead(Node* node){
//      if(head == node){
//        return RC::SUCCESS;
//      }else if(node == tail){
//        tail = node->pre;
//        tail->next = nullptr;
//      }else{
//        node->pre->next = node->next;
//        node->next->pre = node->pre;
//      }
//      node->next = head;
//      node->pre = nullptr;
//      head->pre = node;
//      head = node;
//      return RC::SUCCESS;
//    }

//    RC setHead(Node* node){
//      if(head == nullptr && tail == nullptr){
//        head = node;
//        tail = node;
//      }else if(head == tail){
//        tail->pre = node;
//        node->next = head;
//        head = node;
//      }else{
//        node->next = head;
//        node->pre = nullptr;
//        head->pre = node;
//        head = node;
//      }
//      return RC::SUCCESS;
//    }
//
//    RC delNode(Node* node){
//      if(node == head && node == tail){
//        tail = nullptr;
//        head = nullptr;
//      }else if(node == head){
//        head = node->next;
//        head->pre = nullptr;
//      }else if(node == tail){
//        tail = node->pre;
//        tail->next = nullptr;
//      }else {
//        node->pre->next = node->next;
//        node->next->pre = node->pre;
//      }
//      page_table.erase(std::pair<int, PageNum>(frame->file_desc, frame->page.page_num));
//      allocated_size--;
//      delete node;
//      return RC::SUCCESS;
//    }

//  Frame *getFrame() { return head->frame; }

//  bool *getAllocated() { return true; }

public:
    int size;
    int allocated_size;

//    Node *head = nullptr;
//    Node *tail = nullptr;
    std::list<Frame*> allocated_frames;
    std::map<std::pair<int, PageNum>, Frame*> page_table;
    std::stack<int> free_frame;
    Frame * frames = nullptr;
//    bool *allocated = nullptr;
};

class DiskBufferPool {
public:
  /**
  * 创建一个名称为指定文件名的分页文件
  */
  RC create_file(const char *file_name);

  /**
   * 根据文件名打开一个分页文件，返回文件ID
   * @return
   */
  RC open_file(const char *file_name, int *file_id);

  /**
   * 关闭fileID对应的分页文件
   */
  RC close_file(int file_id);

  /**
   * 根据文件ID和页号获取指定页面到缓冲区，返回页面句柄指针。
   * @return
   */
  RC get_this_page(int file_id, PageNum page_num, BPPageHandle *page_handle);

  /**
   * 在指定文件中分配一个新的页面，并将其放入缓冲区，返回页面句柄指针。
   * 分配页面时，如果文件中有空闲页，就直接分配一个空闲页；
   * 如果文件中没有空闲页，则扩展文件规模来增加新的空闲页。
   */
  RC allocate_page(int file_id, BPPageHandle *page_handle);

  /**
   * 根据页面句柄指针返回对应的页面号
   */
  RC get_page_num(BPPageHandle *page_handle, PageNum *page_num);

  /**
   * 根据页面句柄指针返回对应的数据区指针
   */
  RC get_data(BPPageHandle *page_handle, char **data);

  /**
   * 丢弃文件中编号为pageNum的页面，将其变为空闲页
   */
  RC dispose_page(int file_id, PageNum page_num);

  /**
   * 释放指定文件关联的页的内存， 如果已经脏， 则刷到磁盘，除了pinned page
   * @param file_handle
   * @param page_num 如果不指定page_num 将刷新所有页
   */
  RC force_page(int file_id, PageNum page_num);

  /**
   * 标记指定页面为“脏”页。如果修改了页面的内容，则应调用此函数，
   * 以便该页面被淘汰出缓冲区时系统将新的页面数据写入磁盘文件
   */
  RC mark_dirty(BPPageHandle *page_handle);

  /**
   * 此函数用于解除pageHandle对应页面的驻留缓冲区限制。
   * 在调用GetThisPage或AllocatePage函数将一个页面读入缓冲区后，
   * 该页面被设置为驻留缓冲区状态，以防止其在处理过程中被置换出去，
   * 因此在该页面使用完之后应调用此函数解除该限制，使得该页面此后可以正常地被淘汰出缓冲区
   */
  RC unpin_page(BPPageHandle *page_handle);

  /**
   * 获取文件的总页数
   */
  RC get_page_count(int file_id, int *page_count);

  RC flush_all_pages(int file_id);

protected:
  RC allocate_block(Frame **buf);
  RC dispose_block(Frame *buf);

  /**
   * 刷新指定文件关联的所有脏页到磁盘，除了pinned page
   * @param file_handle
   * @param page_num 如果不指定page_num 将刷新所有页
   */
  RC force_page(BPFileHandle *file_handle, PageNum page_num);
  RC force_all_pages(BPFileHandle *file_handle);
  RC check_file_id(int file_id);
  RC check_page_num(PageNum page_num, BPFileHandle *file_handle);
  RC load_page(PageNum page_num, BPFileHandle *file_handle, Frame *frame);
  RC flush_block(Frame *frame);

private:
  BPManager bp_manager_;
  BPFileHandle *open_list_[MAX_OPEN_FILE] = {nullptr};
};

DiskBufferPool *theGlobalDiskBufferPool();

#endif //__OBSERVER_STORAGE_COMMON_PAGE_MANAGER_H_
