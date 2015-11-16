CREATE DATABASE  IF NOT EXISTS `project3-nudb` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `project3-nudb`;
-- MySQL dump 10.13  Distrib 5.6.24, for Win64 (x86_64)
--
-- Host: localhost    Database: project3-nudb
-- ------------------------------------------------------
-- Server version	5.6.26-log

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
-- Table structure for table `classroom`
--

DROP TABLE IF EXISTS `classroom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `classroom` (
  `ClassroomId` varchar(6) NOT NULL DEFAULT '',
  `Seats` int(11) NOT NULL,
  `Type` varchar(7) DEFAULT NULL,
  PRIMARY KEY (`ClassroomId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `classroom`
--

LOCK TABLES `classroom` WRITE;
/*!40000 ALTER TABLE `classroom` DISABLE KEYS */;
INSERT INTO `classroom` VALUES ('CAR157',290,'tiered'),('CAR159',290,'tiered'),('CAR173',127,'tiered'),('CAR175',160,'tiered'),('CAR273',160,'tiered'),('CAR275',160,'tiered'),('CAR373',160,'tiered'),('CAR375',160,'tiered'),('CheLT1',300,'tiered'),('CheLT2',145,'tiered'),('CheLT3',300,'tiered'),('CheLT4',145,'tiered'),('EA403',40,'flat'),('EA404',40,'flat'),('EA405',40,'flat'),('EA406',40,'flat'),('EAA',500,'sloping'),('EALT',200,'sloping'),('MechLT',100,'tiered'),('SITLT',50,'sloping');
/*!40000 ALTER TABLE `classroom` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `faculty`
--

DROP TABLE IF EXISTS `faculty`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `faculty` (
  `Id` char(9) NOT NULL DEFAULT '',
  `Name` varchar(20) NOT NULL,
  `DeptId` char(3) NOT NULL,
  `Password` varchar(10) NOT NULL,
  `Address` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `faculty`
--

LOCK TABLES `faculty` WRITE;
/*!40000 ALTER TABLE `faculty` DISABLE KEYS */;
INSERT INTO `faculty` VALUES ('0123456','Peter Eades','SIT','graphs','Carlton'),('0123459','Tasos Viglas','SIT','algos','Rhodes'),('0987654','Simon Poon','SIT','pony','Sydney'),('1122334','Irena Koprinska','SIT','volleyball','Glebe'),('1234567','Jon Patrick','SIT','english','Glebe'),('1237890','Joseph Davis','SIT','abcd',NULL),('2345678','Zhiyong Wang','SIT','stuff','Maroubra'),('3456789','Geoffrey Kennedy','SIT','projects','Cronulla'),('4567890','Vera Chung','SIT','time','Ryde'),('4657890','Alan Fekete','SIT','opera','Cammeray'),('4856787','Joachim Gudmundsson','SIT','squares','Nicta'),('5678901','Michael Charleston','SIT','dollars','Rose Bay'),('6339103','Uwe Roehm','SIT','sailing','Cremorne'),('6789012','Kalina Yacef','SIT','elearn','Newtown'),('7123459','Bernhard Scholz','SIT','plang','Ultimo'),('7890123','Bing Bing Zhou','SIT','hpc','Chatswood'),('7891234','Sanjay Chawla','SIT','cricket','Neutral Bay'),('8901234','Bob Kummerfeld','SIT','gigs','Kings Cross'),('9012345','Tom Chai','SIT','bio','Redfern');
/*!40000 ALTER TABLE `faculty` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `lecture`
--

DROP TABLE IF EXISTS `lecture`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lecture` (
  `UoSCode` char(8) NOT NULL DEFAULT '',
  `Semester` char(2) NOT NULL DEFAULT '',
  `Year` int(11) NOT NULL DEFAULT '0',
  `ClassTime` varchar(5) DEFAULT NULL,
  `ClassroomId` varchar(6) NOT NULL DEFAULT '',
  PRIMARY KEY (`UoSCode`,`Semester`,`Year`,`ClassroomId`),
  UNIQUE KEY `ClassroomConflict` (`ClassroomId`,`Semester`,`Year`,`ClassTime`),
  CONSTRAINT `fk1` FOREIGN KEY (`UoSCode`, `Semester`, `Year`) REFERENCES `uosoffering` (`UoSCode`, `Semester`, `Year`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk2` FOREIGN KEY (`ClassroomId`) REFERENCES `classroom` (`ClassroomId`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `lecture`
--

LOCK TABLES `lecture` WRITE;
/*!40000 ALTER TABLE `lecture` DISABLE KEYS */;
INSERT INTO `lecture` VALUES ('COMP5045','Q1',2015,'Fri10','CAR159'),('INFO3220','Q1',2015,'Tue15','CAR159'),('COMP5045','Q1',2016,'Fri10','CAR159'),('INFO2120','Q1',2016,'Tue13','CAR159'),('INFO3220','Q1',2016,'Tue15','CAR159'),('INFO2005','Q2',2011,'Mon09','CAR159'),('COMP3615','Q2',2015,'Wed15','CAR159'),('COMP3615','Q2',2016,'Wed15','CAR159'),('INFO2120','Q1',2013,'Mon11','CAR175'),('INFO3402','Q1',2015,'Thu12','CAR175'),('INFO3402','Q1',2016,'Thu12','CAR175'),('INFO1105','Q2',2015,'Wed12','CAR175'),('INFO1105','Q2',2016,'Wed12','CAR175'),('INFO1003','Q1',2013,'Mon12','CheLT4'),('COMP2129','Q1',2015,'Fri15','CheLT4'),('INFO1103','Q1',2015,'Mon12','CheLT4'),('COMP3308','Q1',2015,'Tue13','CheLT4'),('COMP2129','Q1',2016,'Fri15','CheLT4'),('INFO1103','Q1',2016,'Mon12','CheLT4'),('COMP3308','Q1',2016,'Tue13','CheLT4'),('INFO1003','Q2',2013,'Mon12','CheLT4'),('INFO3404','Q2',2015,'Mon09','CheLT4'),('COMP3520','Q2',2015,'Thu15','CheLT4'),('INFO1103','Q2',2015,'Tue12','CheLT4'),('COMP3520','Q2',2016,'Thu15','CheLT4'),('INFO1103','Q2',2016,'Tue12','CheLT4'),('COMP5338','Q1',2013,'Tue18','EA404'),('COMP5348','Q1',2015,'Thu10','EA404'),('COMP5348','Q1',2016,'Thu10','EA404'),('COMP3109','Q2',2015,'Wed10','EA404'),('COMP3109','Q2',2016,'Wed10','EA404'),('INFO3005','Q1',2012,'Wed09','EALT'),('COMP5046','Q1',2015,'Mon13','EALT'),('INFO2120','Q1',2016,'Mon09','EALT'),('COMP5046','Q1',2016,'Mon13','EALT'),('INFO2110','Q2',2015,'Fri12','EALT'),('INFO3315','Q2',2015,'Mon15','EALT'),('INFO2110','Q2',2016,'Fri12','EALT'),('INFO3315','Q2',2016,'Mon15','EALT'),('COMP3419','Q1',2015,'Mon10','SITLT'),('COMP3419','Q1',2016,'Mon10','SITLT'),('COMP5138','Q2',2013,'Mon18','SITLT'),('COMP2007','Q2',2015,'Tue10','SITLT'),('COMP2007','Q2',2016,'Tue10','SITLT');
/*!40000 ALTER TABLE `lecture` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `requires`
--

DROP TABLE IF EXISTS `requires`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `requires` (
  `UoSCode` char(8) NOT NULL DEFAULT '',
  `PrereqUoSCode` char(8) NOT NULL DEFAULT '',
  `EnforcedSince` date NOT NULL,
  PRIMARY KEY (`UoSCode`,`PrereqUoSCode`),
  KEY `PrereqUoSCode` (`PrereqUoSCode`),
  CONSTRAINT `requires_ibfk_1` FOREIGN KEY (`UoSCode`) REFERENCES `unitofstudy` (`UoSCode`),
  CONSTRAINT `requires_ibfk_2` FOREIGN KEY (`PrereqUoSCode`) REFERENCES `unitofstudy` (`UoSCode`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `requires`
--

LOCK TABLES `requires` WRITE;
/*!40000 ALTER TABLE `requires` DISABLE KEYS */;
INSERT INTO `requires` VALUES ('COMP2007','INFO1105','2015-01-01'),('COMP2129','INFO1103','2015-01-01'),('COMP3109','COMP2007','2015-01-01'),('COMP3419','COMP2007','2015-01-01'),('COMP3520','COMP2129','2015-01-01'),('COMP3520','INFO1105','2015-01-01'),('COMP3615','INFO3402','2015-01-01'),('COMP5045','COMP2007','2015-01-01'),('COMP5046','COMP3308','2015-01-01'),('COMP5338','COMP5138','2011-01-01'),('COMP5338','INFO2120','2011-01-01'),('COMP5348','INFO3220','2015-01-01'),('INFO1105','INFO1103','2015-01-01'),('INFO2005','INFO1003','2009-01-01'),('INFO2110','INFO1003','2015-01-01'),('INFO2120','INFO1003','2009-01-01'),('INFO3005','INFO2005','2009-01-01'),('INFO3220','INFO1105','2015-01-01'),('INFO3220','INFO2110','2015-01-01'),('INFO3315','INFO2110','2015-01-01'),('INFO3402','INFO2110','2015-01-01'),('INFO3404','INFO2120','2011-11-01');
/*!40000 ALTER TABLE `requires` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `student`
--

DROP TABLE IF EXISTS `student`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `student` (
  `Id` int(11) NOT NULL DEFAULT '0',
  `Name` varchar(20) NOT NULL,
  `Password` varchar(10) NOT NULL,
  `Address` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `student`
--

LOCK TABLES `student` WRITE;
/*!40000 ALTER TABLE `student` DISABLE KEYS */;
INSERT INTO `student` VALUES (3213,'Linda Smith','lunch','Cronulla'),(5123,'Harry Jenkins','butterflY','Blacktown');
/*!40000 ALTER TABLE `student` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transcript`
--

DROP TABLE IF EXISTS `transcript`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `transcript` (
  `StudId` int(11) NOT NULL DEFAULT '0',
  `UoSCode` char(8) NOT NULL DEFAULT '',
  `Semester` char(2) NOT NULL DEFAULT '',
  `Year` int(11) NOT NULL DEFAULT '0',
  `Grade` varchar(2) DEFAULT NULL,
  PRIMARY KEY (`StudId`,`UoSCode`,`Semester`,`Year`),
  KEY `fk1_idx` (`UoSCode`,`Semester`,`Year`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transcript`
--

LOCK TABLES `transcript` WRITE;
/*!40000 ALTER TABLE `transcript` DISABLE KEYS */;
INSERT INTO `transcript` VALUES (3213,'COMP2007','Q2',2015,'CR'),(3213,'COMP2129','Q1',2015,NULL),(3213,'COMP3109','Q2',2016,NULL),(3213,'COMP3419','Q1',2015,'D'),(3213,'COMP3615','Q2',2016,NULL),(3213,'INFO1003','Q1',2015,'CR'),(3213,'INFO1103','Q1',2015,'D'),(3213,'INFO1105','Q2',2015,'D'),(3213,'INFO2110','Q2',2015,'D'),(3213,'INFO2120','Q1',2015,NULL),(3213,'INFO3220','Q1',2015,'D'),(3213,'INFO3402','Q1',2015,'D'),(5123,'INFO1003','Q1',2015,'P'),(5123,'INFO1103','Q1',2015,NULL),(5123,'INFO1105','Q2',2015,'CR'),(5123,'INFO2110','Q2',2015,'P'),(5123,'INFO2120','Q1',2015,NULL);
/*!40000 ALTER TABLE `transcript` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `unitofstudy`
--

DROP TABLE IF EXISTS `unitofstudy`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `unitofstudy` (
  `UoSCode` char(8) NOT NULL DEFAULT '',
  `DeptId` char(3) NOT NULL,
  `UoSName` varchar(40) NOT NULL,
  `Credits` int(11) NOT NULL,
  PRIMARY KEY (`UoSCode`),
  UNIQUE KEY `DeptId` (`DeptId`,`UoSName`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `unitofstudy`
--

LOCK TABLES `unitofstudy` WRITE;
/*!40000 ALTER TABLE `unitofstudy` DISABLE KEYS */;
INSERT INTO `unitofstudy` VALUES ('COMP2007','SIT','Algorithms and Complexity',6),('COMP2129','SIT','Operating Systems and Machine Principles',6),('COMP3109','SIT','Programming Languages and Paradigms',6),('COMP3308','SIT','Introduction to Artificial Intelligence',6),('COMP3419','SIT','Graphics and Multimedia',6),('COMP3520','SIT','Operating Systems Internals',6),('COMP3615','SIT','Software Development Project',6),('COMP5045','SIT','Computational Geometry',6),('COMP5046','SIT','Statistical Natural Language Processing',6),('COMP5138','SIT','Relational Database Management Systems',6),('COMP5338','SIT','Advanced Data Models',6),('COMP5348','SIT','Enterprise Scale Software Architecture',6),('INFO1003','SIT','Introduction to IT',6),('INFO1103','SIT','Introduction to Programming',6),('INFO1105','SIT','Data Structures',6),('INFO2005','SIT','Database Management Introductory',3),('INFO2110','SIT','Systems Analysis and Modelling',6),('INFO2120','SIT','Database Systems I',6),('INFO3005','SIT','Organisational Database Systems',3),('INFO3220','SIT','Object Oriented Design',6),('INFO3315','SIT','Human-Computer Interaction',6),('INFO3402','SIT','Management of IT Projects and Systems',6),('INFO3404','SIT','Database Systems II',6),('MATH1002','MAT','Linear Algebra',3);
/*!40000 ALTER TABLE `unitofstudy` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `uosoffering`
--

DROP TABLE IF EXISTS `uosoffering`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `uosoffering` (
  `UoSCode` char(8) NOT NULL DEFAULT '',
  `Semester` char(2) NOT NULL DEFAULT '',
  `Year` int(11) NOT NULL DEFAULT '0',
  `Textbook` varchar(50) DEFAULT NULL,
  `Enrollment` int(11) DEFAULT NULL,
  `MaxEnrollment` int(11) DEFAULT NULL,
  `InstructorId` char(9) DEFAULT NULL,
  PRIMARY KEY (`UoSCode`,`Semester`,`Year`),
  KEY `InstructorId` (`InstructorId`),
  CONSTRAINT `uosoffering_ibfk_1` FOREIGN KEY (`UoSCode`) REFERENCES `unitofstudy` (`UoSCode`),
  CONSTRAINT `uosoffering_ibfk_2` FOREIGN KEY (`UoSCode`, `Semester`) REFERENCES `whenoffered` (`UoSCode`, `Semester`),
  CONSTRAINT `uosoffering_ibfk_3` FOREIGN KEY (`InstructorId`) REFERENCES `faculty` (`Id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `uosoffering`
--

LOCK TABLES `uosoffering` WRITE;
/*!40000 ALTER TABLE `uosoffering` DISABLE KEYS */;
INSERT INTO `uosoffering` VALUES ('COMP2007','Q2',2015,'Kleinberg/Tardos',111,200,'0123459'),('COMP2007','Q2',2016,'Kleinberg/Tardos',110,200,'0123459'),('COMP2129','Q1',2015,'Kay/Kummerfeld',66,200,'8901234'),('COMP2129','Q1',2016,'Kay/Kummerfeld',68,200,'8901234'),('COMP3109','Q2',2015,'none',30,200,'7123459'),('COMP3109','Q2',2016,'none',82,200,'7123459'),('COMP3308','Q1',2015,'Russell/Norvig',34,200,'1122334'),('COMP3308','Q1',2016,'Russell/Norvig',45,200,'1122334'),('COMP3419','Q1',2015,'none',37,200,'9012345'),('COMP3419','Q1',2016,'none',37,200,'9012345'),('COMP3520','Q2',2015,'Silberschatz/Galvin/Gagne',52,200,'7890123'),('COMP3520','Q2',2016,'Silberschatz/Galvin/Gagne',43,200,'7890123'),('COMP3615','Q2',2015,'none',76,200,'6789012'),('COMP3615','Q2',2016,'none',64,200,'6789012'),('COMP5045','Q1',2015,'de Berg/Cheong/van Kreveld/Overmars',12,200,'4856787'),('COMP5045','Q1',2016,'de Berg/Cheong/van Kreveld/Overmars',18,200,'4856787'),('COMP5046','Q1',2015,'none',29,200,'1234567'),('COMP5046','Q1',2016,'none',23,200,'1234567'),('COMP5138','Q2',2013,'Ramakrishnan/Gehrke',60,100,'1237890'),('COMP5338','Q1',2013,'none',32,50,'6339103'),('COMP5338','Q2',2013,'none',30,50,'7891234'),('COMP5348','Q1',2015,'Gorton',8,200,'4657890'),('COMP5348','Q1',2016,'Gorton',23,200,'4657890'),('INFO1003','Q1',2013,'Snyder',150,200,'0987654'),('INFO1003','Q1',2015,'Snyder',150,200,'0987654'),('INFO1003','Q1',2016,'Snyder',150,200,'0987654'),('INFO1003','Q2',2013,'Snyder',80,200,'0987654'),('INFO1003','Q2',2015,'Snyder',80,200,'0987654'),('INFO1003','Q2',2016,'Snyder',80,200,'0987654'),('INFO1103','Q1',2015,'Rober Sedgewick/Kevin Wayne',123,200,'0123456'),('INFO1103','Q1',2016,'Rober Sedgewick/Kevin Wayne',154,200,'0123456'),('INFO1103','Q2',2015,'Rober Sedgewick/Kevin Wayne',101,200,'0123456'),('INFO1103','Q2',2016,'Rober Sedgewick/Kevin Wayne',165,200,'0123456'),('INFO1105','Q2',2015,'Frank M. Carrano/Janet J. Prichard',76,200,'2345678'),('INFO1105','Q2',2016,'Frank M. Carrano/Janet J. Prichard',85,200,'2345678'),('INFO2005','Q2',2011,'Hoffer',370,400,'6339103'),('INFO2110','Q2',2015,'Hoffer/George/Valacich',86,200,'4657890'),('INFO2110','Q2',2016,'Hoffer/George/Valacich',76,200,'4657890'),('INFO2120','Q1',2013,'Kifer/Bernstein/Lewis',140,200,'6339103'),('INFO2120','Q1',2015,'Kifer/Bernstein/Lewis',178,200,'6339103'),('INFO2120','Q1',2016,'Kifer/Bernstein/Lewis',178,200,'6339103'),('INFO3005','Q1',2012,'Hoffer',100,150,'1122334'),('INFO3220','Q1',2015,'none',79,200,'5678901'),('INFO3220','Q1',2016,'none',72,200,'5678901'),('INFO3315','Q2',2015,'Helen Sharp/Yvonne Rogers/Jenny Preece',24,200,'4567890'),('INFO3315','Q2',2016,'Helen Sharp/Yvonne Rogers/Jenny Preece',32,200,'4567890'),('INFO3402','Q1',2015,'none',36,200,'3456789'),('INFO3402','Q1',2016,'none',27,200,'3456789'),('INFO3404','Q2',2015,'Ramakrishnan/Gehrke',80,150,'7891234');
/*!40000 ALTER TABLE `uosoffering` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `whenoffered`
--

DROP TABLE IF EXISTS `whenoffered`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `whenoffered` (
  `UoSCode` char(8) NOT NULL DEFAULT '',
  `Semester` char(2) NOT NULL DEFAULT '',
  PRIMARY KEY (`UoSCode`,`Semester`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `whenoffered`
--

LOCK TABLES `whenoffered` WRITE;
/*!40000 ALTER TABLE `whenoffered` DISABLE KEYS */;
INSERT INTO `whenoffered` VALUES ('COMP2007','Q2'),('COMP2129','Q1'),('COMP2129','Q3'),('COMP3109','Q2'),('COMP3308','Q1'),('COMP3419','Q1'),('COMP3520','Q2'),('COMP3615','Q2'),('COMP5045','Q1'),('COMP5046','Q1'),('COMP5138','Q1'),('COMP5138','Q2'),('COMP5138','Q4'),('COMP5338','Q1'),('COMP5338','Q2'),('COMP5338','Q3'),('COMP5348','Q1'),('INFO1003','Q1'),('INFO1003','Q2'),('INFO1103','Q1'),('INFO1103','Q2'),('INFO1103','Q3'),('INFO1105','Q2'),('INFO2005','Q2'),('INFO2110','Q2'),('INFO2110','Q3'),('INFO2120','Q1'),('INFO3005','Q1'),('INFO3220','Q1'),('INFO3225','Q4'),('INFO3315','Q2'),('INFO3402','Q1'),('INFO3404','Q2'),('MATH1002','Q1'),('MATH1002','Q2');
/*!40000 ALTER TABLE `whenoffered` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-11-09 19:35:44
