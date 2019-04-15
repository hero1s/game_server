#-*- coding: UTF-8 -*-

xls_path = 'xls/'
lua_path = 'lua/'
txt_path = 'txt/'

RES_FOR_CLT = 1
RES_FOR_SVR = 2
RES_FOR_CLT_AND_SVR = 3

# 转表配置
confs = (
    ######################################################################
    {
        "ResType" : RES_FOR_CLT_AND_SVR,
        "Filename" : {
            "Input"  : u"玩家等级.xlsx",
            "Output" : "player_levels",
        },
        "SheetNames" : [
            u'Sheet1',
        ],
        "DataType" : "PlayerLevels",
        "Fields" : {
            u"等级"    : "PlayerLevels.list.id",
            u"经验值"  : "PlayerLevels.list.exp",
        },
        "Keywords" : {
        }
    },
	######################################################################
    {
        "ResType" : RES_FOR_CLT_AND_SVR,
        "Filename" : {
            "Input"  : u"关卡表.xlsx",
            "Output" : "copy_infos",
        },
        "SheetNames" : [
            u'paral',
        ],
        "DataType" : "CopyInfos",
        "Fields" : {
            u"关卡id"       : "CopyInfos.list.id",
            u"章节"         : "CopyInfos.list.chapterID",
			u"章节名称"     : "CopyInfos.list.chapterName",
            u"关卡名称"     : "CopyInfos.list.missionName",
			u"关卡类型"     : "CopyInfos.list.missionType",
            u"描述"         : "CopyInfos.list.missionDesc",
			u"玩家等级"     : "CopyInfos.list.playerLevel",
            u"前置关卡"     : "CopyInfos.list.preMission",
			u"指定角色"     : "CopyInfos.list.roleIDLimit",
            u"解锁条件4"    : "CopyInfos.list.taskLimit",
			u"游戏币奖励"   : "CopyInfos.list.silverAward",
			u"经验奖励"     : "CopyInfos.list.expAward",
            u"道具id"       : "CopyInfos.list.awardList.itemID",
			u"数量"         : "CopyInfos.list.awardList.count",
            u"权重"         : "CopyInfos.list.awardList.weight",
        },
        "Keywords" : {
        }
    },
	######################################################################
    {
        "ResType" : RES_FOR_CLT_AND_SVR,
        "Filename" : {
            "Input"  : u"角色表.xlsx",
            "Output" : "roles",
        },
        "SheetNames" : [
            u'角色属性表',
        ],
        "DataType" : "Roles",
        "Fields" : {
            u"角色id"         : "Roles.list.id",
			u"角色名字"       : "Roles.list.name",
			u"图标id"         : "Roles.list.icon",
			u"评级"           : "Roles.list.starLv",
			u"职业"           : "Roles.list.job",
			u"等级"           : "Roles.list.level",
			u"攻击"           : "Roles.list.attack",
			u"防御"           : "Roles.list.defense",
			u"生命"           : "Roles.list.hp",
			u"游戏币加成"     : "Roles.list.silverPlus",
			u"解锁条件"       : "Roles.list.unlock",
			u"购买消耗材料"   : "Roles.list.buyMaterial",
			u"升阶消耗游戏币" : "Roles.list.upgradeMaterial",
			u"子弹特效"       : "Roles.list.bulletEffect",
			u"技能"			  : "Roles.list.skill",
			u"模型"           : "Roles.list.model",
			u"hp系数"         : "Roles.list.hpFactor",
			u"atk系数"        : "Roles.list.atFactor",
			u"def系数"        : "Roles.list.deFactor",
			u"hpup"           : "Roles.list.hpGrow",
			u"atkup"          : "Roles.list.atGrow",
			u"defup"          : "Roles.list.deGrow",
			u"升星道具1"      : "Roles.list.star1ItemID",
			u"数量1"          : "Roles.list.star1ItemNum",
			u"升星道具2"      : "Roles.list.star2ItemID",
			u"数量2"          : "Roles.list.star2ItemNum",
			u"升星道具3"      : "Roles.list.star3ItemID",
			u"数量3"          : "Roles.list.star3ItemNum",
            u"移动速度"       : "Roles.list.moveSpeed",
            u"移动范围"       : "Roles.list.moveRange",
            u"类型"           : "Roles.list.type",
        },
        "Keywords" : {
        }
    },
	######################################################################
    {
        "ResType" : RES_FOR_CLT_AND_SVR,
        "Filename" : {
            "Input"  : u"角色表.xlsx",
            "Output" : "role_levels",
        },
        "SheetNames" : [
            u'角色升级表',
        ],
        "DataType" : "RoleLevels",
        "Fields" : {
            u"等级"       : "RoleLevels.list.id",
			u"游戏币消耗" : "RoleLevels.list.silver",
			u"RMB消耗"    : "RoleLevels.list.gold",
		},
        "Keywords" : {
        }
    },
    ######################################################################
    {
        "ResType" : RES_FOR_CLT_AND_SVR,
        "Filename" : {
            "Input"  : u"出生表.xlsx",
            "Output" : "player_births",
        },
        "SheetNames" : [
            u'Sheet1',
        ],
        "DataType" : "PlayerBirths",
        "Fields" : {
            u"默认id"         : "PlayerBirths.list.id",
			u"默认角色"       : "PlayerBirths.list.roleID",
			u"默认关卡"       : "PlayerBirths.list.passID",
			u"默认道具1"      : "PlayerBirths.list.res.resID",
			u"默认道具1数量"  : "PlayerBirths.list.res.resNum",
		},
        "Keywords" : {
        }
    },
)
