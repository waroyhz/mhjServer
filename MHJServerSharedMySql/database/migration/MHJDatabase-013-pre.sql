/* This file was generated by ODB, object-relational mapping (ORM)
 * compiler for C++.
 */

CREATE TABLE `admin_entity` (
  `id` INT(11) NOT NULL PRIMARY KEY AUTO_INCREMENT,
  `UserName` LONGTEXT NOT NULL,
  `UserTitle` LONGTEXT NOT NULL,
  `Password` LONGTEXT NOT NULL,
  `userToken` CHAR(255) NULL,
  `tokenReplaceTime` DATETIME NULL)
 ENGINE=InnoDB;

CREATE INDEX `UserName_i`
  ON `admin_entity` (`UserName`);

CREATE INDEX `Password_i`
  ON `admin_entity` (`Password`);

UPDATE `schema_version`
  SET `version` = 13, `migration` = 1
  WHERE `name` = '';
