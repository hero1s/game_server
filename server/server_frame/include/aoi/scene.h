
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <unordered_map>
#include "quad_tree.hpp"
#include "utility/comm_macro.h"

struct player {
    //uid-version, version用于优化差异比较计算，通过2次遍历即可得出进出视野结果
    using entity_view_t = std::unordered_map<math::objectid_t, uint8_t>;
    uint8_t version = 1;
    float view_w = 20.0f;//视野长宽
    float view_h = 20.0f;
    entity_view_t view;

    virtual void view_event(std::vector<math::objectid_t>& enter_list, std::vector<math::objectid_t>& leave_list) { }
};

class CScene {
public:
    CScene(float x_, float y_, float width_, float height_)
    {
        math::rect rc(x_, y_, width_, height_);
        qtree = std::make_shared<math::quad_tree<36>>(rc);
        is_need_check = true;
    }

    virtual ~CScene() { }

    void insert(math::objectid_t uid, float x, float y)
    {
        qtree->insert(uid, x, y);
        players.try_emplace(uid, player());
        is_need_check = true;
        check_aoi();
    }

    void update(math::objectid_t uid, float x, float y)
    {
        qtree->update(uid, x, y);
        is_need_check = true;
    }

    void erase(math::objectid_t uid)
    {
        qtree->erase(uid);
        players.erase(uid);
        is_need_check = true;
        check_aoi();
    }

    void check_aoi()
    {
        if (is_need_check) {
            for (auto& p : players) {
                query_result.clear();
                qtree->query(p.first, p.second.view_w, p.second.view_h, query_result);
                compare_view(p.first, p.second, query_result);//第一次遍历，更新版本号
                process_view_event(p.first, p.second);//第一次遍历打印结果
            }
            is_need_check = false;
        }
    }

protected:
    void compare_view(math::objectid_t uid, player& p, const std::vector<math::objectid_t>& query_result)
    {
        p.version += 2;
        for (auto& otherid : query_result) {
            if (otherid==uid) {
                continue;
            }
            auto [iter,success] = p.view.try_emplace(otherid, p.version);//新进入玩家
            if (!success)//玩家已经在视野内
            {
                iter->second = p.version-1;//更新已经在视野内玩家的版本号,比当前版本号小1
            }
        }
    }

    void process_view_event(math::objectid_t uid, player& p)
    {
        enter_list.clear();
        leave_list.clear();
        for (auto& v : p.view) {
            if (v.second==p.version) {
                //进入视野
                LOG_DEBUG("player {} enter {} view", v.first, uid);
                enter_list.emplace_back(v.first);
            }
            else if (v.second!=(p.version-1))//出视野
            {
                LOG_DEBUG("player {} leave {} view", v.first, uid);
                //注意这里要从 p.view 删除已经出视野的玩家
                leave_list.emplace_back(v.first);
            }
        }
        p.view_event(enter_list, leave_list);
        for (auto& it:leave_list) {
            p.view.erase(it);
        }
    }

protected:
    std::shared_ptr<math::quad_tree<36>> qtree;
    std::unordered_map<math::objectid_t, player> players;
    std::vector<math::objectid_t> query_result;
    std::vector<math::objectid_t> enter_list;
    std::vector<math::objectid_t> leave_list;
    bool is_need_check;

};

