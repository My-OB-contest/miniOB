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

#ifndef __OBSERVER_SQL_EXECUTE_STAGE_H__
#define __OBSERVER_SQL_EXECUTE_STAGE_H__

#include "common/seda/stage.h"
#include "sql/parser/parse.h"
#include "rc.h"
#include "storage/trx/trx.h"
/* @author: huahui 
 * @what for: 必做题，聚合查询 
 * -----------------------------------------------------------------------------------------------------------------
 */
#include "sql/executor/tuple.h"
/* ------------------------------------------------------------------------------------------------------------
 */


class SessionEvent;

class ExecuteStage : public common::Stage {
public:
  ~ExecuteStage();
  static Stage *make_stage(const std::string &tag);

protected:
  // common function
  ExecuteStage(const char *tag);
  bool set_properties() override;

  bool initialize() override;
  void cleanup() override;
  void handle_event(common::StageEvent *event) override;
  void callback_event(common::StageEvent *event,
                     common::CallbackContext *context) override;

  void handle_request(common::StageEvent *event);
  RC do_select(const char *db, Query *sql, SessionEvent *session_event);
  RC select_check(const char *db, const Selects &selects);
  RC update_check(const char *db, const Updates &updates);  /* @author: huahui  @what for: 元数据校验-----------*/
  RC projection(std::vector<TupleSet> &tuplesets,const Selects &selects);
protected:
  /*
   * @author: huahui
   * @what for: 必做题，查询元数据校验
   * begin -------------------------------------------------------------------------------------------
   */
  // 检查以下错误
  // select c1 from t1, t2
  // select t100.c1 from t1,t2  t100不在t1,t2中
  // select t1.c100 from t1,t2  c100不在t1中
  RC check_attr_for_multitable(const char *db, const Selects &selects, const RelAttr &relattr);

  // 从长度为value_num的values中，判断有没有date属性
  // 若有，则判断date是否合法
  RC check_date_from_values(int value_num, const Value *values);
  RC check_insert_stat(const Inserts &inserts, SessionEvent *session_event);
  /*end ----------------------------------------------------------------------------------------------*/



  /* @author: huahui 
	 * @what for: 必做题，聚合查询 
   * tuple_sets是多个单表查询的结果
	 * -----------------------------------------------------------------------------------------------------------------
	 */
  // 对聚合属性做合法性校验，比如AVG(birthday)肯定是错的
  RC check_agg(const char *db, const Selects &selects, std::vector<const RelAttr *> &relattrs);
  
  RC have_agg_from_selections(const Selects &selection, bool &hagg, std::vector<const RelAttr *> &relattrs);
  RC agg_select_from_tupleset(Trx *trx, const char *db, const Selects &selects, TupleSet &tuple_set, std::vector<const RelAttr *> &relattrs, TupleSet &agg_res);
  /* ------------------------------------------------------------------------------------------------------------
	 */
  
  /* @author: huahui  @what for: null ------------------------------------------------------------------------------*/
  // 检验select和update的语句中的where语句是否符合要求
  // 检查属性, 表名是否合法
  RC check_condition(int condition_num, const Condition *conditions, const Table * table);
  /* --------------------------------------------------------------------------------------------------------------*/

  /* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  bool convert_condexps_to_conds(int condition_num, ConditionExp condition_exps[], Condition conds[]);
  bool convert_to_selects(const AdvSelects &adv_selects, Selects &selects);
  RC do_advselects(Trx *trx, const AdvSelects &adv_selects, const char *db, TupleSet &res_tupleset);
  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
private:
  Stage *default_storage_stage_ = nullptr;
  Stage *mem_storage_stage_ = nullptr;
};

#endif //__OBSERVER_SQL_EXECUTE_STAGE_H__
