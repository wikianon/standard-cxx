CREATE  TABLE IF NOT EXISTS `industryhacks`.`module` (
  `id` INT(11) NOT NULL ,
  `modulename` VARCHAR(200) NOT NULL ,
  `modulemenu` VARCHAR(50) NOT NULL ,
  PRIMARY KEY (`id`) )
ENGINE = InnoDB;

CREATE  TABLE IF NOT EXISTS `industryhacks`.`widget` (
  `id` INT(11) NOT NULL ,
  `idmodule` INT(11) NOT NULL ,
  `widgetname` VARCHAR(200) NOT NULL ,
  `widgetmenu` VARCHAR(50) NULL ,
  `widgettype` INT(2) NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_widget_module1` (`idmodule` ASC) ,
  CONSTRAINT `fk_widget_module1`
    FOREIGN KEY (`idmodule` )
    REFERENCES `industryhacks`.`module` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE  TABLE IF NOT EXISTS `industryhacks`.`revision` (
  `id` INT(11) NOT NULL ,
  `datetime` DATETIME NOT NULL ,
  `iduser` INT(11) NOT NULL ,
  `idorganization` INT(11) NOT NULL ,
  PRIMARY KEY (`id`) ,
  INDEX `fk_revision_user1` (`iduser` ASC) ,
  INDEX `fk_revision_organization1` (`idorganization` ASC) ,
  CONSTRAINT `fk_revision_user1`
    FOREIGN KEY (`iduser` )
    REFERENCES `industryhacks`.`user` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_revision_organization1`
    FOREIGN KEY (`idorganization` )
    REFERENCES `industryhacks`.`organization` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE  TABLE IF NOT EXISTS `industryhacks`.`module_revision` (
  `idrevision` INT(11) NOT NULL ,
  `idmodule` INT(11) NOT NULL ,
  PRIMARY KEY (`idrevision`, `idmodule`) ,
  INDEX `fk_module_revision_module1` (`idmodule` ASC) ,
  CONSTRAINT `fk_module_revision_revision1`
    FOREIGN KEY (`idrevision` )
    REFERENCES `industryhacks`.`revision` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_module_revision_module1`
    FOREIGN KEY (`idmodule` )
    REFERENCES `industryhacks`.`module` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE  TABLE IF NOT EXISTS `industryhacks`.`widget_revision` (
  `idrevision` INT(11) NOT NULL ,
  `idwidget` INT(11) NOT NULL ,
  `accessmode` INT(2) NOT NULL ,
  PRIMARY KEY (`idrevision`, `idwidget`) ,
  INDEX `fk_widget_revision_widget1` (`idwidget` ASC) ,
  CONSTRAINT `fk_widget_revision_revision1`
    FOREIGN KEY (`idrevision` )
    REFERENCES `industryhacks`.`revision` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_widget_revision_widget1`
    FOREIGN KEY (`idwidget` )
    REFERENCES `industryhacks`.`widget` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE  TABLE IF NOT EXISTS `industryhacks`.`organization` (
  `id` INT(11) NOT NULL ,
  `name` VARCHAR(100) NOT NULL ,
  `hostaddress` VARCHAR(200) NOT NULL ,
  `hostdatabase` VARCHAR(200) NOT NULL ,
  `hostuser` VARCHAR(200) NOT NULL ,
  `hostpassword` VARCHAR(200) NOT NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `name_UNIQUE` (`name` ASC) )
ENGINE = InnoDB;

CREATE  TABLE IF NOT EXISTS `industryhacks`.`sequences` (
  `name` VARCHAR(200) NOT NULL ,
  `initialvalue` BIGINT(12) NOT NULL ,
  `nextvalue` BIGINT(12) NOT NULL ,
  UNIQUE INDEX `name_UNIQUE` (`name` ASC) ,
  PRIMARY KEY (`name`) )
ENGINE = InnoDB;

CREATE  TABLE IF NOT EXISTS `industryhacks`.`user` (
  `id` INT(11) NOT NULL ,
  `datetime` VARCHAR(200) NOT NULL ,
  `loginname` VARCHAR(200) NOT NULL ,
  `password` VARCHAR(200) NOT NULL ,
  `username` VARCHAR(200) NOT NULL ,
  `email` VARCHAR(200) NULL ,
  PRIMARY KEY (`id`) ,
  UNIQUE INDEX `loginname_UNIQUE` (`loginname` ASC) )
ENGINE = InnoDB;