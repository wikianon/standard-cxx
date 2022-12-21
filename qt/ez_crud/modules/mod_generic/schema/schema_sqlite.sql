CREATE  TABLE IF NOT EXISTS `categoria` (
  `id` INT(11) NOT NULL ,
  `nombre` VARCHAR(50) NULL ,
  PRIMARY KEY (`id`) );

CREATE  TABLE IF NOT EXISTS `persona` (
  `id` INT(11) NOT NULL ,
  `tipopersona` INT(4) NOT NULL ,
  `razonsocial` VARCHAR(100) NULL ,
  `ruc` VARCHAR(50) NULL ,
  `barrio` VARCHAR(100) NULL ,
  `calle1` VARCHAR(100) NULL ,
  `calle2` VARCHAR(100) NULL ,
  `telefono1` VARCHAR(100) NULL ,
  `telefono2` VARCHAR(100) NULL ,
  `email1` VARCHAR(100) NULL ,
  `email2` VARCHAR(100) NULL ,
  PRIMARY KEY (`id`) );

CREATE  TABLE IF NOT EXISTS `persona_categoria` (
  `idpersona` INT(11) NOT NULL ,
  `idcategoria` INT(11) NOT NULL ,
  PRIMARY KEY (`idpersona`, `idcategoria`) ,
  CONSTRAINT `fk_persona_has_categoria_persona1`
    FOREIGN KEY (`idpersona` )
    REFERENCES `persona` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_persona_has_categoria_categoria1`
    FOREIGN KEY (`idcategoria` )
    REFERENCES `categoria` (`id` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);

CREATE  TABLE IF NOT EXISTS `sequences` (
  `name` VARCHAR(200) NOT NULL ,
  `initialvalue` BIGINT(12) NOT NULL ,
  `nextvalue` BIGINT(12) NOT NULL ,
  PRIMARY KEY (`name`) ,
  UNIQUE (`name`) );