-- MySQL dump 10.13  Distrib 5.5.55, for Linux (x86_64)
--
-- Host: localhost    Database: 
-- ------------------------------------------------------
-- Server version	5.5.55

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `chess`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `chess` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `chess`;

--
-- Table structure for table `agent_player`
--

DROP TABLE IF EXISTS `agent_player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agent_player` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '合伙人id',
  `root_id` int(11) NOT NULL DEFAULT '0' COMMENT '总代的id',
  `agent_uid` int(11) NOT NULL DEFAULT '0' COMMENT '代理的uid(或玩家id)',
  `player_uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家的uid,玩家的uid为0表示，此玩家未激活',
  `union_id` varchar(128) NOT NULL DEFAULT '' COMMENT '玩家的微信union_id',
  `download_time` int(11) NOT NULL DEFAULT '0' COMMENT '加入的时间',
  `activate_time` int(11) NOT NULL DEFAULT '0' COMMENT '激活时间',
  PRIMARY KEY (`id`),
  UNIQUE KEY `union_id` (`union_id`) USING BTREE,
  UNIQUE KEY `agent_union` (`agent_uid`,`player_uid`) USING BTREE,
  KEY `download_time` (`download_time`),
  KEY `activate_time` (`activate_time`),
  KEY `group_id` (`group_id`,`player_uid`,`activate_time`) USING BTREE,
  KEY `root_id` (`root_id`,`player_uid`,`activate_time`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='合伙人系统模块里的代理与玩家的隶属关系';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `app_order`
--

DROP TABLE IF EXISTS `app_order`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `app_order` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '支付id',
  `pay_type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '支付类型,1-苹果，2-微信,3-支付宝,4-爱贝支付',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家的id',
  `transaction_id` varchar(32) NOT NULL DEFAULT '' COMMENT '微信支付订单号',
  `num` int(11) NOT NULL DEFAULT '0' COMMENT '购买的数量',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '金额，乘以100,展示要除以100',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '1-未支付,2-已关闭,3-支付错误,4-支付成功,未发货,5-已发货',
  `order_time` int(11) NOT NULL DEFAULT '0' COMMENT '下单时间',
  `pay_time` int(11) NOT NULL DEFAULT '0' COMMENT '支付时间',
  `delivery_time` int(11) NOT NULL DEFAULT '0' COMMENT '发货时间',
  `remark` varchar(128) NOT NULL DEFAULT '' COMMENT '备注',
  PRIMARY KEY (`id`),
  KEY `order_time` (`order_time`) USING BTREE,
  KEY `transaction_id` (`transaction_id`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `uid` (`uid`) USING BTREE,
  KEY `type` (`pay_type`)
) ENGINE=InnoDB AUTO_INCREMENT=94 DEFAULT CHARSET=utf8 COMMENT='商城商品订单';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `client_feedback`
--

