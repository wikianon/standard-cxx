INSERT INTO mne_crm.company (companyid, createdate, createuser, modifydate, modifyuser, name, ownerid, sorting)
  VALUES ('000000000001', 0, 'admindb', 0, 'admindb', 'own company', NULL, NULL);
  
INSERT INTO mne_crm.companydata (createdate, createuser, modifydate, modifyuser, companydataid, telefon, http, email, categorie, language, fax, mobile)
  VALUES (0, 'admindb', 0, 'admindb', '000000000001', '', '', '', '', 'en', '', '');

INSERT INTO mne_crm.companyown (companyid, companyownid, createdate, createuser, modifydate, modifyuser, currency, prefix, blz, bank, account) 
  VALUES ('000000000001', '000000000001', 0, 'admindb', 0, 'admindb', 'EUR', '', '', '', '');

INSERT INTO mne_crm.person (createdate, createuser, firstname, lastname, modifydate, modifyuser, personid, refid, sex, sign, ownerid, sorting)
  VALUES (0, 'admindb', 'database', 'administrator', 0, 'admindb', '53e324940000', '000000000001', 0, '', NULL, NULL);
 
INSERT INTO mne_crm.persondata (createdate, createuser, modifydate, modifyuser, email, telephonpriv, telephonoffice, http, birthday, persondataid, telephonmobil, language, color, role)
 VALUES (0, 'admindb', 0, 'admindb', '', '', '', '', NULL, '53e324940000', '', '', NULL, '');
 
INSERT INTO mne_crm.personowndata (color, createdate, createuser, loginname, modifydate, modifyuser, personid, personowndataid,  active) 
 VALUES ('', 0, 'admindb', 'admindb', 0, 'admindb', '53e324940000', '53e3249d0000', false);
 
INSERT INTO mne_personnal.personowndata ( createdate, createuser,  modifydate, modifyuser, personid, personowndataid, unitcost, wtime ) 
 VALUES ( 0, 'admindb', 0, 'admindb', '53e324940000', '53e3249d0000', 0, 0 );
  