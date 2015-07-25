-- MySQL dump 10.13  Distrib 5.6.25, for Win64 (x86_64)
--
-- Host: localhost    Database: eGameserver
-- ------------------------------------------------------
-- Server version	5.6.25-log

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
-- Table structure for table `shop_info`
--

DROP TABLE IF EXISTS `shop_info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `shop_info` (
  `guid` char(40) NOT NULL DEFAULT '{00000000-0000-0000-0000-000000000000}',
  `name` varchar(128) NOT NULL DEFAULT '',
  `desc` varchar(256) NOT NULL DEFAULT '',
  `price` decimal(10,2) DEFAULT '0.00',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `shop_info`
--

LOCK TABLES `shop_info` WRITE;
/*!40000 ALTER TABLE `shop_info` DISABLE KEYS */;
INSERT INTO `shop_info` VALUES ('{10CB01D2-2842-11E5-93FA-74D4355E1191}','test_goods_name_2','test_goods_desc2:this is the desc of goods',0.75),('{3022150E-2842-11E5-93FA-74D4355E1191}','test_goods_name_3','test_goods_desc3:this is the desc of goods',24.00),('{51BF8170-2842-11E5-93FA-74D4355E1191}','九转还魂丹','可以为你延续一次生命，及其宝贵，慎用，土豪随意！！！',99.00),('{6DE24D23-2841-11E5-93FA-74D4355E1191}','test_goods_name_1','test_goods_desc1:this is the desc of goods',5.90),('{B4EA86AE-2842-11E5-93FA-74D4355E1191}','女王的浴衣','送给女神的情趣内衣，女神穿上香香哒',39.00);
/*!40000 ALTER TABLE `shop_info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_account`
--

DROP TABLE IF EXISTS `user_account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_account` (
  `guid` char(40) NOT NULL DEFAULT '{00000000-0000-0000-0000-000000000000}',
  `account` varchar(128) NOT NULL DEFAULT '',
  `password` varchar(128) NOT NULL DEFAULT '',
  `name` varchar(128) NOT NULL DEFAULT '',
  `alias` varchar(128) NOT NULL DEFAULT '',
  `last_login_datetime` datetime DEFAULT '0000-00-00 00:00:00',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_account`
--

LOCK TABLES `user_account` WRITE;
/*!40000 ALTER TABLE `user_account` DISABLE KEYS */;
INSERT INTO `user_account` VALUES ('{53A7B5D3-811A-4C0B-8D6C-80A34A1C684E}','test2','test2','kafdjkldsjfkjaksljfkl','dfjakfjkadsjfkdsfdsfdsfds','2015-07-12 19:18:25'),('{CCBA08E0-93A3-44CB-A869-8BC82FAC55D0}','test','test','testUSer','test','2015-07-12 11:49:05'),('{ED122C40-F7E7-4F71-B8F4-C1E6803BC62C}','test1','test1','testUSer111111','test1','2015-07-12 20:31:19');
/*!40000 ALTER TABLE `user_account` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_bag`
--

DROP TABLE IF EXISTS `user_bag`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_bag` (
  `user_guid` char(40) NOT NULL DEFAULT '{00000000-0000-0000-0000-000000000000}',
  `goods_guid` char(40) NOT NULL DEFAULT '{00000000-0000-0000-0000-000000000000}',
  `count` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_bag`
--

LOCK TABLES `user_bag` WRITE;
/*!40000 ALTER TABLE `user_bag` DISABLE KEYS */;
INSERT INTO `user_bag` VALUES ('{00000000-0000-0000-0000-000000000000}','{B4EA86AE-2842-11E5-93FA-74D4355E1191}',3),('{ED122C40-F7E7-4F71-B8F4-C1E6803BC62C}','{B4EA86AE-2842-11E5-93FA-74D4355E1191}',7),('{ED122C40-F7E7-4F71-B8F4-C1E6803BC62C}','{51BF8170-2842-11E5-93FA-74D4355E1191}',2),('{ED122C40-F7E7-4F71-B8F4-C1E6803BC62C}','{6DE24D23-2841-11E5-93FA-74D4355E1191}',1);
/*!40000 ALTER TABLE `user_bag` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_property`
--

DROP TABLE IF EXISTS `user_property`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_property` (
  `user_guid` char(40) NOT NULL DEFAULT '{00000000-0000-0000-0000-000000000000}',
  `level` int(11) NOT NULL DEFAULT '0',
  `bag_type` int(11) NOT NULL DEFAULT '0',
  `money` decimal(10,2) DEFAULT '0.00',
  `coordinate_x` int(11) NOT NULL DEFAULT '0',
  `coordinate_y` int(11) NOT NULL DEFAULT '0',
  `map_index` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`user_guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_property`
--

LOCK TABLES `user_property` WRITE;
/*!40000 ALTER TABLE `user_property` DISABLE KEYS */;
INSERT INTO `user_property` VALUES ('{53A7B5D3-811A-4C0B-8D6C-80A34A1C684E}',15,3,2600.00,27,26,5),('{ED122C40-F7E7-4F71-B8F4-C1E6803BC62C}',50,24,699.87,6,7,4);
/*!40000 ALTER TABLE `user_property` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-07-12 21:03:58