DROP TABLE IF EXISTS `client_feedback`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_feedback` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '用户反馈id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '用户id',
  `type` tinyint(64) NOT NULL DEFAULT '0' COMMENT '反馈问题的类型',
  `content` varchar(2048) NOT NULL DEFAULT '' COMMENT '反馈内容',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '状态(0未处理,1已处理)',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `update_time` int(11) NOT NULL DEFAULT '0' COMMENT '处理此反馈的时间',
  `operator_id` int(11) NOT NULL DEFAULT '0' COMMENT '操作者,处理此反馈的人',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`),
  KEY `status` (`status`),
  KEY `create` (`create_time`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='游戏客户端的反馈';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `client_feedback_url`
--

DROP TABLE IF EXISTS `client_feedback_url`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_feedback_url` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '反馈id',
  `fid` int(11) NOT NULL DEFAULT '0' COMMENT '反馈id',
  `url` varchar(128) NOT NULL DEFAULT '' COMMENT '图片地址',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='客户端玩家的反馈的图片)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `club`
--

DROP TABLE IF EXISTS `club`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `club` (
  `id` int(20) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `password` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部ID(俱乐部密码)',
  `name` varchar(16) NOT NULL DEFAULT '' COMMENT '俱乐部名字',
  `host_id` int(11) NOT NULL DEFAULT '0' COMMENT '创建者ID',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `play_rule` varchar(4096) NOT NULL DEFAULT '' COMMENT '玩法规则,游戏前端已经组装好的字符串',
  `play_option` tinyint(4) NOT NULL DEFAULT '0' COMMENT '玩法选择,比如三公这款游戏有明庄之类的',
  `is_audit` tinyint(4) NOT NULL DEFAULT '1' COMMENT '是否需要审核加入，1-是，0-否',
  `is_open` tinyint(4) NOT NULL DEFAULT '1' COMMENT '0-不公布,1-公布战绩',
  `is_contest` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否有权限,默认没有权限,0-没有权限上下分，1-有权限上下分',
  `contest_time` int(11) NOT NULL DEFAULT '0' COMMENT '统计积分和表情的起始时间',
  `status` tinyint(4) NOT NULL DEFAULT '1' COMMENT '状态,1-正常，2-解散,3-冻结',
  `floor_num` tinyint(4) NOT NULL DEFAULT '5' COMMENT '每个俱乐部最多能创建的楼层数,暂时默认是5',
  `table_num` int(11) NOT NULL DEFAULT '20' COMMENT '可以开多少张桌子',
  `member` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部成员个数(只包含正常及禁言的),反范式设计',
  `total_card` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部消耗的总房卡数,实时统计',
  `total_table` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部总开房数,实时统计',
  `day_card` int(11) NOT NULL DEFAULT '0' COMMENT '当天消耗的房卡数',
  `day_table` int(11) NOT NULL DEFAULT '0' COMMENT '当天开的桌子数',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '修改俱乐部名字的时间，每天只能修改一次',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `create_time` (`create_time`) USING BTREE,
  KEY `host_id` (`host_id`) USING BTREE,
  KEY `game_id` (`game_id`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `password` (`password`,`status`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=1000083 DEFAULT CHARSET=utf8 COMMENT='俱乐部';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `club_floor`
--

DROP TABLE IF EXISTS `club_floor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `club_floor` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '楼层id',
  `club_id` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部id, 不是俱乐部密码',
  `floor_name` varchar(32) NOT NULL DEFAULT '' COMMENT '楼层名字',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `play_rule` varchar(4096) NOT NULL DEFAULT '' COMMENT '玩法规则,游戏前端已经组装好的字符串',
  `contest_rule` varchar(4096) NOT NULL DEFAULT '' COMMENT '比赛规则,json字符串',
  `floor_notice` varchar(4096) NOT NULL DEFAULT '' COMMENT '楼层公告',
  `play_option` tinyint(4) NOT NULL DEFAULT '0' COMMENT '玩法选择,比如三公这款游戏有明庄之类的',
  `table_num` int(11) NOT NULL DEFAULT '20' COMMENT '可以开多少张桌子',
  `is_delete` tinyint(4) NOT NULL DEFAULT '0' COMMENT '楼层是否已经删除了,1-是, 0-否',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '楼层创建时间',
  PRIMARY KEY (`id`),
  KEY `game_id` (`game_id`),
  KEY `create_time` (`create_time`),
  KEY `cid_delete` (`club_id`,`is_delete`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=228 DEFAULT CHARSET=utf8 COMMENT='俱乐部楼层一对关关系表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `club_member`
--

DROP TABLE IF EXISTS `club_member`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `club_member` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '用户ID',
  `cid` bigint(11) NOT NULL DEFAULT '0' COMMENT 'club表的id,不是俱乐部密码',
  `role` tinyint(4) NOT NULL DEFAULT '1' COMMENT 'i在成员角色,1-普通成员，2-管理员，3-局长',
  `status` tinyint(4) NOT NULL DEFAULT '1' COMMENT '状态, 1-待审核,2-拒绝,3-正常(审核通过),4-禁言,5-已删除',
  `play_num` int(11) NOT NULL DEFAULT '0' COMMENT '局数',
  `play_win` int(11) NOT NULL DEFAULT '0' COMMENT '大赢家次数',
  `win_num` int(11) NOT NULL DEFAULT '0' COMMENT '打积分场时赢的次数，只统计积分场的,不输不赢不统计',
  `lose_num` int(11) NOT NULL DEFAULT '0' COMMENT '打积分场时输的次数，只统计积分场的,不输不赢不统计',
  `score` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部积分',
  `froze_score` int(11) NOT NULL DEFAULT '0' COMMENT '冻结积分',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '表情数',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '加入俱乐部时间',
  PRIMARY KEY (`cid`,`uid`),
  KEY `createtime` (`create_time`) USING BTREE,
  KEY `role` (`role`),
  KEY `status` (`status`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `score` (`score`),
  KEY `fee` (`fee`),
  KEY `win_num` (`win_num`),
  KEY `lose_num` (`lose_num`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='俱乐部成员列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `club_score`
--

DROP TABLE IF EXISTS `club_score`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `club_score` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '积分上下分流水账id',
  `club_id` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `score` int(11) NOT NULL DEFAULT '0' COMMENT '正数为增加分数,负数时减少分数',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '表情数',
  `operator_uid` int(11) NOT NULL DEFAULT '0' COMMENT '操作者的uid',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间',
  `is_delete` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否删除，默认是否,0-否,1-是',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `operuid` (`operator_uid`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE,
  KEY `club_id_delete` (`club_id`,`is_delete`,`uid`) USING BTREE,
  KEY `club_id_id` (`id`,`club_id`)
) ENGINE=InnoDB AUTO_INCREMENT=62 DEFAULT CHARSET=utf8 COMMENT='每个楼层玩家上下分的流水账';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `emailfull`
--

DROP TABLE IF EXISTS `emailfull`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emailfull` (
  `efid` int(11) NOT NULL AUTO_INCREMENT COMMENT '邮件id',
  `title` varchar(50) NOT NULL DEFAULT '' COMMENT '标题',
  `content` varchar(4096) NOT NULL DEFAULT '' COMMENT '内容',
  `attachment` varchar(256) NOT NULL DEFAULT '' COMMENT '附件,type: 1-房卡,2-金币 [{"qty":10,"type":1},{"qty":20,"type":2}]',
  `nickname` varchar(50) NOT NULL DEFAULT '' COMMENT '来自',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '发布时间',
  `expiretime` int(11) NOT NULL DEFAULT '0' COMMENT '过期时间',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '状态(0正常,1下架)',
  PRIMARY KEY (`efid`),
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `expiretime` (`expiretime`) USING BTREE,
  KEY `status` (`status`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='全服邮件';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `emailuser`
--

DROP TABLE IF EXISTS `emailuser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `emailuser` (
  `eid` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '邮件id',
  `type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '消息类型(1系统消息2兑换通知,3支付通知,4好友邮件)',
  `suid` int(11) NOT NULL DEFAULT '0' COMMENT '发送者id,0-系统发送，非好友之间发送',
  `nickname` varchar(16) NOT NULL DEFAULT '' COMMENT '发送者昵称，如系统消息',
  `ruid` int(11) NOT NULL DEFAULT '0' COMMENT '接收者id',
  `title` varchar(64) NOT NULL DEFAULT '' COMMENT '标题',
  `content` varchar(4096) NOT NULL DEFAULT '' COMMENT '内容',
  `attachment` varchar(256) NOT NULL DEFAULT '' COMMENT '附件,type: 1-房卡,2-金币 [{"qty":10,"type":1},{"qty":20,"type":2}]',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '状态(0未读,1已读)',
  `is_get` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否已经领取附件里的礼物或赠品之类的(0未领取,1已领取)',
  `send_time` int(11) NOT NULL DEFAULT '0' COMMENT '发送时间',
  `operate_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '通过领取邮件获得的钻石类型, 1-手动增加,2-购买流出,3-绑定手机赠送流出,4-活动赠送流出,5-手动赠送流出,6-玩家注册赠送流出',
  PRIMARY KEY (`eid`),
  KEY `suid` (`suid`) USING BTREE,
  KEY `ruid` (`ruid`) USING BTREE,
  KEY `ptime` (`send_time`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4973 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='个人邮件';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `forbiduser`
--

DROP TABLE IF EXISTS `forbiduser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `forbiduser` (
  `fid` int(10) NOT NULL AUTO_INCREMENT COMMENT '封号id',
  `uid` int(10) NOT NULL COMMENT '用户id',
  `ptype` tinyint(1) NOT NULL DEFAULT '1' COMMENT '操作类型1封号,2解封,0封号解除',
  `ftime` int(11) unsigned NOT NULL COMMENT '封号截止时间0表示永久',
  `ptime` int(11) unsigned NOT NULL COMMENT '操作时间',
  `reason` varchar(100) NOT NULL COMMENT '封号原因',
  PRIMARY KEY (`fid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `id_factory`
--

DROP TABLE IF EXISTS `id_factory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `id_factory` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '俱乐部id,或是玩家id',
  `is_good` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否靓号,1-是，0-否',
  `club_use` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否已经使用了',
  `user_use` tinyint(4) NOT NULL DEFAULT '0' COMMENT '玩家用户id是否已经被使用,1-是，0-否',
  `agent_use` tinyint(4) NOT NULL DEFAULT '0' COMMENT 'h5用户id是否已经被使用,1-是，0-否',
  PRIMARY KEY (`id`),
  KEY `is_good` (`is_good`) USING BTREE,
  KEY `club_use` (`club_use`) USING BTREE,
  KEY `user_use` (`user_use`) USING BTREE,
  KEY `agent_use` (`agent_use`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=1000100 DEFAULT CHARSET=utf8 COMMENT='ID记录表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lottery_consume_record`
--

DROP TABLE IF EXISTS `lottery_consume_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lottery_consume_record` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '钻石购买抽奖次数id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `diamond` int(11) NOT NULL DEFAULT '0' COMMENT '钻石数',
  `lottery_count` int(11) NOT NULL DEFAULT '0' COMMENT '抽奖次数',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '购买时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家用钻石购买抽奖次数的记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lottery_exchange`
--

DROP TABLE IF EXISTS `lottery_exchange`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lottery_exchange` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '钻石兑换抽奖次数表自增id',
  `diamond` int(11) NOT NULL DEFAULT '0' COMMENT '钻石数量',
  `lottery_count` int(11) NOT NULL DEFAULT '0' COMMENT '抽奖次数',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='钻石兑换抽奖次数表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lottery_product`
--

DROP TABLE IF EXISTS `lottery_product`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lottery_product` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '奖品列表',
  `name` varchar(64) NOT NULL DEFAULT '' COMMENT '奖品名称',
  `piece` int(11) NOT NULL DEFAULT '0' COMMENT '碎片',
  `prob` int(11) NOT NULL DEFAULT '0' COMMENT '概率,碎片越多的用户抽到此奖的概率越小',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否已上线的产品, 1-已上线, 0-未上线',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '奖品类型, 1-钻石, 2-非钻石',
  `value` int(11) NOT NULL DEFAULT '0' COMMENT '奖品数量',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '更新时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `status` (`status`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lottery_record`
--

DROP TABLE IF EXISTS `lottery_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lottery_record` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '抽奖记录id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `lottery_id` int(11) NOT NULL DEFAULT '0' COMMENT '奖品id',
  `piece` int(11) NOT NULL DEFAULT '0' COMMENT '奖品的碎片数',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家抽奖记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `lottery_user`
--

DROP TABLE IF EXISTS `lottery_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lottery_user` (
  `id` bigint(20) NOT NULL,
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家uid',
  `lottery_id` int(11) NOT NULL DEFAULT '0' COMMENT '奖品id',
  `piece` int(11) NOT NULL DEFAULT '0' COMMENT '奖品的碎片数'
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家抽中的奖品列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `netdelay`
--

DROP TABLE IF EXISTS `netdelay`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `netdelay` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家ID',
  `totalnum` int(11) NOT NULL DEFAULT '0' COMMENT '统计次数',
  `maxdelay` int(11) NOT NULL DEFAULT '0' COMMENT '最大延迟',
  `meandelay` int(11) NOT NULL DEFAULT '0' COMMENT '平均延迟',
  `lastdelay` int(11) NOT NULL DEFAULT '0' COMMENT '最近延迟',
  `lastnetname` varchar(64) NOT NULL DEFAULT '' COMMENT '网络名称',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='记录玩家网络状况';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `onlinecount`
--

DROP TABLE IF EXISTS `onlinecount`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `onlinecount` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `ctime` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '记录时间戳',
  `online` int(10) NOT NULL DEFAULT '0' COMMENT '玩家在线',
  `robot` int(10) NOT NULL DEFAULT '0' COMMENT '机器人在线',
  `play` int(10) NOT NULL DEFAULT '0' COMMENT '玩家在玩',
  `robotplay` int(10) NOT NULL DEFAULT '0' COMMENT '机器人在玩',
  PRIMARY KEY (`id`),
  UNIQUE KEY `ctime_unique` (`ctime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=69691 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='玩家实时在线人数统计表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `onlinedetail`
--

DROP TABLE IF EXISTS `onlinedetail`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `onlinedetail` (
  `uid` int(10) NOT NULL COMMENT '用户id',
  `svrid` int(10) NOT NULL DEFAULT '0' COMMENT '服务器id',
  `roomid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id，非0表示玩家在玩游戏',
  `tableid` bigint(20) NOT NULL DEFAULT '0' COMMENT '所在桌子',
  `clubid` bigint(20) NOT NULL DEFAULT '0' COMMENT '俱乐部ID',
  `isrobot` int(11) NOT NULL DEFAULT '0' COMMENT '是否机器人',
  `coin` bigint(20) NOT NULL DEFAULT '0' COMMENT '财富币',
  `safecoin` bigint(11) NOT NULL DEFAULT '0' COMMENT '保险箱财富币',
  PRIMARY KEY (`uid`),
  KEY `svrid` (`svrid`) USING BTREE,
  KEY `clubid` (`clubid`) USING BTREE,
  KEY `roomid` (`roomid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='用户在线表，只要用户在线就会此表有个记录，退出就会删除此记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `passport`
--

DROP TABLE IF EXISTS `passport`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `passport` (
  `uid` int(10) NOT NULL COMMENT '玩家uid',
  `phone` char(12) NOT NULL DEFAULT '' COMMENT '手机号',
  `password` varchar(50) NOT NULL DEFAULT '' COMMENT '密码',
  `email` varchar(30) NOT NULL DEFAULT '' COMMENT '邮箱',
  `name` varchar(50) NOT NULL DEFAULT '' COMMENT '账号名称',
  `type` int(10) NOT NULL DEFAULT '0' COMMENT '注册类型（0游客1手机2微信）',
  `union_id` varchar(128) NOT NULL DEFAULT '' COMMENT '微信的union_id',
  `open_id` varchar(128) NOT NULL DEFAULT '' COMMENT '第三方账号id',
  `access_token` varchar(128) NOT NULL DEFAULT '' COMMENT '微信用户的token',
  `siteid` int(10) NOT NULL DEFAULT '1' COMMENT '代理商渠道id',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `openid` (`open_id`) USING BTREE,
  KEY `phone` (`phone`) USING BTREE,
  KEY `accesstoken` (`access_token`) USING BTREE,
  KEY `type` (`type`) USING BTREE,
  KEY `union_id` (`union_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='玩家通行证';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `phone_log`
--

DROP TABLE IF EXISTS `phone_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `phone_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '玩家绑定手机自增id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `phone` char(11) NOT NULL DEFAULT '' COMMENT '手机号',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '绑定时间',
  PRIMARY KEY (`id`),
  KEY `phone` (`phone`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COMMENT='玩家绑定过的手机记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `proom`
--

DROP TABLE IF EXISTS `proom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `proom` (
  `tableid` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '桌子ID',
  `hostid` int(11) NOT NULL DEFAULT '0' COMMENT '房主ID',
  `passwd` varchar(20) NOT NULL DEFAULT '' COMMENT '桌子密码',
  `idxround` tinyint(4) NOT NULL DEFAULT '0' COMMENT '局数索引',
  `dueround` smallint(11) NOT NULL DEFAULT '0' COMMENT '打多少局结束的局数',
  `duetime` int(11) NOT NULL DEFAULT '0' COMMENT '过期时间',
  `starttime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间打牌的时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间打牌时间',
  `isclose` tinyint(6) NOT NULL DEFAULT '0' COMMENT '0未结束1正常结束2过期结束3投票解散',
  `basescore` int(11) NOT NULL DEFAULT '0' COMMENT '底分',
  `entermin` int(11) NOT NULL DEFAULT '0' COMMENT '最小进入',
  `seatnum` int(11) NOT NULL DEFAULT '0' COMMENT '座位数',
  `consume` smallint(6) NOT NULL DEFAULT '0' COMMENT '消费类型,1-房卡，2-金币',
  `gametype` smallint(11) NOT NULL DEFAULT '0' COMMENT '游戏类型',
  `playtype` smallint(11) NOT NULL DEFAULT '0' COMMENT '玩法',
  `createtime` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `curround` smallint(11) NOT NULL DEFAULT '0' COMMENT '当前局数',
  `gameinfo` varchar(8192) NOT NULL DEFAULT '' COMMENT '游戏信息',
  `createtype` tinyint(11) NOT NULL DEFAULT '0' COMMENT '0普通模式1积分模式',
  `matchparam` varchar(4096) NOT NULL DEFAULT '' COMMENT '比赛场参数',
  `addparam` varchar(8192) NOT NULL DEFAULT '' COMMENT '附加参数字符串',
  `rent` tinyint(11) NOT NULL DEFAULT '0' COMMENT '房费方式',
  `fee` smallint(11) NOT NULL DEFAULT '0' COMMENT '房费,消耗的房卡数',
  `clubid` bigint(20) NOT NULL DEFAULT '0' COMMENT '所属俱乐部ID',
  `clubfloor` bigint(20) NOT NULL DEFAULT '0' COMMENT '俱乐部楼层id',
  `clubpos` smallint(11) NOT NULL DEFAULT '0' COMMENT '俱乐部桌子位置',
  `csvrid` smallint(6) DEFAULT '0' COMMENT '创建的服务器ID',
  PRIMARY KEY (`tableid`),
  KEY `starttime` (`starttime`) USING BTREE,
  KEY `endtime` (`endtime`) USING BTREE,
  KEY `duetime` (`duetime`) USING BTREE,
  KEY `createtime` (`createtime`) USING BTREE,
  KEY `hostid` (`hostid`) USING BTREE,
  KEY `gametype` (`gametype`) USING BTREE,
  KEY `clubid` (`clubid`) USING BTREE,
  KEY `isclose` (`isclose`),
  KEY `csvrid` (`csvrid`) USING BTREE,
  KEY `playtype` (`playtype`) USING BTREE,
  KEY `createtype` (`createtype`) USING BTREE,
  KEY `clubfloor` (`clubfloor`) USING BTREE,
  KEY `passwd` (`passwd`)
) ENGINE=InnoDB AUTO_INCREMENT=10985 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `proomlog`
--

DROP TABLE IF EXISTS `proomlog`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `proomlog` (
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `tableid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `jointime` int(11) NOT NULL DEFAULT '0' COMMENT '加入时间',
  `play` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否参与玩',
  `score` int(11) NOT NULL DEFAULT '0' COMMENT '输赢分数',
  `bigwin` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否大赢家',
  `is_win` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否赢， 1-用户赢了，0-用户输了',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '抽水',
  PRIMARY KEY (`uid`,`tableid`),
  KEY `jointime` (`jointime`) USING BTREE,
  KEY `tableid` (`tableid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `umission`
--

DROP TABLE IF EXISTS `umission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `umission` (
  `uid` int(10) NOT NULL COMMENT '用户id',
  `msid` int(10) NOT NULL COMMENT '任务id',
  `rtimes` int(10) NOT NULL COMMENT '达成次数',
  `ctimes` int(10) NOT NULL COMMENT '可完成次数',
  `ptime` int(11) NOT NULL COMMENT '操作时间',
  `cptime` int(11) NOT NULL COMMENT '完成时间',
  PRIMARY KEY (`uid`,`msid`),
  KEY `msid` (`msid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `uid` int(10) NOT NULL COMMENT '用户id',
  `nickname` varchar(100) CHARACTER SET utf8mb4 NOT NULL DEFAULT '' COMMENT '用户昵称',
  `rmcard` bigint(20) NOT NULL DEFAULT '0' COMMENT '房卡',
  `rmcard_forze` bigint(20) NOT NULL DEFAULT '0' COMMENT '冻结房卡',
  `coin` bigint(20) NOT NULL DEFAULT '0' COMMENT '金币',
  `safecoin` bigint(20) NOT NULL DEFAULT '0' COMMENT '保险箱金币',
  `vip` int(11) NOT NULL DEFAULT '0' COMMENT 'VIP',
  `clogin` int(10) NOT NULL DEFAULT '0' COMMENT '连续登陆天数',
  `weeklogin` int(10) NOT NULL DEFAULT '0' COMMENT '本周累计登陆',
  `alllogin` int(10) NOT NULL DEFAULT '0' COMMENT '累计登录天数',
  `sex` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '性别，0-未知，2-女，1-男',
  `reward` varchar(20) NOT NULL DEFAULT '00000000' COMMENT '奖励标记',
  `bankrupt` int(10) NOT NULL DEFAULT '0' COMMENT '破产补助次数',
  `imageurl` varchar(512) NOT NULL DEFAULT '' COMMENT '头像地址(默认为空，也就是默认头像)',
  `safepwd` varchar(12) NOT NULL DEFAULT '' COMMENT '保险箱密码',
  `phone` char(12) NOT NULL DEFAULT '' COMMENT '玩家的手机号',
  `phone_time` int(11) NOT NULL DEFAULT '0' COMMENT '首次绑定手机号的时间',
  `logintime` int(11) NOT NULL DEFAULT '0' COMMENT '最后登陆时间',
  `loginip` varchar(20) NOT NULL DEFAULT '' COMMENT '最后登陆ip',
  `lon` decimal(13,6) NOT NULL DEFAULT '0.000000',
  `lat` decimal(13,6) NOT NULL DEFAULT '0.000000',
  `city_code` mediumint(6) NOT NULL DEFAULT '430100' COMMENT '国家颁发的城市编码,默认为长沙市的城市编码:430100',
  `city_name` varchar(32) NOT NULL DEFAULT '' COMMENT '用户所在城市名字',
  `model` varchar(64) NOT NULL DEFAULT '' COMMENT '设备类型(设备型号)',
  `lversionid` varchar(64) NOT NULL DEFAULT '' COMMENT '登陆版本号',
  `offlinetime` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '最后离开游戏时间',
  `rtime` int(11) NOT NULL DEFAULT '0' COMMENT '注册时间',
  `frozen` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否冻结用户1-是，0-否',
  `frozen_reason` varchar(64) NOT NULL DEFAULT '' COMMENT '冻结原因',
  `efid` int(11) NOT NULL DEFAULT '0' COMMENT '全服邮件id,此玩家已经读取到的最大的全服邮件id',
  `identity` char(18) NOT NULL DEFAULT '' COMMENT '18位的身份证号码',
  `real_name` varchar(16) NOT NULL DEFAULT '' COMMENT '玩家真实名字',
  PRIMARY KEY (`uid`),
  KEY `phone` (`phone`) USING BTREE,
  KEY `logintime` (`logintime`) USING BTREE,
  KEY `rtime` (`rtime`) USING BTREE,
  KEY `offlinetime` (`offlinetime`) USING BTREE,
  KEY `rmcard` (`rmcard`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='玩家信息表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_attr`
--

DROP TABLE IF EXISTS `user_attr`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_attr` (
  `uid` int(11) NOT NULL,
  `consume_card` int(11) NOT NULL DEFAULT '0' COMMENT '玩家累计消耗的房卡',
  `lottery_count` int(11) NOT NULL DEFAULT '0' COMMENT '剩余抽奖次数',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家属性,钻石数值之类的';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_game`
--

DROP TABLE IF EXISTS `user_game`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_game` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家的uid',
  `game_type` smallint(6) NOT NULL DEFAULT '0' COMMENT '游戏类型',
  `play_type` smallint(6) NOT NULL DEFAULT '0' COMMENT '游戏玩法',
  `play_num` int(11) NOT NULL DEFAULT '0' COMMENT '玩的局数',
  PRIMARY KEY (`uid`,`game_type`,`play_type`),
  KEY `game_play` (`game_type`,`play_type`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='玩家参与每款游戏的局数的';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `white_list`
--

DROP TABLE IF EXISTS `white_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `white_list` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '白名单用户',
  `is_contest` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否有权限,默认没有权限,0-没有权限上下分，1-有权限上下分',
  PRIMARY KEY (`uid`),
  KEY `create_time` (`create_time`),
  KEY `is_contest` (`is_contest`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='上下分白名单';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Current Database: `chess_center`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `chess_center` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `chess_center`;

--
-- Table structure for table `group_game`
--

DROP TABLE IF EXISTS `group_game`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_game` (
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '大区的id',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '游戏的id',
  `operator_uid` int(11) NOT NULL DEFAULT '0' COMMENT '操作人的id',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`group_id`,`game_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='大区分配的游戏';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_agent_bank`
--

DROP TABLE IF EXISTS `h5_agent_bank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_agent_bank` (
  `uid` int(11) NOT NULL COMMENT '代理的uid',
  `name` varchar(16) NOT NULL DEFAULT '' COMMENT '真实姓名',
  `bank_name` varchar(64) NOT NULL DEFAULT '' COMMENT '开户行，如工商银行',
  `province` varchar(32) NOT NULL DEFAULT '' COMMENT '开户行所在省',
  `city` varchar(32) NOT NULL DEFAULT '' COMMENT '开户行所有市',
  `card_num` varchar(32) NOT NULL DEFAULT '' COMMENT '银行卡号',
  `branch` varchar(64) NOT NULL DEFAULT '' COMMENT '银行支行',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '绑定时间',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='代理银行卡信息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_agent_player`
--

DROP TABLE IF EXISTS `h5_agent_player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_agent_player` (
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `player_id` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `remark` varchar(32) NOT NULL DEFAULT '' COMMENT '玩家备注，方便代理识别',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间',
  PRIMARY KEY (`agent_id`,`player_id`),
  KEY `player_id` (`agent_id`,`create_time`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='代理与玩家关系表,只要玩家在此代理充值过，就记录此表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_auth_code`
--

DROP TABLE IF EXISTS `h5_auth_code`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_auth_code` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `auth_code` char(8) NOT NULL DEFAULT '' COMMENT '一级代理的授权码',
  `level` tinyint(4) NOT NULL DEFAULT '2' COMMENT '1-一级代理授权码, 2-二级代理授权码',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '授权码生成时间，如果是今天，则不能再生成，要次日才能生成',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `auth_code` (`auth_code`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='代理授权码';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_card_cash`
--

DROP TABLE IF EXISTS `h5_card_cash`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_card_cash` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '房卡数量,实时的数据',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '金额，存储时乘以100，展示时除以100, 单位元，',
  `total_card_num` int(11) NOT NULL DEFAULT '0' COMMENT '总购房卡数,累加的，历史所有的卡累加的',
  `open_fee` int(11) NOT NULL DEFAULT '0' COMMENT '开通费用,存储时乘以100，展示时除以100,，单位元，',
  `withdraw_time` int(11) NOT NULL DEFAULT '0' COMMENT '最近一次提现时间',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='代理的房卡数量，金额,一些数值类的信息';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_card_record`
--

DROP TABLE IF EXISTS `h5_card_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_card_record` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '代理房卡记录id',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `player_id` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '房卡买卖数量',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '买卖总价，乘以100',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '记录类型,1-买房卡,2-卖房卡(代充),3-卖房卡(直充),4-送房卡,5-获赠房卡,6-系统赠送',
  `pay_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '支付方式，1-余额支付，2-微信支付，3-支付宝支付',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间',
  PRIMARY KEY (`id`),
  KEY `act` (`agent_id`,`create_time`,`type`) USING BTREE,
  KEY `type` (`type`),
  KEY `player_id` (`player_id`,`agent_id`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=300 DEFAULT CHARSET=utf8 COMMENT='这张记录表是以代理为主角\r\n1-买房卡，对于买者(代理)来说是收入，所以为正数\r\n2-卖房卡(代充),对于卖者(代理)来说是付出,所以为负数\r\n3-卖房卡(直充),对于卖者(代理)来说是付出,所以以为负数,直充是直接从系统里卖出房卡，实际并没有减少代理的房卡\r\n4-送房卡，对于赠送者(代理)来说是付出，所以为负数\r\n5-获赠房卡，对于获赠者(代理)来说是收入，所以为正数\r\n6-系统赠送房卡，对于获赠者(代理或玩家)来说是收入，所以为正数';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_cash_record`
--

DROP TABLE IF EXISTS `h5_cash_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_cash_record` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '代理金额流水账id',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `user_type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '用户类型，用以表示child_id是哪种用户类型，1-代理,2-玩家',
  `child_id` int(11) NOT NULL DEFAULT '0' COMMENT '玩家直充id或二级代理id,用于记录一级代理的提成从哪个二级代理来的',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '流水账类型，1-提现退回,2-提现,3-余额买卡,4-本代理玩家直充,5-下级开店提成,6-下级进货提成,7-下级玩家直充提成',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '代理的房卡剩余数量',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '金额，乘以100',
  `month` smallint(11) NOT NULL DEFAULT '0' COMMENT '按月分帐单1801表示，2018年1月份',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '流水账时间',
  PRIMARY KEY (`id`),
  KEY `agent_type` (`agent_id`,`type`,`create_time`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE,
  KEY `agent_month` (`agent_id`,`month`,`create_time`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=143 DEFAULT CHARSET=utf8 COMMENT='代理金额流水账';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_feedback`
--

DROP TABLE IF EXISTS `h5_feedback`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_feedback` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '用户反馈id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '用户id',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '反馈问题的类型',
  `content` varchar(2048) NOT NULL DEFAULT '' COMMENT '反馈内容',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '状态(0未处理,1已处理)',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `update_time` int(11) NOT NULL DEFAULT '0' COMMENT '处理此反馈的时间',
  `operator_id` int(11) NOT NULL DEFAULT '0' COMMENT '操作者,处理此反馈的人',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`),
  KEY `status` (`status`),
  KEY `create` (`create_time`)
) ENGINE=InnoDB AUTO_INCREMENT=33 DEFAULT CHARSET=utf8 COMMENT='代理反馈';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_feedback_url`
--

DROP TABLE IF EXISTS `h5_feedback_url`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_feedback_url` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '反馈的图片url',
  `fid` int(11) NOT NULL DEFAULT '0' COMMENT '反馈的id',
  `url` varchar(128) NOT NULL DEFAULT '' COMMENT '图片地址',
  PRIMARY KEY (`id`),
  KEY `fid_url` (`fid`,`url`)
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='代理反馈的图片';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_order`
--

DROP TABLE IF EXISTS `h5_order`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_order` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '支付id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家或代理的id',
  `transaction_id` varchar(32) NOT NULL DEFAULT '' COMMENT '微信支付订单号',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id,玩家直充时，是在哪个代理里买的',
  `order_type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '订单类型, 1-购买钻石,2-购买代理名额',
  `user_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '用户类型，1-代理，2-玩家',
  `pay_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT ' 支付类型,1-余额支付,2-微信支付,3-支付宝支付',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '房卡数量',
  `give_num` int(11) NOT NULL DEFAULT '0' COMMENT '搞活动，系统赠送的房卡',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '金额，乘以100,展示要除以100',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '1-未支付,2-已关闭,3-支付错误,4-支付成功,未发货,5-已发货',
  `is_first` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否第一次充值,1-是,0-否',
  `remark` varchar(128) NOT NULL DEFAULT '' COMMENT '备注',
  `order_time` int(11) NOT NULL DEFAULT '0' COMMENT '下单时间',
  `pay_time` int(11) NOT NULL DEFAULT '0' COMMENT '支付时间',
  `delivery_time` int(11) NOT NULL DEFAULT '0' COMMENT '发货时间',
  PRIMARY KEY (`id`),
  KEY `user_uid` (`user_type`,`uid`) USING BTREE,
  KEY `order_time` (`order_time`) USING BTREE,
  KEY `transaction_id` (`transaction_id`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `uid` (`uid`) USING BTREE,
  KEY `order_type` (`order_type`)
) ENGINE=InnoDB AUTO_INCREMENT=100 DEFAULT CHARSET=utf8 COMMENT='代理或玩家买房卡订单';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_user`
--

DROP TABLE IF EXISTS `h5_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_user` (
  `uid` int(11) NOT NULL COMMENT 'h5用户id',
  `login_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '1-微信登录,2-手机登录(默认的)',
  `pid` int(11) NOT NULL DEFAULT '0' COMMENT '二级代理所属的一级代理',
  `level` tinyint(4) NOT NULL DEFAULT '0' COMMENT '代理等级，1-一级代理,2-二级代理',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '所属大区',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理的游戏id',
  `nickname` varchar(32) NOT NULL DEFAULT '' COMMENT '昵称',
  `remark` varchar(32) NOT NULL DEFAULT '' COMMENT '上级代理设置的备注',
  `phone` varchar(32) NOT NULL DEFAULT '' COMMENT '手机号',
  `password` char(32) NOT NULL DEFAULT '' COMMENT '密码',
  `open_id` varchar(128) NOT NULL DEFAULT '' COMMENT '微信登录的open_id',
  `union_id` varchar(128) NOT NULL DEFAULT '' COMMENT '微信的union_id',
  `sex` tinyint(4) NOT NULL DEFAULT '0' COMMENT '用户的性别，值为1时是男性，值为2时是女性，值为0时是未知',
  `province` varchar(32) NOT NULL DEFAULT '' COMMENT '省份',
  `city` varchar(64) NOT NULL DEFAULT '' COMMENT '城市',
  `head_img_url` varchar(256) NOT NULL DEFAULT '' COMMENT '用户头像',
  `shop_url` varchar(128) NOT NULL DEFAULT '' COMMENT '代理店铺二维码url',
  `name_url` varchar(128) NOT NULL DEFAULT '' COMMENT '个人名片url',
  `login_url` varchar(128) NOT NULL DEFAULT '' COMMENT '下载登录界面的url',
  `agent_num` smallint(6) NOT NULL DEFAULT '50' COMMENT '一级代理能发展二级代理的名额',
  `is_pass` tinyint(4) NOT NULL DEFAULT '1' COMMENT '是否通过考核,1-是,0-否,默认不管一级还是二级都通过考核',
  `pass_time` int(11) NOT NULL DEFAULT '0' COMMENT '考核时间，用于记录当月是否考核了',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '0-注册成功，1-输入授权成功，2-绑定手机成功,3-人工审核不通过,4-人工审核通过,5-第一次充值成功',
  `is_withdraw` tinyint(4) NOT NULL DEFAULT '1' COMMENT '代理是否可以提现，1-可以提现,0-不能提现',
  `frozen` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否冻结,1-是，0-否,2-注销',
  `frozen_reason` varchar(32) NOT NULL DEFAULT '' COMMENT '冻结理由',
  `agent_time` int(11) NOT NULL DEFAULT '0' COMMENT '成为正式代理的时间,即审核通过且第一次充值成功的时间',
  `login_time` int(11) NOT NULL DEFAULT '0' COMMENT '登录时间，首次注册成功登录时不算',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间，注册成为代理的时间',
  `invite_name` varchar(64) NOT NULL DEFAULT '' COMMENT '备注此代理是哪一个人推荐的',
  `identity` char(18) NOT NULL DEFAULT '' COMMENT '18位的身份证号码',
  `real_name` varchar(16) NOT NULL DEFAULT '' COMMENT '代理真实名字',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `open_id` (`open_id`) USING BTREE,
  KEY `pid_status_agent` (`pid`,`status`,`agent_time`) USING BTREE,
  KEY `is_pass` (`is_pass`) USING BTREE,
  KEY `is_frozen` (`frozen`) USING BTREE,
  KEY `group_id` (`group_id`) USING BTREE,
  KEY `phone_password` (`phone`,`password`) USING BTREE,
  KEY `status_level` (`status`,`level`) USING BTREE,
  KEY `create_time` (`create_time`),
  KEY `union_id` (`union_id`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='代理用户表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_withdraw_record`
--

DROP TABLE IF EXISTS `h5_withdraw_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_withdraw_record` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '代理提现记录id',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '提现金额，存储时要乘以100，展示时要除以100',
  `server_charge` int(11) NOT NULL DEFAULT '0' COMMENT '手续费，金额,展示时要除以100',
  `tax` int(11) NOT NULL DEFAULT '0' COMMENT '税费,金额,展示时要除以100',
  `real_income` int(11) NOT NULL DEFAULT '0' COMMENT '代理实现收入',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '1-审批中,2-审批成功,3-审批失败,4-支付中,5-支付失败,6-支付成功',
  `fail_reason` varchar(64) NOT NULL DEFAULT '' COMMENT '提现失败时填写',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '提现时间',
  `pay_time` int(11) NOT NULL DEFAULT '0' COMMENT '打款到代理银行帐号成功时间',
  PRIMARY KEY (`id`),
  KEY `agent_id` (`agent_id`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=utf8 COMMENT='代理提现记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `operation_log`
--

DROP TABLE IF EXISTS `operation_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `operation_log` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '操作日志自增id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '用户id',
  `ip` char(15) NOT NULL DEFAULT '' COMMENT '操作都的ip',
  `module` tinyint(4) NOT NULL DEFAULT '0' COMMENT '模块',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '操作类型,1-登录,2-退出,3-删除,4-新增,5-更新',
  `msg` varchar(2048) NOT NULL DEFAULT '' COMMENT '日志信息',
  `detail` varchar(2048) NOT NULL DEFAULT '' COMMENT '日志详情,拼接的字符串',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '日志生成的时间',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`),
  KEY `module` (`module`),
  KEY `type` (`type`),
  KEY `create_time` (`create_time`)
) ENGINE=InnoDB AUTO_INCREMENT=2292 DEFAULT CHARSET=utf8 COMMENT='操作日志,要定时清楚以前的日志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pc_group`
--

DROP TABLE IF EXISTS `pc_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pc_group` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '大区名称自增id',
  `name` varchar(32) NOT NULL DEFAULT '' COMMENT '大区的名字',
  `mutable` tinyint(4) NOT NULL DEFAULT '1' COMMENT '是否可修改删除,1-可以,0-不可以',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `bonus_rate` int(11) NOT NULL DEFAULT '0' COMMENT '大区提成比例,比例是10000, 展示时要除以10000,存时要乘以10000',
  PRIMARY KEY (`id`,`name`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 COMMENT='大区名字不能重复';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pc_menu`
--

DROP TABLE IF EXISTS `pc_menu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pc_menu` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '菜单的id',
  `parent_id` int(11) NOT NULL COMMENT '父亲id',
  `name` varchar(32) NOT NULL DEFAULT '' COMMENT '菜单名字',
  `module_name` varchar(32) NOT NULL DEFAULT '' COMMENT '模块名字',
  `url` varchar(64) NOT NULL DEFAULT '' COMMENT '菜单跳转链接',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=631 DEFAULT CHARSET=utf8 COMMENT='菜单列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pc_role`
--

DROP TABLE IF EXISTS `pc_role`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pc_role` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT ' 角色id',
  `creator_id` int(11) NOT NULL DEFAULT '0' COMMENT '创建此角色的用户id',
  `name` varchar(32) NOT NULL DEFAULT '' COMMENT '角色名称',
  `mutable` tinyint(4) NOT NULL DEFAULT '1' COMMENT '是否可修改删除,1-可以,0-不可以',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '角色创建时间',
  `menu` varchar(4096) NOT NULL DEFAULT '' COMMENT '权限菜单数组,对应pc_menu表,存储形式[1,3,5,7,9]',
  PRIMARY KEY (`id`),
  KEY `mutable` (`mutable`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8 COMMENT='角色表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pc_user`
--

DROP TABLE IF EXISTS `pc_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pc_user` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `role_id` int(11) NOT NULL DEFAULT '0' COMMENT '角色id',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '此帐号管理的大区的id,0-超级管理员,-1-此表示未分配属于哪个大区,非零-普通成员',
  `email` varchar(64) NOT NULL DEFAULT '' COMMENT '邮箱',
  `name` varchar(64) NOT NULL DEFAULT '' COMMENT 'pc登录用户名',
  `password` char(32) NOT NULL DEFAULT '' COMMENT '密码',
  `real_name` varchar(12) NOT NULL DEFAULT '' COMMENT '真实名字',
  `frozen` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否冻结，1-是，0-否',
  `mutable` tinyint(4) NOT NULL DEFAULT '1' COMMENT '是否可修改删除,1-可以,0-不可以',
  `login_time` int(11) NOT NULL DEFAULT '0' COMMENT '最后登录时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`uid`),
  KEY `group_id` (`group_id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8 COMMENT='pc登录用户表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `system_user`
--

DROP TABLE IF EXISTS `system_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_user` (
  `uid` int(11) NOT NULL COMMENT '系统帐号id',
  `user_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '系统账号类型,1-客户端购买, 2-客户端系统赠送, 3-H5购买,4-H5赠送,5-合伙人购买,6-合伙人赠送',
  `total_diamond` bigint(20) NOT NULL DEFAULT '0' COMMENT '投入的总钻石总数',
  `remain_diamond` bigint(20) NOT NULL DEFAULT '0' COMMENT '剩余的总钻石数',
  `cost_diamond` bigint(20) NOT NULL DEFAULT '0' COMMENT '消耗的总钻石数',
  `month_cost` bigint(20) NOT NULL DEFAULT '0' COMMENT '当月消耗的,实时数据',
  `week_cost` bigint(20) NOT NULL DEFAULT '0' COMMENT '本周消耗的钻石数,实时的',
  `day_cost` bigint(20) NOT NULL DEFAULT '0' COMMENT '当天消耗的钻石数,实时数据',
  `remark` varchar(255) NOT NULL DEFAULT '' COMMENT '备注',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='系统账号配置,系统钻石数';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Current Database: `chess_log`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `chess_log` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `chess_log`;

--
-- Table structure for table `agent_login_log`
--

DROP TABLE IF EXISTS `agent_login_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agent_login_log` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '代理的uid',
  `login_time` int(11) NOT NULL DEFAULT '0' COMMENT '登录时间',
  `ip` char(15) NOT NULL DEFAULT '' COMMENT '登录ip',
  `login_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '登录方式,2-手机，1-微信',
  PRIMARY KEY (`login_time`,`uid`),
  KEY `uid` (`uid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='代理登录详细日志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `client_error`
--

DROP TABLE IF EXISTS `client_error`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_error` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '错误日志id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '玩家用户ID',
  `msg` text NOT NULL COMMENT '报错内容',
  `ip` char(15) NOT NULL DEFAULT '' COMMENT 'IP地址',
  `device_id` varchar(32) NOT NULL DEFAULT '' COMMENT '设备号ID',
  `model` varchar(50) NOT NULL DEFAULT '' COMMENT '设备型号',
  `version_id` varchar(20) NOT NULL DEFAULT '' COMMENT '版本信息',
  `error_time` int(11) NOT NULL DEFAULT '0' COMMENT '报错时间,由服务器生成',
  `hot_version` varchar(50) NOT NULL DEFAULT '' COMMENT '热更版本',
  `md5` char(32) NOT NULL DEFAULT '' COMMENT '报错内容的md5值，为了去重',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否已经处理了, 0-未处理,1-已处理',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `error_time` (`error_time`) USING BTREE,
  KEY `md5` (`md5`),
  KEY `status` (`status`)
) ENGINE=InnoDB AUTO_INCREMENT=5782 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='客户端上报堆栈错误';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `client_operation_error`
--

DROP TABLE IF EXISTS `client_operation_error`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `client_operation_error` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '客户手动提交的错误日志',
  `uid` int(11) NOT NULL COMMENT '玩家id',
  `msg` varchar(2048) NOT NULL DEFAULT '' COMMENT '玩家手动填写的信息',
  `device_id` varchar(32) NOT NULL DEFAULT '' COMMENT '设备号ID',
  `model` varchar(50) NOT NULL DEFAULT '' COMMENT '设备型号',
  `ip` char(15) NOT NULL DEFAULT '' COMMENT '客户端ip',
  `app_version` varchar(20) NOT NULL DEFAULT '' COMMENT 'app版本(大厅版本)',
  `game_version` varchar(255) NOT NULL DEFAULT '' COMMENT '游戏版本',
  `log_url` varchar(512) NOT NULL DEFAULT '0' COMMENT '日志文件id,对应的表client_operation_log',
  `error_time` int(11) NOT NULL DEFAULT '0' COMMENT '报错时间,由服务器生成',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`),
  KEY `error_time` (`error_time`)
) ENGINE=InnoDB AUTO_INCREMENT=157 DEFAULT CHARSET=utf8 COMMENT='客户端手动上报的详细错误';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_agent`
--

DROP TABLE IF EXISTS `day_agent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_agent` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间,记录形式,20180101',
  `level` tinyint(4) NOT NULL DEFAULT '2' COMMENT '1-一级代理,2-二级代理',
  `pid` int(11) NOT NULL DEFAULT '0' COMMENT '二级代理，隶属哪个代理',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理的id',
  `agent_nickname` varchar(32) NOT NULL DEFAULT '' COMMENT '代理的昵称',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '大区id',
  `group_name` varchar(32) NOT NULL DEFAULT '' COMMENT '大区名字',
  `buy` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡数量',
  `buy_growth` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡环比增长',
  `give` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送的房卡',
  `give_growth` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送房卡环比增长',
  `auto` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量',
  `auto_growth` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量环比增长',
  `handle` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量',
  `handle_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量环比增长',
  `rest` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡数量',
  `rest_growth` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡环比增长',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额',
  `cash_growth` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额环比增长',
  `agent_order` int(11) NOT NULL DEFAULT '0' COMMENT '代理每天的订单金额(包括进化,及开通费用)',
  `agent_order_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代理订单金额与昨天对比增长情况',
  `player_order` int(11) NOT NULL DEFAULT '0' COMMENT '玩家在此代理的店铺直充的金额(玩家购买房卡)',
  `player_order_growth` int(11) NOT NULL DEFAULT '0' COMMENT '玩家店铺直充的订单金额增长情况',
  PRIMARY KEY (`create_time`,`agent_id`),
  KEY `auto` (`create_time`,`auto`) USING BTREE,
  KEY `auto_growth` (`create_time`,`auto_growth`) USING BTREE,
  KEY `buy` (`create_time`,`buy`) USING BTREE,
  KEY `buy_growth` (`create_time`,`buy_growth`) USING BTREE,
  KEY `give` (`create_time`,`give`) USING BTREE,
  KEY `give_growth` (`create_time`,`give_growth`) USING BTREE,
  KEY `handle` (`create_time`,`handle`) USING BTREE,
  KEY `handle_growth` (`create_time`,`handle_growth`) USING BTREE,
  KEY `rest` (`create_time`,`rest`),
  KEY `rest_growth` (`create_time`,`rest_growth`),
  KEY `cash` (`create_time`,`cash`),
  KEY `cash_growth` (`create_time`,`cash_growth`) USING BTREE,
  KEY `group_id` (`create_time`,`group_id`) USING BTREE,
  KEY `pid` (`create_time`,`pid`) USING BTREE,
  KEY `level` (`create_time`,`level`),
  KEY `agent_order` (`create_time`,`agent_order`) USING BTREE,
  KEY `player_order` (`create_time`,`player_order`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='统计一级代理及二级代理自己本身的的各种情况,房卡金额';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_agent_level1`
--

DROP TABLE IF EXISTS `day_agent_level1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_agent_level1` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间,记录形式,20180101',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理的id',
  `agent_nickname` varchar(32) NOT NULL DEFAULT '' COMMENT '代理的昵称',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '大区id',
  `group_name` varchar(32) NOT NULL DEFAULT '' COMMENT '大区名字',
  `buy` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡数量',
  `buy_growth` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡环比增长',
  `give` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送的房卡',
  `give_growth` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送房卡环比增长',
  `auto` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量',
  `auto_growth` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量环比增长',
  `handle` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量',
  `handle_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量环比增长',
  `rest` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡数量',
  `rest_growth` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡环比增长',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额',
  `cash_growth` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额环比增长',
  `agent_num` int(11) NOT NULL DEFAULT '0' COMMENT '代理数量',
  `agent_num_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代理数量环比增长',
  PRIMARY KEY (`create_time`,`agent_id`),
  KEY `create_group` (`create_time`,`group_id`) USING BTREE,
  KEY `agent_id` (`create_time`,`agent_id`) USING BTREE,
  KEY `buy` (`create_time`,`buy`) USING BTREE,
  KEY `buy_growth` (`create_time`,`buy_growth`) USING BTREE,
  KEY `give` (`create_time`,`give`),
  KEY `give_growth` (`create_time`,`give_growth`),
  KEY `auto` (`create_time`,`auto`),
  KEY `auto_growth` (`create_time`,`auto_growth`),
  KEY `handle` (`create_time`,`handle`),
  KEY `handle_growth` (`create_time`,`handle_growth`),
  KEY `rest` (`create_time`,`rest`),
  KEY `rest_growth` (`create_time`,`rest_growth`),
  KEY `cash` (`create_time`,`cash`),
  KEY `cash_growth` (`create_time`,`cash_growth`),
  KEY `agent_num` (`create_time`,`agent_num`),
  KEY `agent_num_growth` (`create_time`,`agent_num_growth`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='统计一级代理(一级代理本身及名下所有二级代理之和)的各种情况,房卡金额';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_club`
--

DROP TABLE IF EXISTS `day_club`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_club` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '统计时间',
  `club_id` int(11) NOT NULL DEFAULT '0' COMMENT '所属俱乐部的id',
  `password` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部ID(俱乐部密码)',
  `club_name` varchar(32) NOT NULL DEFAULT '0' COMMENT '俱乐部名字',
  `host_id` int(11) NOT NULL DEFAULT '0' COMMENT '创建人的id(游戏id)',
  `host_name` varchar(32) NOT NULL DEFAULT '' COMMENT '局长的昵称',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部游戏id',
  `game_name` varchar(255) NOT NULL DEFAULT '' COMMENT '游戏名字',
  `status` tinyint(4) NOT NULL COMMENT '俱乐部状态,1-正常，2-解散,3-冻结',
  `member` int(11) NOT NULL DEFAULT '0' COMMENT '俱乐部成员数',
  `table` int(11) NOT NULL DEFAULT '0' COMMENT '开的桌子数',
  `card` int(11) NOT NULL DEFAULT '0' COMMENT '消耗房卡数',
  PRIMARY KEY (`create_time`,`club_id`),
  KEY `host_id` (`host_id`) USING BTREE,
  KEY `game_id` (`game_id`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `password` (`password`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='统计俱乐部每天消耗的房卡数及开的桌数';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_game_stat`
--

DROP TABLE IF EXISTS `day_game_stat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_game_stat` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '统计时间',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `table_num` int(11) NOT NULL DEFAULT '0' COMMENT '开桌数',
  `round_num` int(11) NOT NULL DEFAULT '0' COMMENT '小局数',
  `diamond` int(11) NOT NULL DEFAULT '0' COMMENT '消耗了多少钻石',
  `user_num` int(11) NOT NULL DEFAULT '0' COMMENT '玩此款游戏的玩家数',
  PRIMARY KEY (`create_time`,`game_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='每天每款游戏开桌数及小局数';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_group`
--

DROP TABLE IF EXISTS `day_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_group` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间,记录时间20180101',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '大区id',
  `group_name` varchar(32) NOT NULL DEFAULT '' COMMENT '大区名字',
  `buy` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡数量',
  `buy_growth` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡环比增长',
  `give` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送的房卡',
  `give_growth` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送房卡环比增长',
  `auto` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量',
  `auto_growth` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量环比增长',
  `handle` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量',
  `handle_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量环比增长',
  `rest` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡数量',
  `rest_growth` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡环比增长',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额',
  `cash_growth` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额环比增长',
  `agent_num` int(11) NOT NULL DEFAULT '0' COMMENT '代理数量',
  `agent_num_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代理数量环比增长',
  `agent_order` int(11) NOT NULL DEFAULT '0' COMMENT '大区名下所有代理的订单金额',
  `agent_order_growth` int(11) NOT NULL DEFAULT '0' COMMENT '大区名下所有代理订单金额增长情况',
  `player_order` int(11) NOT NULL DEFAULT '0' COMMENT '大区名下所有代理的玩家直充订单金额',
  `player_order_growth` int(11) NOT NULL DEFAULT '0' COMMENT '大区名下所有代理的玩家店铺直充的订单金额增长情况',
  `bonus` int(11) NOT NULL DEFAULT '0' COMMENT '大区每天的分成',
  `bonus_rate` int(11) NOT NULL DEFAULT '0' COMMENT '大区当天的提成比例',
  `bonus_growth` int(11) NOT NULL DEFAULT '0' COMMENT '大区提成对比昨天的增长情况',
  PRIMARY KEY (`create_time`,`group_id`),
  KEY `buy` (`create_time`,`buy`),
  KEY `buy_growth` (`create_time`,`buy_growth`),
  KEY `give` (`create_time`,`give`),
  KEY `give_growth` (`create_time`,`give_growth`),
  KEY `auto` (`create_time`,`auto`),
  KEY `auto_growth` (`create_time`,`auto_growth`),
  KEY `handle` (`create_time`,`handle`),
  KEY `handle_growth` (`create_time`,`handle_growth`),
  KEY `rest` (`create_time`,`rest`),
  KEY `rest_growth` (`create_time`,`rest_growth`),
  KEY `cash` (`create_time`,`cash`),
  KEY `cash_growth` (`create_time`,`cash_growth`),
  KEY `agent_num` (`create_time`,`agent_num`),
  KEY `agent_num_growth` (`create_time`,`agent_num_growth`),
  KEY `agent_order` (`create_time`,`agent_order`) USING BTREE,
  KEY `player_order` (`create_time`,`player_order`),
  KEY `bonus` (`create_time`,`bonus`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='统计大区的各种情况,房卡金额';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_player_info`
--

DROP TABLE IF EXISTS `day_player_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_player_info` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录形式,20180102',
  `dau` int(11) NOT NULL DEFAULT '0' COMMENT '日活',
  `mau` int(11) NOT NULL DEFAULT '0' COMMENT '月活',
  `day_register` int(11) NOT NULL COMMENT '当日注册用户',
  `total_register` int(11) NOT NULL DEFAULT '0' COMMENT '累计注册用户',
  `two_day` smallint(6) NOT NULL DEFAULT '0' COMMENT '次日用户留率,百分比形式,保留两位小数，比如6123表示61.23%,',
  `three_day` smallint(6) NOT NULL DEFAULT '0' COMMENT '3日用户留率,百分比形式,保留两位小数，比如6123表示61.23%,',
  `seven_day` smallint(6) NOT NULL DEFAULT '0' COMMENT '7日用户留率,百分比形式,保留两位小数，比如6123表示61.23%,',
  `user_num` int(11) NOT NULL DEFAULT '0' COMMENT '每天在玩的玩家(实际打了游戏的才算)',
  PRIMARY KEY (`create_time`),
  KEY `dau` (`dau`) USING BTREE,
  KEY `mau` (`mau`) USING BTREE,
  KEY `day_register` (`day_register`) USING BTREE,
  KEY `total_register` (`total_register`) USING BTREE,
  KEY `two_day` (`two_day`) USING BTREE,
  KEY `three_day` (`three_day`) USING BTREE,
  KEY `seven_day` (`seven_day`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='用户留存率，日注册用户之类的';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_system_user`
--

DROP TABLE IF EXISTS `day_system_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_system_user` (
  `sys_uid` int(11) NOT NULL DEFAULT '0' COMMENT '系统帐号id',
  `diamond` bigint(20) NOT NULL DEFAULT '0' COMMENT '消耗的钻石',
  `operate_type` tinyint(4) NOT NULL DEFAULT '0',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间',
  PRIMARY KEY (`create_time`,`operate_type`,`sys_uid`),
  KEY `operate_type` (`operate_type`,`sys_uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='系统帐号流水的统计';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `day_trans_stat`
--

DROP TABLE IF EXISTS `day_trans_stat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `day_trans_stat` (
  `day` int(11) NOT NULL DEFAULT '0' COMMENT '统计时间',
  `send` int(11) NOT NULL DEFAULT '0' COMMENT '总共派发',
  `cost` int(11) NOT NULL DEFAULT '0' COMMENT '总消耗的',
  `remain` int(11) NOT NULL DEFAULT '0' COMMENT '总剩余',
  `table_num` int(11) NOT NULL DEFAULT '0' COMMENT '开桌数',
  `round_num` int(11) NOT NULL DEFAULT '0' COMMENT '小局数',
  PRIMARY KEY (`day`),
  KEY `table_num` (`table_num`),
  KEY `round_num` (`round_num`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='统计每日金流总数,每天所有游戏开桌数及小局数';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame`
--

DROP TABLE IF EXISTS `daygame`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180515`
--

DROP TABLE IF EXISTS `daygame20180515`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180515` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=39 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180516`
--

DROP TABLE IF EXISTS `daygame20180516`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180516` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180517`
--

DROP TABLE IF EXISTS `daygame20180517`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180517` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=106 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180518`
--

DROP TABLE IF EXISTS `daygame20180518`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180518` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=176 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180519`
--

DROP TABLE IF EXISTS `daygame20180519`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180519` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180520`
--

DROP TABLE IF EXISTS `daygame20180520`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180520` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180521`
--

DROP TABLE IF EXISTS `daygame20180521`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180521` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180522`
--

DROP TABLE IF EXISTS `daygame20180522`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180522` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180523`
--

DROP TABLE IF EXISTS `daygame20180523`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180523` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180524`
--

DROP TABLE IF EXISTS `daygame20180524`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180524` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180525`
--

DROP TABLE IF EXISTS `daygame20180525`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180525` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180526`
--

DROP TABLE IF EXISTS `daygame20180526`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180526` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180527`
--

DROP TABLE IF EXISTS `daygame20180527`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180527` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180528`
--

DROP TABLE IF EXISTS `daygame20180528`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180528` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180529`
--

DROP TABLE IF EXISTS `daygame20180529`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180529` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=71 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180530`
--

DROP TABLE IF EXISTS `daygame20180530`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180530` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=55 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180531`
--

DROP TABLE IF EXISTS `daygame20180531`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180531` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180601`
--

DROP TABLE IF EXISTS `daygame20180601`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180601` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180602`
--

DROP TABLE IF EXISTS `daygame20180602`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180602` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180603`
--

DROP TABLE IF EXISTS `daygame20180603`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180603` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180604`
--

DROP TABLE IF EXISTS `daygame20180604`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180604` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=109 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180605`
--

DROP TABLE IF EXISTS `daygame20180605`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180605` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180606`
--

DROP TABLE IF EXISTS `daygame20180606`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180606` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=106 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180607`
--

DROP TABLE IF EXISTS `daygame20180607`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180607` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180608`
--

DROP TABLE IF EXISTS `daygame20180608`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180608` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=28 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180609`
--

DROP TABLE IF EXISTS `daygame20180609`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180609` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(10) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(10) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=265 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180610`
--

DROP TABLE IF EXISTS `daygame20180610`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180610` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180611`
--

DROP TABLE IF EXISTS `daygame20180611`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180611` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=169 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180612`
--

DROP TABLE IF EXISTS `daygame20180612`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180612` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180613`
--

DROP TABLE IF EXISTS `daygame20180613`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180613` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180614`
--

DROP TABLE IF EXISTS `daygame20180614`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180614` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=64 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180615`
--

DROP TABLE IF EXISTS `daygame20180615`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180615` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=56 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180616`
--

DROP TABLE IF EXISTS `daygame20180616`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180616` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=53 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180617`
--

DROP TABLE IF EXISTS `daygame20180617`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180617` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180618`
--

DROP TABLE IF EXISTS `daygame20180618`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180618` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180619`
--

DROP TABLE IF EXISTS `daygame20180619`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180619` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180620`
--

DROP TABLE IF EXISTS `daygame20180620`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180620` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=29 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180621`
--

DROP TABLE IF EXISTS `daygame20180621`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180621` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180622`
--

DROP TABLE IF EXISTS `daygame20180622`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180622` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=61 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180623`
--

DROP TABLE IF EXISTS `daygame20180623`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180623` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=77 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180624`
--

DROP TABLE IF EXISTS `daygame20180624`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180624` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180625`
--

DROP TABLE IF EXISTS `daygame20180625`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180625` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=65 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180626`
--

DROP TABLE IF EXISTS `daygame20180626`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180626` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=63 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180627`
--

DROP TABLE IF EXISTS `daygame20180627`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180627` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=43 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180628`
--

DROP TABLE IF EXISTS `daygame20180628`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180628` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180629`
--

DROP TABLE IF EXISTS `daygame20180629`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180629` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180630`
--

DROP TABLE IF EXISTS `daygame20180630`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180630` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180701`
--

DROP TABLE IF EXISTS `daygame20180701`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180701` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180702`
--

DROP TABLE IF EXISTS `daygame20180702`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180702` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180703`
--

DROP TABLE IF EXISTS `daygame20180703`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180703` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180704`
--

DROP TABLE IF EXISTS `daygame20180704`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180704` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180705`
--

DROP TABLE IF EXISTS `daygame20180705`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180705` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=41 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180706`
--

DROP TABLE IF EXISTS `daygame20180706`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180706` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=40 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180707`
--

DROP TABLE IF EXISTS `daygame20180707`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180707` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180708`
--

DROP TABLE IF EXISTS `daygame20180708`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180708` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180709`
--

DROP TABLE IF EXISTS `daygame20180709`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180709` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180710`
--

DROP TABLE IF EXISTS `daygame20180710`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180710` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180711`
--

DROP TABLE IF EXISTS `daygame20180711`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180711` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180712`
--

DROP TABLE IF EXISTS `daygame20180712`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180712` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180713`
--

DROP TABLE IF EXISTS `daygame20180713`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180713` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=33 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180714`
--

DROP TABLE IF EXISTS `daygame20180714`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180714` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180715`
--

DROP TABLE IF EXISTS `daygame20180715`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180715` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180716`
--

DROP TABLE IF EXISTS `daygame20180716`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180716` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180717`
--

DROP TABLE IF EXISTS `daygame20180717`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180717` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180718`
--

DROP TABLE IF EXISTS `daygame20180718`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180718` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180719`
--

DROP TABLE IF EXISTS `daygame20180719`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180719` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180720`
--

DROP TABLE IF EXISTS `daygame20180720`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180720` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180721`
--

DROP TABLE IF EXISTS `daygame20180721`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180721` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180722`
--

DROP TABLE IF EXISTS `daygame20180722`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180722` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180723`
--

DROP TABLE IF EXISTS `daygame20180723`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180723` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180724`
--

DROP TABLE IF EXISTS `daygame20180724`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180724` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180725`
--

DROP TABLE IF EXISTS `daygame20180725`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180725` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180726`
--

DROP TABLE IF EXISTS `daygame20180726`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180726` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180727`
--

DROP TABLE IF EXISTS `daygame20180727`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180727` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180728`
--

DROP TABLE IF EXISTS `daygame20180728`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180728` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180729`
--

DROP TABLE IF EXISTS `daygame20180729`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180729` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180730`
--

DROP TABLE IF EXISTS `daygame20180730`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180730` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180731`
--

DROP TABLE IF EXISTS `daygame20180731`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180731` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180801`
--

DROP TABLE IF EXISTS `daygame20180801`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180801` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180802`
--

DROP TABLE IF EXISTS `daygame20180802`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180802` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180803`
--

DROP TABLE IF EXISTS `daygame20180803`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180803` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=173 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180804`
--

DROP TABLE IF EXISTS `daygame20180804`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180804` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=440 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180805`
--

DROP TABLE IF EXISTS `daygame20180805`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180805` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180806`
--

DROP TABLE IF EXISTS `daygame20180806`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180806` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180807`
--

DROP TABLE IF EXISTS `daygame20180807`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180807` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180808`
--

DROP TABLE IF EXISTS `daygame20180808`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180808` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180809`
--

DROP TABLE IF EXISTS `daygame20180809`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180809` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180810`
--

DROP TABLE IF EXISTS `daygame20180810`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180810` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180811`
--

DROP TABLE IF EXISTS `daygame20180811`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180811` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180812`
--

DROP TABLE IF EXISTS `daygame20180812`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180812` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180813`
--

DROP TABLE IF EXISTS `daygame20180813`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180813` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180814`
--

DROP TABLE IF EXISTS `daygame20180814`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180814` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180815`
--

DROP TABLE IF EXISTS `daygame20180815`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180815` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180816`
--

DROP TABLE IF EXISTS `daygame20180816`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180816` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180817`
--

DROP TABLE IF EXISTS `daygame20180817`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180817` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180818`
--

DROP TABLE IF EXISTS `daygame20180818`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180818` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180819`
--

DROP TABLE IF EXISTS `daygame20180819`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180819` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180820`
--

DROP TABLE IF EXISTS `daygame20180820`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180820` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180821`
--

DROP TABLE IF EXISTS `daygame20180821`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180821` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180822`
--

DROP TABLE IF EXISTS `daygame20180822`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180822` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180823`
--

DROP TABLE IF EXISTS `daygame20180823`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180823` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180824`
--

DROP TABLE IF EXISTS `daygame20180824`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180824` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180825`
--

DROP TABLE IF EXISTS `daygame20180825`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180825` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180826`
--

DROP TABLE IF EXISTS `daygame20180826`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180826` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180827`
--

DROP TABLE IF EXISTS `daygame20180827`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180827` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180828`
--

DROP TABLE IF EXISTS `daygame20180828`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180828` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180829`
--

DROP TABLE IF EXISTS `daygame20180829`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180829` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180830`
--

DROP TABLE IF EXISTS `daygame20180830`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180830` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180831`
--

DROP TABLE IF EXISTS `daygame20180831`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180831` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180901`
--

DROP TABLE IF EXISTS `daygame20180901`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180901` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180902`
--

DROP TABLE IF EXISTS `daygame20180902`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180902` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180903`
--

DROP TABLE IF EXISTS `daygame20180903`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180903` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180904`
--

DROP TABLE IF EXISTS `daygame20180904`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180904` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180905`
--

DROP TABLE IF EXISTS `daygame20180905`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180905` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180906`
--

DROP TABLE IF EXISTS `daygame20180906`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180906` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180907`
--

DROP TABLE IF EXISTS `daygame20180907`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180907` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180908`
--

DROP TABLE IF EXISTS `daygame20180908`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180908` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180909`
--

DROP TABLE IF EXISTS `daygame20180909`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180909` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180910`
--

DROP TABLE IF EXISTS `daygame20180910`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180910` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180911`
--

DROP TABLE IF EXISTS `daygame20180911`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180911` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180912`
--

DROP TABLE IF EXISTS `daygame20180912`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180912` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180913`
--

DROP TABLE IF EXISTS `daygame20180913`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180913` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=36 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180914`
--

DROP TABLE IF EXISTS `daygame20180914`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180914` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180915`
--

DROP TABLE IF EXISTS `daygame20180915`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180915` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180916`
--

DROP TABLE IF EXISTS `daygame20180916`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180916` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180917`
--

DROP TABLE IF EXISTS `daygame20180917`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180917` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180918`
--

DROP TABLE IF EXISTS `daygame20180918`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180918` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180919`
--

DROP TABLE IF EXISTS `daygame20180919`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180919` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180920`
--

DROP TABLE IF EXISTS `daygame20180920`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180920` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180921`
--

DROP TABLE IF EXISTS `daygame20180921`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180921` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180922`
--

DROP TABLE IF EXISTS `daygame20180922`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180922` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180923`
--

DROP TABLE IF EXISTS `daygame20180923`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180923` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180924`
--

DROP TABLE IF EXISTS `daygame20180924`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180924` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180925`
--

DROP TABLE IF EXISTS `daygame20180925`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180925` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180926`
--

DROP TABLE IF EXISTS `daygame20180926`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180926` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180927`
--

DROP TABLE IF EXISTS `daygame20180927`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180927` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180928`
--

DROP TABLE IF EXISTS `daygame20180928`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180928` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180929`
--

DROP TABLE IF EXISTS `daygame20180929`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180929` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20180930`
--

DROP TABLE IF EXISTS `daygame20180930`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20180930` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181001`
--

DROP TABLE IF EXISTS `daygame20181001`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181001` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181002`
--

DROP TABLE IF EXISTS `daygame20181002`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181002` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181003`
--

DROP TABLE IF EXISTS `daygame20181003`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181003` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181004`
--

DROP TABLE IF EXISTS `daygame20181004`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181004` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181005`
--

DROP TABLE IF EXISTS `daygame20181005`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181005` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181006`
--

DROP TABLE IF EXISTS `daygame20181006`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181006` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181007`
--

DROP TABLE IF EXISTS `daygame20181007`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181007` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181008`
--

DROP TABLE IF EXISTS `daygame20181008`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181008` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181009`
--

DROP TABLE IF EXISTS `daygame20181009`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181009` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daygame20181010`
--

DROP TABLE IF EXISTS `daygame20181010`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daygame20181010` (
  `dgid` int(10) NOT NULL AUTO_INCREMENT COMMENT '主键',
  `gid` smallint(5) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `tid` bigint(20) NOT NULL DEFAULT '0' COMMENT '桌子id',
  `rid` int(10) NOT NULL DEFAULT '0' COMMENT '房间id',
  `roomtype` smallint(5) NOT NULL DEFAULT '0' COMMENT '房间类型',
  `consume` tinyint(2) NOT NULL DEFAULT '0' COMMENT '消费类型',
  `play_type` tinyint(2) NOT NULL DEFAULT '0' COMMENT '玩法类型',
  `basescore` int(10) NOT NULL DEFAULT '0' COMMENT '底注',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '游戏抽水',
  `begintime` int(11) NOT NULL DEFAULT '0' COMMENT '开始时间',
  `endtime` int(11) NOT NULL DEFAULT '0' COMMENT '结束时间',
  `uids` text NOT NULL COMMENT '用户id们',
  `content` text NOT NULL COMMENT '牌局详情',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id',
  PRIMARY KEY (`dgid`),
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `tid` (`tid`) USING BTREE,
  KEY `btime` (`begintime`) USING BTREE,
  KEY `etime` (`endtime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC COMMENT='日游戏记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans`
--

DROP TABLE IF EXISTS `daytrans`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180514`
--

DROP TABLE IF EXISTS `daytrans20180514`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180514` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180515`
--

DROP TABLE IF EXISTS `daytrans20180515`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180515` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180516`
--

DROP TABLE IF EXISTS `daytrans20180516`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180516` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180517`
--

DROP TABLE IF EXISTS `daytrans20180517`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180517` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180518`
--

DROP TABLE IF EXISTS `daytrans20180518`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180518` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=42 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180519`
--

DROP TABLE IF EXISTS `daytrans20180519`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180519` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180520`
--

DROP TABLE IF EXISTS `daytrans20180520`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180520` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180521`
--

DROP TABLE IF EXISTS `daytrans20180521`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180521` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180522`
--

DROP TABLE IF EXISTS `daytrans20180522`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180522` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180523`
--

DROP TABLE IF EXISTS `daytrans20180523`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180523` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180524`
--

DROP TABLE IF EXISTS `daytrans20180524`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180524` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180525`
--

DROP TABLE IF EXISTS `daytrans20180525`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180525` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180526`
--

DROP TABLE IF EXISTS `daytrans20180526`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180526` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180527`
--

DROP TABLE IF EXISTS `daytrans20180527`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180527` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180528`
--

DROP TABLE IF EXISTS `daytrans20180528`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180528` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180529`
--

DROP TABLE IF EXISTS `daytrans20180529`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180529` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=27 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180530`
--

DROP TABLE IF EXISTS `daytrans20180530`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180530` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180531`
--

DROP TABLE IF EXISTS `daytrans20180531`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180531` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180601`
--

DROP TABLE IF EXISTS `daytrans20180601`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180601` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180602`
--

DROP TABLE IF EXISTS `daytrans20180602`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180602` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180603`
--

DROP TABLE IF EXISTS `daytrans20180603`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180603` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180604`
--

DROP TABLE IF EXISTS `daytrans20180604`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180604` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180605`
--

DROP TABLE IF EXISTS `daytrans20180605`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180605` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180606`
--

DROP TABLE IF EXISTS `daytrans20180606`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180606` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180607`
--

DROP TABLE IF EXISTS `daytrans20180607`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180607` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180608`
--

DROP TABLE IF EXISTS `daytrans20180608`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180608` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180609`
--

DROP TABLE IF EXISTS `daytrans20180609`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180609` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` bigint(20) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=42 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180610`
--

DROP TABLE IF EXISTS `daytrans20180610`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180610` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180611`
--

DROP TABLE IF EXISTS `daytrans20180611`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180611` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=26 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180612`
--

DROP TABLE IF EXISTS `daytrans20180612`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180612` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180613`
--

DROP TABLE IF EXISTS `daytrans20180613`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180613` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180614`
--

DROP TABLE IF EXISTS `daytrans20180614`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180614` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180615`
--

DROP TABLE IF EXISTS `daytrans20180615`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180615` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180616`
--

DROP TABLE IF EXISTS `daytrans20180616`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180616` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180617`
--

DROP TABLE IF EXISTS `daytrans20180617`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180617` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180618`
--

DROP TABLE IF EXISTS `daytrans20180618`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180618` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180619`
--

DROP TABLE IF EXISTS `daytrans20180619`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180619` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180620`
--

DROP TABLE IF EXISTS `daytrans20180620`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180620` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180621`
--

DROP TABLE IF EXISTS `daytrans20180621`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180621` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180622`
--

DROP TABLE IF EXISTS `daytrans20180622`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180622` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180623`
--

DROP TABLE IF EXISTS `daytrans20180623`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180623` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180624`
--

DROP TABLE IF EXISTS `daytrans20180624`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180624` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180625`
--

DROP TABLE IF EXISTS `daytrans20180625`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180625` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180626`
--

DROP TABLE IF EXISTS `daytrans20180626`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180626` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180627`
--

DROP TABLE IF EXISTS `daytrans20180627`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180627` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180628`
--

DROP TABLE IF EXISTS `daytrans20180628`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180628` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180629`
--

DROP TABLE IF EXISTS `daytrans20180629`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180629` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180630`
--

DROP TABLE IF EXISTS `daytrans20180630`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180630` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180701`
--

DROP TABLE IF EXISTS `daytrans20180701`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180701` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180702`
--

DROP TABLE IF EXISTS `daytrans20180702`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180702` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180703`
--

DROP TABLE IF EXISTS `daytrans20180703`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180703` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180704`
--

DROP TABLE IF EXISTS `daytrans20180704`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180704` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180705`
--

DROP TABLE IF EXISTS `daytrans20180705`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180705` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=20 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180706`
--

DROP TABLE IF EXISTS `daytrans20180706`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180706` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180707`
--

DROP TABLE IF EXISTS `daytrans20180707`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180707` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180708`
--

DROP TABLE IF EXISTS `daytrans20180708`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180708` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180709`
--

DROP TABLE IF EXISTS `daytrans20180709`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180709` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180710`
--

DROP TABLE IF EXISTS `daytrans20180710`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180710` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180711`
--

DROP TABLE IF EXISTS `daytrans20180711`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180711` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180712`
--

DROP TABLE IF EXISTS `daytrans20180712`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180712` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180713`
--

DROP TABLE IF EXISTS `daytrans20180713`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180713` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180714`
--

DROP TABLE IF EXISTS `daytrans20180714`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180714` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180715`
--

DROP TABLE IF EXISTS `daytrans20180715`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180715` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180716`
--

DROP TABLE IF EXISTS `daytrans20180716`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180716` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180717`
--

DROP TABLE IF EXISTS `daytrans20180717`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180717` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180718`
--

DROP TABLE IF EXISTS `daytrans20180718`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180718` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180719`
--

DROP TABLE IF EXISTS `daytrans20180719`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180719` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180720`
--

DROP TABLE IF EXISTS `daytrans20180720`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180720` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180721`
--

DROP TABLE IF EXISTS `daytrans20180721`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180721` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180722`
--

DROP TABLE IF EXISTS `daytrans20180722`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180722` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180723`
--

DROP TABLE IF EXISTS `daytrans20180723`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180723` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180724`
--

DROP TABLE IF EXISTS `daytrans20180724`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180724` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180725`
--

DROP TABLE IF EXISTS `daytrans20180725`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180725` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180726`
--

DROP TABLE IF EXISTS `daytrans20180726`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180726` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180727`
--

DROP TABLE IF EXISTS `daytrans20180727`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180727` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180728`
--

DROP TABLE IF EXISTS `daytrans20180728`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180728` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180729`
--

DROP TABLE IF EXISTS `daytrans20180729`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180729` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180730`
--

DROP TABLE IF EXISTS `daytrans20180730`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180730` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180731`
--

DROP TABLE IF EXISTS `daytrans20180731`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180731` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180801`
--

DROP TABLE IF EXISTS `daytrans20180801`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180801` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180802`
--

DROP TABLE IF EXISTS `daytrans20180802`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180802` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180803`
--

DROP TABLE IF EXISTS `daytrans20180803`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180803` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180804`
--

DROP TABLE IF EXISTS `daytrans20180804`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180804` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=54 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180805`
--

DROP TABLE IF EXISTS `daytrans20180805`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180805` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180806`
--

DROP TABLE IF EXISTS `daytrans20180806`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180806` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180807`
--

DROP TABLE IF EXISTS `daytrans20180807`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180807` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180808`
--

DROP TABLE IF EXISTS `daytrans20180808`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180808` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180809`
--

DROP TABLE IF EXISTS `daytrans20180809`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180809` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180810`
--

DROP TABLE IF EXISTS `daytrans20180810`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180810` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180811`
--

DROP TABLE IF EXISTS `daytrans20180811`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180811` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180812`
--

DROP TABLE IF EXISTS `daytrans20180812`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180812` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180813`
--

DROP TABLE IF EXISTS `daytrans20180813`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180813` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180814`
--

DROP TABLE IF EXISTS `daytrans20180814`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180814` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180815`
--

DROP TABLE IF EXISTS `daytrans20180815`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180815` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180816`
--

DROP TABLE IF EXISTS `daytrans20180816`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180816` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180817`
--

DROP TABLE IF EXISTS `daytrans20180817`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180817` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180818`
--

DROP TABLE IF EXISTS `daytrans20180818`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180818` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180819`
--

DROP TABLE IF EXISTS `daytrans20180819`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180819` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180820`
--

DROP TABLE IF EXISTS `daytrans20180820`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180820` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180821`
--

DROP TABLE IF EXISTS `daytrans20180821`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180821` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180822`
--

DROP TABLE IF EXISTS `daytrans20180822`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180822` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180823`
--

DROP TABLE IF EXISTS `daytrans20180823`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180823` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180824`
--

DROP TABLE IF EXISTS `daytrans20180824`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180824` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180825`
--

DROP TABLE IF EXISTS `daytrans20180825`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180825` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180826`
--

DROP TABLE IF EXISTS `daytrans20180826`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180826` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180827`
--

DROP TABLE IF EXISTS `daytrans20180827`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180827` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180828`
--

DROP TABLE IF EXISTS `daytrans20180828`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180828` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180829`
--

DROP TABLE IF EXISTS `daytrans20180829`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180829` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180830`
--

DROP TABLE IF EXISTS `daytrans20180830`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180830` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180831`
--

DROP TABLE IF EXISTS `daytrans20180831`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180831` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180901`
--

DROP TABLE IF EXISTS `daytrans20180901`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180901` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180902`
--

DROP TABLE IF EXISTS `daytrans20180902`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180902` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180903`
--

DROP TABLE IF EXISTS `daytrans20180903`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180903` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180904`
--

DROP TABLE IF EXISTS `daytrans20180904`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180904` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180905`
--

DROP TABLE IF EXISTS `daytrans20180905`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180905` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180906`
--

DROP TABLE IF EXISTS `daytrans20180906`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180906` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180907`
--

DROP TABLE IF EXISTS `daytrans20180907`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180907` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180908`
--

DROP TABLE IF EXISTS `daytrans20180908`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180908` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180909`
--

DROP TABLE IF EXISTS `daytrans20180909`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180909` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180910`
--

DROP TABLE IF EXISTS `daytrans20180910`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180910` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180911`
--

DROP TABLE IF EXISTS `daytrans20180911`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180911` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180912`
--

DROP TABLE IF EXISTS `daytrans20180912`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180912` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180913`
--

DROP TABLE IF EXISTS `daytrans20180913`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180913` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180914`
--

DROP TABLE IF EXISTS `daytrans20180914`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180914` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180915`
--

DROP TABLE IF EXISTS `daytrans20180915`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180915` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180916`
--

DROP TABLE IF EXISTS `daytrans20180916`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180916` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180917`
--

DROP TABLE IF EXISTS `daytrans20180917`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180917` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180918`
--

DROP TABLE IF EXISTS `daytrans20180918`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180918` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180919`
--

DROP TABLE IF EXISTS `daytrans20180919`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180919` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180920`
--

DROP TABLE IF EXISTS `daytrans20180920`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180920` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180921`
--

DROP TABLE IF EXISTS `daytrans20180921`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180921` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180922`
--

DROP TABLE IF EXISTS `daytrans20180922`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180922` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180923`
--

DROP TABLE IF EXISTS `daytrans20180923`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180923` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180924`
--

DROP TABLE IF EXISTS `daytrans20180924`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180924` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180925`
--

DROP TABLE IF EXISTS `daytrans20180925`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180925` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180926`
--

DROP TABLE IF EXISTS `daytrans20180926`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180926` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180927`
--

DROP TABLE IF EXISTS `daytrans20180927`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180927` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180928`
--

DROP TABLE IF EXISTS `daytrans20180928`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180928` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180929`
--

DROP TABLE IF EXISTS `daytrans20180929`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180929` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20180930`
--

DROP TABLE IF EXISTS `daytrans20180930`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20180930` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181001`
--

DROP TABLE IF EXISTS `daytrans20181001`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181001` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181002`
--

DROP TABLE IF EXISTS `daytrans20181002`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181002` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181003`
--

DROP TABLE IF EXISTS `daytrans20181003`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181003` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181004`
--

DROP TABLE IF EXISTS `daytrans20181004`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181004` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181005`
--

DROP TABLE IF EXISTS `daytrans20181005`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181005` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181006`
--

DROP TABLE IF EXISTS `daytrans20181006`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181006` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181007`
--

DROP TABLE IF EXISTS `daytrans20181007`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181007` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181008`
--

DROP TABLE IF EXISTS `daytrans20181008`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181008` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181009`
--

DROP TABLE IF EXISTS `daytrans20181009`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181009` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `daytrans20181010`
--

DROP TABLE IF EXISTS `daytrans20181010`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `daytrans20181010` (
  `tid` int(10) NOT NULL AUTO_INCREMENT COMMENT '事务id',
  `uid` int(10) NOT NULL DEFAULT '0' COMMENT '用户id',
  `atype` tinyint(2) NOT NULL DEFAULT '0' COMMENT '货币类型',
  `ptype` smallint(5) NOT NULL DEFAULT '0' COMMENT '操作类型',
  `sptype` int(10) NOT NULL DEFAULT '0' COMMENT '子操作类型',
  `amount` bigint(20) NOT NULL DEFAULT '0' COMMENT '金额',
  `oldv` bigint(20) NOT NULL DEFAULT '0' COMMENT '老数据',
  `newv` bigint(20) NOT NULL DEFAULT '0' COMMENT '新数据',
  `ptime` int(11) NOT NULL DEFAULT '0' COMMENT '操作时间',
  `chessid` varchar(50) NOT NULL DEFAULT '' COMMENT '牌局id, 由房间id之类的拼接起来的',
  PRIMARY KEY (`tid`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `chessid` (`chessid`) USING BTREE,
  KEY `ptime` (`ptime`) USING BTREE,
  KEY `atype` (`atype`) USING BTREE,
  KEY `ptype` (`ptype`) USING BTREE,
  KEY `sptype` (`sptype`) USING BTREE,
  KEY `amount` (`amount`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每日金流详细记录';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `event_log`
--

DROP TABLE IF EXISTS `event_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `event_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `module` tinyint(4) NOT NULL DEFAULT '0' COMMENT '事件模块',
  `event_type` int(11) NOT NULL DEFAULT '0' COMMENT '事件类型',
  `user_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '用户类型，1-代理，2-玩家',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '代理或玩家id',
  `msg` varchar(256) NOT NULL DEFAULT '' COMMENT '信息',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '日志创建时间',
  PRIMARY KEY (`id`),
  KEY `createtime` (`create_time`) USING BTREE,
  KEY `uid` (`uid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=utf8 COMMENT='主要记录各种事件的情况';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `month_agent`
--

DROP TABLE IF EXISTS `month_agent`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `month_agent` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间,记录形式20180101',
  `level` tinyint(4) NOT NULL DEFAULT '2' COMMENT '1-一级代理,2-二级代理',
  `pid` int(11) NOT NULL DEFAULT '0' COMMENT '二级代理，隶属哪个代理',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理的id',
  `agent_nickname` varchar(32) NOT NULL DEFAULT '' COMMENT '代理的昵称',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '大区id',
  `group_name` varchar(32) NOT NULL DEFAULT '' COMMENT '大区名字',
  `buy` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡数量',
  `buy_growth` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡环比增长',
  `give` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送的房卡',
  `give_growth` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送房卡环比增长',
  `auto` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量',
  `auto_growth` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量环比增长',
  `handle` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量',
  `handle_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量环比增长',
  `rest` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡数量',
  `rest_growth` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡环比增长',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额',
  `cash_growth` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额环比增长',
  `agent_order` int(11) NOT NULL DEFAULT '0' COMMENT '代理每月的订单金额(包括进化,及开通费用)',
  `agent_order_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代理订单金额与上个月对比增长情况',
  `player_order` int(11) NOT NULL DEFAULT '0' COMMENT '玩家在此代理的店铺直充的金额(玩家购买房卡)',
  `player_order_growth` int(11) DEFAULT '0' COMMENT '玩家店铺直充的订单金额增长情况',
  PRIMARY KEY (`create_time`,`agent_id`),
  KEY `level` (`create_time`,`level`),
  KEY `pid` (`create_time`,`pid`),
  KEY `group_id` (`create_time`,`group_id`),
  KEY `buy` (`create_time`,`buy`),
  KEY `buy_growth` (`create_time`,`buy_growth`),
  KEY `give` (`create_time`,`give`),
  KEY `give_growth` (`create_time`,`give_growth`),
  KEY `auto` (`create_time`,`auto`),
  KEY `auto_growth` (`create_time`,`auto_growth`),
  KEY `handle` (`create_time`,`handle`),
  KEY `handle_growth` (`create_time`,`handle_growth`),
  KEY `rest` (`create_time`,`rest`),
  KEY `rest_growth` (`create_time`,`rest_growth`),
  KEY `cash` (`create_time`,`cash`),
  KEY `cash_growth` (`create_time`,`cash_growth`),
  KEY `agent_order` (`create_time`,`agent_order`) USING BTREE,
  KEY `player_order` (`create_time`,`player_order`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='统计代理的各种情况,房卡金额';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `month_agent_level1`
--

DROP TABLE IF EXISTS `month_agent_level1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `month_agent_level1` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间,记录形式20180101',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理的id',
  `agent_nickname` varchar(32) NOT NULL DEFAULT '' COMMENT '代理的昵称',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '大区id',
  `group_name` varchar(32) NOT NULL DEFAULT '' COMMENT '大区名字',
  `buy` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡数量',
  `buy_growth` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡环比增长',
  `give` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送的房卡',
  `give_growth` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送房卡环比增长',
  `auto` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量',
  `auto_growth` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量环比增长',
  `handle` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量',
  `handle_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量环比增长',
  `rest` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡数量',
  `rest_growth` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡环比增长',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额',
  `cash_growth` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额环比增长',
  `agent_num` int(11) NOT NULL DEFAULT '0' COMMENT '代理数量',
  `agent_num_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代理数量环比增长',
  PRIMARY KEY (`create_time`,`agent_id`),
  KEY `group_id` (`create_time`,`group_id`),
  KEY `buy` (`create_time`,`buy`),
  KEY `buy_growth` (`create_time`,`buy_growth`),
  KEY `give` (`create_time`,`give`),
  KEY `give_growth` (`create_time`,`give_growth`),
  KEY `auto` (`create_time`,`auto`),
  KEY `auto_growth` (`create_time`,`auto_growth`),
  KEY `handle` (`create_time`,`handle`),
  KEY `handle_growth` (`create_time`,`handle_growth`),
  KEY `rest` (`create_time`,`rest`),
  KEY `rest_growth` (`create_time`,`rest_growth`),
  KEY `cash` (`create_time`,`cash`),
  KEY `cash_growth` (`create_time`,`cash_growth`),
  KEY `agent_num` (`create_time`,`agent_num`),
  KEY `agent_num_growth` (`create_time`,`agent_num_growth`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='统计代理的各种情况,房卡金额';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `month_group`
--

DROP TABLE IF EXISTS `month_group`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `month_group` (
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间,记录形式201801,只记录到月',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '大区id',
  `group_name` varchar(32) NOT NULL DEFAULT '' COMMENT '大区名字',
  `buy` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡数量',
  `buy_growth` int(11) NOT NULL DEFAULT '0' COMMENT '买房卡环比增长',
  `give` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送的房卡',
  `give_growth` int(11) NOT NULL DEFAULT '0' COMMENT '系统赠送房卡环比增长',
  `auto` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量',
  `auto_growth` int(11) NOT NULL DEFAULT '0' COMMENT '直充房卡数量环比增长',
  `handle` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量',
  `handle_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代充房卡数量环比增长',
  `rest` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡数量',
  `rest_growth` int(11) NOT NULL DEFAULT '0' COMMENT '剩余房卡环比增长',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额',
  `cash_growth` int(11) NOT NULL DEFAULT '0' COMMENT '可提现金额环比增长',
  `agent_num` int(11) NOT NULL DEFAULT '0' COMMENT '代理数量',
  `agent_num_growth` int(11) NOT NULL DEFAULT '0' COMMENT '代理数量环比增长',
  `agent_order` int(11) NOT NULL DEFAULT '0' COMMENT '大区名下所有代理的订单金额',
  `agent_order_growth` int(11) NOT NULL DEFAULT '0' COMMENT '大区名下所有代理订单金额增长情况',
  `player_order` int(11) NOT NULL DEFAULT '0' COMMENT '大区名下所有代理的玩家直充订单金额',
  `player_order_growth` int(11) NOT NULL DEFAULT '0' COMMENT '大区名下所有代理的玩家店铺直充的订单金额增长情况',
  `bonus` int(11) NOT NULL DEFAULT '0' COMMENT '大区每个月的提成',
  `bonus_growth` int(11) NOT NULL DEFAULT '0' COMMENT '大区提成对比上个月的增长情况',
  PRIMARY KEY (`create_time`,`group_id`),
  KEY `buy` (`create_time`,`buy`),
  KEY `buy_growth` (`create_time`,`buy_growth`),
  KEY `give` (`create_time`,`give`),
  KEY `give_growth` (`create_time`,`give_growth`),
  KEY `auto` (`create_time`,`auto`),
  KEY `auto_growth` (`create_time`,`auto_growth`),
  KEY `handle` (`create_time`,`handle`),
  KEY `handle_growth` (`create_time`,`handle_growth`),
  KEY `rest` (`create_time`,`rest`),
  KEY `rest_growth` (`create_time`,`rest_growth`),
  KEY `cash` (`create_time`,`cash`),
  KEY `cash_growth` (`create_time`,`cash_growth`),
  KEY `agent_num` (`create_time`,`agent_num`),
  KEY `agent_num_growth` (`create_time`,`agent_num_growth`),
  KEY `agent_order` (`create_time`,`agent_order`),
  KEY `player_order` (`create_time`,`player_order`),
  KEY `bonus` (`create_time`,`bonus`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='统计代理的各种情况,房卡金额';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `month_system_user`
--

DROP TABLE IF EXISTS `month_system_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `month_system_user` (
  `sys_uid` int(11) NOT NULL DEFAULT '0' COMMENT '系统帐号id',
  `diamond` bigint(20) NOT NULL DEFAULT '0' COMMENT '消耗的钻石',
  `operate_type` tinyint(4) NOT NULL DEFAULT '0',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间',
  PRIMARY KEY (`create_time`,`operate_type`,`sys_uid`),
  KEY `operate_type` (`operate_type`,`sys_uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='系统帐号流水的统计,月报表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `player_login_log`
--

DROP TABLE IF EXISTS `player_login_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `player_login_log` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家uid',
  `login_time` int(11) NOT NULL DEFAULT '0' COMMENT '登录时间',
  `ip` char(15) NOT NULL DEFAULT '' COMMENT '登录ip',
  `device_id` varchar(64) NOT NULL DEFAULT '' COMMENT '设备ID',
  `model` varchar(128) NOT NULL DEFAULT '' COMMENT '设备类型',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `login_time_uid` (`login_time`,`uid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=20363 DEFAULT CHARSET=utf8 COMMENT='玩家登录详细日志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `record_day`
--

DROP TABLE IF EXISTS `record_day`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `record_day` (
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '用于表示统计类型',
  `record_day` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间,时间戳形式',
  `cost_time` int(11) NOT NULL DEFAULT '0' COMMENT '执行消息的时间',
  PRIMARY KEY (`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='安全机制用于标记数据统计统计到第几天了,主要是防止停服了或断电了这期间的数据没有统计上来。\r\n1-day_agent\r\n2-day_group\r\n3-month_agent\r\n4-month_group\r\n5-day_club';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `register_count`
--

DROP TABLE IF EXISTS `register_count`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `register_count` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `count` int(11) NOT NULL DEFAULT '0' COMMENT '注册人数',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '统计时间',
  PRIMARY KEY (`id`),
  KEY `create_time` (`create_time`)
) ENGINE=InnoDB AUTO_INCREMENT=2090 DEFAULT CHARSET=utf8 COMMENT='每半小时统计一次，统计的是半小时内的注册玩家数';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `system_user_tran`
--

DROP TABLE IF EXISTS `system_user_tran`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `system_user_tran` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `sys_uid` int(11) NOT NULL DEFAULT '0' COMMENT '系统帐号ID',
  `diamond` bigint(20) NOT NULL DEFAULT '0' COMMENT '钻石数,负数表示流出,正数表示流入',
  `user_uid` int(11) NOT NULL DEFAULT '0' COMMENT '钻石流出到用户ID(代理ID,玩家ID)',
  `operate_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '钻石类型, 1-手动增加流入,2-购买流出,3-绑定手机赠送流出,4-活动赠送流出,5-手动赠送流出(后台直接修改玩家的数值),6-手动回收流入(后台直接修改玩家的数值), 7-玩家注册赠送流出,8-购买赠送流出',
  `operate_uid` int(11) NOT NULL DEFAULT '-1' COMMENT '操作人的uid(PC端的uid),-1-系统',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间',
  PRIMARY KEY (`id`),
  KEY `create_time` (`create_time`),
  KEY `sys_uid` (`sys_uid`),
  KEY `operate_type` (`operate_type`)
) ENGINE=InnoDB AUTO_INCREMENT=3671 DEFAULT CHARSET=utf8 COMMENT='系统帐号的流水';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Current Database: `chess_log2`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `chess_log2` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `chess_log2`;

--
-- Table structure for table `submit_log`
--

DROP TABLE IF EXISTS `submit_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `submit_log` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '打点日志记录',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `status` int(11) NOT NULL DEFAULT '0' COMMENT '状态',
  `mod` int(11) NOT NULL DEFAULT '0' COMMENT '模块分类',
  `msg` text NOT NULL COMMENT '打点日志',
  `model` varchar(128) NOT NULL DEFAULT '' COMMENT '设备型号',
  `device_id` varchar(32) NOT NULL DEFAULT '' COMMENT '设备号ID',
  `version_id` varchar(32) NOT NULL DEFAULT '' COMMENT '版本信息',
  `is_valid` tinyint(4) NOT NULL DEFAULT '1' COMMENT '此信息是否有效, 1-有效,2-无效',
  `error_time` int(11) NOT NULL DEFAULT '0' COMMENT '打点日志时间',
  `hot_version` varchar(32) NOT NULL DEFAULT '' COMMENT '热更版本',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `mod` (`mod`) USING BTREE,
  KEY `is_valid` (`is_valid`) USING BTREE,
  KEY `device_id` (`device_id`) USING BTREE,
  KEY `error_time` (`error_time`,`uid`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=42 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Current Database: `chess_partner`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `chess_partner` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `chess_partner`;

--
-- Table structure for table `partner_agent_bank`
--

DROP TABLE IF EXISTS `partner_agent_bank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_agent_bank` (
  `uid` int(11) NOT NULL COMMENT '代理的uid',
  `name` varchar(16) NOT NULL DEFAULT '' COMMENT '真实姓名',
  `bank_name` varchar(64) NOT NULL DEFAULT '' COMMENT '开户行，如工商银行',
  `province` varchar(32) NOT NULL DEFAULT '' COMMENT '开户行所在省',
  `city` varchar(32) NOT NULL DEFAULT '' COMMENT '开户行所有市',
  `card_num` varchar(32) NOT NULL DEFAULT '' COMMENT '银行卡号',
  `branch` varchar(64) NOT NULL DEFAULT '' COMMENT '银行支行',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '绑定时间',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_agent_player`
--

DROP TABLE IF EXISTS `partner_agent_player`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_agent_player` (
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `player_id` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `remark` varchar(32) NOT NULL DEFAULT '' COMMENT '玩家备注，方便代理识别',
  `create_time` int(11) NOT NULL,
  PRIMARY KEY (`agent_id`,`player_id`),
  KEY `player_id` (`agent_id`,`create_time`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_card_cash`
--

DROP TABLE IF EXISTS `partner_card_cash`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_card_cash` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '房卡数量,实时的数据',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '金额，存储时乘以100，展示时除以100, 单位元，',
  `total_card_num` int(11) NOT NULL DEFAULT '0' COMMENT '总购房卡数,累加的，历史所有的卡累加的',
  `open_fee` int(11) NOT NULL DEFAULT '0' COMMENT '开通费用,存储时乘以100，展示时除以100,，单位元，',
  `withdraw_time` int(11) NOT NULL DEFAULT '0' COMMENT '最近一次提现时间',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_card_record`
--

DROP TABLE IF EXISTS `partner_card_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_card_record` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '代理房卡记录id',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `player_id` int(11) NOT NULL DEFAULT '0' COMMENT '玩家id',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '房卡买卖数量',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '买卖总价，乘以100',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '记录类型,1-买房卡,2-卖房卡(代充),,4-送房卡,5-获赠房卡,6-系统赠送',
  `pay_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '支付方式，1-余额支付，2-微信支付，3-支付宝支付',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '记录时间',
  PRIMARY KEY (`id`),
  KEY `act` (`agent_id`,`create_time`,`type`) USING BTREE,
  KEY `type` (`type`) USING BTREE,
  KEY `player_id` (`player_id`,`agent_id`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_cash_record`
--

DROP TABLE IF EXISTS `partner_cash_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_cash_record` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '代理金额流水账id',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `child_id` int(11) NOT NULL DEFAULT '0' COMMENT '从哪个代理ID里产生的收入',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '流水账类型，1-提现退回,2-提现,3-代理提成，4-余额买卡,5-余额购买代理名额, 100-代理充值金额',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '代理的房卡剩余数量',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '提成金额，乘以100',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '充值金额,乘以100,展示时除以100',
  `month` smallint(11) NOT NULL DEFAULT '0' COMMENT '按月分帐单1801表示，2018年1月份',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '流水账时间',
  PRIMARY KEY (`id`),
  KEY `agent_type` (`agent_id`,`type`,`create_time`) USING BTREE,
  KEY `create_time` (`create_time`),
  KEY `agent_month` (`agent_id`,`month`,`type`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_cash_summary`
--

DROP TABLE IF EXISTS `partner_cash_summary`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_cash_summary` (
  `uid` int(11) NOT NULL COMMENT '合伙人系统代理id',
  `pid` int(11) NOT NULL DEFAULT '0' COMMENT '上级代理id',
  `total_amount` int(11) NOT NULL DEFAULT '0' COMMENT '总充值(自己和下级的充值总和)',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='合伙人系统代理提成汇总';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_feedback`
--

DROP TABLE IF EXISTS `partner_feedback`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_feedback` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '用户反馈id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '用户id',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '反馈问题的类型',
  `content` varchar(2048) NOT NULL DEFAULT '' COMMENT '反馈内容',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '状态(0未处理,1已处理)',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `update_time` int(11) NOT NULL DEFAULT '0' COMMENT '处理此反馈的时间',
  `operator_id` int(11) NOT NULL DEFAULT '0' COMMENT '操作者,处理此反馈的人',
  PRIMARY KEY (`id`),
  KEY `uid` (`uid`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `create` (`create_time`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_feedback_url`
--

DROP TABLE IF EXISTS `partner_feedback_url`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_feedback_url` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '反馈的图片url',
  `fid` int(11) NOT NULL DEFAULT '0' COMMENT '反馈的id',
  `url` varchar(128) NOT NULL DEFAULT '' COMMENT '图片地址',
  PRIMARY KEY (`id`),
  KEY `fid_url` (`fid`,`url`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_login_log`
--

DROP TABLE IF EXISTS `partner_login_log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_login_log` (
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '代理的uid',
  `login_time` int(11) NOT NULL DEFAULT '0' COMMENT '登录时间',
  `ip` char(15) NOT NULL DEFAULT '' COMMENT '登录ip',
  `login_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '登录方式,2-手机，1-微信',
  PRIMARY KEY (`login_time`,`uid`),
  KEY `uid` (`uid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='代理登录详细日志';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_order`
--

DROP TABLE IF EXISTS `partner_order`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_order` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '支付id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '玩家或代理的id',
  `transaction_id` varchar(32) NOT NULL DEFAULT '' COMMENT '微信支付订单号',
  `order_type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '订单类型, 1-购买钻石,2-购买代理名额',
  `pay_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT ' 支付类型,1-余额支付,2-微信支付,3-支付宝支付',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '房卡数量',
  `give_num` int(11) NOT NULL DEFAULT '0' COMMENT '搞活动，系统赠送的房卡',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '金额，乘以100,展示要除以100',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '1-未支付,2-已关闭,3-支付错误,4-支付成功,未发货,5-已发货',
  `is_first` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否第一次充值,1-是,0-否',
  `remark` varchar(128) NOT NULL DEFAULT '' COMMENT '备注',
  `order_time` int(11) NOT NULL DEFAULT '0' COMMENT '下单时间',
  `pay_time` int(11) NOT NULL DEFAULT '0' COMMENT '支付时间',
  `delivery_time` int(11) NOT NULL DEFAULT '0' COMMENT '发货时间',
  PRIMARY KEY (`id`),
  KEY `user_uid` (`uid`) USING BTREE,
  KEY `order_time` (`order_time`) USING BTREE,
  KEY `transaction_id` (`transaction_id`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `uid` (`uid`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_user`
--

DROP TABLE IF EXISTS `partner_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_user` (
  `uid` int(11) NOT NULL COMMENT 'h5用户id',
  `login_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '1-微信登录,2-手机登录(默认的)',
  `pid` int(11) NOT NULL DEFAULT '0' COMMENT '上级代理的uid, 0-时表示总代',
  `level` tinyint(4) NOT NULL DEFAULT '0' COMMENT '代理等级，1-一级代理,2-二级代理',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '所属合伙人',
  `root_id` int(11) NOT NULL DEFAULT '0' COMMENT '总代的uid,总代如果是自己就填自己的uid',
  `nickname` varchar(32) NOT NULL DEFAULT '' COMMENT '昵称',
  `remark` varchar(32) NOT NULL DEFAULT '' COMMENT '上级代理设置的备注',
  `phone` char(11) NOT NULL DEFAULT '' COMMENT '手机号',
  `password` char(32) NOT NULL DEFAULT '' COMMENT '密码',
  `open_id` varchar(128) NOT NULL DEFAULT '' COMMENT '微信登录的open_id',
  `union_id` varchar(128) NOT NULL DEFAULT '' COMMENT '微信的union_id',
  `sex` tinyint(4) NOT NULL DEFAULT '0' COMMENT '用户的性别，值为1时是男性，值为2时是女性，值为0时是未知',
  `province` varchar(32) NOT NULL DEFAULT '' COMMENT '省份',
  `city` varchar(64) NOT NULL DEFAULT '' COMMENT '城市',
  `head_img_url` varchar(256) NOT NULL DEFAULT '' COMMENT '用户头像',
  `name_url` varchar(128) NOT NULL DEFAULT '' COMMENT '个人名片url',
  `login_url` varchar(128) NOT NULL DEFAULT '' COMMENT '下载登录界面的url',
  `agent_num` smallint(6) NOT NULL DEFAULT '50' COMMENT '一级代理能发展二级代理的名额',
  `is_pass` tinyint(4) NOT NULL DEFAULT '1' COMMENT '是否通过考核,1-是,0-否,默认不管一级还是二级都通过考核',
  `pass_time` int(11) NOT NULL DEFAULT '0' COMMENT '考核时间，用于记录当月是否考核了',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '0-注册成功,2-绑定手机成功,3-人工审核不通过,4-人工审核通过,5-第一次充值成功',
  `is_withdraw` tinyint(4) NOT NULL DEFAULT '1' COMMENT '代理是否可以提现，1-可以提现,0-不能提现',
  `frozen` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否冻结,1-是，0-否,2-注销',
  `frozen_reason` varchar(32) NOT NULL DEFAULT '' COMMENT '冻结理由',
  `agent_time` int(11) NOT NULL DEFAULT '0' COMMENT '成为正式代理的时间,即审核通过且第一次充值成功的时间',
  `login_time` int(11) NOT NULL DEFAULT '0' COMMENT '登录时间，首次注册成功登录时不算',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间，注册成为代理的时间',
  `identity` char(18) NOT NULL DEFAULT '' COMMENT '18位的身份证号码',
  `real_name` varchar(16) NOT NULL DEFAULT '' COMMENT '代理真实名字',
  `bonus_rate` int(11) NOT NULL DEFAULT '0' COMMENT '自己的提成百分比',
  `bonus_rate1` int(11) NOT NULL DEFAULT '0' COMMENT '直属一级的提成比例',
  `bonus_rate2` int(11) NOT NULL DEFAULT '0' COMMENT '直属二级的提成比例',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `open_id` (`open_id`) USING BTREE,
  KEY `pid_status_agent` (`pid`,`status`,`agent_time`) USING BTREE,
  KEY `is_pass` (`is_pass`) USING BTREE,
  KEY `is_frozen` (`frozen`) USING BTREE,
  KEY `group_id` (`group_id`) USING BTREE,
  KEY `phone_password` (`phone`,`password`) USING BTREE,
  KEY `status_level` (`status`,`level`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE,
  KEY `union_id` (`union_id`) USING BTREE,
  KEY `group_status_time` (`group_id`,`status`,`create_time`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `partner_withdraw_record`
--

DROP TABLE IF EXISTS `partner_withdraw_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `partner_withdraw_record` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '代理提现记录id',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '代理id',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '提现金额，存储时要乘以100，展示时要除以100',
  `server_charge` int(11) NOT NULL DEFAULT '0' COMMENT '手续费，金额,展示时要除以100',
  `tax` int(11) NOT NULL DEFAULT '0' COMMENT '税费,金额,展示时要除以100',
  `real_income` int(11) NOT NULL DEFAULT '0' COMMENT '代理实现收入',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '1-审批中,2-审批成功,3-审批失败,4-支付中,5-支付失败,6-支付成功',
  `fail_reason` varchar(64) NOT NULL DEFAULT '' COMMENT '提现失败时填写',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '提现时间',
  `pay_time` int(11) NOT NULL DEFAULT '0' COMMENT '打款到代理银行帐号成功时间',
  PRIMARY KEY (`id`),
  KEY `agent_id` (`agent_id`) USING BTREE,
  KEY `status` (`status`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pb_cash_record`
--

DROP TABLE IF EXISTS `pb_cash_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pb_cash_record` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '代理金额流水账id',
  `uid` int(11) NOT NULL DEFAULT '0' COMMENT '合伙人的uid',
  `agent_id` int(11) NOT NULL DEFAULT '0' COMMENT '合伙人系统的代理uid',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '流水账类型，1-提现退回,2-提现,3-代理提成',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '代理的房卡剩余数量',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '金额，乘以100',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '充值金额,乘以100,展示时除以100',
  `month` smallint(11) NOT NULL DEFAULT '0' COMMENT '按月分帐单1801表示，2018年1月份',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '流水账时间',
  PRIMARY KEY (`id`),
  KEY `agent_type` (`uid`,`type`,`create_time`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE,
  KEY `agent_month` (`agent_id`,`month`,`create_time`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pb_user`
--

DROP TABLE IF EXISTS `pb_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pb_user` (
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `agent_num` int(11) NOT NULL DEFAULT '50' COMMENT '可以发展总代的数量限制,默认50个',
  `name` varchar(64) NOT NULL DEFAULT '' COMMENT 'pc登录用户名',
  `phone` char(11) NOT NULL DEFAULT '' COMMENT '手机号',
  `password` char(32) NOT NULL DEFAULT '' COMMENT '密码',
  `real_name` varchar(12) NOT NULL DEFAULT '' COMMENT '真实名字',
  `name_url` varchar(128) NOT NULL DEFAULT '' COMMENT '招募总代的二维码',
  `frozen` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否冻结，1-是，0-否',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '合伙人的收入',
  `withdraw_time` int(11) NOT NULL DEFAULT '0' COMMENT '提现时间',
  `login_time` int(11) NOT NULL DEFAULT '0' COMMENT '最后登录时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `bonus_rate` int(11) NOT NULL DEFAULT '0' COMMENT '公司提成比例',
  PRIMARY KEY (`uid`),
  KEY `group_id` (`agent_num`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pc_cash_record`
--

DROP TABLE IF EXISTS `pc_cash_record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pc_cash_record` (
  `id` bigint(11) NOT NULL AUTO_INCREMENT COMMENT '代理金额流水账id',
  `group_id` int(11) NOT NULL DEFAULT '0' COMMENT '合伙人id',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '代理的房卡剩余数量',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '金额，乘以100',
  `cash` int(11) NOT NULL DEFAULT '0' COMMENT '充值金额,乘以100,展示时除以100',
  `month` smallint(11) NOT NULL DEFAULT '0' COMMENT '按月分帐单1801表示，2018年1月份',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '流水账时间',
  PRIMARY KEY (`id`),
  KEY `agent_type` (`create_time`) USING BTREE,
  KEY `create_time` (`create_time`) USING BTREE,
  KEY `agent_month` (`group_id`,`month`,`create_time`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Current Database: `chess_sysdata`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `chess_sysdata` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `chess_sysdata`;

--
-- Table structure for table `activity_notice`
--

DROP TABLE IF EXISTS `activity_notice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `activity_notice` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '1-活动公告,2-系统公告',
  `title` varchar(50) NOT NULL DEFAULT '' COMMENT '标题',
  `notice_url` varchar(512) NOT NULL DEFAULT '' COMMENT '公告图片url',
  `detail_url` varchar(512) NOT NULL DEFAULT '' COMMENT '活动详情跳转url',
  `content` varchar(4096) NOT NULL COMMENT '内容',
  `show_now` tinyint(4) NOT NULL DEFAULT '0' COMMENT '登录时弹出的公告, 0-普通,不弹，1-每次登录弹,2-每天首次登录弹',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '此公告的状态(0正常,1下架)',
  `publish_time` int(11) NOT NULL DEFAULT '0' COMMENT '公告时间',
  `expire_time` int(11) NOT NULL DEFAULT '0' COMMENT '过期时间',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '最后修改时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `publish` (`publish_time`) USING BTREE,
  KEY `create` (`create_time`) USING BTREE,
  KEY `expire` (`expire_time`),
  KEY `type` (`type`),
  KEY `status` (`status`),
  KEY `modify_time` (`modify_time`)
) ENGINE=InnoDB AUTO_INCREMENT=72 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `agent_notice`
--

DROP TABLE IF EXISTS `agent_notice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `agent_notice` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '招募代理公告自增id',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `content` varchar(512) NOT NULL DEFAULT '' COMMENT '内容',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '最后修改时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  UNIQUE KEY `game_id` (`game_id`),
  KEY `create` (`create_time`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='每款游戏只有一份代理公告';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `app_version`
--

DROP TABLE IF EXISTS `app_version`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `app_version` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'app版本自增id',
  `type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '1-安卓版本,2-苹果版本',
  `download_url` varchar(128) NOT NULL DEFAULT '' COMMENT '下载地址',
  `change_log` varchar(4096) NOT NULL DEFAULT '' COMMENT '变动说明',
  `version` varchar(16) NOT NULL DEFAULT '' COMMENT '版本号',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '此版本的APP, 1-上架，0-下架',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '更新时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `status` (`status`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 COMMENT='APP版本控制';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `area_game_list`
--

DROP TABLE IF EXISTS `area_game_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `area_game_list` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '区域游戏列表id',
  `city_code` int(6) NOT NULL DEFAULT '0' COMMENT '游戏所属城市',
  `district_code` int(6) NOT NULL DEFAULT '0' COMMENT '游戏所属区或县',
  `position` smallint(4) NOT NULL DEFAULT '9999' COMMENT '游戏排序,1,2,3,4,5,6,7,8........,  9999表示默认排序',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '游戏id,对应的是game_list的game_id',
  PRIMARY KEY (`id`),
  KEY `city_position` (`city_code`,`position`) USING BTREE,
  KEY `game_id` (`game_id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=5431 DEFAULT CHARSET=utf8 COMMENT='区域游戏列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `card_test_cfg`
--

DROP TABLE IF EXISTS `card_test_cfg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `card_test_cfg` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `game_type` int(11) NOT NULL DEFAULT '0' COMMENT '游戏类型',
  `play_type` int(11) NOT NULL DEFAULT '0' COMMENT '游戏玩法',
  `cards` varchar(2048) NOT NULL DEFAULT '' COMMENT '配置牌型',
  `remark` varchar(128) NOT NULL DEFAULT '' COMMENT '备注',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '0-不生效，1-生效',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8 COMMENT='配置游戏牌型';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `diamond_coin`
--

DROP TABLE IF EXISTS `diamond_coin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `diamond_coin` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '钻石兑换金币自增id',
  `diamond` int(11) NOT NULL DEFAULT '0' COMMENT '钻石数',
  `coin` int(11) NOT NULL DEFAULT '0' COMMENT '金币数',
  `title` varchar(16) NOT NULL DEFAULT '' COMMENT '商品标题',
  `sub_title` varchar(16) NOT NULL DEFAULT '' COMMENT '商品副标题',
  PRIMARY KEY (`id`),
  KEY `diamond` (`diamond`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `game_list`
--

DROP TABLE IF EXISTS `game_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `game_list` (
  `game_id` smallint(6) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '1-麻将,2-扑克,3-字牌,4-其他',
  `name` varchar(32) NOT NULL DEFAULT '' COMMENT '游戏名字',
  `game_type` tinyint(11) NOT NULL DEFAULT '0' COMMENT '游戏大类型',
  `play_type` tinyint(11) NOT NULL DEFAULT '0' COMMENT '游戏子类型',
  `package_url` varchar(128) NOT NULL DEFAULT '' COMMENT '游戏包下载地址',
  `version` varchar(32) NOT NULL DEFAULT '' COMMENT '游戏版本',
  `test_version` varchar(32) NOT NULL DEFAULT '' COMMENT '测试版本号',
  `app_version` varchar(32) NOT NULL DEFAULT '1.0.0.1' COMMENT 'app版本(大厅版本)',
  `ios_version` varchar(32) NOT NULL DEFAULT '' COMMENT 'ios大厅版本',
  `logo_url` varchar(128) NOT NULL DEFAULT '' COMMENT 'logo图片url地址',
  `main` varchar(64) NOT NULL DEFAULT '' COMMENT '主类',
  `show` tinyint(4) NOT NULL DEFAULT '1' COMMENT '1-大厅与俱乐部都显示,2-只在俱乐部显示',
  `user_vip` tinyint(4) NOT NULL DEFAULT '0' COMMENT '用户等级，什么用户等级才可以看到此游戏',
  `size` varchar(8) NOT NULL DEFAULT '' COMMENT '游戏包大小',
  `is_publish` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否发布上线了,1-上线，0-未上线',
  `publish_time` int(11) NOT NULL DEFAULT '0' COMMENT '发布时间',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '更新时间',
  `is_delete` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否已经删除，1-是,0-否',
  `is_contest` tinyint(4) NOT NULL DEFAULT '0' COMMENT '此游戏是否可以上下分进行比赛,0-不可上下分, 1-可上下分',
  `is_test` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否为公测, 0-否，1-是, 默认为否',
  PRIMARY KEY (`game_id`),
  UNIQUE KEY `game_play` (`game_type`,`play_type`) USING BTREE,
  KEY `is_publish` (`is_publish`) USING BTREE,
  KEY `user_vip` (`user_vip`) USING BTREE,
  KEY `is_delete_type` (`is_delete`,`type`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='所有的游戏列表';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `game_notice`
--

DROP TABLE IF EXISTS `game_notice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `game_notice` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '子游戏大厅里游戏公告id',
  `game_id` int(11) NOT NULL DEFAULT '0' COMMENT '此公告绑定的游戏id,0-表示所有游戏',
  `title` varchar(50) NOT NULL DEFAULT '' COMMENT '游戏公告标题',
  `content` varchar(1024) NOT NULL DEFAULT '' COMMENT '公告内容',
  `notice_url` varchar(512) NOT NULL DEFAULT '' COMMENT '公告图片url',
  `detail_url` varchar(512) NOT NULL DEFAULT '' COMMENT '活动详情跳转url',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '此公告的状态(0正常,1下架)',
  `publish_time` int(11) NOT NULL DEFAULT '0' COMMENT '公告发布时间',
  `expire_time` int(11) NOT NULL DEFAULT '0' COMMENT '过期时间',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '最后修改时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `game_id` (`game_id`),
  KEY `publish` (`publish_time`),
  KEY `expire` (`expire_time`),
  KEY `create` (`create_time`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='子游戏大厅里的游戏公告';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_card_price`
--

DROP TABLE IF EXISTS `h5_card_price`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_card_price` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '房卡列表id',
  `type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '买房卡的用户类型,1-一级代理买房卡,2-二级代理买房卡,3-玩家买房卡,4-代理赠送房卡,5-合伙人买房卡',
  `num` int(11) NOT NULL DEFAULT '0' COMMENT '购买的房卡数量',
  `give_num` int(11) NOT NULL DEFAULT '0' COMMENT '购买时系统赠送的房卡数',
  PRIMARY KEY (`id`),
  KEY `type` (`type`),
  KEY `num` (`num`)
) ENGINE=InnoDB AUTO_INCREMENT=33 DEFAULT CHARSET=utf8 COMMENT='房卡定价列表,\r\n一级代理考核通过,进货价为一级的，\r\n考核不通过，则是为二级进货价\r\n\r\n二级代理不用考核，一直为二级进货价';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_notice`
--

DROP TABLE IF EXISTS `h5_notice`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_notice` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT 'h5公告',
  `title` varchar(50) NOT NULL DEFAULT '' COMMENT '标题',
  `content` varchar(4096) NOT NULL DEFAULT '' COMMENT '内容',
  `publish_time` int(11) NOT NULL DEFAULT '0' COMMENT '公告发布时间',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '最后修改时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `publish` (`publish_time`),
  KEY `create` (`create_time`),
  KEY `modify_time` (`modify_time`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8 COMMENT='代理助手公告';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_rule`
--

DROP TABLE IF EXISTS `h5_rule`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_rule` (
  `type` int(11) NOT NULL DEFAULT '0' COMMENT '5-一级收入:下级开店提成,6-一级收入:下级进货提成,7-一级收入:下级玩家直充提成,8-一级收入,本级玩家直充,9-二级收入,本级玩家直充,10-手续费2%,11-税费5%,12-最小提现金额,13-一级代理每个月消耗房卡数,14-代理名额,15-玩家绑定手机送房卡数',
  `name` varchar(16) NOT NULL DEFAULT '' COMMENT '类型对应的中文名',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '提成的金额',
  `scale` smallint(4) NOT NULL DEFAULT '0' COMMENT '比例，前端展示要除以此比例',
  `unit` char(1) NOT NULL DEFAULT '' COMMENT '单位',
  PRIMARY KEY (`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='提成提现之类的规则表,  比例有万分比,百分比';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `h5_top_up`
--

DROP TABLE IF EXISTS `h5_top_up`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `h5_top_up` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '代理第一次的金额及房卡',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '第一次充值的金额,存储时乘以100,展示时除以100',
  `card_num` int(11) NOT NULL DEFAULT '0' COMMENT '第一次购买能获得的房卡数',
  `level` tinyint(4) NOT NULL DEFAULT '0' COMMENT '代理等级,1-一级,2-二级, 11-合伙人模式下的总代, 12-合伙人模式下的非总代',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8 COMMENT='代理成为正式代理的第一次充值的配置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `horse`
--

DROP TABLE IF EXISTS `horse`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `horse` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '跑马灯的id',
  `type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '跑马灯类型,1-游戏客户端, 2-代理助手',
  `content` varchar(512) NOT NULL DEFAULT '' COMMENT '跑马灯的内容',
  `publish_time` int(11) NOT NULL DEFAULT '0' COMMENT '发布时间',
  `expire_time` int(11) NOT NULL DEFAULT '0' COMMENT '过期时间',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '修改时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `type` (`type`),
  KEY `publish_expire` (`publish_time`,`expire_time`,`type`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 COMMENT='游戏客户端的及h5的跑马灯';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `itemcfg`
--

DROP TABLE IF EXISTS `itemcfg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `itemcfg` (
  `id` int(11) NOT NULL DEFAULT '0' COMMENT '物品ID',
  `name` varchar(20) NOT NULL DEFAULT '' COMMENT '道具名称',
  `used` tinyint(4) NOT NULL DEFAULT '0' COMMENT '能否使用',
  `script` varchar(40) NOT NULL DEFAULT '' COMMENT '脚本函数名',
  `icon` varchar(40) NOT NULL DEFAULT '' COMMENT '图标',
  `tips` varchar(128) NOT NULL DEFAULT '' COMMENT '道具说明',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `mission`
--

DROP TABLE IF EXISTS `mission`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mission` (
  `msid` int(10) NOT NULL AUTO_INCREMENT COMMENT '任务id',
  `type` int(5) NOT NULL COMMENT '动作类型',
  `status` tinyint(1) NOT NULL COMMENT '0正常1已禁用',
  `auto` tinyint(1) NOT NULL COMMENT '是否自动完成',
  `title` varchar(20) NOT NULL COMMENT '标题',
  `desc` text NOT NULL COMMENT '描述',
  `cate1` int(11) NOT NULL COMMENT '分类1',
  `cate2` int(11) NOT NULL COMMENT '分类2',
  `cate3` int(11) NOT NULL COMMENT '分类3',
  `cate4` int(11) NOT NULL COMMENT '分类4',
  `mtimes` int(11) NOT NULL COMMENT '达到次数',
  `straight` int(11) NOT NULL COMMENT '是否连续',
  `cycle` tinyint(1) NOT NULL DEFAULT '1' COMMENT '周期(1每日2每周3每月)',
  `cycletimes` int(5) NOT NULL DEFAULT '1' COMMENT '可完成次数',
  `icon` varchar(50) NOT NULL COMMENT '图标',
  PRIMARY KEY (`msid`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `missionaction`
--

DROP TABLE IF EXISTS `missionaction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `missionaction` (
  `type` int(4) NOT NULL COMMENT '动作类型',
  `name` varchar(20) NOT NULL COMMENT '动作名称',
  PRIMARY KEY (`type`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `missionprize`
--

DROP TABLE IF EXISTS `missionprize`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `missionprize` (
  `msid` int(10) NOT NULL COMMENT '任务id',
  `poid` int(10) NOT NULL COMMENT '道具id',
  `qty` int(10) NOT NULL COMMENT '数量',
  PRIMARY KEY (`msid`,`poid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `other_conf`
--

DROP TABLE IF EXISTS `other_conf`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `other_conf` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '类型 2-开屏广告,3-客服,4-合作(代理),5-合作(商务)',
  `name` varchar(32) NOT NULL DEFAULT '' COMMENT '名字',
  `value` varchar(512) NOT NULL DEFAULT '' COMMENT '对应的值',
  `url` varchar(256) NOT NULL DEFAULT '' COMMENT '对应的url',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 COMMENT='客户端其它一些零散的配置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `pay_type`
--

DROP TABLE IF EXISTS `pay_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pay_type` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '支付方式自增id',
  `pay_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '支付平台，1-爱贝,2-贝付宝',
  `platform_type` tinyint(4) NOT NULL DEFAULT '0' COMMENT '平台类型,1-pc后台管理系统,2-代理助手系统,3-游戏客户端系统',
  `percent` tinyint(4) NOT NULL DEFAULT '0' COMMENT '百分比, 取值0-100,注意总和是100%，不要超过100%',
  `remark` varchar(32) NOT NULL DEFAULT '' COMMENT '备注',
  PRIMARY KEY (`id`),
  KEY `pay_type` (`pay_type`),
  KEY `type` (`platform_type`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='控制客户端及H5端支付方式/支付比例。';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `platform`
--

DROP TABLE IF EXISTS `platform`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `platform` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '平台服自增id',
  `uuid` varchar(64) NOT NULL DEFAULT '' COMMENT '设置uuid',
  `name` varchar(32) NOT NULL DEFAULT '' COMMENT '服务器名字',
  `type` tinyint(4) NOT NULL DEFAULT '1' COMMENT '服务器类型,1-pc后台管理系统,2-代理助手系统,3-游戏客户端系统',
  `ip` varchar(128) NOT NULL DEFAULT '' COMMENT 'nginx监听的ip(对外展示的ip)',
  `http_port` smallint(6) NOT NULL DEFAULT '0' COMMENT 'nginx监听的端口(对外展示的端口)',
  `admin_port` smallint(6) NOT NULL DEFAULT '0' COMMENT '监控端口',
  `status` tinyint(4) NOT NULL DEFAULT '1' COMMENT '服务器状态, 1-正常,2-停服,3-异常',
  `execute_task` tinyint(4) NOT NULL DEFAULT '0' COMMENT '是否执行定时任务,0-没有，1-执行',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '修改时间',
  `restart_count` smallint(6) NOT NULL DEFAULT '0' COMMENT '重启次数',
  `start_time` int(11) NOT NULL DEFAULT '0' COMMENT '进程启动时间',
  `report_time` int(11) NOT NULL DEFAULT '0' COMMENT '上报时间',
  `load` int(11) NOT NULL DEFAULT '0' COMMENT '每2分钟访问次数(负载)',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8 COMMENT='平台服配置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `roomcardcfg`
--

DROP TABLE IF EXISTS `roomcardcfg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `roomcardcfg` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `game_id` smallint(6) NOT NULL DEFAULT '0' COMMENT '游戏id',
  `game_type` int(11) NOT NULL DEFAULT '0' COMMENT '游戏类型',
  `play_type` int(11) NOT NULL DEFAULT '0' COMMENT '玩法',
  `idx` tinyint(4) NOT NULL DEFAULT '0' COMMENT '序号,1,2,3',
  `round` int(11) NOT NULL DEFAULT '1' COMMENT '局数',
  `card` int(11) NOT NULL DEFAULT '0' COMMENT '房卡数量',
  `card_aa` int(11) NOT NULL DEFAULT '0' COMMENT 'AA支付',
  `remark` varchar(128) NOT NULL DEFAULT '' COMMENT '备注',
  PRIMARY KEY (`id`),
  KEY `gameid` (`game_id`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=120 DEFAULT CHARSET=utf8 COMMENT='房卡收费配置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `roomcfg`
--

DROP TABLE IF EXISTS `roomcfg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `roomcfg` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '序号',
  `gametype` tinyint(2) NOT NULL COMMENT '1斗地主,2梭哈',
  `gamesubtype` int(11) NOT NULL COMMENT '游戏类型',
  `roomid` int(11) NOT NULL COMMENT '房间ID',
  `name` varchar(32) NOT NULL COMMENT '房间名',
  `consume` int(11) NOT NULL DEFAULT '1' COMMENT '消费类型(1积分，2财富币)',
  `entermin` bigint(20) NOT NULL COMMENT '进入门槛',
  `entermax` bigint(20) NOT NULL COMMENT '进入限制',
  `isopen` int(11) NOT NULL COMMENT '是否开放',
  `basescore` int(11) NOT NULL COMMENT '底分',
  `roomtype` int(11) NOT NULL COMMENT '房间类型(0普通房1私人房2比赛房)',
  `tablenum` int(11) NOT NULL DEFAULT '100' COMMENT '桌子数',
  `marry` int(11) NOT NULL DEFAULT '1' COMMENT '匹配方式(0不匹配，1自动匹配）',
  `limitenter` int(11) NOT NULL DEFAULT '1' COMMENT '限制进入',
  `showonline` int(11) NOT NULL DEFAULT '0' COMMENT '显示在线倍数',
  `sitdown` int(11) NOT NULL DEFAULT '0' COMMENT '坐下携带',
  `feetype` int(11) NOT NULL DEFAULT '0' COMMENT '台费类型',
  `fee` int(11) NOT NULL DEFAULT '0' COMMENT '台费值',
  `seatnum` int(11) NOT NULL DEFAULT '0' COMMENT '座位数',
  `showtype` int(11) NOT NULL DEFAULT '0' COMMENT '显示类型',
  `showpic` int(11) NOT NULL DEFAULT '1' COMMENT '显示图片',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `serverinfo`
--

DROP TABLE IF EXISTS `serverinfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `serverinfo` (
  `svrid` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '服务器ID',
  `name` varchar(128) NOT NULL DEFAULT '' COMMENT '服务器名字',
  `group` int(10) NOT NULL DEFAULT '0' COMMENT '服务器组',
  `svr_type` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '服务器类型(1大厅，2游戏服)',
  `game_type` int(10) NOT NULL DEFAULT '0' COMMENT '游戏类型(1斗地主2梭哈3斗牛)',
  `game_subtype` int(10) NOT NULL DEFAULT '1' COMMENT '1普通，2比赛，3组局',
  `play_types` varchar(256) NOT NULL DEFAULT '' COMMENT '开启玩法(默认全部开启)',
  `svrip` varchar(128) NOT NULL DEFAULT '' COMMENT '服务器IP',
  `svrlanip` varchar(128) NOT NULL DEFAULT '0' COMMENT '内网ip',
  `svrport` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '服务器端口',
  `svrlanport` int(10) NOT NULL DEFAULT '0' COMMENT '服务器对服务器端口',
  `phpport` int(10) NOT NULL DEFAULT '0' COMMENT 'web连接端口',
  `repair_state` tinyint(4) NOT NULL DEFAULT '0' COMMENT '维护状态1，正常状态0',
  `state` int(10) unsigned NOT NULL DEFAULT '1' COMMENT '服务器状态,1-正常，0-异常',
  `onlines` int(10) unsigned NOT NULL DEFAULT '0' COMMENT '在线人数',
  `report_time` int(11) NOT NULL DEFAULT '0' COMMENT '上报时间',
  `start_count` smallint(6) DEFAULT '0' COMMENT '启动次数',
  `start_time` int(11) DEFAULT '0' COMMENT '启动时间',
  PRIMARY KEY (`svrid`),
  KEY `group` (`group`) USING BTREE,
  KEY `svr_type` (`svr_type`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='大厅游戏服配置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `store`
--

DROP TABLE IF EXISTS `store`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `store` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '商城商品自增id',
  `product_id` int(11) NOT NULL DEFAULT '0' COMMENT '对应苹果服务器配置的商品id',
  `num` int(11) NOT NULL DEFAULT '0' COMMENT '购买的数量',
  `amount` int(11) NOT NULL DEFAULT '0' COMMENT '价格,存储时要X100， 展示时要除以100',
  `status` tinyint(4) NOT NULL DEFAULT '0' COMMENT '0-正常的价格，1-是特惠',
  PRIMARY KEY (`id`),
  KEY `num` (`num`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8 COMMENT='商城商品配置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `sysconfig`
--

DROP TABLE IF EXISTS `sysconfig`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sysconfig` (
  `id` int(11) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `sckey` varchar(20) NOT NULL COMMENT 'key',
  `value` text NOT NULL COMMENT '值',
  `mean` varchar(128) NOT NULL DEFAULT '' COMMENT '意义',
  `value_type` varchar(20) NOT NULL DEFAULT '' COMMENT 'string,int,json,json[]',
  `modify_time` int(11) NOT NULL DEFAULT '0' COMMENT '修改时间',
  `create_time` int(11) NOT NULL DEFAULT '0' COMMENT '创建时间',
  PRIMARY KEY (`id`),
  KEY `key` (`sckey`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT COMMENT='系统配置';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Current Database: `mysql`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `mysql` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `mysql`;

--
-- Table structure for table `columns_priv`
--

DROP TABLE IF EXISTS `columns_priv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `columns_priv` (
  `Host` char(60) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Db` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `User` char(16) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Table_name` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Column_name` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `Column_priv` set('Select','Insert','Update','References') CHARACTER SET utf8 NOT NULL DEFAULT '',
  PRIMARY KEY (`Host`,`Db`,`User`,`Table_name`,`Column_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='Column privileges';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `db`
--

DROP TABLE IF EXISTS `db`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `db` (
  `Host` char(60) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Db` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `User` char(16) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Select_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Insert_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Update_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Delete_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Drop_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Grant_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `References_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Index_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Alter_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_tmp_table_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Lock_tables_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_view_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Show_view_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_routine_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Alter_routine_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Execute_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Event_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Trigger_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  PRIMARY KEY (`Host`,`Db`,`User`),
  KEY `User` (`User`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='Database privileges';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `event`
--

DROP TABLE IF EXISTS `event`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `event` (
  `db` char(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `name` char(64) NOT NULL DEFAULT '',
  `body` longblob NOT NULL,
  `definer` char(77) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `execute_at` datetime DEFAULT NULL,
  `interval_value` int(11) DEFAULT NULL,
  `interval_field` enum('YEAR','QUARTER','MONTH','DAY','HOUR','MINUTE','WEEK','SECOND','MICROSECOND','YEAR_MONTH','DAY_HOUR','DAY_MINUTE','DAY_SECOND','HOUR_MINUTE','HOUR_SECOND','MINUTE_SECOND','DAY_MICROSECOND','HOUR_MICROSECOND','MINUTE_MICROSECOND','SECOND_MICROSECOND') DEFAULT NULL,
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `modified` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `last_executed` datetime DEFAULT NULL,
  `starts` datetime DEFAULT NULL,
  `ends` datetime DEFAULT NULL,
  `status` enum('ENABLED','DISABLED','SLAVESIDE_DISABLED') NOT NULL DEFAULT 'ENABLED',
  `on_completion` enum('DROP','PRESERVE') NOT NULL DEFAULT 'DROP',
  `sql_mode` set('REAL_AS_FLOAT','PIPES_AS_CONCAT','ANSI_QUOTES','IGNORE_SPACE','NOT_USED','ONLY_FULL_GROUP_BY','NO_UNSIGNED_SUBTRACTION','NO_DIR_IN_CREATE','POSTGRESQL','ORACLE','MSSQL','DB2','MAXDB','NO_KEY_OPTIONS','NO_TABLE_OPTIONS','NO_FIELD_OPTIONS','MYSQL323','MYSQL40','ANSI','NO_AUTO_VALUE_ON_ZERO','NO_BACKSLASH_ESCAPES','STRICT_TRANS_TABLES','STRICT_ALL_TABLES','NO_ZERO_IN_DATE','NO_ZERO_DATE','INVALID_DATES','ERROR_FOR_DIVISION_BY_ZERO','TRADITIONAL','NO_AUTO_CREATE_USER','HIGH_NOT_PRECEDENCE','NO_ENGINE_SUBSTITUTION','PAD_CHAR_TO_FULL_LENGTH') NOT NULL DEFAULT '',
  `comment` char(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `originator` int(10) unsigned NOT NULL,
  `time_zone` char(64) CHARACTER SET latin1 NOT NULL DEFAULT 'SYSTEM',
  `character_set_client` char(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `collation_connection` char(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `db_collation` char(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `body_utf8` longblob,
  PRIMARY KEY (`db`,`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Events';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `func`
--

DROP TABLE IF EXISTS `func`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `func` (
  `name` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `ret` tinyint(1) NOT NULL DEFAULT '0',
  `dl` char(128) COLLATE utf8_bin NOT NULL DEFAULT '',
  `type` enum('function','aggregate') CHARACTER SET utf8 NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='User defined functions';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `help_category`
--

DROP TABLE IF EXISTS `help_category`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `help_category` (
  `help_category_id` smallint(5) unsigned NOT NULL,
  `name` char(64) NOT NULL,
  `parent_category_id` smallint(5) unsigned DEFAULT NULL,
  `url` text NOT NULL,
  PRIMARY KEY (`help_category_id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='help categories';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `help_keyword`
--

DROP TABLE IF EXISTS `help_keyword`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `help_keyword` (
  `help_keyword_id` int(10) unsigned NOT NULL,
  `name` char(64) NOT NULL,
  PRIMARY KEY (`help_keyword_id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='help keywords';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `help_relation`
--

DROP TABLE IF EXISTS `help_relation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `help_relation` (
  `help_topic_id` int(10) unsigned NOT NULL,
  `help_keyword_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`help_keyword_id`,`help_topic_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='keyword-topic relation';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `help_topic`
--

DROP TABLE IF EXISTS `help_topic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `help_topic` (
  `help_topic_id` int(10) unsigned NOT NULL,
  `name` char(64) NOT NULL,
  `help_category_id` smallint(5) unsigned NOT NULL,
  `description` text NOT NULL,
  `example` text NOT NULL,
  `url` text NOT NULL,
  PRIMARY KEY (`help_topic_id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='help topics';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `host`
--

DROP TABLE IF EXISTS `host`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `host` (
  `Host` char(60) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Db` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Select_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Insert_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Update_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Delete_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Drop_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Grant_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `References_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Index_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Alter_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_tmp_table_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Lock_tables_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_view_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Show_view_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_routine_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Alter_routine_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Execute_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Trigger_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  PRIMARY KEY (`Host`,`Db`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='Host privileges;  Merged with database privileges';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `ndb_binlog_index`
--

DROP TABLE IF EXISTS `ndb_binlog_index`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ndb_binlog_index` (
  `Position` bigint(20) unsigned NOT NULL,
  `File` varchar(255) NOT NULL,
  `epoch` bigint(20) unsigned NOT NULL,
  `inserts` bigint(20) unsigned NOT NULL,
  `updates` bigint(20) unsigned NOT NULL,
  `deletes` bigint(20) unsigned NOT NULL,
  `schemaops` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`epoch`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `plugin`
--

DROP TABLE IF EXISTS `plugin`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `plugin` (
  `name` varchar(64) NOT NULL DEFAULT '',
  `dl` varchar(128) NOT NULL DEFAULT '',
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='MySQL plugins';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `proc`
--

DROP TABLE IF EXISTS `proc`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `proc` (
  `db` char(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `name` char(64) NOT NULL DEFAULT '',
  `type` enum('FUNCTION','PROCEDURE') NOT NULL,
  `specific_name` char(64) NOT NULL DEFAULT '',
  `language` enum('SQL') NOT NULL DEFAULT 'SQL',
  `sql_data_access` enum('CONTAINS_SQL','NO_SQL','READS_SQL_DATA','MODIFIES_SQL_DATA') NOT NULL DEFAULT 'CONTAINS_SQL',
  `is_deterministic` enum('YES','NO') NOT NULL DEFAULT 'NO',
  `security_type` enum('INVOKER','DEFINER') NOT NULL DEFAULT 'DEFINER',
  `param_list` blob NOT NULL,
  `returns` longblob NOT NULL,
  `body` longblob NOT NULL,
  `definer` char(77) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL DEFAULT '',
  `created` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `modified` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `sql_mode` set('REAL_AS_FLOAT','PIPES_AS_CONCAT','ANSI_QUOTES','IGNORE_SPACE','NOT_USED','ONLY_FULL_GROUP_BY','NO_UNSIGNED_SUBTRACTION','NO_DIR_IN_CREATE','POSTGRESQL','ORACLE','MSSQL','DB2','MAXDB','NO_KEY_OPTIONS','NO_TABLE_OPTIONS','NO_FIELD_OPTIONS','MYSQL323','MYSQL40','ANSI','NO_AUTO_VALUE_ON_ZERO','NO_BACKSLASH_ESCAPES','STRICT_TRANS_TABLES','STRICT_ALL_TABLES','NO_ZERO_IN_DATE','NO_ZERO_DATE','INVALID_DATES','ERROR_FOR_DIVISION_BY_ZERO','TRADITIONAL','NO_AUTO_CREATE_USER','HIGH_NOT_PRECEDENCE','NO_ENGINE_SUBSTITUTION','PAD_CHAR_TO_FULL_LENGTH') NOT NULL DEFAULT '',
  `comment` text CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `character_set_client` char(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `collation_connection` char(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `db_collation` char(32) CHARACTER SET utf8 COLLATE utf8_bin DEFAULT NULL,
  `body_utf8` longblob,
  PRIMARY KEY (`db`,`name`,`type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Stored Procedures';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `procs_priv`
--

DROP TABLE IF EXISTS `procs_priv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `procs_priv` (
  `Host` char(60) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Db` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `User` char(16) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Routine_name` char(64) CHARACTER SET utf8 NOT NULL DEFAULT '',
  `Routine_type` enum('FUNCTION','PROCEDURE') COLLATE utf8_bin NOT NULL,
  `Grantor` char(77) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Proc_priv` set('Execute','Alter Routine','Grant') CHARACTER SET utf8 NOT NULL DEFAULT '',
  `Timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`Host`,`Db`,`User`,`Routine_name`,`Routine_type`),
  KEY `Grantor` (`Grantor`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='Procedure privileges';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `proxies_priv`
--

DROP TABLE IF EXISTS `proxies_priv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `proxies_priv` (
  `Host` char(60) COLLATE utf8_bin NOT NULL DEFAULT '',
  `User` char(16) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Proxied_host` char(60) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Proxied_user` char(16) COLLATE utf8_bin NOT NULL DEFAULT '',
  `With_grant` tinyint(1) NOT NULL DEFAULT '0',
  `Grantor` char(77) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`Host`,`User`,`Proxied_host`,`Proxied_user`),
  KEY `Grantor` (`Grantor`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='User proxy privileges';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `servers`
--

DROP TABLE IF EXISTS `servers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `servers` (
  `Server_name` char(64) NOT NULL DEFAULT '',
  `Host` char(64) NOT NULL DEFAULT '',
  `Db` char(64) NOT NULL DEFAULT '',
  `Username` char(64) NOT NULL DEFAULT '',
  `Password` char(64) NOT NULL DEFAULT '',
  `Port` int(4) NOT NULL DEFAULT '0',
  `Socket` char(64) NOT NULL DEFAULT '',
  `Wrapper` char(64) NOT NULL DEFAULT '',
  `Owner` char(64) NOT NULL DEFAULT '',
  PRIMARY KEY (`Server_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='MySQL Foreign Servers table';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `tables_priv`
--

DROP TABLE IF EXISTS `tables_priv`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tables_priv` (
  `Host` char(60) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Db` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `User` char(16) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Table_name` char(64) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Grantor` char(77) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `Table_priv` set('Select','Insert','Update','Delete','Create','Drop','Grant','References','Index','Alter','Create View','Show view','Trigger') CHARACTER SET utf8 NOT NULL DEFAULT '',
  `Column_priv` set('Select','Insert','Update','References') CHARACTER SET utf8 NOT NULL DEFAULT '',
  PRIMARY KEY (`Host`,`Db`,`User`,`Table_name`),
  KEY `Grantor` (`Grantor`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='Table privileges';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `time_zone`
--

DROP TABLE IF EXISTS `time_zone`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `time_zone` (
  `Time_zone_id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Use_leap_seconds` enum('Y','N') NOT NULL DEFAULT 'N',
  PRIMARY KEY (`Time_zone_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Time zones';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `time_zone_leap_second`
--

DROP TABLE IF EXISTS `time_zone_leap_second`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `time_zone_leap_second` (
  `Transition_time` bigint(20) NOT NULL,
  `Correction` int(11) NOT NULL,
  PRIMARY KEY (`Transition_time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Leap seconds information for time zones';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `time_zone_name`
--

DROP TABLE IF EXISTS `time_zone_name`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `time_zone_name` (
  `Name` char(64) NOT NULL,
  `Time_zone_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Time zone names';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `time_zone_transition`
--

DROP TABLE IF EXISTS `time_zone_transition`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `time_zone_transition` (
  `Time_zone_id` int(10) unsigned NOT NULL,
  `Transition_time` bigint(20) NOT NULL,
  `Transition_type_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`Time_zone_id`,`Transition_time`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Time zone transitions';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `time_zone_transition_type`
--

DROP TABLE IF EXISTS `time_zone_transition_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `time_zone_transition_type` (
  `Time_zone_id` int(10) unsigned NOT NULL,
  `Transition_type_id` int(10) unsigned NOT NULL,
  `Offset` int(11) NOT NULL DEFAULT '0',
  `Is_DST` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Abbreviation` char(8) NOT NULL DEFAULT '',
  PRIMARY KEY (`Time_zone_id`,`Transition_type_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='Time zone transition types';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `Host` char(60) COLLATE utf8_bin NOT NULL DEFAULT '',
  `User` char(16) COLLATE utf8_bin NOT NULL DEFAULT '',
  `Password` char(41) CHARACTER SET latin1 COLLATE latin1_bin NOT NULL DEFAULT '',
  `Select_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Insert_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Update_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Delete_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Drop_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Reload_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Shutdown_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Process_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `File_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Grant_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `References_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Index_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Alter_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Show_db_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Super_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_tmp_table_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Lock_tables_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Execute_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Repl_slave_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Repl_client_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_view_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Show_view_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_routine_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Alter_routine_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_user_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Event_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Trigger_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `Create_tablespace_priv` enum('N','Y') CHARACTER SET utf8 NOT NULL DEFAULT 'N',
  `ssl_type` enum('','ANY','X509','SPECIFIED') CHARACTER SET utf8 NOT NULL DEFAULT '',
  `ssl_cipher` blob NOT NULL,
  `x509_issuer` blob NOT NULL,
  `x509_subject` blob NOT NULL,
  `max_questions` int(11) unsigned NOT NULL DEFAULT '0',
  `max_updates` int(11) unsigned NOT NULL DEFAULT '0',
  `max_connections` int(11) unsigned NOT NULL DEFAULT '0',
  `max_user_connections` int(11) unsigned NOT NULL DEFAULT '0',
  `plugin` char(64) COLLATE utf8_bin DEFAULT '',
  `authentication_string` text COLLATE utf8_bin,
  PRIMARY KEY (`Host`,`User`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin COMMENT='Users and global privileges';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `general_log`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `general_log` (
  `event_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `user_host` mediumtext NOT NULL,
  `thread_id` int(11) NOT NULL,
  `server_id` int(10) unsigned NOT NULL,
  `command_type` varchar(64) NOT NULL,
  `argument` mediumtext NOT NULL
) ENGINE=CSV DEFAULT CHARSET=utf8 COMMENT='General log';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `slow_log`
--

/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE IF NOT EXISTS `slow_log` (
  `start_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `user_host` mediumtext NOT NULL,
  `query_time` time NOT NULL,
  `lock_time` time NOT NULL,
  `rows_sent` int(11) NOT NULL,
  `rows_examined` int(11) NOT NULL,
  `db` varchar(512) NOT NULL,
  `last_insert_id` int(11) NOT NULL,
  `insert_id` int(11) NOT NULL,
  `server_id` int(10) unsigned NOT NULL,
  `sql_text` mediumtext NOT NULL
) ENGINE=CSV DEFAULT CHARSET=utf8 COMMENT='Slow log';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Current Database: `test`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `test` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `test`;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user` (
  `id` int(11) NOT NULL DEFAULT '0',
  `name` char(20) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_1`
--

DROP TABLE IF EXISTS `user_1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_1` (
  `id` int(11) NOT NULL DEFAULT '0',
  `name` char(20) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_2`
--

DROP TABLE IF EXISTS `user_2`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_2` (
  `id` int(11) NOT NULL DEFAULT '0',
  `name` char(20) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_bak`
--

DROP TABLE IF EXISTS `user_bak`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_bak` (
  `id` int(11) NOT NULL DEFAULT '0',
  `name` char(20) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-10-09 14:26:23
