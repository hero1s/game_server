
#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <unordered_map>
#include <functional>
#include "quad_tree.hpp"
#include "utility/comm_macro.h"

namespace aoi {
    class CScene;

    class CSceneObj {
        friend CScene;

    protected:
        //uid-version, version�����Ż�����Ƚϼ��㣬ͨ��2�α������ɵó�������Ұ���
        using entity_view_t = std::unordered_map<math::objectid_t, uint8_t>;
        uint8_t version = 1;
        float view_w = 20.0f;//��Ұ����
        float view_h = 20.0f;
        entity_view_t view;
        bool need_check_view = true;//�Ƿ���Ҫ��Ұ���(��̬��Ʒ����Ҫ)
        math::objectid_t view_uid;

        virtual void view_event(std::vector<math::objectid_t> &enter_list, std::vector<math::objectid_t> &leave_list) {}
    };

    class CScene {
    public:
        CScene(float x_, float y_, float width_, float height_) {
            math::rect rc(x_, y_, width_, height_);
            qtree = std::make_shared<math::quad_tree<36>>(rc);
            is_need_check = true;
        }

        virtual ~CScene() {}

        void insert(CSceneObj* pObj, float x, float y) {
            qtree->insert(pObj->view_uid, x, y);
            scene_objs.try_emplace(pObj->view_uid,pObj);
            is_need_check = true;
            check_aoi();
        }

        void update(math::objectid_t uid, float x, float y) {
            qtree->update(uid, x, y);
            is_need_check = true;
        }

        void erase(math::objectid_t uid) {
            qtree->erase(uid);
            scene_objs.erase(uid);
            is_need_check = true;
            check_aoi();
        }

        void check_aoi() {
            if (is_need_check) {
                for (auto [uid,pObj] : scene_objs) {
                    if(!pObj->need_check_view)continue;
                    query_result.clear();
                    qtree->query(uid, pObj->view_w, pObj->view_h, query_result);
                    compare_view(uid, pObj, query_result);//��һ�α��������°汾��
                    process_view_event(uid, pObj);//��һ�α�����ӡ���
                }
                is_need_check = false;
            }
        }

    protected:
        void compare_view(math::objectid_t uid, CSceneObj* pObj, const std::vector<math::objectid_t> &query_result) {
            pObj->version += 2;
            for (auto &otherid : query_result) {
                if (otherid == uid) {
                    continue;
                }
                auto[iter, success] = pObj->view.try_emplace(otherid, pObj->version);//�½������
                if (!success)//����Ѿ�����Ұ��
                {
                    iter->second = pObj->version - 1;//�����Ѿ�����Ұ����ҵİ汾��,�ȵ�ǰ�汾��С1
                }
            }
        }

        void process_view_event(math::objectid_t uid, CSceneObj* pObj) {
            enter_list.clear();
            leave_list.clear();
            for (auto &v : pObj->view) {
                if (v.second == pObj->version) {
                    //������Ұ
                    LOG_DEBUG("player {} enter {} view", v.first, uid);
                    enter_list.emplace_back(v.first);
                } else if (v.second != (pObj->version - 1))//����Ұ
                {
                    LOG_DEBUG("player {} leave {} view", v.first, uid);
                    //ע������Ҫ�� p.view ɾ���Ѿ�����Ұ�����
                    leave_list.emplace_back(v.first);
                }
            }
            if(!enter_list.empty() || !leave_list.empty()) {
                pObj->view_event(enter_list, leave_list);
            }
            for (auto &it:leave_list) {
                pObj->view.erase(it);
            }
        }

    protected:
        std::shared_ptr<math::quad_tree<36>> qtree;
        std::unordered_map<math::objectid_t, CSceneObj*> scene_objs;
        std::vector<math::objectid_t> query_result;
        std::vector<math::objectid_t> enter_list;
        std::vector<math::objectid_t> leave_list;
        bool is_need_check;
    };
};

