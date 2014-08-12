-- MySQL dump 10.13  Distrib 5.5.10, for Linux (x86_64)
--
-- Host: localhost    Database: dawn
-- ------------------------------------------------------
-- Server version	5.5.10-log

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
-- Current Database: `dawn`
--

/*!40000 DROP DATABASE IF EXISTS `dawn`*/;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `dawn` /*!40100 DEFAULT CHARACTER SET latin1 */;

USE `dawn`;

--
-- Table structure for table `errors`
--

DROP TABLE IF EXISTS `errors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `errors` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `serverid` smallint(5) unsigned DEFAULT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `errno` smallint(6) DEFAULT NULL,
  `error` text,
  `packet` text,
  `additionaldesc` text,
  `command` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2498 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `errors_backup`
--

DROP TABLE IF EXISTS `errors_backup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `errors_backup` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `serverid` smallint(5) unsigned DEFAULT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `errno` smallint(6) DEFAULT NULL,
  `error` text,
  `packet` text,
  `additionaldesc` text,
  `command` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2172 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `games`
--

DROP TABLE IF EXISTS `games`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `games` (
  `id` tinyint(3) unsigned NOT NULL AUTO_INCREMENT,
  `short` varchar(8) NOT NULL,
  `name` varchar(32) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `players`
--

DROP TABLE IF EXISTS `players`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `players` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ip` int(10) unsigned DEFAULT NULL,
  `pbguid` varchar(32) NOT NULL,
  `etproguid` varchar(40) DEFAULT NULL,
  `nick` varchar(35) NOT NULL,
  `colorless` varchar(35) NOT NULL,
  `os` enum('windows','linux','unkown') DEFAULT 'windows',
  `slacid` int(10) unsigned NOT NULL DEFAULT '0',
  `slacnick` varchar(16) NOT NULL DEFAULT '',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=416971 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `privileges`
--

DROP TABLE IF EXISTS `privileges`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `privileges` (
  `id` mediumint(8) unsigned NOT NULL AUTO_INCREMENT,
  `flagserver` tinyint(1) NOT NULL DEFAULT '0',
  `flagplayerentry` tinyint(1) NOT NULL DEFAULT '0',
  `deleteserver` tinyint(1) NOT NULL DEFAULT '0',
  `deleteplayerentry` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `servers`
--

DROP TABLE IF EXISTS `servers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `servers` (
  `id` smallint(5) unsigned NOT NULL AUTO_INCREMENT,
  `userid` mediumint(8) unsigned NOT NULL,
  `gameid` tinyint(3) unsigned NOT NULL,
  `nextcheck` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `lastonline` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `ip` int(10) unsigned NOT NULL,
  `port` int(11) NOT NULL,
  `password` varchar(128) NOT NULL,
  `name` varchar(50) NOT NULL DEFAULT '',
  `passfails` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `connectionfails` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `spoofed` varchar(256) DEFAULT NULL,
  `added_at` datetime NOT NULL,
  `added_from_ip` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=71 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stats_players_alltime_byid`
--

DROP TABLE IF EXISTS `stats_players_alltime_byid`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stats_players_alltime_byid` (
  `playerid` int(10) unsigned NOT NULL,
  `minutesplayed` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`playerid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stats_players_alltime_bypb`
--

DROP TABLE IF EXISTS `stats_players_alltime_bypb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stats_players_alltime_bypb` (
  `pbguid` char(32) NOT NULL DEFAULT '',
  `minutesplayed` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`pbguid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stats_players_alltime_byslac`
--

DROP TABLE IF EXISTS `stats_players_alltime_byslac`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stats_players_alltime_byslac` (
  `slacid` int(10) unsigned NOT NULL DEFAULT '0',
  `minutesplayed` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`slacid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `stats_servers_alltime`
--

DROP TABLE IF EXISTS `stats_servers_alltime`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `stats_servers_alltime` (
  `serverid` int(10) unsigned NOT NULL,
  `minutesplayed` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`serverid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `times`
--

DROP TABLE IF EXISTS `times`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `times` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `serverid` smallint(5) unsigned NOT NULL,
  `playerid` int(10) unsigned DEFAULT NULL,
  `playedfrom` datetime NOT NULL,
  `playedto` datetime NOT NULL,
  `spoofed` varchar(256) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=833124 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `times_monthly`
--

DROP TABLE IF EXISTS `times_monthly`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `times_monthly` (
  `serverid` smallint(5) unsigned NOT NULL,
  `playerid` int(10) unsigned NOT NULL,
  `month` smallint(5) unsigned NOT NULL,
  `year` smallint(5) unsigned NOT NULL,
  `minutesplayed` bigint(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `times_monthly_pb`
--

DROP TABLE IF EXISTS `times_monthly_pb`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `times_monthly_pb` (
  `serverid` smallint(5) unsigned NOT NULL,
  `pbguid` char(32) DEFAULT NULL,
  `month` smallint(5) unsigned NOT NULL,
  `year` smallint(5) unsigned NOT NULL,
  `minutesplayed` bigint(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `times_monthly_slac`
--

DROP TABLE IF EXISTS `times_monthly_slac`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `times_monthly_slac` (
  `serverid` smallint(5) unsigned NOT NULL,
  `slacid` int(10) unsigned DEFAULT NULL,
  `month` smallint(5) unsigned NOT NULL,
  `year` smallint(5) unsigned NOT NULL,
  `minutesplayed` bigint(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `userid` mediumint(8) unsigned NOT NULL AUTO_INCREMENT,
  `crossfireid` bigint(20) unsigned DEFAULT NULL,
  `username` varchar(32) DEFAULT NULL,
  `password` varchar(32) DEFAULT NULL,
  `date_registered` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `added_from_ip` int(10) unsigned DEFAULT NULL,
  `last_query` int(10) unsigned DEFAULT '0',
  PRIMARY KEY (`userid`),
  UNIQUE KEY `crossfireid` (`crossfireid`)
) ENGINE=MyISAM AUTO_INCREMENT=36783 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `yaet_players`
--

DROP TABLE IF EXISTS `yaet_players`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yaet_players` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `pbguid` varchar(32) NOT NULL,
  `etproguid` varchar(40) NOT NULL,
  `nick` varchar(35) NOT NULL,
  `colorless` varchar(35) NOT NULL,
  `slacid` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `pbguid` (`pbguid`,`etproguid`,`nick`,`slacid`)
) ENGINE=InnoDB AUTO_INCREMENT=150 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `yaet_servers`
--

DROP TABLE IF EXISTS `yaet_servers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yaet_servers` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `gameid` tinyint(4) NOT NULL,
  `ip` int(10) unsigned NOT NULL,
  `port` int(10) unsigned NOT NULL,
  `name` varchar(64) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `gameid` (`gameid`,`ip`,`port`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `yaet_times`
--

DROP TABLE IF EXISTS `yaet_times`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yaet_times` (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `isspotter` tinyint(3) unsigned DEFAULT '0',
  `serverid` int(10) unsigned NOT NULL,
  `playerid` int(10) unsigned NOT NULL,
  `spotterid` mediumint(8) NOT NULL,
  `playedfrom` datetime NOT NULL,
  `playedto` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=55 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `yaet_users`
--

DROP TABLE IF EXISTS `yaet_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yaet_users` (
  `userid` mediumint(8) unsigned NOT NULL AUTO_INCREMENT,
  `last_status` enum('offline','onlineNotPlaying','onlineWithET') NOT NULL DEFAULT 'offline',
  `last_seen` datetime DEFAULT NULL,
  `last_etplayerid` smallint(5) unsigned DEFAULT NULL,
  `last_etplayernick` varchar(32) NOT NULL,
  `last_serverid` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB AUTO_INCREMENT=13944 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `yaet_users_history`
--

DROP TABLE IF EXISTS `yaet_users_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `yaet_users_history` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `userid` mediumint(8) unsigned NOT NULL,
  `serverid` int(10) unsigned NOT NULL,
  `playedfrom` datetime NOT NULL,
  `playedto` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping routines for database 'dawn'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2011-08-06 14:28:47
USE dawn;
INSERT INTO users (userid,crossfireid,username,password,date_registered,added_from_ip) VALUES (13943,13943,'trakos','***REMOVED***','2010-08-19 23:04:33',NULL);
INSERT INTO servers (userid,gameid,ip,port,password) VALUES (13943,1,3585748629,27960,'***REMOVED***');
CREATE USER 'dawn'@'localhost' identified by '***REMOVED***';
GRANT ALL ON dawn.* TO 'dawn'@'localhost';
