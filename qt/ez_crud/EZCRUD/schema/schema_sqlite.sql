CREATE  TABLE IF NOT EXISTS `module` (
  `id` INT(11) NOT NULL ,
  `modulename` VARCHAR(200) NOT NULL ,
  `modulemenu` VARCHAR(50) NOT NULL ,
  PRIMARY KEY (`id`) );


CREATE  TABLE IF NOT EXISTS `widget` (
  `id` INT(11) NOT NULL ,
  `idmodule` INT(11) NOT NULL ,
  `widgetname` VARCHAR(200) NOT NULL ,
  `widgetmenu` VARCHAR(50) NULL ,
  `widgettype` INT(2) NOT NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_widget_module1`
    FOREIGN KEY (`idmodule` )
    REFERENCES `module` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


CREATE  TABLE IF NOT EXISTS `revision` (
  `id` INT(11) NOT NULL ,
  `datetime` DATE NOT NULL ,
  `iduser` INT(11) NOT NULL ,
  `idorganization` INT(11) NOT NULL ,
  PRIMARY KEY (`id`) ,
  CONSTRAINT `fk_revision_user1`
    FOREIGN KEY (`iduser` )
    REFERENCES `user` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_revision_organization1`
    FOREIGN KEY (`idorganization` )
    REFERENCES `organization` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


CREATE  TABLE IF NOT EXISTS `module_revision` (
  `idrevision` INT(11) NOT NULL ,
  `idmodule` INT(11) NOT NULL ,
  PRIMARY KEY (`idrevision`, `idmodule`) ,
  CONSTRAINT `fk_module_revision_revision1`
    FOREIGN KEY (`idrevision` )
    REFERENCES `revision` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_module_revision_module1`
    FOREIGN KEY (`idmodule` )
    REFERENCES `module` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


CREATE  TABLE IF NOT EXISTS `widget_revision` (
  `idrevision` INT(11) NOT NULL ,
  `idwidget` INT(11) NOT NULL ,
  `accessmode` INT(2) NOT NULL ,
  PRIMARY KEY (`idrevision`, `idwidget`) ,
  CONSTRAINT `fk_widget_revision_revision1`
    FOREIGN KEY (`idrevision` )
    REFERENCES `revision` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_widget_revision_widget1`
    FOREIGN KEY (`idwidget` )
    REFERENCES `widget` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


CREATE  TABLE IF NOT EXISTS `organization` (
  `id` INT(11) NOT NULL ,
  `name` VARCHAR(100) NOT NULL ,
  `hostaddress` VARCHAR(200) NOT NULL ,
  `hostdatabase` VARCHAR(200) NOT NULL ,
  `hostuser` VARCHAR(200) NOT NULL ,
  `hostpassword` VARCHAR(200) NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE (`name`) );


CREATE  TABLE IF NOT EXISTS `sequences` (
  `name` VARCHAR(200) NOT NULL ,
  `initialvalue` BIGINT(12) NOT NULL ,
  `nextvalue` BIGINT(12) NOT NULL ,
  UNIQUE (`name`) ,
  PRIMARY KEY (`name`) );


CREATE  TABLE IF NOT EXISTS `user` (
  `id` INT(11) NOT NULL ,
  `datetime` DATE NOT NULL ,
  `loginname` VARCHAR(200) NOT NULL ,
  `password` VARCHAR(200) NOT NULL ,
  `username` VARCHAR(200) NOT NULL ,
  `email` VARCHAR(200) NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE (`loginname`) );
